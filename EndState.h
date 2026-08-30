#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class EndState : public State {
public:
    EndState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;
private:
    sf::Font font;
    sf::Text winText;
};