#include "FloatingTextComponent.h"
#include "GameObject.h"

void FloatingTextComponent::Init(const std::string& text)
{
    displayText = text;
}

void FloatingTextComponent::Start()
{
    textRenderer = GetOwner()->CreateComponent<TextRenderer>(displayText);
    textRenderer->SetColor(sf::Color::Yellow);
}

void FloatingTextComponent::Update(float deltaTime)
{
    auto pos = GetOwner()->GetPosition();
    pos.y -= speed * deltaTime;
    GetOwner()->SetPosition(pos);

    lifetime -= deltaTime;

    if (lifetime <= 0.f)
    {
        GetOwner()->MarkForDeletion();
    }
}