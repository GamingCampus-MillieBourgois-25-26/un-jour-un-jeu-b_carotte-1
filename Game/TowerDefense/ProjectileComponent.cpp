#include "ProjectileComponent.h"
#include "GameObject.h"
#include <cmath>
#include "EnemyComponent.h"
#include <iostream>

void ProjectileComponent::Activate(GameObject* _target)
{
    target = _target;
}

void ProjectileComponent::Update(float _deltaTime)
{
    if (!target)
    {
        GetOwner()->Disable();
        GetOwner()->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
        return;
    }

    auto* enemyComp = target->GetComponent<EnemyComponent>();

    if (!enemyComp || !enemyComp->IsActive())
    {
        GetOwner()->Disable();
        GetOwner()->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
        return;
    }

    Maths::Vector2f currentPos = GetOwner()->GetPosition();
    Maths::Vector2f targetPos = target->GetPosition();

    Maths::Vector2f dir = targetPos - currentPos;

    float length = sqrt(dir.x * dir.x + dir.y * dir.y);



    if (length < 5.f)
    {
        auto* enemyComp = target->GetComponent<EnemyComponent>();

        if (enemyComp)
            enemyComp->TakeDamage(10);

        GetOwner()->Disable();

        // 💥 AJOUT CRUCIAL
        GetOwner()->SetPosition(Maths::Vector2f(-1000.f, -1000.f));

        return;
    }

    dir.x /= length;
    dir.y /= length;

    float angle = atan2(dir.y, dir.x) * 180.f / 3.14159f + 90.f;
    GetOwner()->SetRotation(sf::degrees(angle));

    currentPos.x += dir.x * speed * _deltaTime;
    currentPos.y += dir.y * speed * _deltaTime;

    GetOwner()->SetPosition(currentPos);
}