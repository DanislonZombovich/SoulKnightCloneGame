#include "PauseState.h"
#include "Game.h"
#include "MenuState.h"

PauseState::PauseState(Game& game) : State(game),
    pauseText(font, "PAUSED", 60),
    resumeText(font, "Resume", 40),
    menuText(font, "Main Menu", 40)
{
    if (!font.openFromFile("assets/fonts/arial.ttf")) {}
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(60);
    pauseText.setFillColor(sf::Color::Red);
    pauseText.setPosition({1280/2.f - 120, 100});

    resumeText.setFont(font);
    resumeText.setString("Resume");
    resumeText.setCharacterSize(40);
    resumeText.setPosition({1280/2.f - 80, 300});

    menuText.setFont(font);
    menuText.setString("Main Menu");
    menuText.setCharacterSize(40);
    menuText.setPosition({1280/2.f - 120, 400});
}

void PauseState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::W || key->code == sf::Keyboard::Key::Up) {
            selected = (selected - 1 + 2) % 2;
        } else if (key->code == sf::Keyboard::Key::S || key->code == sf::Keyboard::Key::Down) {
            selected = (selected + 1) % 2;
        } else if (key->code == sf::Keyboard::Key::Enter) {
            if (selected == 0) {
                game.popState();
            } else {
                game.changeState(std::make_unique<MenuState>(game));
            }
        }
    }
}

void PauseState::update(sf::Time dt) {}

void PauseState::render() {
    sf::RectangleShape overlay(sf::Vector2f(1280, 720));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    game.window.draw(overlay);

    game.window.draw(pauseText);
    resumeText.setFillColor(selected == 0 ? sf::Color::Yellow : sf::Color::White);
    menuText.setFillColor(selected == 1 ? sf::Color::Yellow : sf::Color::White);
    game.window.draw(resumeText);
    game.window.draw(menuText);
}