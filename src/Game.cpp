#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Rectangle.h"
#include "Vampire.h"
#include "SpeedBuff.h"

Game::Game(sf::RenderWindow& window) :
    m_window(window),
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_vampireCooldown(2.0f),
    m_nextVampireCooldown(2.0f),
    m_powerUpCooldown(5.0f),
    m_nextPowerUpCooldown(5.0f)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
}

bool Game::initialise()
{
    /*
        PAY ATTENTION HIVER!
            If you want to load any assets (fonts, textures) please use the pattern shown below
    */

    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

    resetLevel();
    return true;
}

void Game::resetLevel()
{
    m_pVampires.clear();
    m_pPowerUps.clear();
    m_powerUpCooldown = 5.0f;
    m_pausedTime = 0.0f;
    m_pClock->restart();

    m_pPlayer->initialise();
    m_pClock->restart();
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (m_pClock->getElapsedTime().asSeconds() >= 3.f)
            {
                m_state = State::ACTIVE;
                m_pClock->restart();
            }
        }
        break;

        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->update(deltaTime);

            vampireSpawner(deltaTime);
            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
            }

            powerUpSpawner(deltaTime);
            for (auto& temp : m_pPowerUps)
            {
                temp->update(deltaTime);
            }

            if (m_pPlayer->isDead())
            {
                m_state = State::WAITING;
                resetLevel();
            }
        }
        break;

        case State::PAUSED:
        {
            m_pGameInput->update(deltaTime);
        }
        break;
    }

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            continue;
        }
        i++;
    }
    i = 0;
    while (i < m_pPowerUps.size())
    {
        if (m_pPowerUps[i]->isPickedUp())
        {
            std::swap(m_pPowerUps[i], m_pPowerUps.back());
            m_pPowerUps.pop_back();
            m_powerUpCount--;
            if (m_powerUpCount < 0)
                m_powerUpCount = 0;
            continue;
        }
        i++;
    }

    if (m_screenFlash)
    {
        m_flashDuration -= deltaTime;
        if (m_flashDuration <= 0.0f)
            m_screenFlash = false;
    }

    if (m_screenShake)
    {
        m_shakeDuration -= deltaTime;
        if (m_shakeDuration <= 0.0f)
        {
            m_screenShake = false;
            m_window.setView(m_originalView);
        }
        else
        {
            float offsetX = (rand() % 100 - 50) / 50.0f * m_shakeIntensity;
            float offsetY = (rand() % 100 - 50) / 50.0f * m_shakeIntensity;
            sf::View shakeView = m_originalView;
            shakeView.move(offsetX, offsetY);
            m_window.setView(shakeView);
        }
    }
}

