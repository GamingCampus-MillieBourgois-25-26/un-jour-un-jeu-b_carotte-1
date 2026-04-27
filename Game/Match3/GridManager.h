#pragma once

#include <vector>
#include <map>
#include <string>
#include "Core/Component.h"
#include "Maths/Vector2.h"

class GameObject;

namespace Match3 {

    enum class CandyType {
        EMPTY = 0,
        RED,
        BLUE,
        GREEN,
        YELLOW,
        PURPLE,
        COUNT
    };

    class GridManager : public Component
    {
    public:
        void Start() override;
        void Update(float _delta_time) override;

        void SetPool(std::map<CandyType, std::vector<GameObject*>>* _pool) {
            pool = _pool;
        }

        CandyType GetCandyAt(int x, int y) const;
        Maths::Vector2f GetWorldPosition(int x, int y) const;

    private:
        int width = 8;
        int height = 8;
        float cellSize = 64.f;

        std::vector<std::vector<CandyType>> grid;

        std::map<CandyType, std::vector<GameObject*>>* pool = nullptr;

        Maths::Vector2i selectedTile = { -1, -1 };

        void InitializeGrid();
        void HandleInput();
        void SpawnCandyVisual(CandyType _type, int _x, int _y);

        bool CheckMatches();
        void ResolveMatches();
    };
}