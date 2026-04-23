#pragma once
#include "Core/Scene.h"
#include "Core/Component.h"
#include "../Ennemis/EnemySpawner.h"
#include "../Ennemis/EnemyMovement.h"
#include "Components/SpriteRenderer.h"
#include "../Towers/Tower.h"
#include "Engine.h"
#include "Modules/AssetsModule.h"

namespace TowerDefenseAlice
{
    class TDScene : public Scene
    {
    public:
        TDScene() : Scene("TowerDefenseAlice", true)
        {
            auto* assets = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();
            Texture* tex = assets->LoadAsset<Texture>("ennemi.png");
            Texture* towerTex = assets->LoadAsset<Texture>("tour.png");
            std::vector<Maths::Vector2f> road;
            road.push_back({ 400.f, 100.f }); 
            road.push_back({ 400.f, 700.f });
            road.push_back({ 1100.f, 700.f });
            road.push_back({ 1100.f, 400.f });
            road.push_back({ 1600.f, 400.f });
            GameObject* spawnerObj = CreateGameObject("Spawner");
            EnemySpawner* spawnerComp = spawnerObj->CreateComponent<TowerDefenseAlice::EnemySpawner>();
            spawnerObj->SetPosition({ 50.f, 50.f });
            for (int i = 0; i < 20; i++)
            {
                GameObject* enemy = CreateGameObject("Pool_Enemy_" + std::to_string(i));
                EnemyMovement* move = enemy->CreateComponent<TowerDefenseAlice::EnemyMovement>();
                if (move)
                    move->waypoints = road;
                if (tex)
                    enemy->CreateComponent<SpriteRenderer>(tex);
                enemy->Disable();
                spawnerComp->AddToPool(enemy);
            }
            GameObject* myTower = CreateGameObject("Tower1");
            myTower->SetPosition({ 400.f, 300.f });
            myTower->CreateComponent<Tower>();
            if (towerTex) {
                myTower->CreateComponent<SpriteRenderer>(towerTex);
            }
        }
    };
}