#pragma once
#include "Core/Scene.h"
#include "../PlayerController.h"
#include "Components/SpriteRenderer.h"
#include "Modules/AssetsModule.h"
#include "Assets/Texture.h"
#include "Engine.h"
#include <iostream>
namespace BulletHellAlice
{
    class GameScene : public Scene
    {
    public:
        GameScene() : Scene("BulletHellScene", true) {}

        void Start() const
        {
            Scene* nonConstScene = const_cast<GameScene*>(this);

            // 1. Création du GameObject Joueur
            GameObject* player = nonConstScene->CreateGameObject("Player");
            player->SetPosition({ 400.f, 500.f }); // Position de départ en bas au centre

            // 2. Récupération de l'AssetsModule pour charger l'image
            // On passe par l'Engine pour accéder au ModuleManager
            Engine* engine = Engine::GetInstance();
            AssetsModule* am = engine->GetModuleManager()->GetModule<AssetsModule>();

            // Remplace "player.png" par le nom exact de ton fichier dans le dossier Assets
            Texture* playerTex = am->LoadAsset<Texture>("player.png");

            if (playerTex)
            {
                std::cout << "hello";
                player->CreateComponent<SpriteRenderer>(playerTex);
            }

            // 3. Ajout du script de mouvement
            player->CreateComponent<PlayerController>();
        }
    };
}