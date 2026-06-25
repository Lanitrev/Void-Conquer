// Game/Components/HealthComponent.h
#pragma once

struct HealthComponent
{
    float currentHP = 100.f;
    float maxHP = 100.f;
    bool isAlive = true;

    // === TARCZA / POLE SI£OWE ===
    float currentShield = 0.f;
    float maxShield = 0.f;

    void takeDamage(float damage)
    {
        // Najpierw tarcza
        if (currentShield > 0.f)
        {
            if (damage <= currentShield)
            {
                currentShield -= damage;
                return;
            }
            else
            {
                damage -= currentShield;
                currentShield = 0.f;
            }
        }
        currentHP -= damage;
        if (currentHP <= 0.f)
        {
            currentHP = 0.f;
            isAlive = false;
        }
    }

    void heal(float amount)
    {
        currentHP += amount;
        if (currentHP > maxHP) currentHP = maxHP;
    }

    void rechargeShield(float amount)
    {
        currentShield += amount;
        if (currentShield > maxShield) currentShield = maxShield;
    }
};