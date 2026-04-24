#include "ClickComponent.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "WindowModule.h"
#include "Engine.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "TextRenderer.h"
#include "Scene.h"
TextRenderer* ClickComponent::scoreText = nullptr;

void ClickComponent::Update(float deltaTime)
{
    
    auto* sprite = GetOwner()->GetComponent<SpriteRenderer>();
    if (!sprite) return;

    auto window = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<WindowModule>()
        ->GetWindow();

    auto mouse = sf::Mouse::getPosition(*window);

    auto pos = GetOwner()->GetPosition();
    auto size = sprite->GetSize();

    // sprite centré
    pos.x -= size.x / 2;
    pos.y -= size.y / 2;

    // 🎯 CLICK
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (canClick)
        {
            if (mouse.x > pos.x &&
                mouse.x < pos.x + size.x &&
                mouse.y > pos.y &&
                mouse.y < pos.y + size.y)
            {
                score++;
                std::cout << "score\n";

                if (scoreText)
                {
                    scoreText->SetText("Score: " + std::to_string(score));
                }

                // 💥 effet zoom
                targetScale = 1.2f;
            }

            canClick = false;
        }
    }
    else
    {
        canClick = true;
    }

    // 🎯 RETOUR PROGRESSIF
    float speed = 5.f;

    // interpolation vers 1.0
    currentScale += (1.f - currentScale) * speed * deltaTime;

    // si clic → boost
    if (targetScale > 1.f)
    {
        currentScale = targetScale;
        targetScale = 1.f;
    }

    // appliquer scale
    GetOwner()->SetScale({ currentScale, currentScale });


}

void ClickComponent::Start()
{
    GameObject* textGO = GetOwner()->GetScene()->CreateGameObject("ScoreText");

    scoreText = textGO->CreateComponent<TextRenderer>("Score: 0");
    scoreText->SetColor(sf::Color::White);

    textGO->SetPosition({ 20.f, 20.f });
}

