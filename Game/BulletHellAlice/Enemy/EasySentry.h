#pragma once
#include "Core/Component.h"
#include "Projectile.h"
#include "Core/Scene.h"

namespace BulletHellAlice {
    class EasySentry : public Component {
    public:
        float timer = 0.0f;
        float fireRate = 2.5f;
        Maths::Vector2f targetPos = { 400.f, 300.f };

        void Update(float _dt) override {
            timer += _dt;
            if (timer >= fireRate) {
                ShootAtPlayer();
                timer = 0.0f;
            }
        }

        void ShootAtPlayer() {
            Scene* scene = GetOwner()->GetScene();
            Maths::Vector2f myPos = GetOwner()->GetPosition();

            GameObject* player = nullptr;
            for (auto& objPtr : scene->GetGameObjects()) {
                if (objPtr->GetName() == "Player") {
                    player = objPtr.get();
                    break;
                }
            }

            if (!player) return; 

            Maths::Vector2f playerPos = player->GetPosition();
            Maths::Vector2f dir = { playerPos.x - myPos.x, playerPos.y - myPos.y };

            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length != 0) { dir.x /= length; dir.y /= length; }

            for (auto& objPtr : scene->GetGameObjects()) {
                GameObject* obj = objPtr.get();
                if (obj->GetName().find("SentryBullet") != std::string::npos) {
                    auto* p = obj->GetComponent<Projectile>();
                    if (p && !p->isActive) {
                        obj->SetPosition(myPos);
                        p->direction = dir;
                        p->speed = 150.0f;
                        p->isActive = true;
                        break;
                    }
                }
            }
        }
    };
}