#include "UIComponent.h"
#include "TDScene.h"
#include "TowerComponent.h"
#include <iostream>
#include <cmath>
#include <cstdint>

void UIComponent::Render(sf::RenderWindow* window)
{
    static sf::Font font;
    static bool initialized = false;

    if (!initialized)
    {
        if (!font.openFromFile("arial.ttf"))
        {
            std::cout << "FONT ERROR" << std::endl;
        }
        initialized = true;
    }

    TD::TDScene* scene = static_cast<TD::TDScene*>(GetOwner()->GetScene());
    if (!scene)
        return;

    TD::GameState state = scene->GetGameState();

    sf::Text text(font);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);

    // =============================
    // 🎮 MENU
    // =============================
    if (state == TD::GameState::Menu)
    {
        text.setString(
            "TOWER DEFENSE\n\n"
            "[ENTER] START\n"
            "[ESC] QUIT"
        );

        text.setPosition({ 600.f, 300.f });
        window->draw(text);
        return;
    }

    // =============================
    // 💀 GAME OVER
    // =============================
    if (state == TD::GameState::GameOver)
    {
        text.setString(
            "GAME OVER\n\n"
            "[ENTER] RESTART"
        );

        text.setPosition({ 600.f, 300.f });
        window->draw(text);
        return;
    }

    // =============================
    // 🏆 VICTORY
    // =============================
    if (state == TD::GameState::Victory)
    {
        text.setString(
            "VICTORY!\n\n"
            "2000 ENEMIES KILLED"
        );

        text.setPosition({ 600.f, 300.f });
        window->draw(text);
        return;
    }

    // =============================
    // 🟩 GAMEPLAY UI (Playing uniquement)
    // =============================

    // 💰 PANEL GLOBAL
    sf::RectangleShape globalPanel;
    globalPanel.setSize(sf::Vector2f(250.f, 100.f));
    globalPanel.setPosition({ 20.f, 20.f });
    globalPanel.setFillColor(sf::Color(20, 20, 20, 200));
    globalPanel.setOutlineThickness(2.f);
    globalPanel.setOutlineColor(sf::Color(100, 100, 100));

    sf::Text globalText(font);
    globalText.setCharacterSize(24);
    globalText.setFillColor(sf::Color::White);

    std::string globalUI =
        "Money: " + std::to_string(scene->GetMoney()) +
        "\nWave: " + std::to_string(scene->GetWave());

    globalText.setString(globalUI);
    globalText.setPosition({ 35.f, 35.f });

    window->draw(globalPanel);
    window->draw(globalText);

    // =============================
    // 🗼 PANEL TOUR
    // =============================
    if (!scene->IsTowerUIVisible())
        return;

    GameObject* tower = scene->GetSelectedTower();
    if (!tower)
        return;

    TowerComponent* comp = nullptr;

    for (auto& c : tower->GetComponents())
    {
        comp = dynamic_cast<TowerComponent*>(c.get());
        if (comp)
            break;
    }

    if (!comp)
        return;

    float screenWidth = window->getSize().x;

    sf::RectangleShape panel;
    panel.setSize(sf::Vector2f(320.f, 220.f));
    panel.setPosition({ screenWidth - 340.f, 20.f });
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(100, 100, 100));

    // animation
    static float t = 0.f;
    t += 0.05f;
    float alpha = 180 + std::sin(t) * 20;

    panel.setFillColor(sf::Color(40, 40, 40, static_cast<uint8_t>(alpha)));

    sf::Text towerText(font);
    towerText.setCharacterSize(24);
    towerText.setLineSpacing(1.3f);

    std::string uiText =
        "TOWER\n"
        "Level: " + std::to_string(comp->GetLevel()) +
        "\nUpgrade: " + std::to_string(comp->upgradeCost) +
        "\nRange: " + std::to_string((int)comp->GetRange()) +
        "\nSpeed: " + std::to_string(comp->GetFireRate()) +
        "\n\n[U] Upgrade\n[TAB] Hide";

    if (scene->GetMoney() < comp->upgradeCost)
        towerText.setFillColor(sf::Color(255, 100, 100));
    else
        towerText.setFillColor(sf::Color(100, 255, 100));

    towerText.setString(uiText);
    towerText.setPosition({ screenWidth - 320.f, 40.f });

    window->draw(panel);
    window->draw(towerText);
}