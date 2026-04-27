#include "Engine.h"
#include "SceneModule.h"

#include "MainScene.h"

int main(int argc, const char** argv)
{
    Engine* engine = Engine::GetInstance();

    engine->Init(argc, argv);


    engine->GetModuleManager()
        ->GetModule<SceneModule>()
        ->SetScene<MainScene>();

    engine->Run();

    return 0;
}