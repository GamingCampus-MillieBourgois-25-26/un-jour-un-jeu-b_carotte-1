#include "EnemyComponent.h"
#include "GameObject.h"
#include "TDScene.h"
#include <cmath>
#include <iostream>
#include "RectangleShapeRenderer.h"

void EnemyComponent::Activate(const std::vector<Maths::Vector2f>& _path, int wave)
{
    path = _path;
    currentIndex = 1;
    isActive = true;
    hp = maxHp;

    // 💥 scaling
    maxHp = 20 + wave * 5;
    hp = maxHp;
    speed = 80.f + wave * 2;

    GetOwner()->SetPosition(path[0]);
    if (hpBg) hpBg->Enable();
    if (hpFill) hpFill->Enable();
}

bool EnemyComponent::IsActive() const
{
    return isActive;
}

void EnemyComponent::TakeDamage(int dmg)
{
    hp -= dmg;

    std::cout << "Enemy hit, HP = " << hp << std::endl;

    if (hp <= 0)
    {
        isActive = false;

        GetOwner()->SetPosition(Maths::Vector2f(-1000.f, -1000.f));

        TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

        if (hpBg)
        {
            hpBg->Disable();
            hpBg->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
        }

        if (hpFill)
        {
            hpFill->Disable();
            hpFill->SetPosition(Maths::Vector2f(-1000.f, -1000.f));
        }

        if (scene)
        {
            scene->AddMoney(5); 
            scene->AddKill();
        }
    }
    float ratio = (float)hp / (float)maxHp;

    if (hpFill)
    {
        auto* renderer = hpFill->GetComponent<RectangleShapeRenderer>();
        renderer->SetSize(Maths::Vector2f(40.f * ratio, 5.f));
    }
}

void EnemyComponent::Update(float _deltaTime)
{
    TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

    if (!scene || scene->GetGameState() != TD::GameState::Playing)
        return;

    if (!isActive)
        return;

    if (currentIndex >= path.size())
    {
        TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

        isActive = false;
        if (scene)
            scene->GameOver();

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

            TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());
            if (scene)
                scene->GameOver();

            GetOwner()->Disable();
            return;
        }

        target = path[currentIndex];
        direction = target - currentPos;

        length = sqrt(direction.x * direction.x + direction.y * direction.y);
    }

    if (isActive && hpBg && hpFill)
    {
        Maths::Vector2f pos = GetOwner()->GetPosition();

        hpBg->SetPosition(pos + Maths::Vector2f(-20.f, -30.f));
        hpFill->SetPosition(pos + Maths::Vector2f(-20.f, -30.f));
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

void EnemyComponent::SetHPBar(GameObject* bg, GameObject* fill)
{
    hpBg = bg;
    hpFill = fill;
}