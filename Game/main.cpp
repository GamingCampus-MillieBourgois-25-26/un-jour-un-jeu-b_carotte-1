#include "Engine.h"
#include "Modules/SceneModule.h"
#include "BulletHellAlice/Scenes/GameScene.h"

int main(int argc, char* argv[])
{
    Engine* engine = Engine::GetInstance();
    engine->Init(argc, (const char**)argv);

    auto* sceneModule = engine->GetModuleManager()->GetModule<SceneModule>();

    if (sceneModule)
    {
        sceneModule->CreateScene<BulletHellAlice::GameScene>();

        sceneModule->SetScene<BulletHellAlice::GameScene>();
    }

    engine->Run();

    return 0;
}