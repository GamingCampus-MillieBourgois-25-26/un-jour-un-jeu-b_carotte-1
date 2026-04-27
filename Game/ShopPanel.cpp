#include "ShopPanel.h"
#include "GameObject.h"
#include "ShopItem.h"
#include "Scene.h"
#include "AnimalData.h"

void ShopPanel::Start()
{
    rectangle = GetOwner()->CreateComponent<RectangleShapeRenderer>();
    rectangle->SetSize({ 500.f, 800.f });
    rectangle->SetColor(sf::Color(40, 40, 40, 200));

    GetOwner()->SetPosition({ 1000.f, 0.f });

    std::vector<AnimalData> animals =
    {
        {"Chat", 10, 1, "chicken.png"},
        {"Chien", 50, 5, "pig.png"},
        {"Lion", 200, 20, "crocodile.png"}
    };

    float y = 150.f;

    for (auto& data : animals)
    {
        GameObject* item = GetOwner()->GetScene()->CreateGameObject("Item");

        // 👇 alignement gauche du panel
        item->SetPosition({ 1050.f, y });

        auto* comp = item->CreateComponent<ShopItem>();
        comp->Init(data);

        y += 120.f;
    }
}