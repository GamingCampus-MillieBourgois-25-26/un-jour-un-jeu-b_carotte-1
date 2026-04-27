#pragma once

#include "Core/Scene.h"
#include <vector>
#include "Maths/Vector2.h"
#include "EnemyComponent.h"

class GameObject;

namespace TD
{
    enum class GameState
    {
        Menu,
        Playing,
        GameOver,
        Victory
    };


    class TDScene final : public Scene
    {
    public:
        TDScene();

        void SpawnEnemy();
        void CreateEnemyPool();

        void CreateTowerPool();
        void PlaceTower(const Maths::Vector2f& position);

        void CreateProjectilePool();
        GameObject* SpawnProjectile(const Maths::Vector2f& pos, GameObject* target);

        void Update(float _deltaTime);
        void TryPlaceTower();

        bool canClick = true;

        GameObject* GetPreviewTower();
        void AddMoney(int amount);

        int GetMoney() const { return money; }
        void SetWave(int wave) { currentWave = wave; }

        GameObject* previewOverlay = nullptr;

        GameObject* GetPreviewOverlay() { return previewOverlay; }

        bool IsOnPath(const Maths::Vector2f& pos);
        void CreatePathBlocks();
        bool IsOnTower(const Maths::Vector2f& pos);
        int GetWave() const;
        void TryUpgradeTower(GameObject* tower);
        const std::vector<GameObject*>& GetTowers() const { return towerPool; }
        sf::RenderWindow* window = nullptr;
        void SetSelectedTower(GameObject* tower);
        GameObject* GetSelectedTower() const;

        bool showTowerUI = true;

        void ToggleTowerUI() { showTowerUI = !showTowerUI; }
        bool IsTowerUIVisible() const { return showTowerUI; }
        void StartGame();
        void GameOver();
        void AddKill();
        GameState GetGameState() const { return gameState; }


        int enemiesKilled = 0;



    private:
        void CreateMapSprite();
        void CreatePath();
        GameObject* selectedTower = nullptr;
        GameState gameState;

        int money = 100;
        int towerCost = 20;
        int currentWave = 1;

        std::vector<Maths::Vector2f> pathPoints;

        std::vector<GameObject*> enemyPool;
        std::vector<GameObject*> towerPool;
        std::vector<GameObject*> projectilePool;
        GameObject* previewTower = nullptr;
        std::vector<GameObject*> pathBlocks;
    };
}