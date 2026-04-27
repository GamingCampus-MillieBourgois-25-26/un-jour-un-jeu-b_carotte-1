#include "BulletHellPlayer.h"

#include "Core/GameObject.h"
#include "Modules/InputModule.h"
#include "Engine.h"

void BulletHellPlayer::Update(const float deltaTime)
{
    Maths::Vector2f position = GetOwner()->GetPosition();

    if (InputModule::GetKey(sf::Keyboard::Key::D))
        position.x += speed * deltaTime;

    if (InputModule::GetKey(sf::Keyboard::Key::Q))
        position.x -= speed * deltaTime;

    if (InputModule::GetKey(sf::Keyboard::Key::Z))
        position.y -= speed * deltaTime;

    if (InputModule::GetKey(sf::Keyboard::Key::S))
        position.y += speed * deltaTime;

    GetOwner()->SetPosition(position);
}