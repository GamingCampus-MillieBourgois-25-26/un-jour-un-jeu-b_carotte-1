#include "TowerComponent.h"
#include "GameObject.h"
#include <cmath>
#include <iostream>
#include "EnemyComponent.h"

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

    timer = 0.f;

    Maths::Vector2f towerPos = GetOwner()->GetPosition();

    for (GameObject* enemyGO : *enemies)
    {
        if (!enemyGO)
            continue;

        if (!enemyGO->IsEnabled())
            continue;

        Maths::Vector2f enemyPos = enemyGO->GetPosition();

        Maths::Vector2f dir = enemyPos - towerPos;
        float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist <= range)
        {
            auto* enemyComp = enemyGO->GetComponent<EnemyComponent>();

            if (enemyComp)
            {
                enemyComp->TakeDamage(10);
            }
            break;
        }
    }
}