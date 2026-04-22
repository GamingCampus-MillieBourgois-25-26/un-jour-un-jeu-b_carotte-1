#pragma once
#include "Core/Component.h"
#include <SFML/Graphics.hpp>
#include "PlayerController.h"

namespace BulletHellAlice {
    class HealthBarRenderer : public Component {
    public:
        void Render(sf::RenderWindow* _window) override {
            PlayerController* ctrl = GetOwner()->GetComponent<PlayerController>();
            if (!ctrl || ctrl->isDead) return;

            Maths::Vector2f pos = GetOwner()->GetPosition();
            float ratio = ctrl->health / ctrl->maxHealth;

            sf::RectangleShape background({ 50.f, 5.f });
            background.setFillColor(sf::Color::Red);
            background.setPosition({ pos.x, pos.y + 45.f }); 

            sf::RectangleShape foreground({ 50.f * ratio, 5.f });
            foreground.setFillColor(sf::Color::Green);
            foreground.setPosition({ pos.x, pos.y + 45.f });

            _window->draw(background);
            _window->draw(foreground);
        }
    };
}