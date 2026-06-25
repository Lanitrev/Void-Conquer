// Engine/Core/GameLoop.cpp
#include "GameLoop.h"
#include <SFML/Graphics.hpp>
#include "Renderer/Renderer.h"
#include "Game/Systems/MovementSystem.h"
#include <algorithm>   // dla std::min / std::abs
#include <SFML/Window/Keyboard.hpp>  // na wszelki wypadek

extern sf::RenderWindow* g_window;
Renderer* g_renderer = nullptr;

GameLoop::GameLoop()
{
    Logger::init();
    Logger::log("GameLoop v0.0.8 - wiele jednostek + MovementSystem");
    Logger::log("[GameLoop] NavMesh + Theta* przygotowane");
    // Orbitujące odłamki (test dynamicznych przeszkód)
    m_debrisPos = { {300, 200}, {800, 400}, {1200, 150} };
    m_debrisVel = { {80, 40}, {-60, 90}, {45, -70} };
    Logger::log("[GameLoop] Dodano 3 orbitujące odłamki");
}

void GameLoop::run()
{
    Logger::log("Tworzenie jednostek testowych...");

    if (!g_renderer && g_window)
        g_renderer = new Renderer(*g_window);

    // === TWORZYMY 2 JEDNOSTKI ===
    Unit* unit1 = new Unit({ 400, 300 });
    Unit* unit2 = new Unit({ 900, 500 });
    m_units.push_back(unit1);
    m_units.push_back(unit2);

    Logger::log("Utworzono 2 jednostki testowe");

    while (g_window && g_window->isOpen())
    {
        float dt = m_timer.getDeltaTime();

        handleEvents();
        update(dt);
        render();

        m_timer.updateFPS();
        g_window->setTitle("Void Conquer v0.0.8 - FPS: " + std::to_string(m_timer.getFPS()));
    }

    Logger::log("Gra zamknięta poprawnie.");
}

void GameLoop::handleEvents()
{
    sf::Event event;
    while (g_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            g_window->close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            m_isDragging = true;
            m_dragStart = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            m_dragEnd = m_dragStart;

            // Usuwanie przeszkody + MOCNY reset wszystkich jednostek
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            int gx = static_cast<int>(mousePos.x / 32);
            int gy = static_cast<int>(mousePos.y / 32);

            m_tileMap.setObstacle(gx, gy, false);
            m_tileMap.setObstacle(gx + 1, gy, false);
            m_tileMap.setObstacle(gx - 1, gy, false);
            m_tileMap.setObstacle(gx, gy + 1, false);
            m_tileMap.setObstacle(gx, gy - 1, false);

            Logger::log("[GameLoop] Przeszkoda usunięta - SILNY reset wszystkich ścieżek");

            // Silny reset dla WSZYSTKICH jednostek
            for (auto& unit : m_units)
            {
                unit->m_path.clear();
                unit->m_currentWaypoint = 0;
                if (unit->m_movement.hasTarget)
                {
                    unit->m_movement.setTarget(unit->m_movement.targetPos); // wymusza nową ścieżkę
                    unit->m_path = m_pathfinder.findPath(m_tileMap, unit->m_shape.getPosition(), unit->m_movement.targetPos);
                    unit->m_currentWaypoint = 0;
                }
            }
        }

        if (event.type == sf::Event::MouseMoved && m_isDragging)
        {
            m_dragEnd = sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && m_isDragging)
        {
            m_isDragging = false;
            m_dragEnd = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            bool additive = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
            if (!additive)
                for (auto& u : m_units) u->deselect();

            sf::FloatRect rect(std::min(m_dragStart.x, m_dragEnd.x), std::min(m_dragStart.y, m_dragEnd.y),
                std::abs(m_dragEnd.x - m_dragStart.x), std::abs(m_dragEnd.y - m_dragStart.y));

            bool isClick = (std::abs(m_dragEnd.x - m_dragStart.x) < 8 && std::abs(m_dragEnd.y - m_dragStart.y) < 8);

            for (auto& unit : m_units)
            {
                if (isClick)
                {
                    if (unit->contains(m_dragEnd))
                    {
                        unit->select();
                        break;
                    }
                }
                else if (rect.intersects(unit->m_shape.getGlobalBounds()))
                {
                    unit->select();
                }
            }
        }

        // === PPM - rozkaz ruchu z pełną ścieżką ===
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            for (auto& unit : m_units)
            {
                if (unit->m_selected)
                {
                    unit->m_movement.setTarget(mousePos);
                    unit->m_path = m_pathfinder.findPath(m_tileMap, unit->m_shape.getPosition(), mousePos);
                    unit->m_currentWaypoint = 0;
                }
            }
        }

        // === Klawisz R - reset ===
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
        {
            Logger::log("[GameLoop] Reset wszystkich ścieżek (klawisz R)");
            for (auto& unit : m_units)
            {
                unit->m_path.clear();
                unit->m_currentWaypoint = 0;
                if (unit->m_selected && unit->m_movement.hasTarget)
                    unit->setTarget(unit->m_movement.targetPos);
            }
        }
    }
}

