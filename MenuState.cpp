#include "MenuState.h"
#include "PlayingState.h"
#include "Game.h"

MenuState::MenuState(Game& game) : State(game),
    title(font, "SOUL KNIGHT CLONE", 60),
    newGameText(font, "New Game", 40),
    exitText(font, "Exit", 40)
{
    if (!font.openFromFile("assets/fonts/arial.ttf")) {

    }
    title.setFont(font);
    title.setString("SOUL KNIGHT CLONE");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setPosition({1280/2.f - 300, 100});

    newGameText.setFont(font);
    newGameText.setString("New Game");
    newGameText.setCharacterSize(40);
    newGameText.setPosition({1280/2.f - 100, 300});

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(40);
    exitText.setPosition({1280/2.f - 60, 400});
}

void MenuState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::W || key->code == sf::Keyboard::Key::Up) {
            selected = (selected - 1 + 2) % 2;
        } else if (key->code == sf::Keyboard::Key::S || key->code == sf::Keyboard::Key::Down) {
            selected = (selected + 1) % 2;
        } else if (key->code == sf::Keyboard::Key::Enter) {
            if (selected == 0) {
                game.changeState(std::make_unique<PlayingState>(game));
            } else {
                game.window.close();
            }
        }
    }
}

void MenuState::update(sf::Time dt) {}

void MenuState::render() {
    game.window.draw(title);
    newGameText.setFillColor(selected == 0 ? sf::Color::Yellow : sf::Color::White);
    exitText.setFillColor(selected == 1 ? sf::Color::Yellow : sf::Color::White);
    game.window.draw(newGameText);
    game.window.draw(exitText);
}