#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "State.h"

class Game {
public:
    Game();
    void run();
    void changeState(std::unique_ptr<State> state);
    void pushState(std::unique_ptr<State> state);
    void popState();
    sf::RenderWindow window;
private:
    std::stack<std::unique_ptr<State>> states;
    void processEvents();
    void update(sf::Time dt);
    void render();
};