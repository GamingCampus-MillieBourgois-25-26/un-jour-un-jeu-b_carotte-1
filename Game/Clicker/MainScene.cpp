#include "MainScene.h"
#include "Core/GameObject.h"
#include "RectangleShapeRenderer.h"
#include "ClickComponent.h"
#include "SpriteRenderer.h"
#include "AssetsModule.h"
#include "Engine.h"
#include "Components/TextRenderer.h"
#include "TextRenderer.h" 
#include "ShopPanel.h"

MainScene::MainScene() : Scene("MainScene")
{
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    // 🟦 zone gauche
    {
        GameObject* leftZone = CreateGameObject("LeftZone");

        auto* rect = leftZone->CreateComponent<RectangleShapeRenderer>();
        rect->SetSize({ 1000.f, 800.f });
        rect->SetColor(sf::Color(0, 0, 0, 0));

        leftZone->SetPosition({ 0.f, 0.f });
    }

    // 🟥 zone droite
    {
        GameObject* rightZone = CreateGameObject("RightZone");

        auto* rect = rightZone->CreateComponent<RectangleShapeRenderer>();
        rect->SetSize({ 500.f, 800.f });
        rect->SetColor(sf::Color(255, 0, 0, 100));

        rightZone->SetPosition({ 1000.f, 0.f });
    }

    // 🟡 clicker
    {
        GameObject* clicker = CreateGameObject("Clicker");

        clicker->CreateComponent<SpriteRenderer>(
            assets->LoadAsset<Texture>("clicker.png")
        );

        clicker->SetPosition({ 500.f, 400.f });

        clicker->CreateComponent<ClickComponent>();

    }

    {
        GameObject* shop = CreateGameObject("Shop");
        shop->CreateComponent<ShopPanel>();
    }

}