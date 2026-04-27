#pragma once

#include "Core/Scene.h"

class BulletHellScene : public Scene
{
public:
    BulletHellScene();

    void Update(const float deltaTime) const ;
};