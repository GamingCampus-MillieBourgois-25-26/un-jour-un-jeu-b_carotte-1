#pragma once

#include "Core/Component.h"
#include "Maths/Vector2.h"
#include <vector>

class EnemyComponent : public Component
{
public:
    EnemyComponent() = default;

    void Update(float _deltaTime) override;

    void Activate(const std::vector<Maths::Vector2f>& _path);
    bool IsActive() const;

    void TakeDamage(int dmg);

private:
    std::vector<Maths::Vector2f> path;
    int currentIndex = 1;
    float speed = 100.f;
    int hp = 10;

    bool isActive = false;
};