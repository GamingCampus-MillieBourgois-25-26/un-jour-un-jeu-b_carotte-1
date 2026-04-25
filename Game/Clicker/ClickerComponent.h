#pragma once

#include "Core/Component.h"
#include "Modules/InputModule.h"
#include "Components/SpriteRenderer.h"
#include "Core/GameObject.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <string>
#include <cstdint> 

struct FloatingText {
    std::string text;
    sf::Vector2f position;
    float lifetime = 1.0f;
    float alpha = 255.f;
};

class ClickerComponent : public Component
{
public:
    ClickerComponent() : score(0), clickPower(1), upgradeCost(10), currentEvolution(0) {}

    // Ajoute une texture à la liste des évolutions futures
    void AddEvolutionTexture(Texture* _tex) { evolutionTextures.push_back(_tex); }

    void AddScore(int _amount) {
        score += _amount;
        SpawnFloatingText("+" + std::to_string(_amount), { screenCenter.x, screenCenter.y - 50.f });
    }
    long long GetScore() const { return score; }
    int GetUpgradeCost() const { return upgradeCost; }
    void SubtractScore(int _amount) { score -= _amount; }
    void IncreasePower() { clickPower += 1; }
    void MultiplyUpgradeCost(float _mult) { upgradeCost = (int)(upgradeCost * _mult); }

    void Render(sf::RenderWindow* _window) override
    {
        static sf::Font globalFont;
        static bool fontLoaded = false;
        if (!fontLoaded) {
            globalFont.openFromFile("Assets\\pixel.ttf");
            fontLoaded = true;
        }

        if (fontLoaded) {
            sf::Text scoreText(globalFont);
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setCharacterSize(30);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setOutlineColor(sf::Color::Black);
            scoreText.setOutlineThickness(3.0f);
            scoreText.setPosition({ 40.f, 40.f });
            _window->draw(scoreText);
        }

        static sf::Font particleFont;
        static bool fLoaded = false;
        if (!fLoaded) fLoaded = particleFont.openFromFile("Assets\\pixel.ttf");

        if (fLoaded) {
            for (auto& p : m_particles) {
                sf::Text t(particleFont);
                t.setString(p.text);
                t.setCharacterSize(24);
                uint8_t alphaVal = static_cast<uint8_t>(p.alpha > 0 ? p.alpha : 0);
                t.setFillColor(sf::Color(255, 255, 255, alphaVal));
                t.setPosition(p.position);
                _window->draw(t);
            }
        }
    }

    void Update(float _delta_time) override
    {
        int targetEvolution = 0;
        if (clickPower >= 250) targetEvolution = 4; 
        if (clickPower >= 100) targetEvolution = 3;
        else if (clickPower >= 50) targetEvolution = 2;
        else if (clickPower >= 10) targetEvolution = 1; 

        if (targetEvolution > currentEvolution && targetEvolution <= (int)evolutionTextures.size()) {
            SpriteRenderer* oldRenderer = GetOwner()->GetComponent<SpriteRenderer>();
            if (oldRenderer) {
                oldRenderer->MarkForDeletion(); //
                GetOwner()->CreateComponent<SpriteRenderer>(evolutionTextures[targetEvolution - 1]);
                currentEvolution = targetEvolution;

                ApplyScaleCentered(4.0f);
            }
        }

        if (InputModule::GetMouseButtonDown(sf::Mouse::Button::Left)) {
            if (CheckCollision()) OnClicked();
        }

        for (int i = (int)m_particles.size() - 1; i >= 0; i--) {
            m_particles[i].lifetime -= _delta_time;
            m_particles[i].position.y -= 100.f * _delta_time;
            m_particles[i].alpha = (m_particles[i].lifetime) * 255.f;
            if (m_particles[i].lifetime <= 0) m_particles.erase(m_particles.begin() + i);
        }

        float targetScale = 2.0f;
        Maths::Vector2f currentScale = GetOwner()->GetScale();
        if (currentScale.x > targetScale) {
            float nextScale = currentScale.x - (currentScale.x - targetScale) * 15.0f * _delta_time;
            if (nextScale < targetScale) nextScale = targetScale;
            ApplyScaleCentered(nextScale);
        }
    }

private:
    long long score = 0;
    const float baseSize = 64.0f;
    const Maths::Vector2f screenCenter = { 400.0f, 300.0f };
    int clickPower = 1;
    int upgradeCost = 10;

    std::vector<Texture*> evolutionTextures;
    int currentEvolution;
    std::vector<FloatingText> m_particles;

    void SpawnFloatingText(std::string _txt, sf::Vector2f _pos) {
        FloatingText p; p.text = _txt; p.position = _pos;
        m_particles.push_back(p);
    }

    bool CheckCollision() {
        Maths::Vector2i mouse = InputModule::GetMousePosition();
        Maths::Vector2f objPos = GetOwner()->GetPosition();
        Maths::Vector2f scale = GetOwner()->GetScale();
        float currentW = baseSize * scale.x + 130.f;
        float currentH = baseSize * scale.y + 130.f;
        return (mouse.x >= objPos.x && mouse.x <= objPos.x + currentW &&
            mouse.y >= objPos.y && mouse.y <= objPos.y + currentH);
    }

    void OnClicked() {
        score += clickPower;
        Maths::Vector2i mPos = InputModule::GetMousePosition();
        SpawnFloatingText("+" + std::to_string(clickPower), { (float)mPos.x, (float)mPos.y });
        ApplyScaleCentered(2.5f);
    }

    void ApplyScaleCentered(float _s) {
        GetOwner()->SetScale({ _s, _s });
        float offset = (baseSize * _s) / 2.0f;
        GetOwner()->SetPosition({ screenCenter.x - offset, screenCenter.y - offset });
    }
};