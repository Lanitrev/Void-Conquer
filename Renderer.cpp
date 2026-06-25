// Renderer/Renderer.cpp
#include "Renderer.h"
#include "Game/Entities/Unit.h"
#include "../Engine/Core/Logger.h"
#include <cmath>
#include <string>

Renderer::Renderer(sf::RenderWindow& window) : m_window(window)
{
    m_testUnit.setSize(sf::Vector2f(80, 80));
    m_testUnit.setFillColor(sf::Color::Green);
    m_testUnit.setPosition(580, 300);
    m_testUnit.setOutlineThickness(0.f);
    Logger::log("[Renderer] Test unit (zielony kwadrat) utworzony pomyœlnie!");
}

void Renderer::drawTestUnit()
{
    m_window.draw(m_testUnit);
}

bool Renderer::contains(sf::Vector2f point) const
{
    return m_testUnit.getGlobalBounds().contains(point);
}

void Renderer::setSelected(bool selected)
{
    m_selected = selected;
    updateVisual();
    if (selected)
        Logger::log("[Renderer] Jednostka ZAZNACZONA (¿ó³te obramowanie)");
    else
        Logger::log("[Renderer] Jednostka odznaczona");
}

bool Renderer::isSelected() const
{
    return m_selected;
}

void Renderer::setTarget(sf::Vector2f pos)
{
    m_targetPos = pos;
    m_hasTarget = true;
    Logger::log("[Renderer] Rozkaz RUCHU do punktu: " + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y));
}

void Renderer::update(float dt)
{
    if (m_hasTarget)
    {
        sf::Vector2f current = m_testUnit.getPosition();
        sf::Vector2f dir = m_targetPos - current;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < 3.f)   // tolerancja – dotarliœmy
        {
            m_hasTarget = false;
            Logger::log("[Renderer] Jednostka DOTAR£A do celu");
        }
        else
        {
            dir.x /= dist;
            dir.y /= dist;

            float moveDist = m_speed * dt;
            if (moveDist > dist) moveDist = dist;

            m_testUnit.move(dir.x * moveDist, dir.y * moveDist);
        }
    }
}

void Renderer::updateVisual()
{
    if (m_selected)
    {
        m_testUnit.setOutlineThickness(5.f);
        m_testUnit.setOutlineColor(sf::Color::Yellow);
    }
    else
    {
        m_testUnit.setOutlineThickness(0.f);
    }
}
void Renderer::drawUnit(const Unit& unit)
{
    unit.draw(m_window);
}