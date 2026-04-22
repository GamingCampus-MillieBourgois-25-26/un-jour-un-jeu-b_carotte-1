#pragma once
#include "Core/Component.h"
#include "Core/GameObject.h"
#include "Enemy/FanShooter.h"
#include "Enemy/CurtainSpawner.h"
#include "Enemy/EasySentry.h"
#include "PlayerController.h"
#include "HitboxRenderer.h"
#include <vector>
#include <iostream>
#include <string>

namespace BulletHellAlice {
    class ProgressionManager : public Component {
    public:
        float totalTime = 0.0f;
        int currentPhase = -1;

        GameObject* player = nullptr;
        FanShooter* fan = nullptr;
        CurtainSpawner* curtain = nullptr;
        std::vector<EasySentry*> sentries;

        const float RATE_OFF = 9999.0f;
        const float SENTRY_RATE = 2.5f;
        const float FAN_RATE = 1.0f;
        const float RAIN_RATE = 0.15f;

        void Update(float _dt) override {
            totalTime += _dt;

            int targetPhase = 1;
            if (totalTime >= 120.0f)      targetPhase = 4; 
            else if (totalTime >= 60.0f)  targetPhase = 3; 
            else if (totalTime >= 30.0f)  targetPhase = 2; 
            else                          targetPhase = 1; 

            if (targetPhase != currentPhase) {
                ApplyPhaseSettings(targetPhase);
                currentPhase = targetPhase;
            }

            static bool hWasPressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
                if (!hWasPressed) {
                    HitboxRenderer::isVisible = !HitboxRenderer::isVisible;
                    std::cout << "DEBUG HITBOX : " << (HitboxRenderer::isVisible ? "ON" : "OFF") << std::endl;
                    hWasPressed = true;
                }
            }
            else {
                hWasPressed = false;
            }
            CheckCollisions();
        }

    private:
        void SetActorPresence(Component* comp, bool active, Maths::Vector2f activePos) {
            if (!comp || !comp->GetOwner()) return;
            if (active) {
                comp->GetOwner()->SetPosition(activePos);
            }
            else {
                comp->GetOwner()->SetPosition({ -2000.f, -2000.f });
            }
        }

        void ApplyPhaseSettings(int phase) {
            Maths::Vector2f bossPos = { 750.f, 400.f };
            Maths::Vector2f sPos[4] = { {50.f, 50.f}, {1350.f, 50.f}, {1350.f, 650.f}, {50.f, 650.f} };

            switch (phase) {
            case 1:
                if (curtain) curtain->spawnDelay = RATE_OFF;
                if (fan) { fan->fireRate = RATE_OFF; SetActorPresence(fan, false, bossPos); }
                for (int i = 0; i < (int)sentries.size(); ++i) {
                    SetActorPresence(sentries[i], true, sPos[i]);
                    if (sentries[i]) sentries[i]->fireRate = SENTRY_RATE;
                }
                break;

            case 2:
                if (curtain) curtain->spawnDelay = RATE_OFF;
                for (auto* s : sentries) { if (s) s->fireRate = RATE_OFF; SetActorPresence(s, false, { 0,0 }); }
                if (fan) { fan->fireRate = FAN_RATE; SetActorPresence(fan, true, bossPos); }
                break;

            case 3:
                if (fan) { fan->fireRate = RATE_OFF; SetActorPresence(fan, false, bossPos); }
                for (auto* s : sentries) { if (s) s->fireRate = RATE_OFF; SetActorPresence(s, false, { 0,0 }); }
                if (curtain) curtain->spawnDelay = RAIN_RATE;
                break;

            case 4:
                if (curtain) curtain->spawnDelay = RAIN_RATE * 0.8f;
                if (fan) { fan->fireRate = FAN_RATE * 0.8f; SetActorPresence(fan, true, bossPos); }
                for (int i = 0; i < (int)sentries.size(); ++i) {
                    SetActorPresence(sentries[i], true, sPos[i]);
                    if (sentries[i]) sentries[i]->fireRate = SENTRY_RATE * 0.7f;
                }
                break;
            }
        }

        void CheckCollisions() {
            if (!player) return;

            PlayerController* playerCtrl = player->GetComponent<PlayerController>();

            if (!playerCtrl || playerCtrl->isDead || playerCtrl->invulnTimer > 0.0f) return;

            float pScale = 0.5f;
            float pImgW = 112.f; float pImgH = 75.f;
            float pHbW = 95.f * pScale;
            float pHbH = 70.f * pScale;

            float pOffX = (pImgW * pScale - pHbW) / 2.f;
            float pOffY = (pImgH * pScale - pHbH) / 2.f;

            float pX = player->GetPosition().x + pOffX;
            float pY = player->GetPosition().y + pOffY;

            Scene* scene = GetOwner()->GetScene();
            if (!scene) return;

            const auto& allObjects = scene->GetGameObjects();
            for (auto& objPtr : allObjects) {
                GameObject* obj = objPtr.get();
                if (obj == player) continue;

                if (obj->GetName().find("Bullet") != std::string::npos) {
                    auto* proj = obj->GetComponent<Projectile>();
                    if (proj && proj->isActive) {
                        float bScale = 0.5f;
                        float bImgW = 13.f; float bImgH = 54.f;
                        float bHbW = 10.f * bScale; float bHbH = 40.f * bScale;

                        float bOffX = (bImgW * bScale - bHbW) / 2.f;
                        float bOffY = (bImgH * bScale - bHbH) / 2.f;

                        float bX = obj->GetPosition().x + bOffX;
                        float bY = obj->GetPosition().y + bOffY;

                        if ((pX < bX + bHbW) && (pX + pHbW > bX) && (pY < bY + bHbH) && (pY + pHbH > bY)) {
                            playerCtrl->health -= 10.f;
                            proj->isActive = false;
                            obj->SetPosition({ -1000.f, -1000.f });
                            std::cout << "Balle touchee ! Vie : " << playerCtrl->health << std::endl;
                        }
                    }
                }

                else if (obj->GetName() == "Boss" || obj->GetName().find("Sentry") != std::string::npos) {
                    float eX = obj->GetPosition().x;
                    float eY = obj->GetPosition().y;
                    float eW = 103.f;
                    float eH = 84.f;

                    if ((pX < eX + eW) && (pX + pHbW > eX) && (pY < eY + eH) && (pY + pHbH > eY)) {
                        playerCtrl->health -= 50.f;

                        playerCtrl->invulnTimer = playerCtrl->invulnDuration;

                        player->SetPosition({ player->GetPosition().x, player->GetPosition().y + 80.f });

                        std::cout << "Collision Vaisseau ! Alice est invulnerable." << std::endl;
                    }
                }
            }

            if (playerCtrl->health <= 0.f) {
                playerCtrl->health = 0.f;
                playerCtrl->isDead = true;
                std::cout << "CRITICAL ERROR : ALICE.EXE a cesse de fonctionner (Vie a 0)" << std::endl;

				exit(0);
            }
        }
    };
}