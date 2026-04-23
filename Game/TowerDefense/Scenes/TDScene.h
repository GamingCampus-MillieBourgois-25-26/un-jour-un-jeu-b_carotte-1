#pragma once
#include "Core/Scene.h"
#include "Core/Component.h"
#include "../Ennemis/EnemySpawner.h"
#include "../Ennemis/EnemyMovement.h"
#include "Components/SpriteRenderer.h"
#include "../Towers/Tower.h"
#include "../Towers/TowerManager.h"
#include "Engine.h"
#include "Modules/AssetsModule.h"
#include "../Towers/Projectile.h"
#include "../Health/Health.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
namespace TowerDefenseAlice
{
    class TDScene : public Scene
    {
    public:
        TDScene() : Scene("TowerDefenseAlice", true)
        {
            auto* assets = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();
            Texture* backgroundTex = assets->LoadAsset<Texture>("map.png");
            GameObject* background = CreateGameObject("Background");
            if (backgroundTex) {
                background->CreateComponent<SpriteRenderer>(backgroundTex);
            }
            background->SetScale({ 0.7f, 0.7f });


            Texture* tex = assets->LoadAsset<Texture>("ennemi.png");
            Texture* towerTex = assets->LoadAsset<Texture>("tour.png");
            Texture* projTex = assets->LoadAsset<Texture>("projectile.png");

            std::vector<Maths::Vector2f> road;
            road.push_back({ 98.f, 562.f }); 
            road.push_back({ 572.f, 536.f });
            road.push_back({ 575.f, 153.f });
            road.push_back({ 1064.f, 158.f });
            road.push_back({ 1069.f, 652.f });
            road.push_back({ 1600.f, 652.f });

            GameObject* spawnerObj = CreateGameObject("Spawner");
            EnemySpawner* spawnerComp = spawnerObj->CreateComponent<TowerDefenseAlice::EnemySpawner>();
            spawnerObj->SetPosition({ 70.f, -50.f });
            spawnerComp->roadWaypoints = road;

            for (int i = 0; i < 20; i++)
            {
                GameObject* enemy = CreateGameObject("Pool_Enemy_" + std::to_string(i));
                enemy->CreateComponent<TowerDefenseAlice::EnemyMovement>();
                enemy->CreateComponent<Health>();

                if (tex) enemy->CreateComponent<SpriteRenderer>(tex);
				enemy->SetPosition({ -1000.f, -1000.f });
                enemy->Disable();
                spawnerComp->AddToPool(enemy);
            }

            for (int i = 0; i < 10; i++)
            {
                GameObject* t = CreateGameObject("Pool_Tower_" + std::to_string(i));
                t->SetPosition({ -1000.f, -1000.f });
                t->Disable();
                t->CreateComponent<Tower>();
                if (towerTex) {
                    t->CreateComponent<SpriteRenderer>(towerTex);
                }
            }

            for (int i = 0; i < 50; i++)
            {
                GameObject* p = CreateGameObject("Pool_Projectile_" + std::to_string(i));

				p->SetPosition({ -1000.f, -1000.f });
                p->Disable();
                p->CreateComponent<TowerDefenseAlice::Projectile>();
                if (projTex) {
                    p->CreateComponent<SpriteRenderer>(projTex);
                }
            }
            GameObject* manager = CreateGameObject("TowerManager");
            manager->CreateComponent<TowerDefenseAlice::TowerManager> ();
        }
    };
}