#pragma once

#include "Rectangle.h"
#include <SFML/Graphics.hpp>
#include "Game.h"

class Game;

class HealthPack : public Rectangle
{
public:
	HealthPack(Game* game, sf::Vector2f position);
	virtual ~HealthPack(){}

	void update(float deltaTime);

	void setIsPickedUp(bool isPickedUp) { m_isPickedUp = isPickedUp; }
	bool isPickedUp() { return m_isPickedUp; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Game* m_pGame;
	bool m_isPickedUp = false;
};
