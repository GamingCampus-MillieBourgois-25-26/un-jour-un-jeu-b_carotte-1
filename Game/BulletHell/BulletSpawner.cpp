#include "BulletSpawner.h"

static std::vector<BulletSpawnRequest> requests;

void BulletSpawner::RequestSpawn(const BulletSpawnRequest& req)
{
    requests.push_back(req);
}

std::vector<BulletSpawnRequest> BulletSpawner::Consume()
{
    auto copy = requests;
    requests.clear();
    return copy;
}