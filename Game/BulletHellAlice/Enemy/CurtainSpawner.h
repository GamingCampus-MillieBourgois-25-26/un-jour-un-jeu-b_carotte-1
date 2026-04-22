#pragma once
#include "Core/Component.h"
#include "Core/Scene.h"
#include "Projectile.h"

namespace BulletHellAlice {
    class CurtainSpawner : public Component {
    public:
        float timer = 0.0f;
        float spawnDelay = 0.2f; 

        void Update(float _dt) override {
            if (spawnDelay > 1000.0f) return;

            timer += _dt;
            if (timer >= spawnDelay) {
                SpawnRain();
                timer = 0.0f;
            }
        }

        void SpawnRain() {
            Scene* scene = GetOwner()->GetScene();
            if (!scene) return;

            const auto& allObjects = scene->GetGameObjects();
            for (auto& objPtr : allObjects) {
                GameObject* obj = objPtr.get();

                if (obj->GetName().find("RainBullet") != std::string::npos) {
                    auto* p = obj->GetComponent<Projectile>();
                    if (p && !p->isActive) {
                        float randomX = static_cast<float>(std::rand() % 1500);
                        obj->SetPosition(Maths::Vector2f(randomX, -20.0f));
                        p->direction = Maths::Vector2f(0.0f, 1.0f);
                        p->isActive = true;
                        return; 
                    }
                }
            }
        }
    };
}