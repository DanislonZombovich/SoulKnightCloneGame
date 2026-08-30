#include "UI.h"

void UI::draw(sf::RenderWindow& win, Player& player, const std::vector<Enemy>* enemies, Boss* boss) {
    if (!font.openFromFile("assets/fonts/arial.ttf")) {}

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

    Weapon* w = player.getCurrentWeapon();
    sf::Text ammoText(font, std::to_string(w->getCurrentAmmo())+" / "+std::to_string(w->getMagSize()), 24);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition({1200 - ammoText.getLocalBounds().size.x, 670});
    win.draw(ammoText);

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