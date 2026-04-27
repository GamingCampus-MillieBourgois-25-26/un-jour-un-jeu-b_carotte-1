#include "GameManager.h"

GameManager& GameManager::Get()
{
    static GameManager instance;
    return instance;
}

void GameManager::AddPoints(int amount)
{
    m_points += amount;
}

bool GameManager::SpendPoints(int amount)
{
    if (m_points >= amount)
    {
        m_points -= amount;
        return true;
    }
    return false;
}

int GameManager::GetPoints() const
{
    return m_points;
}

int GameManager::GetClickValue() const
{
    return m_clickValue;
}

void GameManager::AddClickBonus(int bonus)
{
    m_clickValue += bonus;
}