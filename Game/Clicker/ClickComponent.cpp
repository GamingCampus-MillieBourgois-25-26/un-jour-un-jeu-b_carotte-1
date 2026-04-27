#include "ClickComponent.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "WindowModule.h"
#include "Engine.h"
#include "TextRenderer.h"
#include "Scene.h"
#include "GameManager.h"
#include "FloatingTextComponent.h"
#include <iostream>

#include <SFML/Window/Mouse.hpp>

void ClickComponent::Update(float deltaTime)
{
    std::cout << "CLICK VALUE: " << GameManager::Get().GetClickValue() << std::endl;
    // 🔥 créer le texte UNE FOIS
    if (!textCreated)
    {
        GameObject* textGO = GetOwner()->GetScene()->CreateGameObject("ScoreText");

        scoreText = textGO->CreateComponent<TextRenderer>("Score: 0");
        scoreText->SetColor(sf::Color::White);

        textGO->SetPosition({ 20.f, 20.f });

        textCreated = true;
    }

    auto* sprite = GetOwner()->GetComponent<SpriteRenderer>();
    if (!sprite) return;

    auto window = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<WindowModule>()
        ->GetWindow();

    auto mouse = sf::Mouse::getPosition(*window);

    auto pos = GetOwner()->GetPosition();
    auto size = sprite->GetSize();

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
                // 💰 points
                GameManager::Get().AddPoints(
                    GameManager::Get().GetClickValue()
                );

                // 🔄 update texte
                if (scoreText)
                {
                    scoreText->SetText(
                        "Score: " + std::to_string(GameManager::Get().GetPoints())
                    );
                }

                // 💥 floating text
                GameObject* text = GetOwner()->GetScene()->CreateGameObject("FloatingText");

                text->SetPosition(GetOwner()->GetPosition());

                auto* comp = text->CreateComponent<FloatingTextComponent>();
                comp->Init("+" + std::to_string(GameManager::Get().GetClickValue()));

                // 💥 zoom
                targetScale = 1.2f;
            }

            canClick = false;
        }
    }
    else
    {
        canClick = true;
    }

    // 🎯 animation scale
    float speed = 5.f;

    currentScale += (1.f - currentScale) * speed * deltaTime;

    if (targetScale > 1.f)
    {
        currentScale = targetScale;
        targetScale = 1.f;
    }

    GetOwner()->SetScale({ currentScale, currentScale });
}