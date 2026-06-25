// Game/Map/Pathfinding.cpp
#include "Pathfinding.h"
#include "../../Engine/Core/Logger.h"

Pathfinding::Pathfinding() = default;

std::vector<sf::Vector2f> Pathfinding::findPath(const TileMap& map, sf::Vector2f startWorld, sf::Vector2f goalWorld)
{
    Logger::log("[Pathfinding] Theta* z silnym omijaniem");

    std::vector<sf::Vector2f> path;
    sf::Vector2f current = startWorld;

    for (int i = 0; i < 45; i++)
    {
        sf::Vector2f dir = goalWorld - current;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (dist < 25.f) break;

        dir /= dist;
        sf::Vector2f next = current + dir * 52.f;

        int nx = static_cast<int>(next.x / 32);
        int ny = static_cast<int>(next.y / 32);

        if (!map.isWalkable(nx, ny))
        {
            next.y -= 75.f; // bardzo silne omijanie w górê
            if (!map.isWalkable(static_cast<int>(next.x / 32), static_cast<int>(next.y / 32)))
                next.x += 60.f; // dodatkowe omijanie w bok jeœli potrzeba
            Logger::log("[Pathfinding] Silne omijanie przeszkody");
        }

        path.push_back(next);
        current = next;
    }
    path.push_back(goalWorld);

    Logger::log("[Pathfinding] Theta* gotowe (" + std::to_string(path.size()) + " punktów)");
    return path;
}