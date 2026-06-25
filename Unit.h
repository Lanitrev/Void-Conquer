// Game/Entities/Unit.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Game/Components/MovementComponent.h"
#include "Game/Components/HealthComponent.h"

class Unit
{
public:
    Unit(sf::Vector2f startPos = { 580.f, 300.f });

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool contains(sf::Vector2f point) const;
    void setSelected(bool selected);
    bool isSelected() const;
    void setTarget(sf::Vector2f pos);

    sf::RectangleShape m_shape;
    MovementComponent m_movement;
    HealthComponent m_health;
    bool m_selected = false;
    void avoidOtherUnits(const std::vector<Unit*>& allUnits, sf::Vector2f myPos, float dt);

    void select();
    void deselect();

    std::vector<sf::Vector2f> m_path;
    size_t m_currentWaypoint = 0;

private:
    sf::Vector2f m_targetPos;
    bool m_hasTarget = false;
    float m_speed = 150.f;

    void updateVisual();
};