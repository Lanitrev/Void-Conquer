#pragma once
#include <SFML/System/Vector2.hpp>

class MovementComponent
{
public:
    MovementComponent();

    void setTarget(sf::Vector2f pos);

    float maxSpeed = 320.f;
    float acceleration = 480.f;
    float turnRate = 6.5f;

    sf::Vector2f velocity = { 0.f, 0.f };
    sf::Vector2f targetPos;
    bool hasTarget = false;
};