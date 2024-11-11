#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Game.h"

GameInput::GameInput(Game* pGame, Player* pPlayer) :
    m_pGame(pGame), m_pPlayer(pPlayer)
{
}

GameInput::~GameInput()
{
}

void GameInput::update(float deltaTime)
{
    if (m_inputData.hasInputs())
    {
        m_pPlayer->move(m_inputData, deltaTime);
    }

    if (m_inputData.m_space)
    {
        m_pPlayer->attack();
    }
    if (m_inputData.m_escape)
        m_pGame->setState(Game::State::PAUSED);
    if (m_pGame->getState() == Game::State::PAUSED && !m_inputData.m_escape)
        m_pGame->setState(Game::State::ACTIVE);
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up || key == sf::Keyboard::W)
    {
        m_inputData.m_movingUp = true;
    }
    else if (key == sf::Keyboard::Down || key == sf::Keyboard::S)
    {
        m_inputData.m_movingDown = true;
    }
    else if (key == sf::Keyboard::Left || key == sf::Keyboard::A)
    {
        m_inputData.m_movingLeft = true;
    }
    else if (key == sf::Keyboard::Right || key == sf::Keyboard::D)
    {
        m_inputData.m_movingRight = true;
    }
    else if (key == sf::Keyboard::Space)
    {
        if (m_inputData.m_spaceReleased)
        {
            m_inputData.m_space = true;
        }
        m_inputData.m_spaceReleased = false;
    }
    else if (key == sf::Keyboard::Escape)
    {
        if (m_inputData.m_escapeReleased)
            m_inputData.m_escape = !m_inputData.m_escape;
        m_inputData.m_escapeReleased = false;
    }
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up || key == sf::Keyboard::W)
    {
        m_inputData.m_movingUp = false;
    }
    else if (key == sf::Keyboard::Down || key == sf::Keyboard::S)
    {
        m_inputData.m_movingDown = false;
    }
    else if (key == sf::Keyboard::Left || key == sf::Keyboard::A)
    {
        m_inputData.m_movingLeft = false;
    }
    else if (key == sf::Keyboard::Right || key == sf::Keyboard::D)
    {
        m_inputData.m_movingRight = false;
    }
    else if (key == sf::Keyboard::Space)
    {
        m_inputData.m_space = false;
        m_inputData.m_spaceReleased = true;
    }
    else if (key == sf::Keyboard::Escape)
    {
        m_inputData.m_escapeReleased = true;
    }
}
