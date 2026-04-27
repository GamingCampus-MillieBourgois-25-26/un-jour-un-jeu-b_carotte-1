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
#include "ProjectileComponent.h"
#include <iostream>
#include "InputComponent.h"
#include "UIComponent.h"

using namespace TD;

TDScene::TDScene() : Scene("TDScene")
{
    gameState = GameState::Menu;

    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    CreateMapSprite();
    CreatePath();
    CreateEnemyPool();
    CreateTowerPool();
    CreatePathBlocks();
    CreateProjectilePool();

    GameObject* waveManager = CreateGameObject("WaveManager");
    waveManager->CreateComponent<WaveManagerComponent>();

    GameObject* input = CreateGameObject("Input");
    auto* inputComp = input->CreateComponent<InputComponent>();

    previewTower = CreateGameObject("PreviewTower");

    previewTower->CreateComponent<SpriteRenderer>(
        Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>()
        ->LoadAsset<Texture>("tower.png")
    );

    previewOverlay = CreateGameObject("PreviewOverlay");

    auto* overlay = previewOverlay->CreateComponent<RectangleShapeRenderer>();
    overlay->SetSize(Maths::Vector2f(30.f, 60.f));
    overlay->SetColor(sf::Color(0, 255, 0, 100));


    GameObject* ui = CreateGameObject("UI");
    ui->CreateComponent<UIComponent>();
}

void TDScene::Update(float _deltaTime)
{
    if (gameState != GameState::Playing)
        return;

    std::cout << "UPDATE" << std::endl;
    Scene::Update(_deltaTime);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (canClick)
        {
            TryPlaceTower();
            canClick = false;

        }
    }
    else
    {
        canClick = true;
    }
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
        {160.f, 350.f},
        {160.f, 250.f},
        {400.f, 250.f},
        {450.f, 210.f},
        {1330.f, 210.f},
        {1330.f, 550.f},
        {600.f, 550.f},
        {560.f, 500.f},
        {560.f, 430.f},
        {400.f, 430.f},
        {380.f, 470.f},
        {310.f, 470.f},
        {300.f, 530.f},
        {200.f, 540.f},
        {170.f, 600.f},
        {150.f, 630.f},
        {150.f, 735.f},
        {600.f, 735.f},
        {650.f, 720.f},
        {1250.f, 720.f},
        {1300.f, 740.f},
        {1400.f, 730.f},
        {1600.f, 730.f},
    };

    for (const auto& point : pathPoints)
    {
        GameObject* node = CreateGameObject("PathNode");

        node->SetPosition(point);

        auto* renderer = node->CreateComponent<RectangleShapeRenderer>();

        renderer->SetSize(Maths::Vector2f(10.f, 10.f));
        renderer->SetColor(sf::Color(0, 0, 0, 0));
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
            {
                int wave = GetWave(); // récupère depuis WaveManager
                enemyComp->Activate(pathPoints, wave);
            }

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

    for (int i = 0; i < 2000; ++i)
    {
        GameObject* enemy = CreateGameObject("Enemy");

        enemy->CreateComponent<SpriteRenderer>(enemyTexture);
        enemy->CreateComponent<EnemyComponent>();

        // 🔴 background (rouge)
        GameObject* hpBg = CreateGameObject("HP_BG");
        auto* bgRenderer = hpBg->CreateComponent<RectangleShapeRenderer>();
        bgRenderer->SetSize(Maths::Vector2f(40.f, 5.f));
        bgRenderer->SetColor(sf::Color::Red);

        // 🟢 fill (vert)
        GameObject* hpFill = CreateGameObject("HP_FILL");
        auto* fillRenderer = hpFill->CreateComponent<RectangleShapeRenderer>();
        fillRenderer->SetSize(Maths::Vector2f(40.f, 5.f));
        fillRenderer->SetColor(sf::Color::Green);

        auto* enemyComp = enemy->GetComponent<EnemyComponent>();
        enemyComp->SetHPBar(hpBg, hpFill);

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

    for (int i = 0; i < 200; ++i)
    {
        GameObject* tower = CreateGameObject("Tower");


        tower->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
        tower->Disable();


        tower->CreateComponent<SpriteRenderer>(towerTexture);

        auto* towerComp = tower->CreateComponent<TowerComponent>();
        towerComp->SetEnemies(&enemyPool);
        towerComp->SetScene(this);

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

void TDScene::CreateProjectilePool()
{
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    Texture* bulletTexture = assets->LoadAsset<Texture>("bullet.png");


    for (int i = 0; i < 30; ++i)
    {
        GameObject* proj = CreateGameObject("Projectile");

        proj->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
        proj->Disable();

        proj->CreateComponent<SpriteRenderer>(bulletTexture);
        proj->SetScale(Maths::Vector2f(1.f, 1.f));

        proj->CreateComponent<ProjectileComponent>();

        proj->Disable();

        projectilePool.push_back(proj);
    }
}

GameObject* TDScene::SpawnProjectile(const Maths::Vector2f& pos, GameObject* target)
{

    std::cout << "SPAWN PROJECTILE" << std::endl;


    for (GameObject* proj : projectilePool)
    {
        if (!proj->IsEnabled())
        {
            proj->SetPosition(pos + Maths::Vector2f(10.f, 10.f));
            proj->Enable();

            auto* comp = proj->GetComponent<ProjectileComponent>();
            if (comp)
                comp->Activate(target);

            return proj;
        }
    }

    return nullptr;
}

void TDScene::TryPlaceTower()
{
    if (money < towerCost)
    {
        std::cout << "Pas assez d'argent !" << std::endl;
        return;
    }

    Maths::Vector2f pos = previewTower->GetPosition();

    if (IsOnPath(pos) || IsOnTower(pos))
    {
        std::cout << "Placement interdit !" << std::endl;
        return;
    }

    Maths::Vector2f worldPos = previewTower->GetPosition();

    PlaceTower(worldPos);


    money -= towerCost;

    std::cout << "Tour placée ! Argent restant: " << money << std::endl;

  
}

GameObject* TDScene::GetPreviewTower()
{
    return previewTower;
}

void TDScene::AddMoney(int amount)
{
    money += amount;
}

bool TDScene::IsOnPath(const Maths::Vector2f& pos)
{
    for (auto* block : pathBlocks)
    {
        auto* r = block->GetComponent<RectangleShapeRenderer>();
        if (!r) continue;

        Maths::Vector2f bPos = block->GetPosition();
        Maths::Vector2f size = r->GetSize();

        if (pos.x > bPos.x &&
            pos.x < bPos.x + size.x &&
            pos.y > bPos.y &&
            pos.y < bPos.y + size.y)
        {
            return true;
        }
    }

    return false;
}

void TDScene::CreatePathBlocks()
{
    // 🔴 1. entrée gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 0.f, 300.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 200.f, 80.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }

    // 🔴 2. montée (gauche vers centre)
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 125.f, 210.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 80.f, 170.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }

    // 🔴 3. ligne du haut
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 200.f, 210.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 250.f, 80.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }

    // 🔴 4. descente droite
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 340.f, 180.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 1020.f, 70.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }

    // 🔴 5. ligne du bas
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 1290.f, 180.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 80.f,420.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }

    // 🔴 6. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 560.f, 510.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 750.f, 80.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 7. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 550.f, 470.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 80.f, 120.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 8. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 360.f, 360.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 250.f, 100.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 9. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 520.f, 400.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 100.f, 180.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 10. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 260.f, 410.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 190.f, 100.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 11. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 150.f, 480.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 210.f, 100.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 12. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 140.f, 510.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 100.f, 150.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 13. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 110.f, 570.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 100.f, 210.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 14. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 110.f, 680.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 630.f, 100.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 15. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 560.f, 660.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 630.f, 110.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
    // 🔴 16. retour vers gauche
    {
        GameObject* block = CreateGameObject("PathBlock");
        block->SetPosition({ 1000.f, 660.f });

        auto* r = block->CreateComponent<RectangleShapeRenderer>();
        r->SetSize({ 630.f, 120.f });
        r->SetColor(sf::Color(255, 0, 0, 0));

        pathBlocks.push_back(block);
    }
}

