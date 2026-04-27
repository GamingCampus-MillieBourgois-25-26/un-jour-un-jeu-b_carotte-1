#include "GridManager.h"
#include "Core/GameObject.h"
#include "Modules/InputModule.h"
#include <iostream>

namespace Match3 {

    void GridManager::Start() {
        InitializeGrid();
    }

    void GridManager::Update(float _delta_time) {
        HandleInput();
    }

    void GridManager::InitializeGrid() {
        grid.resize(width, std::vector<CandyType>(height, CandyType::EMPTY));

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                CandyType newType;
                do {
                    newType = static_cast<CandyType>((rand() % ((int)CandyType::COUNT - 1)) + 1);
                } while ((x >= 2 && grid[x - 1][y] == newType && grid[x - 2][y] == newType) ||
                    (y >= 2 && grid[x][y - 1] == newType && grid[x][y - 2] == newType));

                grid[x][y] = newType;
                SpawnCandyVisual(newType, x, y);
            }
        }
    }

    void GridManager::HandleInput() {
        if (InputModule::GetMouseButtonDown(sf::Mouse::Button::Left)) {

            Maths::Vector2i mPos = InputModule::GetMousePosition();

            Maths::Vector2f gridOrigin = GetOwner()->GetPosition();

            float localX = static_cast<float>(mPos.x) - gridOrigin.x;
            float localY = static_cast<float>(mPos.y) - gridOrigin.y;

            int gx = static_cast<int>(localX / cellSize);
            int gy = static_cast<int>(localY / cellSize);

            if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
                if (selectedTile.x == -1) {
                    selectedTile = Maths::Vector2i(gx, gy);
                    std::cout << "Selection : " << gx << "," << gy << std::endl;
                }
                else {
                    std::cout << "Swap essai entre (" << selectedTile.x << "," << selectedTile.y
                        << ") et (" << gx << "," << gy << ")" << std::endl;
                    selectedTile = Maths::Vector2i(-1, -1);
                }
            }
        }
    }

    void GridManager::SpawnCandyVisual(CandyType _type, int _x, int _y) {
        if (_type == CandyType::EMPTY || pool == nullptr) return;

        std::vector<GameObject*>& list = (*pool)[_type];

        for (GameObject* candyObj : list) {
            if (!candyObj->IsEnabled()) {
                candyObj->Enable();
                candyObj->SetPosition(GetWorldPosition(_x, _y));
                return;
            }
        }
    }

    Maths::Vector2f GridManager::GetWorldPosition(int x, int y) const {
        Maths::Vector2f origin = GetOwner()->GetPosition();
        return Maths::Vector2f(origin.x + (x * cellSize), origin.y + (y * cellSize));
    }

    CandyType GridManager::GetCandyAt(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) return CandyType::EMPTY;
        return grid[x][y];
    }
}