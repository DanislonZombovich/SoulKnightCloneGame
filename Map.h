#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct MapData {
    std::vector<std::vector<int>> tiles;
    sf::Vector2f startPos;
    std::vector<sf::Vector2f> enemySpawns;
    std::vector<std::pair<sf::Vector2f, int>> weaponDrops;
    sf::Vector2f doorPos;
    int doorType = 0;
};

class Map {
public:
    Map(int locationNumber);
    void draw(sf::RenderWindow& win);
    std::vector<sf::FloatRect> getObstacles() const;
    sf::Vector2f getDoorPosition() const;
    int getDoorType() const { return data.doorType; }
    MapData getData() const { return data; }
    sf::Vector2f getStartPosition() const { return startPos; }
    bool isWallAt(int tileX, int tileY) const;
    void removeWallAt(int tileX, int tileY);
    void addWeaponDrop(sf::Vector2f pos, int type);
    std::vector<std::pair<sf::Vector2f, int>>& getWeaponDrops() { return data.weaponDrops; }

private:
    MapData data;
    std::unique_ptr<sf::Texture> floorTex;
    std::unique_ptr<sf::Texture> wallTex;
    std::unique_ptr<sf::Texture> doorTex;
    std::unique_ptr<sf::Texture> bossDoorTex;
    std::unique_ptr<sf::Texture> weaponSMGTex;
    std::unique_ptr<sf::Texture> weaponARTex;
    sf::RectangleShape floorRect;
    sf::RectangleShape wallRect;
    sf::RectangleShape doorRect;
    sf::RectangleShape bossDoorRect;
    sf::RectangleShape weaponSMG;
    sf::RectangleShape weaponAR;
    sf::Vector2f startPos;
};