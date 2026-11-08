#include "Player.h"
#include <cmath>

Player::Player() {
    body.setSize({32, 32});
    body.setOrigin({16, 16});
    playerTexture = std::make_unique<sf::Texture>();
    if (!playerTexture->loadFromFile("assets/textures/player.png")) {
        body.setFillColor(sf::Color::Blue);
    } else {
        body.setTexture(playerTexture.get());
    }
    body.setPosition({200, 200});
    body.setRotation(sf::degrees(90));
    weapons.emplace_back(WeaponType::Pistol);
}

void Player::handleInput() {
    if (stunned) return;
    velocity = {0, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) velocity.y = -speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) velocity.y = speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) velocity.x = -speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) velocity.x = speed;
    if (velocity.x != 0.f && velocity.y != 0.f) {
        float factor = 0.70710678f;
        velocity.x *= factor;
        velocity.y *= factor;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        weapons[currentWeaponIndex].reload();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        static sf::Clock qClock;
        if (qClock.getElapsedTime().asSeconds() > 0.3f) {
            nextWeapon();
            qClock.restart();
        }
    }
}

void Player::update(sf::Time dt, const std::vector<sf::FloatRect>& obstacles) {
    weapons[currentWeaponIndex].update();
    if (stunned) {
        if (stunClock.getElapsedTime().asSeconds() > 5.0f)
            stunned = false;
        return;
    }
    sf::Vector2f move = velocity * dt.asSeconds();
    sf::Vector2f pos = body.getPosition();
    pos.x += move.x;
    body.setPosition(pos);
    for (const auto& obs : obstacles) {
        if (body.getGlobalBounds().findIntersection(obs)) {
            pos.x -= move.x;
            body.setPosition(pos);
            break;
        }
    }
    pos.y += move.y;
    body.setPosition(pos);
    for (const auto& obs : obstacles) {
        if (body.getGlobalBounds().findIntersection(obs)) {
            pos.y -= move.y;
            body.setPosition(pos);
            break;
        }
    }
}

void Player::draw(sf::RenderWindow& win, sf::Vector2f aimDir) {
    float angle = std::atan2(aimDir.y, aimDir.x) * 180.f / 3.14159f;
    body.setRotation(sf::degrees(angle + 90.f));
    win.draw(body);
    body.setRotation(sf::degrees(90));

    sf::Vertex line[2];
    line[0].position = body.getPosition();
    line[1].position = body.getPosition() + aimDir * 40.f;
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
    win.draw(line, 2, sf::PrimitiveType::Lines);
}

void Player::takeDamage(float dmg) { hp -= dmg; if (hp < 0) hp = 0; }
sf::Vector2f Player::getPosition() const { return body.getPosition(); }
sf::FloatRect Player::getBounds() const { return body.getGlobalBounds(); }
void Player::setPosition(sf::Vector2f pos) { body.setPosition(pos); }
void Player::resetHP() { hp = maxHp; }
void Player::increaseMaxHP() { maxHp = 150; hp = 150; }
float Player::getHP() const { return hp; }
float Player::getMaxHP() const { return maxHp; }
Weapon* Player::getCurrentWeapon() { return &weapons[currentWeaponIndex]; }

void Player::pickupWeapon(WeaponType type) {
    for (auto& w : weapons) {
        if (w.getType() == type)
            return;
    }
    weapons.emplace_back(type);
    currentWeaponIndex = weapons.size() - 1;
}

void Player::nextWeapon() {
    if (weapons.empty()) return;
    currentWeaponIndex = (currentWeaponIndex + 1) % weapons.size();
}

void Player::setStunned(bool st) {
    stunned = st;
    if (st) stunClock.restart();
}
bool Player::isStunned() const { return stunned; }