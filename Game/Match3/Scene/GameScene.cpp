#include "GameScene.h"
#include "../GridManager.h"
#include "SpriteRenderer.h"
#include "Modules/AssetsModule.h"

namespace Match3 {
    GameScene::GameScene(const std::string& _name, bool _enabled_at_start)
        : Scene(_name, _enabled_at_start)
    {
        // 1. Créer la logique de grille
        GameObject* gridLogic = CreateGameObject("GridLogic");
        GridManager* manager = gridLogic->CreateComponent<GridManager>();

        // 2. Créer le pool d'objets
        CreatePool();

        // 3. Passer le pool au manager (il faudra ajouter cette méthode dans GridManager)
         manager->SetPool(&candyPool); 
    }

   void GameScene::CreatePool() {
    AssetsModule* assets = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();

    for (int i = 1; i < (int)CandyType::COUNT; ++i) {
        CandyType type = static_cast<CandyType>(i);

        Texture* tex = nullptr;

        switch (type) {
        case CandyType::RED:
            tex = assets->LoadAsset<Texture>("Carre.png");
            break;
        case CandyType::BLUE:
            tex = assets->LoadAsset<Texture>("Triangle.png");
            break;
        case CandyType::GREEN:
            tex = assets->LoadAsset<Texture>("Hexagone.png");
            break;
        case CandyType::YELLOW:
            tex = assets->LoadAsset<Texture>("Diamand.png");
            break;
        case CandyType::PURPLE:
            tex = assets->LoadAsset<Texture>("Lozange.png");
            break;
        default:
            break;
        }

        for (int j = 0; j < 50; ++j) {
            GameObject* candy = CreateGameObject("Candy_" + std::to_string(i));

            auto* renderer = candy->CreateComponent<SpriteRenderer>(tex);

            sf::Vector2u size = tex->GetTexture().getSize();

            float scaleX = 64.f / size.x;
            float scaleY = 64.f / size.y;

            candy->SetScale({ scaleX, scaleY });

            candy->Disable();
            candyPool[type].push_back(candy);
        }
    }
}
}