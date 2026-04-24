#pragma once

#include "Core/Component.h"

class TextRenderer;

class ClickComponent : public Component
{
public:
    void Start() override;     // 🔥 IMPORTANT
    void Update(float deltaTime) override;

private:
    int score = 0;
    bool canClick = true;

    float currentScale = 1.f;
    float targetScale = 1.f;

    TextRenderer* scoreText = nullptr; // 🔥 local au component
};