#include "MovementComponent.h"

MovementComponent::MovementComponent() = default;

void MovementComponent::setTarget(sf::Vector2f pos)
{
    targetPos = pos;
    hasTarget = true;
}