#include "Engine.h"
#include "SceneModule.h"

#include "BulletHell/BulletHellScene.h"
#include "TowerDefense/TDScene.h"
#include "Clicker/MainScene.h"


int main(int argc, const char** argv)
{
    Engine* engine = Engine::GetInstance();

    engine->Init(argc, argv);



    engine->GetModuleManager()->GetModule<SceneModule>()->SetScene<TD::TDScene>();


    engine->Run();

    return 0;
}