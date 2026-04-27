#pragma once

#include "Core/Component.h"
#include "Maths/Vector2.h"

class Bullet : public Component
{
public:
    void Update(const float deltaTime) override;
    void SetDirection(const Maths::Vector2f& dir);

private:
    Maths::Vector2f direction = { 0.f, 1.f };
    float speed = 300.f;
};