// Engine\Core\Timer.h
#pragma once
#include <SFML/System.hpp>

class Timer
{
public:
    Timer();
    float getDeltaTime();
    void updateFPS();
    unsigned int getFPS() const;

private:
    sf::Clock m_clock;
    float m_deltaTime = 0.0f;
    sf::Time m_fpsTimer;
    unsigned int m_fps = 0;
    unsigned int m_frameCount = 0;
};