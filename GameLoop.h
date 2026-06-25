// Engine/Core/GameLoop.h
#pragma once
#include "Timer.h"
#include "Logger.h"
#include <vector>
#include "Game/Entities/Unit.h"
#include "Game/Systems/MovementSystem.h"
#include "Game/Map/TileMap.h"
#include "Game/Map/Pathfinding.h"

class GameLoop
{
public:
    GameLoop();
    void run();

private:
    Timer m_timer;
    std::vector<Unit*> m_units;
    MovementSystem m_movementSystem;

    TileMap m_tileMap;
    Pathfinding m_pathfinder;

    bool m_isDragging = false;
    sf::Vector2f m_dragStart;
    sf::Vector2f m_dragEnd;

    void update(float dt);
    void render();
    void handleEvents();

    std::vector<sf::Vector2f> m_debrisPos;
    std::vector<sf::Vector2f> m_debrisVel;
};