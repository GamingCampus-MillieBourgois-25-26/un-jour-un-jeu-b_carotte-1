#pragma once

#include "Core/Component.h"
#include "RectangleShapeRenderer.h"

class ShopPanel : public Component
{
public:
    void Start() override;

private:
    RectangleShapeRenderer* rectangle = nullptr;
};