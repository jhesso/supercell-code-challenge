#pragma once

#include "Rectangle.h"
#include "Weapon.h"
#include "Constants.h"

#include <memory>

struct InputData;

class Game;
class Weapon;

enum eDirection
{
    LEFT,
    RIGHT
};

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}

    bool initialise();
    void move(InputData inputData, float deltaTime);
    void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }

    Weapon* getWeapon() { return m_pWeapon.get(); }

    float getSpeed(void) { return m_playerSpeed; }
    void setSpeed(float f) { m_playerSpeed = f; }

private:
    bool    m_isDead = false;
    eDirection m_direction = LEFT;
    Game*   m_pGame;
    float   m_playerSpeed = PlayerDefaultSpeed;
    std::unique_ptr<Weapon> m_pWeapon;
};
