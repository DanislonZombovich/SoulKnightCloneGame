#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class PauseState : public State {
public:
    PauseState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;
private:
    sf::Font font;
    sf::Text pauseText;
    sf::Text resumeText;
    sf::Text menuText;
    int selected = 0;
};