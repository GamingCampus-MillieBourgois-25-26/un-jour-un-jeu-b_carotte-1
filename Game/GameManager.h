#pragma once

class GameManager
{
public:
    static GameManager& Get();

    void AddPoints(int amount);
    bool SpendPoints(int amount);

    int GetPoints() const;
    int GetClickValue() const;

    void AddClickBonus(int bonus);

private:
    int m_points = 0;
    int m_clickValue = 1;
};