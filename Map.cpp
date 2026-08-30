#include "Map.h"

Map::Map(int locationNumber) {
    floorTex = std::make_unique<sf::Texture>();
    wallTex = std::make_unique<sf::Texture>();
    doorTex = std::make_unique<sf::Texture>();
    bossDoorTex = std::make_unique<sf::Texture>();
    weaponSMGTex = std::make_unique<sf::Texture>();
    weaponARTex = std::make_unique<sf::Texture>();

    bool hasFloor = floorTex->loadFromFile("assets/textures/floor.png");
    bool hasWall  = wallTex->loadFromFile("assets/textures/wall.png");
    bool hasDoor  = doorTex->loadFromFile("assets/textures/boss_door.png");
    bool hasBossDoor = bossDoorTex->loadFromFile("assets/textures/boss_door.png");
    bool hasSMG   = weaponSMGTex->loadFromFile("assets/textures/weapon_smg.png");
    bool hasAR    = weaponARTex->loadFromFile("assets/textures/weapon_rifle.png");

    floorRect.setSize({64, 64});
    wallRect.setSize({64, 64});
    doorRect.setSize({64, 64});
    bossDoorRect.setSize({64, 64});
    weaponSMG.setSize({32, 32});
    weaponAR.setSize({32, 32});

    if (hasFloor) floorRect.setTexture(floorTex.get());
    else floorRect.setFillColor(sf::Color(50,50,50));
    if (hasWall) wallRect.setTexture(wallTex.get());
    else wallRect.setFillColor(sf::Color(120,120,120));
    if (hasDoor) doorRect.setTexture(doorTex.get());
    else doorRect.setFillColor(sf::Color::Blue);
    if (hasBossDoor) bossDoorRect.setTexture(bossDoorTex.get());
    else bossDoorRect.setFillColor(sf::Color::Red);
    if (hasSMG) weaponSMG.setTexture(weaponSMGTex.get());
    else weaponSMG.setFillColor(sf::Color::Yellow);
    if (hasAR) weaponAR.setTexture(weaponARTex.get());
    else weaponAR.setFillColor(sf::Color::Red);

    auto isFree = [&](int x, int y) -> bool {
        if (x < 0 || x >= 20 || y < 0 || y >= 11) return false;
        return data.tiles[y][x] == 0;
    };


    if (locationNumber == 1) {
        startPos = {100, 100};
        data.tiles.resize(11, std::vector<int>(20, 0));
        for (int x=0; x<20; ++x) { data.tiles[0][x]=1; data.tiles[10][x]=1; }
        for (int y=0; y<11; ++y) { data.tiles[y][0]=1; data.tiles[y][19]=1; }
        data.tiles[3][5]=1; data.tiles[3][6]=1; data.tiles[7][14]=1; data.tiles[7][15]=1;
        data.tiles[5][10]=1; data.tiles[6][10]=1;
        data.tiles[9][17] = 3;
        data.doorPos = {17*64+32.f, 9*64+32.f};
        data.doorType = 1;

        if (isFree(5,2)) data.enemySpawns.push_back({5*64+32, 2*64+32});
        if (isFree(13,3)) data.enemySpawns.push_back({13*64+32, 3*64+32});
        if (isFree(7,7)) data.enemySpawns.push_back({7*64+32, 7*64+32});
        if (isFree(3,4)) data.enemySpawns.push_back({3*64+32, 4*64+32});
        if (isFree(15,6)) data.enemySpawns.push_back({15*64+32, 6*64+32});
        if (isFree(10,3)) data.enemySpawns.push_back({10*64+32, 3*64+32});
        if (isFree(8,8)) data.enemySpawns.push_back({8*64+32, 8*64+32});

        data.weaponDrops.push_back({{17*64+32, 9*64+32}, 0});
    }

    else if (locationNumber == 2) {
        startPos = {100, 100};
        data.tiles.resize(11, std::vector<int>(20, 0));
        for (int x=0; x<20; ++x) { data.tiles[0][x]=1; data.tiles[10][x]=1; }
        for (int y=0; y<11; ++y) { data.tiles[y][0]=1; data.tiles[y][19]=1; }
        data.tiles[4][8]=1; data.tiles[4][9]=1; data.tiles[4][10]=1;
        data.tiles[6][5]=1; data.tiles[7][5]=1;
        data.tiles[9][18] = 3;
        data.doorPos = {18*64+32.f, 9*64+32.f};
        data.doorType = 1;

        if (isFree(3,2)) data.enemySpawns.push_back({3*64+32, 2*64+32});
        if (isFree(14,4)) data.enemySpawns.push_back({14*64+32, 4*64+32});
        if (isFree(8,7)) data.enemySpawns.push_back({8*64+32, 7*64+32});
        if (isFree(6,3)) data.enemySpawns.push_back({6*64+32, 3*64+32});
        if (isFree(16,5)) data.enemySpawns.push_back({16*64+32, 5*64+32});
        if (isFree(4,6)) data.enemySpawns.push_back({4*64+32, 6*64+32});
        if (isFree(11,3)) data.enemySpawns.push_back({11*64+32, 3*64+32});
    }

    else if (locationNumber == 3) {
        startPos = {100, 100};
        data.tiles.resize(11, std::vector<int>(20, 0));
        for (int x=0; x<20; ++x) { data.tiles[0][x]=1; data.tiles[10][x]=1; }
        for (int y=0; y<11; ++y) { data.tiles[y][0]=1; data.tiles[y][19]=1; }
        data.tiles[2][3]=1; data.tiles[3][3]=1; data.tiles[4][4]=1;
        data.tiles[5][9]=1; data.tiles[5][10]=1; data.tiles[6][9]=1; data.tiles[6][10]=1;
        data.tiles[3][15]=1; data.tiles[7][15]=1;
        data.tiles[9][18] = 3;
        data.doorPos = {18*64+32.f, 9*64+32.f};
        data.doorType = 1;

        if (isFree(5,2)) data.enemySpawns.push_back({5*64+32, 2*64+32});
        if (isFree(14,2)) data.enemySpawns.push_back({14*64+32, 2*64+32});
        if (isFree(8,8)) data.enemySpawns.push_back({8*64+32, 8*64+32});
        if (isFree(2,7)) data.enemySpawns.push_back({2*64+32, 7*64+32});
        if (isFree(16,5)) data.enemySpawns.push_back({16*64+32, 5*64+32});
        if (isFree(10,3)) data.enemySpawns.push_back({10*64+32, 3*64+32});
        if (isFree(4,6)) data.enemySpawns.push_back({4*64+32, 6*64+32});
    }

    else if (locationNumber == 4) {
        startPos = {100, 100};
        data.tiles.resize(11, std::vector<int>(20, 0));
        for (int x=0; x<20; ++x) { data.tiles[0][x]=1; data.tiles[10][x]=1; }
        for (int y=0; y<11; ++y) { data.tiles[y][0]=1; data.tiles[y][19]=1; }
        data.tiles[2][4]=1; data.tiles[2][5]=1; data.tiles[2][6]=1;
        data.tiles[8][4]=1; data.tiles[8][5]=1; data.tiles[8][6]=1;
        data.tiles[4][14]=1; data.tiles[5][14]=1; data.tiles[6][14]=1;
        data.tiles[4][6]=1; data.tiles[5][6]=1; data.tiles[6][6]=1;
        data.tiles[9][18] = 3;
        data.doorPos = {18*64+32.f, 9*64+32.f};
        data.doorType = 1;

        if (isFree(3,2)) data.enemySpawns.push_back({3*64+32, 2*64+32});
        if (isFree(15,3)) data.enemySpawns.push_back({15*64+32, 3*64+32});
        if (isFree(10,7)) data.enemySpawns.push_back({10*64+32, 7*64+32});
        if (isFree(2,8)) data.enemySpawns.push_back({2*64+32, 8*64+32});
        if (isFree(17,5)) data.enemySpawns.push_back({17*64+32, 5*64+32});
        if (isFree(6,3)) data.enemySpawns.push_back({6*64+32, 3*64+32});
        if (isFree(12,4)) data.enemySpawns.push_back({12*64+32, 4*64+32});
    }

    else if (locationNumber == 5) {
        startPos = {100, 100};
        data.tiles.resize(11, std::vector<int>(20, 0));
        for (int x=0; x<20; ++x) { data.tiles[0][x]=1; data.tiles[10][x]=1; }
        for (int y=0; y<11; ++y) { data.tiles[y][0]=1; data.tiles[y][19]=1; }
        data.tiles[2][3]=1; data.tiles[2][4]=1; data.tiles[3][3]=1; data.tiles[3][4]=1;
        data.tiles[2][15]=1; data.tiles[2][16]=1; data.tiles[3][15]=1; data.tiles[3][16]=1;
        data.tiles[7][3]=1; data.tiles[7][4]=1; data.tiles[8][3]=1; data.tiles[8][4]=1;
        data.tiles[7][15]=1; data.tiles[7][16]=1; data.tiles[8][15]=1; data.tiles[8][16]=1;
        data.tiles[4][9]=1; data.tiles[4][10]=1; data.tiles[5][10]=1;
        data.tiles[6][9]=1; data.tiles[6][10]=1; data.tiles[5][9]=1;
        data.tiles[9][18] = 4;
        data.doorPos = {18*64+32.f, 9*64+32.f};
        data.doorType = 2;

        if (isFree(6,2)) data.enemySpawns.push_back({6*64+32, 2*64+32});
        if (isFree(13,2)) data.enemySpawns.push_back({13*64+32, 2*64+32});
        if (isFree(10,7)) data.enemySpawns.push_back({10*64+32, 7*64+32});
        if (isFree(3,5)) data.enemySpawns.push_back({3*64+32, 5*64+32});
        if (isFree(16,5)) data.enemySpawns.push_back({16*64+32, 5*64+32});
        if (isFree(7,3)) data.enemySpawns.push_back({7*64+32, 3*64+32});
        if (isFree(14,7)) data.enemySpawns.push_back({14*64+32, 7*64+32});

        data.weaponDrops.push_back({{18*64+32.f, 9*64+32.f}, 1});
    }

    else if (locationNumber == 6) {
        startPos = {640, 600};
        data.tiles.resize(11, std::vector<int>(20, 0));
        for (int x=0; x<20; ++x) { data.tiles[0][x]=1; data.tiles[10][x]=1; }
        for (int y=0; y<11; ++y) { data.tiles[y][0]=1; data.tiles[y][19]=1; }
        data.tiles[2][3] = 1; data.tiles[2][4] = 1;
        data.tiles[8][3] = 1; data.tiles[8][4] = 1;
        data.tiles[2][16] = 1; data.tiles[2][17] = 1;
        data.tiles[8][16] = 1; data.tiles[8][17] = 1;
    }
}

