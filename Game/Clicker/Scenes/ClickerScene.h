#pragma once

#include "Core/Scene.h"
#include "Modules/AssetsModule.h"
#include "Components/SpriteRenderer.h"
#include "../ClickerComponent.h"
#include "Engine.h"
#include "../UpgradeComponent.h"
#include "../AutoClicker.h"

namespace ClickerAlice
{
    class ClickerScene : public Scene
    {
    public:
        ClickerScene() : Scene("BrickClickerScene")
        {
            auto* assets = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();

            Texture* bgTex = assets->LoadAsset<Texture>("backgroundEmpty.png");

            GameObject* bg1 = CreateGameObject("Background1");
            bg1->SetPosition({ 0.f, 0.f });
            bg1->SetScale({ 1.0f, 1.0f });
            bg1->CreateComponent<SpriteRenderer>(bgTex);

            GameObject* bg2 = CreateGameObject("Background2");
            bg2->SetPosition({ 1024.f, 0.f });
            bg2->SetScale({ 1.0f, 1.0f });
            bg2->CreateComponent<SpriteRenderer>(bgTex);

            Texture* tex0 = assets->LoadAsset<Texture>("first_brick.png");
            Texture* tex1 = assets->LoadAsset<Texture>("stonebrick.png");
            Texture* tex2 = assets->LoadAsset<Texture>("bronzebrick.png");
            Texture* tex3 = assets->LoadAsset<Texture>("happybrick.png");
            Texture* tex4 = assets->LoadAsset<Texture>("goldbrick.png");

            GameObject* brickObj = CreateGameObject("MainBrick");
            brickObj->SetScale({ 2.0f, 2.0f });
            brickObj->SetPosition({ 336.0f, 236.0f });

            brickObj->CreateComponent<SpriteRenderer>(tex0);
            auto* clicker = brickObj->CreateComponent<ClickerComponent>();

            clicker->AddEvolutionTexture(tex1);
            clicker->AddEvolutionTexture(tex2);
            clicker->AddEvolutionTexture(tex3);
            clicker->AddEvolutionTexture(tex4);

            GameObject* upgradeObj = CreateGameObject("UpgradeUI");
            upgradeObj->CreateComponent<UpgradeComponent>()->SetTarget(clicker);

            GameObject* autoClickObj = CreateGameObject("AutoClickUI");
            autoClickObj->CreateComponent<AutoClickerComponent>()->SetTarget(clicker);
        }
    };
}