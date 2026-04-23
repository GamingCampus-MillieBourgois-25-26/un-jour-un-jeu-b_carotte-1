#pragma once

#include "Core/Component.h"
#include <vector>

class EnemyComponent;

class TowerComponent : public Component
{
public:
    TowerComponent() = default;

    void Update(float _deltaTime) override;

    void SetEnemies(const std::vector<GameObject*>* _enemies);

private:
    const std::vector<GameObject*>* enemies = nullptr;

    float range = 150.f;
    float fireRate = 1.f;
    float timer = 0.f;
};