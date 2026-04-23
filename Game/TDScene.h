#pragma once

#include "Core/Scene.h"
#include <vector>
#include "Maths/Vector2.h"
#include "EnemyComponent.h"

class GameObject;

namespace TD
{
    class TDScene final : public Scene
    {
    public:
        TDScene();

        void SpawnEnemy();
        void CreateEnemyPool();

        void CreateTowerPool();
        void PlaceTower(const Maths::Vector2f& position);

    private:
        void CreateMapSprite();
        void CreatePath();

        std::vector<Maths::Vector2f> pathPoints;

        std::vector<GameObject*> enemyPool;
        std::vector<GameObject*> towerPool;
    };
}