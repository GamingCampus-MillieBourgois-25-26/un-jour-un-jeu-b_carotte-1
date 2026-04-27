#pragma once

#include "Core/Component.h"
#include "AnimalData.h"
#include "RectangleShapeRenderer.h"
#include "TextRenderer.h"

class SpriteRenderer;

class ShopItem : public Component
{
public:
    void Start() override;
    void Update(float deltaTime) override;

    void Init(const AnimalData& data);

private:
    AnimalData m_data;
    bool m_bought = false;

    SpriteRenderer* m_sprite = nullptr;
    TextRenderer* m_text = nullptr;

    bool canClick = true;
};