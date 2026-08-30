#pragma once
#include <SFML/System/Clock.hpp>

enum class WeaponType { Pistol, SMG, AssaultRifle };

class Weapon {
public:
    Weapon(WeaponType type);
    WeaponType getType() const { return type; }
    bool canFire();
    void fire();
    void reload();
    bool isReloading() const;
    void update();
    int getCurrentAmmo() const;
    int getMagSize() const;
    float getDamage() const;

private:
    WeaponType type;
    int currentAmmo;
    int magSize;
    float damage;
    float reloadTime;
    float fireRate;
    sf::Clock fireClock;
    sf::Clock reloadClock;
    bool reloading = false;
};