// Game/Map/Pathfinding.h
#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "TileMap.h"

class Pathfinding
{
public:
    Pathfinding();

    std::vector<sf::Vector2f> findPath(const TileMap& map, sf::Vector2f startWorld, sf::Vector2f goalWorld);

private:
    int heuristic(int x1, int y1, int x2, int y2) const;
};