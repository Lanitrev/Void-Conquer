// Engine\Core\Application.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GameLoop.h"

class Application
{
public:
    Application();
    void run();

private:
    sf::RenderWindow m_window;
    GameLoop m_gameLoop;

    void handleEvents();
    // void update(float dt);     // stary kod zostawiamy zakomentowany na razie
    // void render();             // stary kod zostawiamy zakomentowany na razie
};