#pragma once
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "../PlayerController.h"
#include "Components/SpriteRenderer.h"
#include "../HitboxRenderer.h" 
#include "../Enemy/FanShooter.h"
#include "../Enemy/CurtainSpawner.h"
#include "../Enemy/EasySentry.h"
#include "../ProgressionManager.h"
#include "../HealthBarRenderer.h"
#include "Modules/AssetsModule.h"
#include "Assets/Texture.h"
#include "Engine.h"
#include <iostream>
#include <string>
#include <vector>

namespace BulletHellAlice
{
    class GameScene : public Scene
    {
    private:
        bool isInitialized = false;
        GameObject* playerRef = nullptr;

    public:
        GameScene() : Scene("BulletHellAlice_Main", true)
        {
            if (isInitialized) return;
            InitBackground();
            InitPlayer();

            InitProjectilePool(200, "RainBullet", "bullet_red.png");
            InitProjectilePool(800, "FanBullet", "bullet_blue.png");
            InitProjectilePool(100, "SentryBullet", "bullet_green.png");

            InitSpawners();
            isInitialized = true;
        }

    private:
        void InitBackground() {
            AssetsModule* am = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();
            Texture* tex = (am) ? am->LoadAsset<Texture>("background.png") : nullptr;
            if (!tex) return;

            for (int x = 0; x < 7; ++x) {
                for (int y = 0; y < 4; ++y) {
                    GameObject* bgPart = CreateGameObject("BG_" + std::to_string(x) + "_" + std::to_string(y));
                    bgPart->SetPosition({ x * 256.f, y * 256.f });
                    bgPart->CreateComponent<SpriteRenderer>(tex);
                    bgPart->Awake(); bgPart->Start();
                }
            }
        }

        void InitPlayer() {
            playerRef = CreateGameObject("Player");
            playerRef->SetPosition({ 750.f, 600.f });
            playerRef->SetScale({ 0.5f, 0.5f });
            playerRef->CreateComponent<HealthBarRenderer>();

            auto* ctrl = playerRef->CreateComponent<PlayerController>();
            ctrl->hitboxRadius = 3.0f;

            auto* debug = playerRef->CreateComponent<HitboxRenderer>();
            debug->size = Maths::Vector2f{ 95.f, 70.f };
            debug->color = sf::Color::Black;

            AssetsModule* am = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();
            if (am) {
                Texture* tex = am->LoadAsset<Texture>("player.png");
                if (tex) playerRef->CreateComponent<SpriteRenderer>(tex);
            }
            playerRef->Awake(); playerRef->Start();
        }

        void InitProjectilePool(int count, std::string name, std::string texturePath) {
            AssetsModule* am = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();
            Texture* bulletTex = (am) ? am->LoadAsset<Texture>(texturePath) : nullptr;

            for (int i = 0; i < count; ++i) {
                GameObject* b = CreateGameObject(name + "_" + std::to_string(i));
                b->SetPosition({ -1000.f, -1000.f });
                b->SetScale({ 0.5f, 0.5f });

                auto* p = b->CreateComponent<Projectile>();
                p->isActive = false;

                auto* debug = b->CreateComponent<HitboxRenderer>();
                debug->size = Maths::Vector2f{ 10.f, 40.f };
                debug->color = sf::Color::Red;

                if (bulletTex) b->CreateComponent<SpriteRenderer>(bulletTex);
                b->Awake(); b->Start();
            }
        }

        void InitSpawners() {
            AssetsModule* am = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();

            GameObject* rainObj = CreateGameObject("GlobalSpawner");
            auto* curtain = rainObj->CreateComponent<CurtainSpawner>();
            curtain->spawnDelay = 9999.0f;
            rainObj->Awake(); rainObj->Start();

            GameObject* boss = CreateGameObject("Boss");
            boss->SetPosition({ 750.f, 400.f });
            auto* fan = boss->CreateComponent<FanShooter>();
            fan->fireRate = 9999.0f;

            auto* debugBoss = boss->CreateComponent<HitboxRenderer>();
            debugBoss->size = Maths::Vector2f{ 90.f, 70.f }; 
            debugBoss->color = sf::Color::Green;

            if (am) {
                Texture* bTex = am->LoadAsset<Texture>("boss.png");
                if (bTex) boss->CreateComponent<SpriteRenderer>(bTex);
            }
            boss->Awake(); boss->Start();

            GameObject* managerObj = CreateGameObject("ProgressionManager");
            auto* prog = managerObj->CreateComponent<ProgressionManager>();
            prog->player = playerRef;
            prog->curtain = curtain;
            prog->fan = fan;

            Maths::Vector2f corners[4] = { {50.f, 50.f}, {1450.f, 50.f}, {1450.f, 750.f}, {50.f, 750.f} };
            Texture* sTex = (am) ? am->LoadAsset<Texture>("sentry.png") : nullptr;

            for (int i = 0; i < 4; ++i) {
                GameObject* sentry = CreateGameObject("Sentry_" + std::to_string(i));
                sentry->SetPosition(corners[i]);
                auto* s = sentry->CreateComponent<EasySentry>();
                s->fireRate = 9999.0f;

                auto* debugS = sentry->CreateComponent<HitboxRenderer>();
                debugS->size = Maths::Vector2f{ 80.f, 60.f };
                debugS->color = sf::Color::Yellow;

                if (sTex) sentry->CreateComponent<SpriteRenderer>(sTex);
                prog->sentries.push_back(s);
                sentry->Awake(); sentry->Start();
            }
            managerObj->Awake(); managerObj->Start();
        }
    };
}