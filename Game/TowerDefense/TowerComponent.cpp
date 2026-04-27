#include "TowerComponent.h"
#include "GameObject.h"
#include <cmath>
#include <iostream>
#include "EnemyComponent.h"
#include "TDScene.h"

void TowerComponent::SetEnemies(const std::vector<GameObject*>* _enemies)
{
    enemies = _enemies;
}

void TowerComponent::Update(float _deltaTime)
{
    if (!enemies)
        return;

    timer += _deltaTime;

    if (timer < fireRate)
        return;

    GameObject* closestEnemy = nullptr;
    float closestDist = 999999.f;

    Maths::Vector2f towerPos = GetOwner()->GetPosition();

    for (GameObject* enemyGO : *enemies)
    {
        if (!enemyGO || !enemyGO->IsEnabled())
            continue;

        Maths::Vector2f enemyPos = enemyGO->GetPosition();

        Maths::Vector2f dir = enemyPos - towerPos;
        float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < closestDist && dist <= range)
        {
            closestDist = dist;
            closestEnemy = enemyGO;
        }
    }

    if (closestEnemy && scene)
    {
        scene->SpawnProjectile(towerPos, closestEnemy);

        // 💥 reset timer UNIQUEMENT si tir
        timer = 0.f;
    }
}

void TowerComponent::SetScene(TD::TDScene* _scene)
{
    scene = _scene;
}


void TowerComponent::Upgrade()
{

    level++;

    // 💥 amélioration
    range += 50.f;
    fireRate *= 0.8f; // tire plus vite

    upgradeCost += 30; // coût augmente

    std::cout << "Tower upgraded to level " << level << std::endl;
}