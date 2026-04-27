#pragma once

#include "Core/Component.h"

class WaveManagerComponent : public Component
{
public:
    void Update(float _deltaTime) override;
    void StartNextWave();
    int GetWave() const { return currentWave; }

private:
    float spawnTimer = 0.f;
    float spawnDelay = 1.f;

    int enemiesToSpawn = 5;
    int enemiesSpawned = 0;

    void SpawnEnemy();

    int currentWave = 1;

    float waveDelay = 3.0f;
    float waveTimer = 0.f;

    bool waveInProgress = false;
};