// Game/Map/TileMap.h
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class TileMap
{
public:
    TileMap(int width = 60, int height = 35);

    bool isWalkable(int x, int y) const;
    void setObstacle(int x, int y, bool obstacle);
    void draw(sf::RenderWindow& window) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    int m_width, m_height;
    std::vector<std::vector<bool>> m_obstacles;
    mutable sf::RectangleShape m_tileShape;   // mutable dla const draw
};