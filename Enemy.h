#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Projectile.h"

class Map;

class Enemy {
public:
    enum Type { Melee, PistolEnemy, SMGEnemy, RifleEnemy };
    Enemy(Type type, sf::Vector2f pos, bool hard = false);
    void update(sf::Time dt, sf::Vector2f playerPos, const Map& map, std::vector<Projectile>& enemyBullets);
    void draw(sf::RenderWindow& win);
    void takeDamage(float dmg);
    bool isDead() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    float getHP() const;
    float getMaxHP() const;
    Type getType() const { return type; }

private:
    Type type;
    std::unique_ptr<sf::Texture> enemyTexture;
    sf::RectangleShape body;
    float hp, maxHp;
    float damage;
    float speed;
    float attackRange;
    float attackCooldown;
    sf::Clock attackClock;
    bool hardMode;
    float lastAngle = 0.f;
};