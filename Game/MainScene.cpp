#include "MainScene.h"
#include "Core/GameObject.h"
#include "RectangleShapeRenderer.h"
#include "ClickComponent.h"
#include "SpriteRenderer.h"
#include "AssetsModule.h"
#include "Engine.h"
#include "TextRenderer.h"

MainScene::MainScene() : Scene("MainScene")
{
    AssetsModule* assets = Engine::GetInstance()
        ->GetModuleManager()
        ->GetModule<AssetsModule>();

    // 🟦 ZONE GAUCHE
    {
        GameObject* leftZone = CreateGameObject("LeftZone");

        auto* rect = leftZone->CreateComponent<RectangleShapeRenderer>();
        rect->SetSize(Maths::Vector2f(1000.f, 800.f));
        rect->SetColor(sf::Color(0, 0, 255, 100));

        leftZone->SetPosition({ 0.f, 0.f });
    }

    // 🟥 ZONE DROITE
    {
        GameObject* rightZone = CreateGameObject("RightZone");

        auto* rect = rightZone->CreateComponent<RectangleShapeRenderer>();
        rect->SetSize(Maths::Vector2f(500.f, 800.f));
        rect->SetColor(sf::Color(0, 0, 255, 100));

        rightZone->SetPosition({ 1000.f, 0.f });
    }

    // 🟡 CLICKER SPRITE
    {
        GameObject* clicker = CreateGameObject("Clicker");

        auto* sprite = clicker->CreateComponent<SpriteRenderer>(
            assets->LoadAsset<Texture>("clicker.png") // 👉 mets ton image ici
        );

        clicker->SetPosition({ 500.f, 400.f }); // centre gauche

        // 👉 logique de clic
        clicker->CreateComponent<ClickComponent>();
    }

}