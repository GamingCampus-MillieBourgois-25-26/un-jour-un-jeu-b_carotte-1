#pragma once
#include "Core/Component.h"
#include "Modules/InputModule.h"
#include "Core/GameObject.h"

namespace BulletHellAlice
{
    class PlayerController : public Component
    {
    public:
        float hitboxRadius = 4.0f; 
        bool isDead = false;
        float health = 100.f;
        float maxHealth = 100.f;

        float invulnTimer = 0.f;
        float invulnDuration = 1.5f;
        Maths::Vector2f GetHitboxCenter() {
            Maths::Vector2f pos = GetOwner()->GetPosition();
            return { pos.x + 16.f, pos.y + 16.f };
        }
        void Update(float _delta_time) override
        {
            if (invulnTimer > 0) {
                invulnTimer -= _delta_time;
            }
            Maths::Vector2f move = { 0.f, 0.f };
            float speed = 400.0f;

            if (InputModule::GetKey(sf::Keyboard::Key::Z) || InputModule::GetKey(sf::Keyboard::Key::Up))
                move.y -= 1.0f;
            if (InputModule::GetKey(sf::Keyboard::Key::S) || InputModule::GetKey(sf::Keyboard::Key::Down))
                move.y += 1.0f;
            if (InputModule::GetKey(sf::Keyboard::Key::Q) || InputModule::GetKey(sf::Keyboard::Key::Left))
                move.x -= 1.0f;
            if (InputModule::GetKey(sf::Keyboard::Key::D) || InputModule::GetKey(sf::Keyboard::Key::Right))
                move.x += 1.0f;

            if (move.x != 0 && move.y != 0) {
                move.x *= 0.707f;
                move.y *= 0.707f;
            }

            if (move.x != 0.f || move.y != 0.f)
            {
                Maths::Vector2f currentPos = GetOwner()->GetPosition();
                currentPos.x += move.x * speed * _delta_time;
                currentPos.y += move.y * speed * _delta_time;
                GetOwner()->SetPosition(currentPos);
            }
        }
    };
}