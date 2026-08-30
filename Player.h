#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Weapon.h"

class Player {
public:
    Player();
    void handleInput();
    void update(sf::Time dt, const std::vector<sf::FloatRect>& obstacles);
    void draw(sf::RenderWindow& win, sf::Vector2f aimDir);
    void takeDamage(float dmg);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void setPosition(sf::Vector2f pos);
    void resetHP();
    void increaseMaxHP();
    float getHP() const;
    float getMaxHP() const;
    Weapon* getCurrentWeapon();
    void pickupWeapon(WeaponType type);
    void nextWeapon();
    void setStunned(bool st);
    bool isStunned() const;

private:
    std::unique_ptr<sf::Texture> playerTexture;
    sf::RectangleShape body;
    sf::Vector2f velocity;
    float speed = 250.f;
    float hp = 100;
    float maxHp = 100;
    bool stunned = false;
    std::vector<Weapon> weapons;
    int currentWeaponIndex = 0;
    sf::Clock stunClock;
};