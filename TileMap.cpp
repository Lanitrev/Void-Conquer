// Game/Map/TileMap.cpp
#include "TileMap.h"
#include "../../Engine/Core/Logger.h"

TileMap::TileMap(int width, int height) : m_width(width), m_height(height)
{
    m_obstacles.resize(height, std::vector<bool>(width, false));

    // Testowe przeszkody
    for (int x = 15; x < 35; x++) m_obstacles[12][x] = true;
    for (int y = 8; y < 18; y++) m_obstacles[y][42] = true;

    m_tileShape.setSize({ 32.f, 32.f });
    m_tileShape.setFillColor(sf::Color(80, 80, 80));
    Logger::log("[TileMap] Grid 60x35 utworzony z przeszkodami");
}

bool TileMap::isWalkable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) return false;
    return !m_obstacles[y][x];
}

void TileMap::setObstacle(int x, int y, bool obstacle)
{
    if (x >= 0 && y >= 0 && x < m_width && y < m_height)
        m_obstacles[y][x] = obstacle;

    Logger::log("[TileMap] Przeszkoda zmieniona na (" + std::to_string(x) + ", " + std::to_string(y) + ")");
}

void TileMap::draw(sf::RenderWindow& window) const
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            if (m_obstacles[y][x])
            {
                m_tileShape.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
                window.draw(m_tileShape);
            }
        }
    }
}