void GameLoop::update(float dt)
{
    // Aktualizacja orbitujących odłamków
    for (size_t i = 0; i < m_debrisPos.size(); i++)
    {
        m_debrisPos[i] += m_debrisVel[i] * dt * 0.8f;

        // Zawijanie po ekranie (kosmiczny efekt)
        if (m_debrisPos[i].x < 0) m_debrisPos[i].x = 1800;
        if (m_debrisPos[i].x > 1800) m_debrisPos[i].x = 0;
        if (m_debrisPos[i].y < 0) m_debrisPos[i].y = 1000;
        if (m_debrisPos[i].y > 1000) m_debrisPos[i].y = 0;
    }

    handleEvents();
    m_movementSystem.update(m_units, dt, m_tileMap);   // przekazuje aktualną mapę

    // Aktualizacja jednostek (jeśli potrzeba)
    for (auto& unit : m_units)
        unit->update(dt);

    m_timer.updateFPS();
    g_window->setTitle("Void Conquer v0.0.9 - FPS: " + std::to_string(m_timer.getFPS()));
}

void GameLoop::render()
{
    g_window->clear(sf::Color(0, 50, 20));

    // Rysuj siatkę z przeszkodami
    m_tileMap.draw(*g_window);

    // Rysuj orbitujące odłamki (czerwone kółka)
    for (auto& pos : m_debrisPos)
    {
        sf::CircleShape dot(12.f);
        dot.setPosition(pos);
        dot.setFillColor(sf::Color::Red);
        dot.setOrigin(12, 12);
        g_window->draw(dot);
    }

    // Rysuj jednostki + HP + tarcza
    for (auto& unit : m_units)
    {
        if (g_renderer)
            g_renderer->drawUnit(*unit);

        // Paski HP + tarcza (z poprzedniego kodu - zostawiamy)
        if (unit->m_health.maxHP > 0)
        {
            float hpPercent = unit->m_health.currentHP / unit->m_health.maxHP;
            float shieldPercent = unit->m_health.currentShield / unit->m_health.maxShield;

            sf::Vector2f pos = unit->m_shape.getPosition();

            // Tarcza (niebieska nad HP)
            sf::RectangleShape shieldBg({ 64.f, 6.f });
            shieldBg.setPosition(pos.x + 8.f, pos.y - 28.f);
            shieldBg.setFillColor(sf::Color(0, 100, 200, 100));
            g_window->draw(shieldBg);

            sf::RectangleShape shieldBar({ 64.f * shieldPercent, 6.f });
            shieldBar.setPosition(pos.x + 8.f, pos.y - 28.f);
            shieldBar.setFillColor(sf::Color(0, 180, 255));
            g_window->draw(shieldBar);

            // HP
            sf::RectangleShape hpBg({ 64.f, 6.f });
            hpBg.setPosition(pos.x + 8.f, pos.y - 18.f);
            hpBg.setFillColor(sf::Color(180, 0, 0));
            g_window->draw(hpBg);

            sf::RectangleShape hpBar({ 64.f * hpPercent, 6.f });
            hpBar.setPosition(pos.x + 8.f, pos.y - 18.f);
            hpBar.setFillColor(sf::Color(0, 220, 80));
            g_window->draw(hpBar);
        }
    }

    // Box select
    if (m_isDragging)
    {
        sf::RectangleShape selRect;
        selRect.setPosition(std::min(m_dragStart.x, m_dragEnd.x), std::min(m_dragStart.y, m_dragEnd.y));
        selRect.setSize(sf::Vector2f(std::abs(m_dragEnd.x - m_dragStart.x), std::abs(m_dragEnd.y - m_dragStart.y)));
        selRect.setFillColor(sf::Color(100, 200, 255, 40));
        selRect.setOutlineColor(sf::Color::Cyan);
        selRect.setOutlineThickness(2.5f);
        g_window->draw(selRect);
    }

    g_window->display();
}