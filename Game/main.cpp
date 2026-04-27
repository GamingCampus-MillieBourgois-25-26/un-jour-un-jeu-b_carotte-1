#include "Engine.h"
#include "Modules/SceneModule.h"
#include "BulletHellAlice/Scenes/GameScene.h"
#include "TowerDefenseAlice/Scenes/TDScene.h"
#include "ClickerAlice/Scenes/ClickerScene.h"
#include "BulletHellLeo/BulletHellScene.h"
#include "TowerDefenseLeo/TDScene.h"
#include "ClickerLeo/MainScene.h"
#include "Match3/Scene/GameScene.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Engine* engine = Engine::GetInstance();
    engine->Init(argc, (const char**)argv);
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<ClickerAlice::ClickerScene>();
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<TowerDefenseAlice::TDScene>();
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<BulletHellAlice::GameScene>();
// 
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<MainScene>();
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<TD::TDScene>();
//    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<BulletHellScene>();

    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<Match3::GameScene>();

    engine->Run();

    return 0;
}