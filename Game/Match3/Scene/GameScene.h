#pragma once
#include "Core/Scene.h"
#include "../GridManager.h"
#include <map>
#include <vector>

namespace Match3 {
    class GameScene : public Scene
    {
    public:
        explicit GameScene(const std::string& _name = "Match3Scene", bool _enabled_at_start = true);

    private:
        // Une map qui stocke des listes d'objets désactivés par type
        std::map<CandyType, std::vector<GameObject*>> candyPool;

        void CreatePool();
    };
}