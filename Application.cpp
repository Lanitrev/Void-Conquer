// Engine\Core\Application.cpp
#include "Application.h"

// Global pointer do okna
sf::RenderWindow* g_window = nullptr;

// Dodajemy globalne wywo³anie (tymczasowe po³¹czenie)
void ApplicationHandleEvents() { /* puste - handleEvents jest teraz w GameLoop */ }

Application::Application()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    m_window.create(desktopMode, "Void Conquer v0.0.6 - RTS in Space", sf::Style::Fullscreen);
    m_window.setFramerateLimit(60);
    g_window = &m_window;

    Logger::init();
    Logger::log("Application - okno utworzone w natywnej rozdzielczoœci pulpitu (Fullscreen)");
}

void Application::run()
{
    Logger::log("Starting GameLoop with event fix...");
    m_gameLoop.run();   // g³ówna pêtla
}