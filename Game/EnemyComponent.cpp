#include "EnemyComponent.h"
#include "GameObject.h"
#include <cmath>

void EnemyComponent::Activate(const std::vector<Maths::Vector2f>& _path)
{
    path = _path;
    currentIndex = 1;
    isActive = true;


    GetOwner()->SetPosition(path[0]);
}

bool EnemyComponent::IsActive() const
{
    return isActive;
}

void EnemyComponent::TakeDamage(int dmg)
{
    hp -= dmg;

    if (hp <= 0)
    {
        GetOwner()->Disable();
    }
}

void EnemyComponent::Update(float _deltaTime)
{
    if (!isActive)
        return;

    if (currentIndex >= path.size())
    {
        isActive = false;
        GetOwner()->Disable(); 
        return;
    }

    Maths::Vector2f currentPos = GetOwner()->GetPosition();
    Maths::Vector2f target = path[currentIndex];

    Maths::Vector2f direction = target - currentPos;

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < 5.f)
    {
        currentIndex++;

        if (currentIndex >= path.size())
        {
            isActive = false;
            GetOwner()->Disable();
            return;
        }

        target = path[currentIndex];
        direction = target - currentPos;

        length = sqrt(direction.x * direction.x + direction.y * direction.y);
    }

    if (length != 0.f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    currentPos.x += direction.x * speed * _deltaTime;
    currentPos.y += direction.y * speed * _deltaTime;

    GetOwner()->SetPosition(currentPos);
}