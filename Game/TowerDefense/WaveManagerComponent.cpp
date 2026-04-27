#include "WaveManagerComponent.h"
#include "GameObject.h"
#include "Engine.h"
#include "Scene.h"
#include <iostream>
#include "TDScene.h"

void WaveManagerComponent::Update(float _deltaTime)
{

    TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());

    if (!scene || scene->GetGameState() != TD::GameState::Playing)
        return;

    if (waveInProgress)
    {
        spawnTimer += _deltaTime;

        if (spawnTimer >= spawnDelay && enemiesSpawned < enemiesToSpawn)
        {
            spawnTimer = 0.f;

            SpawnEnemy();
            enemiesSpawned++;
        }

        if (enemiesSpawned >= enemiesToSpawn)
        {
            waveInProgress = false;
            waveTimer = 0.f;
        }
    }
    else
    {
        waveTimer += _deltaTime;

        if (waveTimer >= waveDelay)
        {
            StartNextWave();
        }
    }
}

void WaveManagerComponent::StartNextWave()
{
    currentWave++;

    enemiesSpawned = 0;

    // 💥 DIFFICULTÉ
    enemiesToSpawn += 5;         // + ennemis
    spawnDelay *= 0.9f;         // spawn plus rapide

    // sécurité
    if (spawnDelay < 0.2f)
        spawnDelay = 0.2f;

    waveInProgress = true;

    std::cout << "Wave " << currentWave << " started!" << std::endl;
}