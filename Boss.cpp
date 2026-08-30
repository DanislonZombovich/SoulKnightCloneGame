#include "Boss.h"
#include "Player.h"
#include "Map.h"
#include <cmath>
#include <cstdlib>

Boss::Boss() {
    bossTexture = std::make_unique<sf::Texture>();
    body.setSize({64, 64});
    body.setOrigin({32, 32});
    if (!bossTexture->loadFromFile("assets/textures/boss_phase2.png")) {
        body.setFillColor(sf::Color(80, 80, 80));
    } else {
        body.setTexture(bossTexture.get());
    }
    body.setPosition({640, 360});

    auraCircle.setRadius(auraRadius);
    auraCircle.setOrigin({auraRadius, auraRadius});
    auraCircle.setFillColor(sf::Color(255, 0, 0, 50));
    auraCircle.setOutlineColor(sf::Color::Red);
    auraCircle.setOutlineThickness(2);

    sweepingBeamRect.setSize({sweepingBeamLength, 10.f});
    sweepingBeamRect.setOrigin({0, 5.f});
    sweepingBeamRect.setFillColor(sf::Color::Cyan);
}

void Boss::reset() {
    phase = BossPhase::Quiz;
    hp = 1000; maxHp = 1000;
    body.setSize({64, 64});
    body.setOrigin({32, 32});
    if (!bossTexture->loadFromFile("assets/textures/boss_phase2.png"))
        body.setFillColor(sf::Color(80, 80, 80));
    else
        body.setTexture(bossTexture.get());
    body.setPosition({640, 360});

    ramming = false;
    rushActive = false;
    sweepingActive = false;
    sweepingWarning = false;
    quizRemaining = 5;

    attackClock.restart(); ramClock.restart(); chargeClock.restart();
    rushCooldownClock.restart(); rushDurationClock.restart();
    auraDamageClock.restart(); sweepingCooldownClock.restart();
}

sf::FloatRect Boss::getBounds() const { return body.getGlobalBounds(); }

void Boss::update(sf::Time dt, Player& player, std::vector<Projectile>& enemyBullets, Map& map) {
    if (phase == BossPhase::Quiz || phase == BossPhase::Dead) return;
    if (phase == BossPhase::Phase2 || phase == BossPhase::Phase3) {
        float moveSpeed = (phase == BossPhase::Phase3) ? 50.f : 80.f;
        if (!rushActive) {
            sf::Vector2f dir = player.getPosition() - body.getPosition();
            float dist = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            if (dist > 0) {
                sf::Vector2f normDir = dir / dist;
                auto obstacles = map.getObstacles();
                sf::Vector2f movement = normDir * moveSpeed * dt.asSeconds();
                sf::Vector2f pos = body.getPosition();
                pos.x += movement.x; body.setPosition(pos);
                for (const auto& obs : obstacles) {
                    if (body.getGlobalBounds().findIntersection(obs)) {
                        pos.x -= movement.x; body.setPosition(pos); break;
                    }
                }
                pos.y += movement.y; body.setPosition(pos);
                for (const auto& obs : obstacles) {
                    if (body.getGlobalBounds().findIntersection(obs)) {
                        pos.y -= movement.y; body.setPosition(pos); break;
                    }
                }
            }
        }

        redAttack(enemyBullets, player.getPosition());
        blueAttack(dt, player);
        ramAttack(dt, player);
        rushAttack(dt, player, map);
        circleAura(dt, player);
        if (phase == BossPhase::Phase3) sweepingBeam(dt, player);
    }
}

