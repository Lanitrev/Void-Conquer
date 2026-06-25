// Game/Entities/Unit.cpp
#include "Unit.h"
#include "Game/Map/Pathfinding.h"
#include "Game/Map/TileMap.h"
#include "../../Engine/Core/Logger.h"

Unit::Unit(sf::Vector2f startPos)
{
    m_shape.setSize(sf::Vector2f(80, 80));
    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition(startPos);
    m_shape.setOutlineThickness(0.f);

    m_health.currentHP = 100.f;
    m_health.maxHP = 100.f;
    m_health.isAlive = true;

    m_health.currentShield = 50.f;
    m_health.maxShield = 50.f;

    m_path.clear();
    m_currentWaypoint = 0;

    Logger::log("[Unit] Testowa jednostka utworzona");
}

void Unit::setTarget(sf::Vector2f pos)
{
    static TileMap tileMap;
    static Pathfinding pathfinder;

    m_movement.setTarget(pos);
    m_path = pathfinder.findPath(tileMap, m_shape.getPosition(), pos);
    m_currentWaypoint = 0;

    Logger::log("[Unit] Rozkaz RUCHU do: " + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + " | Ścieżka " + std::to_string(m_path.size()) + " pkt");
}

void Unit::update(float dt)
{
    // Ruch obsługuje MovementSystem
}

void Unit::select()
{
    m_selected = true;
    updateVisual();
}

void Unit::deselect()
{
    m_selected = false;
    updateVisual();
}

void Unit::updateVisual()
{
    if (m_selected)
        m_shape.setOutlineColor(sf::Color::Yellow);
    else
        m_shape.setOutlineColor(sf::Color::Transparent);
    m_shape.setOutlineThickness(m_selected ? 4.f : 0.f);
}

bool Unit::contains(sf::Vector2f point) const
{
    return m_shape.getGlobalBounds().contains(point);
}

void Unit::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}