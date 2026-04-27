#pragma once

#include "Component.h"
#include <SFML/Graphics.hpp>

class UIComponent : public Component
{
public:
    void Render(sf::RenderWindow* window) override;

private:
};