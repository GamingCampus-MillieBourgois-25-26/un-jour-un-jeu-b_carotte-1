#include "Engine.h"
#include "Modules/SceneModule.h"
#include "BulletHellAlice/Scenes/GameScene.h"
#include "TowerDefense/Scenes/TDScene.h"
#include "Clicker/Scenes/ClickerScene.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Engine* engine = Engine::GetInstance();
    engine->Init(argc, (const char**)argv);
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<ClickerAlice::ClickerScene>();
    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<TowerDefenseAlice::TDScene>();
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<BulletHellAlice::GameScene>();

    engine->Run();

    return 0;
}