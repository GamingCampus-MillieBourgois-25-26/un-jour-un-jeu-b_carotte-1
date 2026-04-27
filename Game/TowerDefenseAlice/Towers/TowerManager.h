#pragma once
#include "Core/Component.h"
#include "Engine.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Modules/InputModule.h"
#include "Modules/WindowModule.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace TowerDefenseAlice
{
    class TowerManager : public Component {
        float placeTimer = 0.0f; 
        float delayBetweenTowers = 0.5f;


    public:
        int playerGold = 100;
        int towerCost = 50;
        void Update(float _delta_time) override {
            placeTimer += _delta_time;
            CheckEnemiesBaseReached();

            auto* moduleManager = Engine::GetInstance()->GetModuleManager();
            auto* windowModule = moduleManager->GetModule<WindowModule>();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && placeTimer >= delayBetweenTowers) {
                if (windowModule) {
                    sf::RenderWindow* windowPtr = windowModule->GetWindow();

                    if (windowPtr) {
                        sf::Vector2i mousePosI = sf::Mouse::getPosition(*windowPtr);
                        Maths::Vector2f posF = {
                            static_cast<float>(mousePosI.x),
                            static_cast<float>(mousePosI.y)
                        };

                        PlaceTower(posF);
                        placeTimer = 0.0f;
                    }
                }
            }
        }

        void AddGold(int _amount) {
            playerGold += _amount;
            std::cout << "Argent gagne ! Total : " << playerGold << std::endl;
        }

    private:
        void CheckEnemiesBaseReached() {
            Scene* scene = GetOwner()->GetScene();
            if (!scene) return;

            const auto& gameObjects = scene->GetGameObjects();
            for (size_t i = 0; i < gameObjects.size(); ++i) {
                GameObject* obj = gameObjects[i].get();

                if (obj->IsEnabled() && obj->GetName().find("Pool_Enemy") != std::string::npos) {

                    if (obj->GetPosition().x >= 1550.0f) {

                        std::cout << "ALERTE : L'ennemi " << obj->GetName() << " a force le passage !" << std::endl;
                        obj->Disable();
                    }
                }
            }
        }
        void PlaceTower(Maths::Vector2f _pos) {
            if (playerGold < towerCost) {
                printf("Pas assez d'or !\n", towerCost, playerGold);
                return;
            }

            Scene* scene = GetOwner()->GetScene();
            if (!scene) return;

            const auto& gameObjects = scene->GetGameObjects();
            for (size_t i = 0; i < gameObjects.size(); ++i) {
                auto* obj = gameObjects[i].get();

                if (obj->GetName().find("Pool_Tower") != std::string::npos && !obj->IsEnabled()) {
                    obj->SetPosition(_pos);
                    obj->Enable();

                    playerGold -= towerCost;
                    std::cout << "Tour pose Argent restant : " << playerGold << std::endl;
                    return;
                }
            }
        }
    };
}