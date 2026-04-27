#pragma once

#include "Core/Component.h"

class BulletHellPlayer : public Component
{
public:
    void Update(const float deltaTime) override;

private:
    float speed = 200.f;
};