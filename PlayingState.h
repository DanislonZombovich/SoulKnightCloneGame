#pragma once
#include "State.h"
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "Boss.h"
#include "UI.h"
#include <vector>

class PlayingState : public State {
public:
    PlayingState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void render() override;
private:
    Player player;
    Map map;
    int currentLocation = 1;
    bool bossRoomEntered = false;
    std::vector<Enemy> enemies;
    std::vector<Projectile> playerBullets;
    std::vector<Projectile> enemyBullets;
    Boss boss;
    UI ui;
    bool bossActive = false;
    bool gameEnded = false;

    void spawnEnemies();
    void checkDoorInteraction();
    void checkWeaponPickup();
    void checkBulletHits();
    void checkPlayerDeath();
    void switchToNextLocation();
    void switchToBossRoom();
    void startBossFight();
    sf::Vector2f getAimDirection();
};