#pragma once

#include "Core/Component.h"
#include "Maths/Vector2.h"
#include <vector>

class EnemyComponent : public Component
{
public:
    EnemyComponent() = default;

    void Update(float _deltaTime) override;

    void Activate(const std::vector<Maths::Vector2f>& _path, int wave);
    bool IsActive() const;

    void TakeDamage(int dmg);
    GameObject* hpBg = nullptr;
    GameObject* hpFill = nullptr;
    void SetHPBar(GameObject* bg, GameObject* fill);

private:
    std::vector<Maths::Vector2f> path;
    int currentIndex = 1;
    float speed = 100.f;
    int hp = 10;
    int maxHp = 100;

    bool isActive = false;
};