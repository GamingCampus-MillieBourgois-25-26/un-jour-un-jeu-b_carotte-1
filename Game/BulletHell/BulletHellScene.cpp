#include "BulletHellScene.h"

#include "Core/GameObject.h"
#include "Components/RectangleShapeRenderer.h"
#include "Components/SpriteRenderer.h"

#include "BulletHellPlayer.h"
#include "Enemy.h"

#include "Modules/AssetsModule.h"
#include "Assets/Texture.h"

BulletHellScene::BulletHellScene()
    : Scene("BulletHellScene")
{
    // PLAYER
    GameObject* player = CreateGameObject("Player");

    player->SetPosition({ 400.f, 300.f });
    player->SetScale({ 0.5f, 0.5f });

    auto* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    auto* texture = assets->LoadAsset<Texture>("player.png");

    player->CreateComponent<SpriteRenderer>(texture);
    player->CreateComponent<BulletHellPlayer>();

    // ENEMY
    GameObject* enemy = CreateGameObject("Enemy");

    enemy->SetPosition({ 400.f, 100.f });

    auto* renderer = enemy->CreateComponent<RectangleShapeRenderer>();
    renderer->SetColor(sf::Color::Red);
    renderer->SetSize({ 50.f, 50.f });

    auto* enemyComp = enemy->CreateComponent<Enemy>();
    enemyComp->SetScene(this);
}