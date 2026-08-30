#include "Game.h"
#include "MenuState.h"
#include <SFML/Audio.hpp>

Game::Game() : window(sf::VideoMode({1280, 720}), "Soul Knight Clone") {
    window.setFramerateLimit(60);


    static sf::Music backgroundMusic;
    if (backgroundMusic.openFromFile("assets/audio/background.ogg")) {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }

    changeState(std::make_unique<MenuState>(*this));
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        processEvents();
        update(dt);
        render();
    }
}

void Game::changeState(std::unique_ptr<State> state) {
    while (!states.empty()) states.pop();
    states.push(std::move(state));
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::popState() {
    if (!states.empty()) states.pop();
}

void Game::processEvents() {
    while (auto eventOpt = window.pollEvent()) {
        sf::Event event = *eventOpt;
        if (event.is<sf::Event::Closed>())
            window.close();
        if (!states.empty())
            states.top()->handleEvent(event);
    }
}

void Game::update(sf::Time dt) {
    if (!states.empty())
        states.top()->update(dt);
}

void Game::render() {
    window.clear(sf::Color::Black);
    if (!states.empty())
        states.top()->render();
    window.display();
}