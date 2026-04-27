#pragma once

#include "Core/Component.h"

class TextRenderer;

class ClickComponent : public Component
{
public:
    void Update(float deltaTime) override;

private:
    bool canClick = true;

    float currentScale = 1.f;
    float targetScale = 1.f;

    TextRenderer* scoreText = nullptr;
<<<<<<< Updated upstream
    bool textCreated = false;
=======
    bool textCreated = false; // 🔥 important
>>>>>>> Stashed changes
};