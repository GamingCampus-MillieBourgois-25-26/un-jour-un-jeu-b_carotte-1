#pragma once

#include "Core/Component.h"
#include "Maths/Vector2.h"

class GameObject;

class ProjectileComponent : public Component
{
public:
    void Update(float _deltaTime) override;

    void Activate(GameObject* _target);

private:
    GameObject* target = nullptr;

    float speed = 500.f;
};