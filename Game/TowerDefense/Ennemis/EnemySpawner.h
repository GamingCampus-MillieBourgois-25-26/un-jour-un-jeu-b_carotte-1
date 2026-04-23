#pragma once
#include "Core/Component.h"
#include "EnemyMovement.h"
#include <vector>

namespace TowerDefenseAlice
{
    class EnemySpawner : public Component
    {
    public:
        std::vector<Maths::Vector2f> roadWaypoints;

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
            for (size_t i = 0; i < pool.size(); ++i) {
                GameObject* enemy = pool[i];

                if (!enemy->IsEnabled()) {
                    auto* move = enemy->GetComponent<TowerDefenseAlice::EnemyMovement>();

                    if (move) {
                        move->Reset();
                        move->waypoints = roadWaypoints; 
                    }

                    enemy->SetPosition(GetOwner()->GetPosition());
                    enemy->Enable();

                    return;
                }
            }
        }
    };
}