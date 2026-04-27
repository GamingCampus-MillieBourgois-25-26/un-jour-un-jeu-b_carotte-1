#pragma once

#include "Core/Component.h"

class Scene;

class Enemy : public Component
{
public:
    void Update(const float deltaTime) override;
    void SetScene(Scene* sceneRef);

private:
    float shootTimer = 0.f;
    float shootInterval = 1.f;

    bool spawnNextFrame = false;

    Scene* scene = nullptr;
};