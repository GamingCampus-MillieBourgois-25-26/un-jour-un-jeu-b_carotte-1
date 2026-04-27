#include "Bullet.h"
#include "Core/GameObject.h"

void Bullet::Update(const float deltaTime)
{
    Maths::Vector2f position = GetOwner()->GetPosition();
    position += direction * speed * deltaTime;
    GetOwner()->SetPosition(position);
}

void Bullet::SetDirection(const Maths::Vector2f& dir)
{
    direction = dir;
}