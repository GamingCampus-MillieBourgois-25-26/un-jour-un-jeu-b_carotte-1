#include "ClickArea.h"
#include "InputModule.h"

void ClickArea::Start()
{
    rectangle = GetOwner()->CreateComponent<RectangleShapeRenderer>();
    rectangle->SetSize({ 700.f, 600.f });
    rectangle->SetColor(sf::Color::Blue);

    GetOwner()->SetPosition({ 0.f, 0.f });
}

void ClickArea::Update(float deltaTime)
{
    if (InputModule::GetMouseButtonDown(sf::Mouse::Button::Left))
    {
        auto mouse = InputModule::GetMousePosition();

        if (mouse.x < 700)
        {
            // 👉 ici on ajoutera les points plus tard
        }
    }
}