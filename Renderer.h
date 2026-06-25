// Renderer/Renderer.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Game/Entities/Unit.h"

class Renderer
{
public:
    Renderer(sf::RenderWindow& window);
    void drawTestUnit();

    // === NOWE: zaznaczanie i ruch jednostki ===
    bool contains(sf::Vector2f point) const;
    void setSelected(bool selected);
    bool isSelected() const;
    void setTarget(sf::Vector2f pos);
    void update(float dt);   // ruch w kierunku celu
    void drawUnit(const Unit& unit);

private:
    sf::RenderWindow& m_window;
    sf::RectangleShape m_testUnit;

    bool m_selected = false;
    sf::Vector2f m_targetPos;
    bool m_hasTarget = false;
    float m_speed = 150.f;   // piksele na sekundę

    void updateVisual();
};