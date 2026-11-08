#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

class UI {
public:
    UI();
    void draw(sf::RenderWindow& win, Player& player, const std::vector<Enemy>* enemies, Boss* boss, bool nearDoor = false, int doorType = 0);
private:
    sf::Font font;
};