#pragma once

#include "Core/Component.h"
#include <vector>

class GameObject;
namespace TD { class TDScene; } 

class TowerComponent : public Component
{
public:
    TowerComponent() = default;

    void Update(float _deltaTime) override;

    void SetEnemies(const std::vector<GameObject*>* _enemies);
    void SetScene(TD::TDScene* _scene);
    void Upgrade();
    int upgradeCost = 50;
    int GetLevel() const { return level; }
    float GetRange() const { return range; }
    float GetFireRate() const { return fireRate; }


private:
    const std::vector<GameObject*>* enemies = nullptr;
    TD::TDScene* scene = nullptr; 

    float range = 300.f;
    float fireRate = 1.f;
    float timer = 0.f;

    int level = 1;

};