#include "InputComponent.h"
#include "GameObject.h"
#include "TDScene.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "SpriteRenderer.h"
#include "RectangleShapeRenderer.h"

void InputComponent::Update(float _deltaTime)
{
    TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

    if (!scene)
        return;

    // 💥 BLOQUE tout sauf menu
    if (scene->GetGameState() != TD::GameState::Playing)
    {
        // autoriser uniquement ENTER pour démarrer
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            scene->StartGame();
        }

        return; // 🔥 STOP TOUT LE RESTE
    }
    static bool canClick = true;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (canClick)
        {
            std::cout << "CLICK DETECTED" << std::endl;

            TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

            if (scene)
            {
                scene->TryPlaceTower();
            }

            canClick = false;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U))
    {
        TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

        if (scene && scene->GetSelectedTower())
        {
            scene->TryUpgradeTower(scene->GetSelectedTower());
        }
    }
    static bool canToggle = true;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab))
    {
        if (canToggle)
        {
            TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

            if (scene)
                scene->ToggleTowerUI();

            canToggle = false;
        }
    }
    else
    {
        canToggle = true;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        if(canClick)
        {
            TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

             if (!scene)
              return;

             sf::Vector2i mouse = sf::Mouse::getPosition();
    
             Maths::Vector2f pos(
               (float)mouse.x - 220.f,
               (float)mouse.y - 190.f
             );

            for (GameObject* tower : scene->GetTowers())
            {
                 if (!tower->IsEnabled())
                    continue;

               Maths::Vector2f tPos = tower->GetPosition();

                float size = 64.f;

                if (pos.x > tPos.x - size / 2 &&
                    pos.x < tPos.x + size / 2 &&
                    pos.y > tPos.y - size / 2 &&
                    pos.y < tPos.y + size / 2)
                {
                    std::cout << "TOWER CLICKED !" << std::endl;

                    scene->SetSelectedTower(tower);
                    break;
                }
            }
        }
        canClick = false;
    }
    else
    {
        canClick = true;
    }


    if (scene && scene->GetPreviewTower())
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition();

        Maths::Vector2f worldPos(
            (float)mousePos.x - 220.f,
            (float)mousePos.y - 190.f
        );

        auto* preview = scene->GetPreviewTower();
        auto* overlayGO = scene->GetPreviewOverlay();

        if (!preview || !overlayGO)
            return;

        // 🟢 position normale
        preview->SetPosition(worldPos);

        // 🔴 OFFSET ICI
        Maths::Vector2f offset(-14.f, -25.f); // ← change ça comme tu veux

        overlayGO->SetPosition(worldPos + offset);

        // 🎨 couleur
        auto* overlay = overlayGO->GetComponent<RectangleShapeRenderer>();

        if (!overlay)
            return;

        if (scene->GetMoney() < 20)
        {
            overlay->SetColor(sf::Color(255, 0, 0, 120));
        }
        else
        {
            overlay->SetColor(sf::Color(0, 255, 0, 120));
        }
        if (scene->IsOnPath(preview->GetPosition()))
        {
            overlay->SetColor(sf::Color(255, 255, 0, 120)); // 🟡 chemin
        }
        if (scene->IsOnPath(preview->GetPosition()) || scene->IsOnTower(preview->GetPosition()))
        {
            overlay->SetColor(sf::Color(255, 0, 0, 120)); // 🔴 interdit
        }
        else if (scene->GetMoney() < 20)
        {
            overlay->SetColor(sf::Color(255, 255, 0, 120)); // 🟡 pas assez argent
        }
        else
        {
            overlay->SetColor(sf::Color(0, 255, 0, 120)); // 🟢 ok
        }
    }

}