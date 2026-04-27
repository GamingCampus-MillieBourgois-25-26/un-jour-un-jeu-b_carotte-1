#include "Game.h"

#include "Engine.h"
#include "SceneModule.h"
#include "BulletHellScene.h"

void Game::Run(int argc, const char** argv)
{
    Engine::GetInstance()->Init(argc, argv);

    Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<SceneModule>()
        ->SetScene<BulletHellScene>();

    Engine::GetInstance()->Run();
}