#include "PlayingState.h"
#include "Game.h"
#include "PauseState.h"
#include "EndState.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

PlayingState::PlayingState(Game& game) : State(game), map(1) {
    player.setPosition(map.getStartPosition());
    spawnEnemies();
}

void PlayingState::handleEvent(const sf::Event& event) {
    if (gameEnded) return;
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Escape)
            game.pushState(std::make_unique<PauseState>(game));
    }
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            if (player.getCurrentWeapon()->getType() != WeaponType::Pistol) return;
            if (player.getCurrentWeapon()->canFire()) {
                player.getCurrentWeapon()->fire();
                sf::Vector2f dir = getAimDirection();
                playerBullets.emplace_back(player.getPosition(), dir, 600.f,
                                           player.getCurrentWeapon()->getDamage(), false);
            }
        }
    }
}

void PlayingState::update(sf::Time dt) {
    if (gameEnded) return;

    player.handleInput();
    player.update(dt, map.getObstacles());

    if (player.getCurrentWeapon()->getType() != WeaponType::Pistol) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (player.getCurrentWeapon()->canFire()) {
                player.getCurrentWeapon()->fire();
                sf::Vector2f dir = getAimDirection();
                playerBullets.emplace_back(player.getPosition(), dir, 600.f,
                                           player.getCurrentWeapon()->getDamage(), false);
            }
        }
    }

    for (auto& b : playerBullets) b.update(dt);
    for (auto& b : enemyBullets) b.update(dt);

    const auto& obstacles = map.getObstacles();
    playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(),
        [&obstacles](Projectile& p) {
            if (p.isExpired()) return true;
            for (const auto& obs : obstacles) {
                if (p.getBounds().findIntersection(obs)) return true;
            }
            return false;
        }), playerBullets.end());

    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [&obstacles](Projectile& p) {
            if (p.isExpired()) return true;
            for (const auto& obs : obstacles) {
                if (p.getBounds().findIntersection(obs)) return true;
            }
            return false;
        }), enemyBullets.end());

    if (!bossRoomEntered) {
        for (auto& e : enemies) {
            if (!e.isDead()) e.update(dt, player.getPosition(), map, enemyBullets);
        }
    }

    if (bossActive && !boss.isDead())
        boss.update(dt, player, enemyBullets, map);

    checkBulletHits();
    checkPlayerDeath();
    checkDoorInteraction();
    checkWeaponPickup();

    if (!bossRoomEntered) {
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e){ return e.isDead(); }), enemies.end());
    }

    if (bossActive && boss.isDead()) {
        gameEnded = true;
        game.changeState(std::make_unique<EndState>(game));
    }
}

void PlayingState::render() {
    map.draw(game.window);
    if (!bossRoomEntered) {
        for (auto& e : enemies) if (!e.isDead()) e.draw(game.window);
    }
    player.draw(game.window, getAimDirection());
    for (auto& b : playerBullets) b.draw(game.window);
    for (auto& b : enemyBullets) b.draw(game.window);
    if (bossActive && !boss.isDead()) boss.draw(game.window);
    ui.draw(game.window, player, bossRoomEntered ? nullptr : &enemies, bossActive ? &boss : nullptr);
}

sf::Vector2f PlayingState::getAimDirection() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);
    sf::Vector2f worldPos = game.window.mapPixelToCoords(mousePos);
    sf::Vector2f dir = worldPos - player.getPosition();
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    return (len != 0) ? dir / len : sf::Vector2f(1,0);
}

void PlayingState::spawnEnemies() {
    enemies.clear();
    if (bossRoomEntered) return;


    const auto& spawns = map.getData().enemySpawns;
    int numAvailable = spawns.size();

    int count = 3 + rand() % 3;
    if (count > numAvailable) count = numAvailable;


    std::vector<sf::Vector2f> shuffled = spawns;
    for (int i = 0; i < numAvailable; ++i) {
        int j = rand() % numAvailable;
        std::swap(shuffled[i], shuffled[j]);
    }

    for (int i = 0; i < count; ++i) {
        Enemy::Type t = (currentLocation >= 2) ? Enemy::RifleEnemy : static_cast<Enemy::Type>(rand() % 4);
        enemies.emplace_back(t, shuffled[i], currentLocation >= 2);
    }
}

void PlayingState::checkDoorInteraction() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && enemies.empty()) {
        sf::Vector2f doorPos = map.getDoorPosition();
        if (doorPos.x >= 0) {
            float dist = std::sqrt((player.getPosition().x-doorPos.x)*(player.getPosition().x-doorPos.x) +
                                   (player.getPosition().y-doorPos.y)*(player.getPosition().y-doorPos.y));
            if (dist < 60) {
                if (map.getDoorType() == 2) {
                    switchToBossRoom();
                } else {
                    switchToNextLocation();
                }
            }
        }
    }
}

void PlayingState::switchToNextLocation() {
    currentLocation++;
    map = Map(currentLocation);
    player.setPosition(map.getStartPosition());
    player.increaseMaxHP();
    spawnEnemies();
}

void PlayingState::switchToBossRoom() {
    currentLocation = 6;
    map = Map(6);
    player.setPosition(map.getStartPosition());
    bossRoomEntered = true;
    enemies.clear();
    boss.reset();
    startBossFight();
}

void PlayingState::startBossFight() {
    bossActive = true;
    boss.setPhase(BossPhase::Phase2);
}

void PlayingState::checkWeaponPickup() {
    auto& drops = map.getWeaponDrops();
    for (auto it = drops.begin(); it != drops.end(); ) {
        float dist = std::sqrt((player.getPosition().x - it->first.x) * (player.getPosition().x - it->first.x) +
                               (player.getPosition().y - it->first.y) * (player.getPosition().y - it->first.y));
        if (dist < 40) {
            WeaponType wt = (it->second == 0) ? WeaponType::SMG : WeaponType::AssaultRifle;
            player.pickupWeapon(wt);
            it = drops.erase(it);
        } else {
            ++it;
        }
    }
}

void PlayingState::checkBulletHits() {
    for (auto& b : playerBullets) {
        if (!bossRoomEntered) {
            for (auto& e : enemies) {
                if (!e.isDead() && b.getBounds().findIntersection(e.getBounds())) {
                    e.takeDamage(b.getDamage());
                    b = Projectile({0,0},{0,0},0,0,false);
                    break;
                }
            }
        }
        if (bossActive && !boss.isDead()) {
            if (b.getBounds().findIntersection(boss.getBounds())) {
                boss.takeDamage(b.getDamage());
                b = Projectile({0,0},{0,0},0,0,false);
            }
        }
    }
    for (auto& b : enemyBullets) {
        if (b.getBounds().findIntersection(player.getBounds())) {
            player.takeDamage(b.getDamage());
            b = Projectile({0,0},{0,0},0,0,false);
        }
    }
    if (!bossRoomEntered) {
        for (auto& e : enemies) {
            if (!e.isDead() && e.getType() == Enemy::Melee && e.getBounds().findIntersection(player.getBounds())) {
                static sf::Clock meleeClock;
                if (meleeClock.getElapsedTime().asSeconds() > 0.5f) {
                    player.takeDamage(10);
                    meleeClock.restart();
                }
            }
        }
    }
}

void PlayingState::checkPlayerDeath() {
    if (player.getHP() <= 0) {
        player.setPosition(map.getStartPosition());
        player.resetHP();
        if (bossRoomEntered) {
            boss.reset();
            startBossFight();
        } else {
            spawnEnemies();
        }
    }
}