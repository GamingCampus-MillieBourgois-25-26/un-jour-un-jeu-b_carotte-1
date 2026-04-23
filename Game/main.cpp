#include "Engine.h"
#include "Modules/SceneModule.h"
#include "BulletHellAlice/Scenes/GameScene.h"
#include "TowerDefense/Scenes/TDScene.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Engine* engine = Engine::GetInstance();
    engine->Init(argc, (const char**)argv);
    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<TowerDefenseAlice::TDScene>();

    engine->Run();

    return 0;
}