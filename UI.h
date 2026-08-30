#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

class UI {
public:
    void draw(sf::RenderWindow& win, Player& player, const std::vector<Enemy>* enemies, Boss* boss);
private:
    sf::Font font;
};