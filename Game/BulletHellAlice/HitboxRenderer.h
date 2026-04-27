#pragma once
#include "Core/Component.h"
#include <SFML/Graphics.hpp>
#include "Core/GameObject.h"
#include "Maths/Vector2.h"

namespace BulletHellAlice {
    class HitboxRenderer : public Component {
    public:
        static bool isVisible;

        Maths::Vector2f size = { 32.f, 32.f };
        sf::Color color = sf::Color::Green;

        void Render(sf::RenderWindow* _window) override {
            if (!isVisible) return;

            Maths::Vector2f scale = GetOwner()->GetScale();
            Maths::Vector2f pos = GetOwner()->GetPosition();

            float hbW = size.x * scale.x;
            float hbH = size.y * scale.y;

            float imgW = 0, imgH = 0;
            if (size.x == 24.f) { imgW = 112.f; imgH = 75.f; }  // Player
            else if (size.x == 10.f) { imgW = 13.f; imgH = 54.f; } // Bullet
            else { imgW = 103.f; imgH = 84.f; }                 // Boss/Sentry

            float offsetX = (imgW * scale.x - hbW) / 2.0f;
            float offsetY = (imgH * scale.y - hbH) / 2.0f;

            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(hbW, hbH));
            shape.setOutlineThickness(1.0f);
            shape.setOutlineColor(color);
            shape.setFillColor(sf::Color::Transparent);

            shape.setPosition(sf::Vector2f(pos.x + offsetX, pos.y + offsetY));

            _window->draw(shape);
        }
    };

    inline bool HitboxRenderer::isVisible = false;
}