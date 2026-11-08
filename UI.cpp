#include "UI.h"
#include <iostream>

UI::UI() {
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        if (!font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            if (!font.openFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
                (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");
            }
        }
    }
}

void UI::draw(sf::RenderWindow& win, Player& player, const std::vector<Enemy>* enemies, Boss* boss, bool nearDoor, int doorType) {
    sf::RectangleShape bg({200, 20});
    bg.setFillColor(sf::Color(50,50,50));
    bg.setPosition({10, 10});
    win.draw(bg);
    sf::RectangleShape hpBar({200 * (player.getHP()/player.getMaxHP()), 20});
    hpBar.setFillColor(sf::Color::Red);
    hpBar.setPosition({10, 10});
    win.draw(hpBar);
    sf::Text hpText(font, std::to_string((int)player.getHP())+"/"+std::to_string((int)player.getMaxHP()), 14);
    hpText.setPosition({10, 30});
    win.draw(hpText);

    if (player.isStunned()) {
        sf::Text stunText(font, "STUNNED!", 20);
        stunText.setFillColor(sf::Color::Yellow);
        stunText.setPosition({10, 55});
        win.draw(stunText);
    }

    Weapon* w = player.getCurrentWeapon();
    std::string wName = "Pistol";
    if (w->getType() == WeaponType::SMG) wName = "SMG";
    else if (w->getType() == WeaponType::AssaultRifle) wName = "Assault Rifle";

    std::string ammoStr;
    if (w->isReloading()) {
        ammoStr = "[" + wName + "] RELOADING...";
    } else {
        ammoStr = "[" + wName + "] " + std::to_string(w->getCurrentAmmo()) + " / " + std::to_string(w->getMagSize());
    }

    sf::Text ammoText(font, ammoStr, 22);
    ammoText.setFillColor(w->isReloading() ? sf::Color(255, 180, 0) : sf::Color::White);
    ammoText.setPosition({1260.f - ammoText.getLocalBounds().size.x, 670.f});
    win.draw(ammoText);

    if (nearDoor) {
        std::string prompt = (doorType == 2) ? "[E] Enter Boss Room" : "[E] Enter Next Room";
        sf::Text doorText(font, prompt, 22);
        doorText.setFillColor(sf::Color::Cyan);
        doorText.setPosition({1280/2.f - doorText.getLocalBounds().size.x / 2.f, 650.f});
        win.draw(doorText);
    }

    if (enemies) {
        for (const auto& e : *enemies) {
            if (e.isDead()) continue;
            sf::Vector2f pos = e.getPosition();
            float w = 40;
            sf::RectangleShape bg({w, 6});
            bg.setFillColor(sf::Color(50,50,50));
            bg.setPosition({pos.x - w/2, pos.y - 25});
            win.draw(bg);
            sf::RectangleShape hp({w * (e.getHP()/e.getMaxHP()), 6});
            hp.setFillColor(sf::Color::Red);
            hp.setPosition({pos.x - w/2, pos.y - 25});
            win.draw(hp);
        }
    }

    if (boss && boss->getPhase() != BossPhase::Quiz && !boss->isDead()) {
        sf::RectangleShape bossBg({400, 30});
        bossBg.setFillColor(sf::Color(50,50,50));
        bossBg.setPosition({440, 10});
        win.draw(bossBg);
        sf::RectangleShape bossHp({400 * (boss->getHP()/boss->getMaxHP()), 30});
        bossHp.setFillColor(sf::Color::Magenta);
        bossHp.setPosition({440, 10});
        win.draw(bossHp);
        sf::Text bossText(font, "BOSS "+std::to_string((int)boss->getHP())+"/"+std::to_string((int)boss->getMaxHP()), 18);
        bossText.setPosition({440, 40});
        win.draw(bossText);
    }
}