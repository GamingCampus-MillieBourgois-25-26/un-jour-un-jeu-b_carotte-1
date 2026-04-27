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
        // manager->SetPool(&candyPool); 
    }

    void GameScene::CreatePool() {
        AssetsModule* assets = Engine::GetInstance()->GetModuleManager()->GetModule<AssetsModule>();

        for (int i = 1; i < (int)CandyType::COUNT; ++i) {
            CandyType type = static_cast<CandyType>(i);

            for (int j = 0; j < 50; ++j) {
                GameObject* candy = CreateGameObject("Candy_" + std::to_string(i));

                // LEO
                // Texture* tex = assets->GetAsset<Texture>("Path/To/Candy" + std::to_string(i));
                // candy->CreateComponent<SpriteRenderer>(tex);

                candy->Disable();
                candyPool[type].push_back(candy);
            }
        }
    }
}