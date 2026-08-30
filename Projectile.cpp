#include "Projectile.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f dir, float speed, float damage, bool enemyBullet)
    : damage(damage), enemyBullet(enemyBullet) {
    shape.setSize({8, 4});
    shape.setFillColor(enemyBullet ? sf::Color::Red : sf::Color::Yellow);
    shape.setPosition(pos);
    float angleRad = std::atan2(dir.y, dir.x);
    shape.setRotation(sf::radians(angleRad));
    velocity = dir * speed;
}

void Projectile::update(sf::Time dt) {
    shape.move(velocity * dt.asSeconds());
    lifetime -= dt.asSeconds();
}

void Projectile::draw(sf::RenderWindow& win) {
    win.draw(shape);
}

sf::FloatRect Projectile::getBounds() const {
    return shape.getGlobalBounds();
}

bool Projectile::isExpired() const { return lifetime <= 0; }
float Projectile::getDamage() const { return damage; }
bool Projectile::isEnemyBullet() const { return enemyBullet; }