void Boss::draw(sf::RenderWindow& win) {
    if (phase == BossPhase::Phase2 || phase == BossPhase::Phase3) {
        win.draw(body);
        auraCircle.setPosition(body.getPosition());
        win.draw(auraCircle);
    }

    if (phase == BossPhase::Phase3 && sweepingWarning) {
        sweepingWarningLine[0].position = body.getPosition();
        sweepingWarningLine[1].position = body.getPosition() +
            sf::Vector2f(std::cos(sweepingBeamAngle), std::sin(sweepingBeamAngle)) * sweepingBeamLength;
        sweepingWarningLine[0].color = sf::Color::Red;
        sweepingWarningLine[1].color = sf::Color::Red;
        win.draw(sweepingWarningLine, 2, sf::PrimitiveType::Lines);
    }
    if (phase == BossPhase::Phase3 && sweepingActive) {
        sweepingBeamRect.setPosition(body.getPosition());
        sweepingBeamRect.setRotation(sf::degrees(sweepingBeamAngle * 180.f / 3.14159f));
        win.draw(sweepingBeamRect);
    }
}

void Boss::takeDamage(float dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
    if (phase == BossPhase::Phase2 && hp <= 0) {
        phase = BossPhase::Phase3;
        hp = 1500; maxHp = 1500;
        body.setSize({80, 80});
        body.setOrigin({40, 40});
        if (!bossTexture->loadFromFile("assets/textures/boss_phase3.png"))
            body.setFillColor(sf::Color(100, 100, 100));
        else
            body.setTexture(bossTexture.get());
    } else if (phase == BossPhase::Phase3 && hp <= 0) {
        phase = BossPhase::Dead;
    }
}

bool Boss::isDead() const { return phase == BossPhase::Dead; }
float Boss::getHP() const { return hp; }
float Boss::getMaxHP() const { return maxHp; }
BossPhase Boss::getPhase() const { return phase; }

void Boss::setPhase(BossPhase p) {
    phase = p;
    if (p == BossPhase::Phase2) {
        hp = 1000; maxHp = 1000;
        body.setSize({64,64});
        body.setOrigin({32,32});
        if (!bossTexture->loadFromFile("assets/textures/boss_phase2.png"))
            body.setFillColor(sf::Color(80, 80, 80));
        else
            body.setTexture(bossTexture.get());
        rushActive = false;
    }
}

void Boss::startQuiz() { quizRemaining = 5; phase = BossPhase::Quiz; }
void Boss::quizCorrectAnswer() { quizRemaining--; if (quizRemaining <= 0) setPhase(BossPhase::Phase2); }


void Boss::redAttack(std::vector<Projectile>& bullets, sf::Vector2f playerPos) {
    if (attackClock.getElapsedTime().asSeconds() < 0.8f) return;
    const int numBullets = 8;
    const float angleStep = 2 * 3.14159f / numBullets;
    for (int i = 0; i < numBullets; ++i) {
        float angle = i * angleStep;
        sf::Vector2f dir(std::cos(angle), std::sin(angle));
        bullets.emplace_back(body.getPosition(), dir, 300.f, 40, true);
    }
    attackClock.restart();
}

void Boss::blueAttack(sf::Time dt, Player& player) {
    if (chargeClock.getElapsedTime().asSeconds() < 2.0f) return;
    static sf::Vector2f targetZone;
    static bool charging = false;
    if (!charging) {
        targetZone = player.getPosition();
        charging = true;
        chargeClock.restart();
    }
    if (charging && chargeClock.getElapsedTime().asSeconds() >= 1.5f) {
        sf::FloatRect zone(targetZone - sf::Vector2f(50,50), {100,100});
        if (zone.contains(player.getPosition())) player.takeDamage(50);
        charging = false;
        chargeClock.restart();
    }
}

void Boss::ramAttack(sf::Time dt, Player& player) {
    if (ramClock.getElapsedTime().asSeconds() < 5.0f) return;
    if (!ramming) { ramming = true; ramTarget = player.getPosition(); ramClock.restart(); }
    if (ramming) {
        sf::Vector2f dir = ramTarget - body.getPosition();
        float dist = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (dist > 10.f) body.move(dir / dist * 350.f * dt.asSeconds());
        else {
            if (body.getGlobalBounds().findIntersection(player.getBounds())) player.setStunned(true);
            ramming = false; ramClock.restart();
        }
    }
}

