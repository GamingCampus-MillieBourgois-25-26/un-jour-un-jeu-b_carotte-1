#pragma once
#include "Core/Component.h"
#include "Core/Scene.h"
#include "Projectile.h"
#include <cmath>
#include <string>
#include <vector>

namespace BulletHellAlice {
    class FanShooter : public Component {
    public:
        float shootTimer = 0.0f;
        float fireRate = 1.0f;
        int bulletCount = 24;

        float movementTimer = 0.0f;
        float movementSpeed = 0.5f;
        float circleRadius = 300.0f;
        Maths::Vector2f screenCenter = { 750.f, 400.f };

        std::vector<Projectile*> bulletCache;
        bool cacheInitialized = false;

        void Update(float _dt) override {
            if (fireRate > 1000.0f) return;

            movementTimer += _dt * movementSpeed;
            float posX = screenCenter.x + std::cos(movementTimer) * circleRadius;
            float posY = screenCenter.y + std::sin(movementTimer) * circleRadius;

            GetOwner()->SetPosition({ posX, posY });

            shootTimer += _dt;
            if (shootTimer >= fireRate) {
                ShootCircle({ posX, posY });
                shootTimer = 0.0f;
            }
        }

        void ShootCircle(Maths::Vector2f currentPos) {
            if (!cacheInitialized) FillBulletCache();

            Maths::Vector2f bossCenter = { currentPos.x + 51.5f, currentPos.y + 42.0f };

            int bulletsActivated = 0;
            for (auto* p : bulletCache) {
                if (p && !p->isActive) {
                    float angle = (bulletsActivated * 2.0f * 3.14159f) / bulletCount;

                    p->GetOwner()->SetPosition(bossCenter);

                    p->direction = Maths::Vector2f(std::cos(angle), std::sin(angle));
                    p->speed = 200.0f;
                    p->isActive = true;

                    bulletsActivated++;
                    if (bulletsActivated >= bulletCount) break;
                }
            }
        }

    private:
        void FillBulletCache() {
            Scene* scene = GetOwner()->GetScene();
            if (!scene) return;
            bulletCache.clear();
            const auto& allObjects = scene->GetGameObjects();
            for (auto& objPtr : allObjects) {
                if (objPtr->GetName().find("FanBullet") != std::string::npos) {
                    auto* p = objPtr->GetComponent<Projectile>();
                    if (p) bulletCache.push_back(p);
                }
            }
            cacheInitialized = true;
        }
    };
}