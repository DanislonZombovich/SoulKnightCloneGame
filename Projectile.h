#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f pos, sf::Vector2f dir, float speed, float damage, bool enemyBullet);
    void update(sf::Time dt);
    void draw(sf::RenderWindow& win);
    sf::FloatRect getBounds() const;
    bool isExpired() const;
    float getDamage() const;
    bool isEnemyBullet() const;
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float damage;
    float lifetime = 3.0f;
    bool enemyBullet;
};