void Boss::rushAttack(sf::Time dt, Player& player, Map& map) {
    if (hp >= 500 || rushCooldownClock.getElapsedTime().asSeconds() < rushCooldown) return;

    if (!rushActive) {
        rushActive = true;
        sf::Vector2f dir = player.getPosition() - body.getPosition();
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        rushDir = (len > 0) ? dir / len : sf::Vector2f(1, 0);
        rushDistanceLeft = maxRushDistance;
        rushDurationClock.restart();
    }

    if (rushActive) {
        float moveStep = rushSpeed * dt.asSeconds();
        if (moveStep > rushDistanceLeft) moveStep = rushDistanceLeft;
        sf::Vector2f movement = rushDir * moveStep;
        auto obstacles = map.getObstacles();
        sf::Vector2f pos = body.getPosition();
        pos.x += movement.x; body.setPosition(pos);
        for (const auto& obs : obstacles) {
            if (body.getGlobalBounds().findIntersection(obs)) {
                pos.x -= movement.x; body.setPosition(pos);
                rushActive = false; rushCooldownClock.restart(); return;
            }
        }
        pos.y += movement.y; body.setPosition(pos);
        for (const auto& obs : obstacles) {
            if (body.getGlobalBounds().findIntersection(obs)) {
                pos.y -= movement.y; body.setPosition(pos);
                rushActive = false; rushCooldownClock.restart(); return;
            }
        }
        rushDistanceLeft -= moveStep;
        if (rushDistanceLeft <= 0 || rushDurationClock.getElapsedTime().asSeconds() >= rushMaxDuration) {
            rushActive = false;
            rushCooldownClock.restart();
        }
    }
}

void Boss::circleAura(sf::Time dt, Player& player) {
    float dist = std::sqrt(
        (player.getPosition().x - body.getPosition().x) * (player.getPosition().x - body.getPosition().x) +
        (player.getPosition().y - body.getPosition().y) * (player.getPosition().y - body.getPosition().y)
    );
    if (dist <= auraRadius) {
        if (auraDamageClock.getElapsedTime().asSeconds() >= auraTickRate) {
            player.takeDamage(auraDamage);
            auraDamageClock.restart();
        }
    }
}

void Boss::sweepingBeam(sf::Time dt, Player& player) {
    if (!sweepingWarning && !sweepingActive && sweepingCooldownClock.getElapsedTime().asSeconds() >= sweepingBeamCooldown) {
        sweepingWarning = true;
        sweepingClock.restart();
        sf::Vector2f dir = player.getPosition() - body.getPosition();
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        sweepingBeamAngle = (len > 0) ? std::atan2(dir.y, dir.x) : 0.f;
    }

    if (sweepingWarning && !sweepingActive) {
        if (sweepingClock.getElapsedTime().asSeconds() >= sweepingBeamWarningTime) {
            sweepingWarning = false;
            sweepingActive = true;
            sweepingClock.restart();
        }
    }

    if (sweepingActive) {
        sf::Vector2f dir(std::cos(sweepingBeamAngle), std::sin(sweepingBeamAngle));
        sf::Vector2f lineEnd = body.getPosition() + dir * sweepingBeamLength;
        sf::Vector2f v = lineEnd - body.getPosition();
        sf::Vector2f w = player.getPosition() - body.getPosition();
        float c1 = v.x*w.x + v.y*w.y;
        if (c1 > 0) {
            float c2 = v.x*v.x + v.y*v.y;
            float b = c1 / c2;
            sf::Vector2f pb = body.getPosition() + b * v;
            float dist = std::sqrt((player.getPosition().x-pb.x)*(player.getPosition().x-pb.x)+(player.getPosition().y-pb.y)*(player.getPosition().y-pb.y));
            if (dist < 15.f) {
                player.takeDamage(9999);
            }
        }

        if (sweepingClock.getElapsedTime().asSeconds() >= sweepingBeamShotDuration) {
            sweepingActive = false;
            sweepingCooldownClock.restart();
        }
    }
}