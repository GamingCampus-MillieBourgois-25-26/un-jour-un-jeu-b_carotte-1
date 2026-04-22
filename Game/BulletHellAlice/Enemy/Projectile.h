#pragma once
#include "Core/Component.h"
#include "Core/GameObject.h"
#include "Maths/Vector2.h"

namespace BulletHellAlice {
    class Projectile : public Component {
    public:
        Maths::Vector2f direction;
        float speed = 200.0f;
        bool isActive = false; 

        void Update(float _dt) override {
            if (!isActive) return;

            Maths::Vector2f pos = GetOwner()->GetPosition();
            pos.x += direction.x * speed * _dt;
            pos.y += direction.y * speed * _dt;
            GetOwner()->SetPosition(pos);

            if (pos.y > 950 || pos.y < -100 || pos.x < -100 || pos.x > 1900) {
                isActive = false;
                GetOwner()->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
            }
        }
    };
}