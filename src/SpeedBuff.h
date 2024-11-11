#pragma once

#include "Rectangle.h"

class Game;

class SpeedBuff : public Rectangle
{
public:
	SpeedBuff(Game* game, sf::Vector2f position);
	virtual ~SpeedBuff(){}

	void update(float deltaTime);

	void setIsPickedUp(bool isPickedUp) { m_isPickedUp = isPickedUp; }
	bool isPickedUp() { return m_isPickedUp; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Game* m_pGame;
	bool m_isPickedUp = false;
};