bool TDScene::IsOnTower(const Maths::Vector2f& pos)
{
    for (GameObject* tower : towerPool)
    {
        if (!tower->IsEnabled())
            continue;

        Maths::Vector2f tPos = tower->GetPosition();

        float size = 64.f; // taille de ta tour (ajuste)

        if (pos.x > tPos.x - size / 2 &&
            pos.x < tPos.x + size / 2 &&
            pos.y > tPos.y - size / 2 &&
            pos.y < tPos.y + size / 2)
        {
            return true;
        }
    }

    return false;
}


int TDScene::GetWave() const
{
    for (const auto& go : GetGameObjects())
    {
        GameObject* obj = go.get();

        auto* wave = obj->GetComponent<WaveManagerComponent>();
        if (wave)
            return wave->GetWave();
    }

    return 0;
}


void TDScene::TryUpgradeTower(GameObject* tower)
{
    if (!tower)
        return;

    TowerComponent* comp = nullptr;

    for (auto& c : tower->GetComponents())
    {
        comp = dynamic_cast<TowerComponent*>(c.get());
        if (comp)
            break;
    }

    if (!comp)
    {
        std::cout << "NO TowerComponent" << std::endl;
        return;
    }

    if (money < comp->upgradeCost)
    {
        std::cout << "Pas assez d'argent !" << std::endl;
        return;
    }

    money -= comp->upgradeCost;

    std::cout << "UPGRADE OK" << std::endl;

    comp->Upgrade();
}

void TDScene::SetSelectedTower(GameObject* tower)
{
    selectedTower = tower;
}

GameObject* TDScene::GetSelectedTower() const
{
    return selectedTower;
}

void TDScene::StartGame()
{
    gameState = GameState::Playing;

    // 💥 reset enemy pool
    for (GameObject* enemy : enemyPool)
    {
        enemy->Disable();
        enemy->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
    }

    enemiesKilled = 0;
}

void TDScene::GameOver()
{
    gameState = GameState::GameOver;
}

void TDScene::AddKill()
{
    enemiesKilled++;

    if (enemiesKilled >= 2000)
    {
        gameState = GameState::Victory;
    }
}