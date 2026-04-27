#pragma once

#include "Core/Component.h"
#include "TextRenderer.h"

class FloatingTextComponent : public Component
{
public:
    void Start() override;
    void Update(float deltaTime) override;

    void Init(const std::string& text);

private:
    TextRenderer* textRenderer = nullptr;
    std::string displayText = "+1"; // 🔥 stocke le vrai texte

    float lifetime = 1.0f;
    float speed = 50.f;
};