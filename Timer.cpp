// Engine\Core\Timer.cpp
#include "Timer.h"

Timer::Timer()
{
    m_fpsTimer = sf::seconds(1.0f);
}

float Timer::getDeltaTime()
{
    m_deltaTime = m_clock.restart().asSeconds();
    return m_deltaTime;
}

void Timer::updateFPS()
{
    m_frameCount++;
    if (m_fpsTimer <= sf::Time::Zero)
    {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer = sf::seconds(1.0f);
    }
    m_fpsTimer -= sf::seconds(m_deltaTime);   // u¿ywamy delta z Application
}

unsigned int Timer::getFPS() const
{
    return m_fps;
}