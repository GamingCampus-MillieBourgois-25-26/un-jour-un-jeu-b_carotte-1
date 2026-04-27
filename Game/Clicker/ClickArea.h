#pragma once

#include "Core/Component.h"
#include "RectangleShapeRenderer.h"

class ClickArea : public Component
{
public:
    void Start() override;
    void Update(float deltaTime) override;

private:
    RectangleShapeRenderer* rectangle = nullptr;
};