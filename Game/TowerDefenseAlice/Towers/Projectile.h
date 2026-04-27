#pragma once
#include "Core/Component.h"
#include "Core/GameObject.h"
#include "Maths/Vector2.h"
#include "../Health/Health.h"
#include <cmath>

namespace TowerDefenseAlice
{
    class Projectile : public Component
    {
    public:
        float speed = 500.0f;
        int damage = 10;
        GameObject* target = nullptr;

        void Update(float _delta_time)
        {
            if (target == nullptr || !target->IsEnabled())
            {
                ResetProjectile();
                return;
            }

            Maths::Vector2f currentPos = GetOwner()->GetPosition();
            Maths::Vector2f enemyCenterOffset = { 32.0f, 32.0f };
            Maths::Vector2f targetPos = target->GetPosition() + enemyCenterOffset;

            Maths::Vector2f diff = targetPos - currentPos;
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            if (distance < 10.0f || distance < speed * _delta_time)
            {
                OnHit();
            }
            else
            {
                Maths::Vector2f direction = { diff.x / distance, diff.y / distance };
                GetOwner()->SetPosition(currentPos + direction * speed * _delta_time);
            }
        }

    private:
        void OnHit()
        {
            if (target)
            {
                Health* health = target->GetComponent<Health>();
                if (health)
                {
                    health->TakeDamage(damage);
                }
            }
            ResetProjectile();
        }

        void ResetProjectile()
        {
            GetOwner()->SetPosition({ -1000.f, -1000.f });
            target = nullptr;          
            GetOwner()->Disable();     
        }
    };
}