void Map::draw(sf::RenderWindow& win) {
    for (int y=0; y<11; ++y) {
        for (int x=0; x<20; ++x) {
            int tile = data.tiles[y][x];
            sf::RectangleShape* shape = &floorRect;
            if (tile == 1) shape = &wallRect;
            else if (tile == 3) shape = &doorRect;
            else if (tile == 4) shape = &bossDoorRect;
            shape->setPosition({x*64.f, y*64.f});
            win.draw(*shape);
        }
    }
    for (auto& drop : data.weaponDrops) {
        sf::RectangleShape* shape = (drop.second == 0) ? &weaponSMG : &weaponAR;
        shape->setPosition({drop.first.x - 16, drop.first.y - 16});
        win.draw(*shape);
    }
}

std::vector<sf::FloatRect> Map::getObstacles() const {
    std::vector<sf::FloatRect> obs;
    for (int y=0; y<11; ++y)
        for (int x=0; x<20; ++x)
            if (data.tiles[y][x] == 1)
                obs.emplace_back(sf::Vector2f(x*64, y*64), sf::Vector2f(64,64));
    return obs;
}

sf::Vector2f Map::getDoorPosition() const {
    return data.doorPos;
}

bool Map::isWallAt(int tileX, int tileY) const {
    if (tileY < 0 || tileY >= static_cast<int>(data.tiles.size())) return false;
    if (tileX < 0 || tileX >= static_cast<int>(data.tiles[0].size())) return false;
    return data.tiles[tileY][tileX] == 1;
}

void Map::removeWallAt(int tileX, int tileY) {
    if (tileY < 0 || tileY >= static_cast<int>(data.tiles.size())) return;
    if (tileX < 0 || tileX >= static_cast<int>(data.tiles[0].size())) return;
    if (data.tiles[tileY][tileX] == 1) data.tiles[tileY][tileX] = 0;
}

void Map::addWeaponDrop(sf::Vector2f pos, int type) {
    data.weaponDrops.push_back({pos, type});
}