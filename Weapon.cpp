#include "Weapon.h"
#include <SFML/System/Time.hpp>

Weapon::Weapon(WeaponType type) : type(type) {
    switch (type) {
        case WeaponType::Pistol:
            magSize = 20; damage = 20; reloadTime = 2.0f; fireRate = 0.2f; break;
        case WeaponType::SMG:
            magSize = 40; damage = 30; reloadTime = 2.0f; fireRate = 0.1f; break;
        case WeaponType::AssaultRifle:
            magSize = 30; damage = 34; reloadTime = 2.0f; fireRate = 0.15f; break;
    }
    currentAmmo = magSize;
}

bool Weapon::canFire() {
    if (reloading) return false;
    if (currentAmmo <= 0) return false;
    if (fireClock.getElapsedTime().asSeconds() < fireRate) return false;
    return true;
}

void Weapon::fire() {
    currentAmmo--;
    if (currentAmmo == 0) reload();
    fireClock.restart();
}

void Weapon::reload() {
    if (currentAmmo == magSize) return;
    reloading = true;
    reloadClock.restart();
}

bool Weapon::isReloading() const { return reloading; }

void Weapon::update() {
    if (reloading && reloadClock.getElapsedTime().asSeconds() >= reloadTime) {
        currentAmmo = magSize;
        reloading = false;
    }
}

int Weapon::getCurrentAmmo() const { return currentAmmo; }
int Weapon::getMagSize() const { return magSize; }
float Weapon::getDamage() const { return damage; }