#include "Enemy.h"
#include "Core/GameObject.h"
#include "Components/RectangleShapeRenderer.h"
#include "Bullet.h"
#include "Core/Scene.h"

#include "Components/SpriteRenderer.h"

void Enemy::SetScene(Scene* sceneRef)
{
    scene = sceneRef;
}

void Enemy::Update(const float deltaTime)
{
    if (spawnNextFrame)
    {
        spawnNextFrame = false;

        if (!scene) return;

        GameObject* bullet = scene->CreateGameObject("EnemyBullet");

        bullet->SetPosition(GetOwner()->GetPosition());
        bullet->SetScale({ 1.f, 1.f });

        auto* renderer = bullet->CreateComponent<RectangleShapeRenderer>();
        renderer->SetColor(sf::Color::Yellow);
        renderer->SetSize({ 10.f, 10.f });

        auto* bulletComp = bullet->CreateComponent<Bullet>();
        bulletComp->SetDirection({ 0.f, 1.f });

        bullet->Awake();
        bullet->Start();
    }

    shootTimer += deltaTime;

    if (shootTimer >= shootInterval)
    {
        shootTimer = 0.f;
        spawnNextFrame = true; 
    }
}