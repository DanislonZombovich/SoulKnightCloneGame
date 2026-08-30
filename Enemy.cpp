#include "Enemy.h"
#include "Map.h"
#include <cmath>

Enemy::Enemy(Type type, sf::Vector2f pos, bool hard) : type(type), hardMode(hard) {
    body.setSize({32, 32});
    body.setOrigin({16, 16});

    enemyTexture = std::make_unique<sf::Texture>();
    std::string textureFile;
    switch (type) {
        case Melee:       textureFile = "assets/textures/enemy_melee.png"; break;
        case PistolEnemy: textureFile = "assets/textures/enemy_pistol.png"; break;
        case SMGEnemy:    textureFile = "assets/textures/enemy_smg.png"; break;
        case RifleEnemy:  textureFile = "assets/textures/enemy_rifle.png"; break;
    }

    if (!enemyTexture->loadFromFile(textureFile)) {
        switch (type) {
            case Melee:       body.setFillColor(sf::Color::Red); break;
            case PistolEnemy: body.setFillColor(sf::Color(255, 165, 0)); break;
            case SMGEnemy:    body.setFillColor(sf::Color(128, 0, 128)); break;
            case RifleEnemy:  body.setFillColor(sf::Color(139, 69, 19)); break;
        }
    } else {
        body.setTexture(enemyTexture.get());
    }
    body.setPosition(pos);
    body.setRotation(sf::degrees(90));

    switch (type) {
        case Melee:
            maxHp = 100; damage = 20; speed = 150; attackRange = 40; attackCooldown = 1.0f; break;
        case PistolEnemy:
            maxHp = 80; damage = 25; speed = 120; attackRange = 300; attackCooldown = 0.2f; break;
        case SMGEnemy:
            maxHp = 70; damage = 15; speed = 140; attackRange = 350; attackCooldown = 0.1f; break;
        case RifleEnemy:
            maxHp = 60; damage = 35; speed = 100; attackRange = 500; attackCooldown = 0.15f; break;
    }
    if (hardMode) {
        maxHp += 50;
        damage += 25;
        speed *= 1.3f;
    }
    hp = maxHp;
}

void Enemy::update(sf::Time dt, sf::Vector2f playerPos, const Map& map, std::vector<Projectile>& enemyBullets) {
    sf::Vector2f dir = playerPos - body.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normDir(0,0);
    if (dist != 0) {
        normDir = dir / dist;
        lastAngle = std::atan2(normDir.y, normDir.x) * 180.f / 3.14159f;
    }

    std::vector<sf::FloatRect> obstacles = map.getObstacles();

    sf::Vector2f movement(0, 0);
    if (type != Melee) {
        if (dist > attackRange * 1.2f)
            movement = normDir * speed * dt.asSeconds();
        else if (dist < attackRange * 0.8f)
            movement = -normDir * speed * dt.asSeconds();
    } else {
        movement = normDir * speed * dt.asSeconds();
    }

    sf::Vector2f pos = body.getPosition();
    pos.x += movement.x;
    body.setPosition(pos);
    for (const auto& obs : obstacles) {
        if (body.getGlobalBounds().findIntersection(obs)) {
            pos.x -= movement.x;
            body.setPosition(pos);
            break;
        }
    }
    pos.y += movement.y;
    body.setPosition(pos);
    for (const auto& obs : obstacles) {
        if (body.getGlobalBounds().findIntersection(obs)) {
            pos.y -= movement.y;
            body.setPosition(pos);
            break;
        }
    }

    if (attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
        if (type == Melee && dist < attackRange) {
            attackClock.restart();
        } else if (type != Melee && dist < attackRange) {
            enemyBullets.emplace_back(body.getPosition(), normDir, 400.f, damage, true);
            attackClock.restart();
        }
    }
}

void Enemy::draw(sf::RenderWindow& win) {
    body.setRotation(sf::degrees(lastAngle + 90.f));
    win.draw(body);
    body.setRotation(sf::degrees(90));
}

void Enemy::takeDamage(float dmg) { hp -= dmg; if (hp < 0) hp = 0; }
bool Enemy::isDead() const { return hp <= 0; }
sf::FloatRect Enemy::getBounds() const { return body.getGlobalBounds(); }
sf::Vector2f Enemy::getPosition() const { return body.getPosition(); }
float Enemy::getHP() const { return hp; }
float Enemy::getMaxHP() const { return maxHp; }