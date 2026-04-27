#pragma once

#include "Component.h"
#include <SFML/Graphics/RenderWindow.hpp>

class InputComponent : public Component
{
public:
    void Update(float dt) override;

    void SetWindow(sf::RenderWindow* win)
    {
        window = win;
    }

private:
    sf::RenderWindow* window = nullptr;
};