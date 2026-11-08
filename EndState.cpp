#include "EndState.h"
#include "Game.h"
#include "MenuState.h"

EndState::EndState(Game& game) : State(game), winText(font, "YOU WIN! CONGRATULATIONS!", 60) {
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        if (!font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            if (!font.openFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
                (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");
            }
        }
    }
    winText.setFont(font);
    winText.setString("YOU WIN! CONGRATULATIONS!");
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Green);
    winText.setPosition({1280/2.f - 400, 300});
}

void EndState::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        game.changeState(std::make_unique<MenuState>(game));
    }
}

void EndState::update(sf::Time dt) {}
void EndState::render() { game.window.draw(winText); }