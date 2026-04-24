#pragma once

#include "Core/Component.h"
#include "Core/GameObject.h"
#include "Modules/InputModule.h"
#include "ClickerComponent.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class UpgradeComponent : public Component
{
public:
    UpgradeComponent() : m_target(nullptr) {}

    void SetTarget(ClickerComponent* _target) { m_target = _target; }

    void Update(float _delta_time) override
    {
        if (InputModule::GetMouseButtonDown(sf::Mouse::Button::Left))
        {
            if (CheckCollision()) BuyUpgrade();
        }
    }

    void Render(sf::RenderWindow* _window) override
    {
        if (!m_target) return;

        float posX = 1250.f;
        float posY = 150.f;
        float width = 220.f;
        float height = 80.f;

        sf::RectangleShape rect;
        rect.setSize({ width, height });
        rect.setPosition({ posX, posY });

        if (m_target->GetScore() >= m_target->GetUpgradeCost())
            rect.setFillColor(sf::Color(46, 204, 113, 220));
        else
            rect.setFillColor(sf::Color(149, 165, 166, 150));

        rect.setOutlineThickness(3.f);
        rect.setOutlineColor(sf::Color::White);
        _window->draw(rect);

        static sf::Font uiFont;
        static bool fontLoaded = false;
        if (!fontLoaded) {
            fontLoaded = uiFont.openFromFile("Assets\\pixel.ttf");
        }

        if (fontLoaded) {
            sf::Text txt(uiFont);

            txt.setString("CLIC POWER +1");
            txt.setCharacterSize(18);
            txt.setFillColor(sf::Color::White);
            txt.setPosition({ posX + 15.f, posY + 10.f });
            _window->draw(txt);

            txt.setString("PRIX: " + std::to_string(m_target->GetUpgradeCost()));
            txt.setCharacterSize(16);
            txt.setFillColor(sf::Color::Yellow);
            txt.setPosition({ posX + 15.f, posY + 45.f });
            _window->draw(txt);
        }
    }

private:
    ClickerComponent* m_target;

    bool CheckCollision() {
        Maths::Vector2i mouse = InputModule::GetMousePosition();
        return (mouse.x >= 1250 && mouse.x <= 1470 && mouse.y >= 150 && mouse.y <= 230);
    }

    void BuyUpgrade() {
        if (m_target && m_target->GetScore() >= m_target->GetUpgradeCost()) {
            m_target->SubtractScore(m_target->GetUpgradeCost());
            m_target->IncreasePower();
            m_target->MultiplyUpgradeCost(1.7f);
        }
    }
};