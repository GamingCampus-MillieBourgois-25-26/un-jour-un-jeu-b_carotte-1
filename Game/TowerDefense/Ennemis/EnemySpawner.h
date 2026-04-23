#pragma once
#include "Core/Component.h"
#include "EnemyMovement.h"
#include <vector>

namespace TowerDefenseAlice
{
    class EnemySpawner : public Component
    {
    public:
        void AddToPool(GameObject* _enemy) {
            pool.push_back(_enemy);
        }

        void Update(float _delta_time) {
            timer += _delta_time;
            if (timer >= spawnRate) {
                SpawnFromPool();
                timer = 0.0f;
            }
        }

    private:
        float spawnRate = 2.0f;
        float timer = 0.0f;
        std::vector<GameObject*> pool;

        void SpawnFromPool() {
            for (GameObject* enemy : pool) {
                if (!enemy->IsEnabled()) {
                    auto* move = enemy->GetComponent<TowerDefenseAlice::EnemyMovement>();
                    if (move) move->Reset();

                    enemy->SetPosition(GetOwner()->GetPosition());
                    enemy->Enable();
                    return;
                }
            }
        }
    };
}