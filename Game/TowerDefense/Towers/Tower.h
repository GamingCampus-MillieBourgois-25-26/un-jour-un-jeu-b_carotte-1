#pragma once
#include "Core/Component.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Maths/Vector2.h"
#include <vector>
#include <cmath>

class Tower : public Component
{
public:
    float range = 300.0f;
    float fireRate = 0.8f; 
    float timer = 0.0f;

    void Update(float _delta_time)
    {
        timer += _delta_time;

        if (timer >= fireRate)
        {
            if (AttackNearestEnemy())
            {
                timer = 0.0f;
            }
        }
    }

private:
    bool AttackNearestEnemy()
    {
        Scene* scene = GetOwner()->GetScene();
        if (!scene) return false;

        GameObject* closestEnemy = nullptr;
        float minDistance = range;

        for (const auto& obj : scene->GetGameObjects())
        {
            if (obj->IsEnabled() && obj->GetName().find("Pool_Enemy") != std::string::npos)
            {
                Maths::Vector2f diff = obj->GetPosition() - GetOwner()->GetPosition();
                float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                if (dist < minDistance)
                {
                    minDistance = dist;
                    closestEnemy = obj.get();
                }
            }
        }

        if (closestEnemy)
        {
            printf("La tour %s tire sur %s !\n", GetOwner()->GetName().c_str(), closestEnemy->GetName().c_str());
            return true;
        }

        return false;
    }
};