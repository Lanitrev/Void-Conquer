// Game/Systems/MovementSystem.h
#pragma once
#include <vector>

class Unit;
class TileMap;

class MovementSystem
{
public:
    void update(std::vector<Unit*>& units, float dt, TileMap& tileMap);
};