#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "Constants.h"

class Player;
// class Game;
class GameInput;
class Vampire;
class SpeedBuff;
class HealthPack;

namespace sf { class Clock; }

class Game : public sf::Drawable
{
public:

    enum class State
    {
        WAITING,
        ACTIVE,
        PAUSED,
    };

    Game(sf::RenderWindow& window);
    ~Game();

    bool initialise();
    void update(float deltaTime);
    void resetLevel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    State getState() const { return m_state; }
    void setState(State state);

    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);

    Player* getPlayer() const;
    sf::Texture* getPlayerTexture() { return &m_playerTexture; }
    sf::Texture* getVampireTexture() { return &m_vampTexture; }

    void vampireSpawner(float deltaTime);
    void powerUpSpawner(float deltaTime);
    void healthPackSpawner(float deltaTime);

    void triggerScreenFlash();
    void triggerScreenShake(float duration, float intensity);

private:
    sf::RenderWindow& m_window;

    std::unique_ptr<Player> m_pPlayer;

    std::vector<std::unique_ptr<Vampire>> m_pVampires;
    std::vector<std::unique_ptr<SpeedBuff>> m_pPowerUps;
    std::vector<std::unique_ptr<HealthPack>> m_pHealthPacks;

    State m_state;
    std::unique_ptr<sf::Clock> m_pClock;
    std::unique_ptr<GameInput> m_pGameInput;

    float m_pausedTime = 0.0f;

    static constexpr float MIN_VAMPIRE_COOLDOWN = 0.1f;

    float m_vampireCooldown = 0.0f;
    float m_nextVampireCooldown = 2.0f;
    int m_spawnCount = 0;

    float m_powerUpCooldown = 0.0f;
    float m_nextPowerUpCooldown = 5.0f;
    int m_powerUpCount = 0;

    float m_healthPackCooldown = 0.0f;
    float m_nextHealthPackCooldown = 2.0f;
    int m_healthPackCount = 0;

    bool m_screenFlash = false;
    float m_flashDuration = 0.0f;
    bool m_screenShake = false;
    float m_shakeDuration = 0.0f;
    float m_shakeIntensity = 0.0f;
    sf::View m_originalView;

    sf::Font m_font;
    sf::Texture m_vampTexture;
    sf::Texture m_playerTexture;
};
