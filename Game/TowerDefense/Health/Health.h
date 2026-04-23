#pragma once
#include "Core/Component.h"
#include "Core/GameObject.h"
#include "../Towers/TowerManager.h"
#include "Core/Scene.h"
#include <cstdio>

namespace TowerDefenseAlice
{
    class Health : public Component
    {
    public:
        int maxHealth = 30;
        int currentHealth = 30;

        void TakeDamage(int _amount)
        {
            currentHealth -= _amount;
            printf("L'ennemi %s a recu %d degats. PV restants : %d\n",
                GetOwner()->GetName().c_str(), _amount, currentHealth);

            if (currentHealth <= 0)
            {
                Die();
            }
        }

        void Die()
        {
            printf("L'ennemi %s est mort ! +10 Gold\n", GetOwner()->GetName().c_str());
			GetOwner()->SetPosition({ -1000.f, -1000.f });
            Scene* scene = GetOwner()->GetScene();
            if (scene)
            {
                for (const auto& obj : scene->GetGameObjects())
                {
                    TowerManager* manager = obj->GetComponent<TowerManager>();
                    if (manager)
                    {
                        manager->AddGold(10);
                        break; 
                    }
                }
            }
            GetOwner()->Disable();
            currentHealth = maxHealth;
        }
    };
}