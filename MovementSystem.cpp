// Game/Systems/MovementSystem.cpp
#include "MovementSystem.h"
#include "Game/Map/TileMap.h"
#include "Game/Entities/Unit.h"
#include <cmath>
#include "../../Engine/Core/Logger.h"

void MovementSystem::update(std::vector<Unit*>& units, float dt, TileMap& tileMap)
{
    for (auto& unit : units)
    {
        if (!unit->m_health.isAlive) continue;
        auto& move = unit->m_movement;
        // Smoothing + ORCA-lite (płynne omijanie odłamków)
        move.velocity *= 0.88f;  // damping – brak szarpania
        if (!unit->m_path.empty() && unit->m_currentWaypoint < unit->m_path.size())
        {
            sf::Vector2f target = unit->m_path[unit->m_currentWaypoint];
            if (unit->m_currentWaypoint + 1 < unit->m_path.size())
                target = unit->m_path[unit->m_currentWaypoint + 1]; // lookahead

            sf::Vector2f current = unit->m_shape.getPosition();
            sf::Vector2f dir = target - current;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist < 40.f) // luźne przejście do następnego waypointu
            {
                unit->m_currentWaypoint++;
                if (unit->m_currentWaypoint >= unit->m_path.size())
                {
                    move.hasTarget = false;
                    unit->m_path.clear();
                    unit->m_currentWaypoint = 0;
                    move.velocity *= 0.4f;
                    Logger::log("[MovementSystem] ✅ Płynnie zakończyła ścieżkę");
                }
            }
            else
            {
                dir /= dist;
                move.velocity += dir * move.acceleration * dt * 1.8f; // szybsze i płynniejsze

                move.velocity *= 0.89f; // damping

                float speedSq = move.velocity.x * move.velocity.x + move.velocity.y * move.velocity.y;
                if (speedSq > move.maxSpeed * move.maxSpeed)
                {
                    float speed = std::sqrt(speedSq);
                    move.velocity /= speed;
                    move.velocity *= move.maxSpeed;
                }

                // Unikanie jednostek + przeszkód
                for (auto& other : units)
                {
                    if (other == unit) continue;
                    sf::Vector2f diff = current - other->m_shape.getPosition();
                    float dSq = diff.x * diff.x + diff.y * diff.y;
                    if (dSq > 0 && dSq < 2500.f)
                    {
                        diff /= std::sqrt(dSq);
                        move.velocity += diff * 220.f * dt;
                    }
                }
                // Silne unikanie + skręt w bok (zamiast zatrzymywania się)
                for (int i = 1; i <= 3; i++)
                {
                    int checkX = static_cast<int>((current.x + move.velocity.x * dt * i * 3) / 32);
                    int checkY = static_cast<int>((current.y + move.velocity.y * dt * i * 3) / 32);
                    if (!tileMap.isWalkable(checkX, checkY))
                    {
                        // Zamiast odbicia - skręt w lewo/prawo
                        move.velocity = sf::Vector2f(-move.velocity.y * 1.2f, move.velocity.x * 1.2f); // skręt 90 stopni
                        Logger::log("[MovementSystem] Skręt w bok od przeszkody");
                        break;
                    }
                }
                int checkX = static_cast<int>((current.x + move.velocity.x * dt * 2) / 32);
                int checkY = static_cast<int>((current.y + move.velocity.y * dt * 2) / 32);
                if (!tileMap.isWalkable(checkX, checkY))
                {
                    move.velocity *= -0.3f;
                }

                unit->m_shape.move(move.velocity * dt);
            }
        }
        else if (move.hasTarget)
        {
            // fallback (bardzo rzadko używany)
            sf::Vector2f current = unit->m_shape.getPosition();
            sf::Vector2f dir = move.targetPos - current;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (dist < 12.f)
            {
                move.hasTarget = false;
                move.velocity = { 0.f, 0.f };
            }
            else
            {
                dir /= dist;
                move.velocity += dir * move.acceleration * dt * 0.8f;
                move.velocity *= 0.94f;
                unit->m_shape.move(move.velocity * dt);
            }
        }
    }
}