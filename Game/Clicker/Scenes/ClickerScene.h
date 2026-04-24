#pragma once

#include "Core/Scene.h"
#include "Modules/AssetsModule.h"
#include "Components/SpriteRenderer.h"
#include "../ClickerComponent.h"
#include "Engine.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../AutoClicker.h"
#include "../UpgradeComponent.h"
#include <string>

namespace ClickerAlice
{
    class ClickerScene : public Scene
    {
    public:
        ClickerScene() : Scene("BrickClickerScene")
        {
            auto* assets = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();

            auto* brickTex = assets->LoadAsset<Texture>("first_brick.png");

            GameObject* brickObj = CreateGameObject("MainBrick");

            float baseScale = 2.0f;
            float offset = (64.0f * baseScale) / 2.0f;
            brickObj->SetScale({ baseScale, baseScale });
            brickObj->SetPosition({ 400.0f - offset, 300.0f - offset });

            brickObj->CreateComponent<SpriteRenderer>(brickTex);
            auto* clicker = brickObj->CreateComponent<ClickerComponent>();

            GameObject* upgradeObj = CreateGameObject("UpgradeUI");
            auto* upgradeComp = upgradeObj->CreateComponent<UpgradeComponent>();

            upgradeComp->SetTarget(clicker);

            GameObject* autoClickObj = CreateGameObject("AutoClickUI");
            auto* autoComp = autoClickObj->CreateComponent<AutoClickerComponent>();
            autoComp->SetTarget(clicker);

        }
    private:
        sf::Font gameFont;
    };

}