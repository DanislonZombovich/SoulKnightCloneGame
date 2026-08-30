#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Projectile.h"

class Player;
class Map;

enum class BossPhase { Quiz, Phase2, Phase3, Dead };

class Boss {
public:
    Boss();
    void update(sf::Time dt, Player& player, std::vector<Projectile>& enemyBullets, Map& map);
    void draw(sf::RenderWindow& win);
    void takeDamage(float dmg);
    bool isDead() const;
    float getHP() const;
    float getMaxHP() const;
    BossPhase getPhase() const;
    sf::FloatRect getBounds() const;
    void setPhase(BossPhase p);
    void reset();
    void startQuiz();
    bool quizActive() const { return phase == BossPhase::Quiz; }
    void quizCorrectAnswer();
    int getQuizQuestionsRemaining() const { return quizRemaining; }

private:
    BossPhase phase = BossPhase::Quiz;
    std::unique_ptr<sf::Texture> bossTexture;
    sf::RectangleShape body;
    float hp = 0, maxHp = 0;
    sf::Clock attackClock, ramClock, chargeClock;
    bool ramming = false;
    sf::Vector2f ramTarget;
    int quizRemaining = 5;

    bool rushActive = false;
    sf::Vector2f rushDir;
    float rushSpeed = 500.f;
    float rushDistanceLeft = 0.f;
    float maxRushDistance = 500.f;
    sf::Clock rushCooldownClock;
    float rushCooldown = 10.0f;
    sf::Clock rushDurationClock;
    float rushMaxDuration = 1.5f;

    void redAttack(std::vector<Projectile>& bullets, sf::Vector2f playerPos);
    void blueAttack(sf::Time dt, Player& player);
    void ramAttack(sf::Time dt, Player& player);
    void rushAttack(sf::Time dt, Player& player, Map& map);
    void circleAura(sf::Time dt, Player& player);

    void sweepingBeam(sf::Time dt, Player& player);
    bool sweepingActive = false;
    bool sweepingWarning = false;
    sf::Clock sweepingClock;
    float sweepingBeamAngle = 0.f;
    const float sweepingBeamLength = 1200.f;
    const float sweepingBeamWarningTime = 1.0f;
    const float sweepingBeamShotDuration = 0.5f;
    const float sweepingBeamCooldown = 4.0f;
    sf::Clock sweepingCooldownClock;
    sf::RectangleShape sweepingBeamRect;
    sf::Vertex sweepingWarningLine[2];

    sf::CircleShape auraCircle;
    sf::Clock auraDamageClock;
    float auraRadius = 120.f;
    float auraDamage = 40.f;
    float auraTickRate = 0.3f;
};