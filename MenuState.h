#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class MenuState : public State {
public:
    MenuState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;
private:
    sf::Font font;
    sf::Text title;
    sf::Text newGameText;
    sf::Text exitText;
    int selected = 0;
};