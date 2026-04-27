#include "ShopItem.h"

#include "SpriteRenderer.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Engine.h"
#include "WindowModule.h"
#include "Scene.h"
#include "AssetsModule.h"
#include "Texture.h"
#include "TextRenderer.h"
#include <iostream>
#include <SFML/Window/Mouse.hpp>

void ShopItem::Init(const AnimalData& data)
{
    m_data = data;
}

void ShopItem::Start()
{
    auto* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    // 🐾 SPRITE (gauche)
    m_sprite = GetOwner()->CreateComponent<SpriteRenderer>(
        assets->LoadAsset<Texture>(m_data.spritePath)
    );

    GetOwner()->SetScale({ 0.5f, 0.5f }); // taille propre

    // 📝 TEXTE (droite)
    GameObject* textGO = GetOwner()->GetScene()->CreateGameObject("ItemText");

    // position à droite du sprite
    textGO->SetPosition(GetOwner()->GetPosition() + Maths::Vector2f(80.f, -10.f));

    m_text = textGO->CreateComponent<TextRenderer>(
        "Cost: " + std::to_string(m_data.price) +
        " (+" + std::to_string(m_data.clickBonus) + ")"
    );

    m_text->SetColor(sf::Color::White);
}

void ShopItem::Update(float deltaTime)
{
    if (m_bought) return;

    auto window = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<WindowModule>()
        ->GetWindow();

    auto mouse = sf::Mouse::getPosition(*window);

    auto pos = GetOwner()->GetPosition();
    auto size = m_sprite->GetSize();

    pos.x -= size.x / 2;
    pos.y -= size.y / 2;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (canClick)
        {
            if (mouse.x > pos.x &&
                mouse.x < pos.x + size.x &&
                mouse.y > pos.y &&
                mouse.y < pos.y + size.y)
            {
                // 💰 ACHAT
                if (GameManager::Get().SpendPoints(m_data.price))
                {
                    m_bought = true;

                    // 🔥 BONUS
                    GameManager::Get().AddClickBonus(m_data.clickBonus);

                    // 🎨 visuel acheté
                    GetOwner()->SetScale({ 0.4f, 0.4f });
                    if (m_text)
                        m_text->SetColor(sf::Color(150, 150, 150));
                }
            }

            canClick = false;
        }
    }
    else
    {
        canClick = true;
    }
}