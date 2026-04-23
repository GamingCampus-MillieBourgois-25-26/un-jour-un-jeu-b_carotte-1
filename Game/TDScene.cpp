#include "TDScene.h"

#include "Engine.h"
#include "GameObject.h"
#include "RectangleShapeRenderer.h"
#include "AssetsModule.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "EnemyComponent.h"
#include "WaveManagerComponent.h"
#include "TowerComponent.h"

using namespace TD;

TDScene::TDScene() : Scene("TDScene")
{
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    CreateMapSprite();
    CreatePath();
    CreateEnemyPool();
    CreateTowerPool();

    GameObject* waveManager = CreateGameObject("WaveManager");
    waveManager->CreateComponent<WaveManagerComponent>();
    PlaceTower(Maths::Vector2f(400.f, 300.f));

}


void TDScene::CreateMapSprite()
{
    // Récupérer le module d'assets
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    // Charger la texture
    Texture* mapTexture = assets->LoadAsset<Texture>("map.png");

    // Créer l'objet
    GameObject* map = CreateGameObject("Map");

    // Ajouter le renderer
    map->CreateComponent<SpriteRenderer>(mapTexture);

    // Position (optionnel)
    map->SetPosition(Maths::Vector2f(750.f, 400.f));
}

void TDScene::CreatePath()
{
    pathPoints = {
        {0.f, 350.f},
        {160.f, 260.f},
        {300.f, 400.f},
        {600.f, 400.f},
        {600.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
        {900.f, 100.f},
    };

    for (const auto& point : pathPoints)
    {
        GameObject* node = CreateGameObject("PathNode");

        node->SetPosition(point);

        auto* renderer = node->CreateComponent<RectangleShapeRenderer>();

        renderer->SetSize(Maths::Vector2f(10.f, 10.f));
        renderer->SetColor(sf::Color::Red);
    }
}

void WaveManagerComponent::SpawnEnemy()
{
    TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

    scene->SpawnEnemy();
}


void TDScene::SpawnEnemy()
{
    for (GameObject* enemy : enemyPool)
    {
        if (!enemy->IsEnabled())
        {
            enemy->Enable();

            auto* enemyComp = enemy->GetComponent<EnemyComponent>();
            if (enemyComp)
                enemyComp->Activate(pathPoints);

            return;
        }
    }
}

void TDScene::CreateEnemyPool()
{
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    Texture* enemyTexture = assets->LoadAsset<Texture>("enemy.png");

    for (int i = 0; i < 20; ++i)
    {
        GameObject* enemy = CreateGameObject("Enemy");

        enemy->CreateComponent<SpriteRenderer>(enemyTexture);
        enemy->CreateComponent<EnemyComponent>();

        enemy->Disable();

        enemyPool.push_back(enemy); // ✔️ correct
    }
}

void TDScene::CreateTowerPool()
{
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    Texture* towerTexture = assets->LoadAsset<Texture>("tower.png");

    for (int i = 0; i < 10; ++i)
    {
        GameObject* tower = CreateGameObject("Tower");

        tower->CreateComponent<SpriteRenderer>(towerTexture);

        auto* towerComp = tower->CreateComponent<TowerComponent>();
        towerComp->SetEnemies(&enemyPool);

        tower->Disable();

        towerPool.push_back(tower);
    }
}

void TDScene::PlaceTower(const Maths::Vector2f& position)
{
    for (GameObject* tower : towerPool)
    {
        if (!tower->IsEnabled())
        {
            tower->Enable();
            tower->SetPosition(position);
            return;
        }
    }
}