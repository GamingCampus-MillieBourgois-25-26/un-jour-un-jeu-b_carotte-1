#include "GridManager.h"
#include "Core/GameObject.h"
#include "Modules/InputModule.h"
#include <iostream>

namespace Match3 {

    void GridManager::Start() {
        InitializeGrid();
        DebugPrintGrid();
    }

    void GridManager::Update(float _delta_time) {
        if (currentState == GameState::IDLE) {
            HandleInput();
        }
    }

    void GridManager::InitializeGrid() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        grid.resize(width, std::vector<CandyType>(height, CandyType::EMPTY));

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                CandyType newType;

                do {
                    newType = static_cast<CandyType>((std::rand() % (static_cast<int>(CandyType::COUNT) - 1)) + 1);
                } while (
                    (x >= 2 && grid[x - 1][y] == newType && grid[x - 2][y] == newType) ||
                    (y >= 2 && grid[x][y - 1] == newType && grid[x][y - 2] == newType)
                    );

                grid[x][y] = newType;
                SpawnCandyVisual(newType, x, y);
            }
        }
    }

    void GridManager::RefillGrid() {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (grid[x][y] == CandyType::EMPTY) {
                    grid[x][y] = static_cast<CandyType>((rand() % ((int)CandyType::COUNT - 1)) + 1);
                }
            }
        }
    }

    void GridManager::HandleInput() {
        if (InputModule::GetMouseButtonDown(sf::Mouse::Button::Left)) {
            Maths::Vector2i mPos = InputModule::GetMousePosition();
            Maths::Vector2f origin = GetOwner()->GetPosition();

            float localX = static_cast<float>(mPos.x) - origin.x;
            float localY = static_cast<float>(mPos.y) - origin.y;

            int gx = static_cast<int>(localX / cellSize);
            int gy = static_cast<int>(localY / cellSize);

            if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
                if (selectedTile.x == -1) {
                    selectedTile = Maths::Vector2i(gx, gy);
                }
                else {
                    int diffX = std::abs(selectedTile.x - gx);
                    int diffY = std::abs(selectedTile.y - gy);

                    if ((diffX == 1 && diffY == 0) || (diffX == 0 && diffY == 1)) {
                        Swap(selectedTile, Maths::Vector2i(gx, gy));
                    }
                    selectedTile = Maths::Vector2i(-1, -1);
                }
            }
        }
    }

    void GridManager::Swap(Maths::Vector2i a, Maths::Vector2i b) {
        CandyType temp = grid[a.x][a.y];
        grid[a.x][a.y] = grid[b.x][b.y];
        grid[b.x][b.y] = temp;

        if (!CheckMatches()) {
            grid[b.x][b.y] = grid[a.x][a.y];
            grid[a.x][a.y] = temp;
            std::cout << "Swap invalide !" << std::endl;
        }
        else {
            std::cout << "Match trouve !" << std::endl;
            ResolveMatches();
        }
    }

    bool GridManager::CheckMatches() {
        bool hasMatch = false;
        std::vector<std::vector<bool>> toDestroy(width, std::vector<bool>(height, false));

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width - 2; ++x) {
                CandyType type = grid[x][y];
                if (type != CandyType::EMPTY && grid[x + 1][y] == type && grid[x + 2][y] == type) {
                    toDestroy[x][y] = toDestroy[x + 1][y] = toDestroy[x + 2][y] = true;
                    hasMatch = true;
                }
            }
        }
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height - 2; ++y) {
                CandyType type = grid[x][y];
                if (type != CandyType::EMPTY && grid[x][y + 1] == type && grid[x][y + 2] == type) {
                    toDestroy[x][y] = toDestroy[x][y + 1] = toDestroy[x][y + 2] = true;
                    hasMatch = true;
                }
            }
        }

        if (hasMatch) {
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    if (toDestroy[x][y]) grid[x][y] = CandyType::EMPTY;
                }
            }
        }
        return hasMatch;
    }

    void GridManager::ResolveMatches() {
        currentState = GameState::CHECKING;

        while (CheckMatches()) {
            UpdateVisuals();
            ApplyGravity();
            RefillGrid();
            UpdateVisuals();
            DebugPrintGrid();
        }

        currentState = GameState::IDLE;
    }

    void GridManager::ApplyGravity()
    {
        for (int x = 0; x < width; ++x) {
            int emptySlot = height - 1;
            for (int y = height - 1; y >= 0; --y) {
                if (grid[x][y] != CandyType::EMPTY) {
                    if (y != emptySlot) {
                        grid[x][emptySlot] = grid[x][y];
                        grid[x][y] = CandyType::EMPTY;
                    }
                    emptySlot--;
                }
            }
        }
    }

    void GridManager::UpdateVisuals()
    {
        if (pool == nullptr) return;

        for (auto const& [type, list] : *pool) {
            for (GameObject* obj : list) {
                obj->Disable();
            }
        }

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                CandyType type = grid[x][y];
                if (type != CandyType::EMPTY) {
                    SpawnCandyVisual(type, x, y);
                }
            }
        }
    }

    void GridManager::SpawnCandyVisual(CandyType _type, int _x, int _y) {
        if (_type == CandyType::EMPTY || pool == nullptr) return;

        std::vector<GameObject*>& list = (*pool)[_type];
        for (GameObject* obj : list) {
            if (!obj->IsEnabled()) {
                obj->Enable();
                obj->SetPosition(GetWorldPosition(_x, _y));
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

    void GridManager::DebugPrintGrid() {
        std::cout << "--- GRID STATE ---" << std::endl;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::cout << (int)grid[x][y] << " ";
            }
            std::cout << std::endl;
        }
    }
}