void Game::setState(State state)
{
    if (state == State::PAUSED && m_state != State::PAUSED)
    {
        m_pausedTime += m_pClock->getElapsedTime().asSeconds();
        std::cout << "Pause time: " << m_pausedTime << std::endl;
    }
    else if (state != State::PAUSED && m_state == State::PAUSED)
    {
        m_pClock->restart();
    }
    m_state = state;
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //  Draw texts.
    if (m_state == State::WAITING)
    {
        sf::Text startText;
        startText.setFont(m_font);
        startText.setString("Game Start!");
        startText.setFillColor(sf::Color::White);
        startText.setPosition(sf::Vector2f((ScreenWidth - startText.getLocalBounds().getSize().x) * 0.5, 80.0f));
        startText.setStyle(sf::Text::Bold);
        target.draw(startText);
    }
    else
    {
        sf::Text timerText;
        timerText.setFont(m_font);
        timerText.setFillColor(sf::Color::White);
        timerText.setStyle(sf::Text::Bold);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        if (m_state != State::PAUSED)
            ss << (m_pausedTime + m_pClock->getElapsedTime().asSeconds());
        else
           ss << m_pausedTime;

        timerText.setString(ss.str());
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);

        sf::Text hpText;
        hpText.setFont(m_font);
        hpText.setFillColor(sf::Color::Red);
        hpText.setCharacterSize(15);
        hpText.setStyle(sf::Text::Bold);

        std::stringstream hpStream;
        hpStream << "HP: " << static_cast<int>(m_pPlayer->getHp());

        hpText.setString(hpStream.str());
        hpText.setPosition(sf::Vector2f((ScreenWidth - hpText.getLocalBounds().getSize().x) * 0.5, 60.0f));
        target.draw(hpText);
    }

    if (m_state == State::PAUSED)
    {
        sf::RectangleShape pauseScreen(sf::Vector2f(ScreenWidth, ScreenHeight));
        pauseScreen.setFillColor(sf::Color(255, 255, 255, 50));
        pauseScreen.setPosition(sf::Vector2f(0, 0));
        target.draw(pauseScreen);
        sf::Text pauseText;
        pauseText.setFont(m_font);
        pauseText.setString("Game Paused");
        pauseText.setFillColor(sf::Color::White);
        pauseText.setPosition(sf::Vector2f((ScreenWidth - pauseText.getLocalBounds().getSize().x) * 0.5, 80.0f));
        pauseText.setStyle(sf::Text::Bold);
        target.draw(pauseText);
    }

    sf::Text controlsText;
    controlsText.setFont(m_font);
    controlsText.setString("WASD/Arrow Keys: Move | SPACE: Attack | Escape: Pause/Unpause");
    controlsText.setFillColor(sf::Color::White);
    controlsText.setPosition(sf::Vector2f(10, ScreenHeight - 20));
    controlsText.setCharacterSize(15);
    controlsText.setStyle(sf::Text::Bold);
    target.draw(controlsText);

    // Draw player.
    m_pPlayer->draw(target, states);

    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }
    for (auto& temp : m_pPowerUps)
    {
        temp->draw(target, states);
    }

    if (m_screenFlash)
    {
        sf::RectangleShape flashOverlay(sf::Vector2f(ScreenWidth, ScreenHeight));
        flashOverlay.setFillColor(sf::Color(255, 0, 0, 128));
        target.draw(flashOverlay);
    }
}


void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

Player* Game::getPlayer() const
{
    return m_pPlayer.get();
}

sf::Vector2f getSpawnPosition(int type) // 1 = vampire, 2 = powerUp
{

    float randomXPos = static_cast<float>(rand() % ScreenWidth);
    float randomYPos = static_cast<float>(rand() % ScreenHeight);

    if (type == 1)
    {
        float distToRight = (float) ScreenWidth - randomXPos;
        float distToBottom = (float) ScreenHeight - randomYPos;

        float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
        float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

        if (abs(xMinDist) < abs(yMinDist))
            randomXPos += xMinDist;
        else
            randomYPos += yMinDist;
    }

    return sf::Vector2f(randomXPos, randomYPos);
}

void Game::vampireSpawner(float deltaTime)
{
    if (m_vampireCooldown > 0.0f)
    {
        m_vampireCooldown -= deltaTime;
        return;
    }

    sf::Vector2f spawnPosition = getSpawnPosition(1);

    m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_spawnCount % 5 == 0)
    {
        m_nextVampireCooldown -= 0.1f;
    }
    m_vampireCooldown = m_nextVampireCooldown;
}

void Game::powerUpSpawner(float deltaTime)
{
    if (m_powerUpCount == 3)
        return;
    if (m_powerUpCooldown > 0.0f)
    {
        m_powerUpCooldown -= deltaTime;
        return;
    }

    sf::Vector2f spawnPosition = getSpawnPosition(2);
    m_pPowerUps.push_back(std::make_unique<SpeedBuff>(this, spawnPosition)); //? to add more powerups need to change <speedBuff> to something else

    m_powerUpCount++;
    m_powerUpCooldown = m_nextPowerUpCooldown;
    // std::cout << "Spawned a powerup!\n";
}

void Game::triggerScreenFlash(void)
{
    m_screenFlash = true;
    m_flashDuration = 0.1f;
}

void Game::triggerScreenShake(float duration, float intensity)
{
    m_screenShake = true;
    m_shakeDuration = duration;
    m_shakeIntensity = intensity;
    m_originalView = m_window.getView();
}
