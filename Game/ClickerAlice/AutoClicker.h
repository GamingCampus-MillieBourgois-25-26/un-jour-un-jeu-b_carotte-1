#pragma once

#include "Core/Component.h"
#include "Modules/InputModule.h"
#include "ClickerComponent.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class AutoClickerComponent : public Component
{
public:
    AutoClickerComponent() : m_target(nullptr), m_timer(0.f), m_level(0), m_cost(50) {}

    void SetTarget(ClickerComponent* _target) { m_target = _target; }

    void Update(float _delta_time) override
    {
        if (m_level > 0 && m_target)
        {
            m_timer += _delta_time;
            if (m_timer >= 1.0f)
            {
                m_target->AddScore(m_level); 
                m_timer = 0.f;
            }
        }

        if (InputModule::GetMouseButtonDown(sf::Mouse::Button::Left))
        {
            if (CheckCollision()) BuyAutoClicker();
        }
    }

    void Render(sf::RenderWindow* _window) override
    {
        if (!m_target) return;

        float posX = 1250.f;
        float posY = 250.f;
        float width = 220.f;
        float height = 80.f;

        sf::RectangleShape rect;
        rect.setSize({ width, height });
        rect.setPosition({ posX, posY });

        if (m_target->GetScore() >= m_cost)
            rect.setFillColor(sf::Color(52, 152, 219, 220));
        else
            rect.setFillColor(sf::Color(149, 165, 166, 150));

        rect.setOutlineThickness(3.f);
        rect.setOutlineColor(sf::Color::White);
        _window->draw(rect);

        static sf::Font uiFont;
        static bool fontLoaded = uiFont.openFromFile("Assets\\pixel.ttf");

        if (fontLoaded) {
            sf::Text txt(uiFont);
            txt.setCharacterSize(16);
            txt.setPosition({ posX + 15.f, posY + 10.f });
            txt.setString("AUTO-CLIC (Lvl " + std::to_string(m_level) + ")");
            _window->draw(txt);

            txt.setString("PRIX: " + std::to_string(m_cost));
            txt.setCharacterSize(14);
            txt.setFillColor(sf::Color::Yellow);
            txt.setPosition({ posX + 15.f, posY + 45.f });
            _window->draw(txt);
        }
    }

private:
    ClickerComponent* m_target;
    float m_timer;
    int m_level;
    int m_cost;

    bool CheckCollision() {
        Maths::Vector2i mouse = InputModule::GetMousePosition();
        return (mouse.x >= 1250 && mouse.x <= 1470 && mouse.y >= 250 && mouse.y <= 330);
    }

    void BuyAutoClicker() {
        if (m_target && m_target->GetScore() >= m_cost) {
            m_target->SubtractScore(m_cost);
            m_level++;
            m_cost = (int)(m_cost * 1.8f);
        }
    }
};