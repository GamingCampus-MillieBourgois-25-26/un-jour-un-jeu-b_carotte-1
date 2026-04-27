#pragma once

#include <vector>
#include "Maths/Vector2.h"

struct BulletSpawnRequest
{
    Maths::Vector2f position;
    Maths::Vector2f direction;
};

class BulletSpawner
{
public:
    static void RequestSpawn(const BulletSpawnRequest& req);
    static std::vector<BulletSpawnRequest> Consume();
};