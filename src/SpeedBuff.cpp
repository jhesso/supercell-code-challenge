#include "SpeedBuff.h"
#include "Constants.h"
#include "Game.h"
#include "Player.h"

SpeedBuff::SpeedBuff(Game* game, sf::Vector2f position) :
	Rectangle(sf::Vector2f(PowerUpWidth, PowerUpHeight)),
	m_pGame(game)
{
	setPosition(position);
	setOrigin(sf::Vector2f(0.0f, 0.0f));
	setIsPickedUp(false);
	setColor(sf::Color::White);
}

void SpeedBuff::update(float deltaTime) //? delta time not needed??
{
	if (m_isPickedUp)
		return;

	Player* pPlayer = m_pGame->getPlayer();

	if (collidesWith(pPlayer))
	{
		m_isPickedUp = true;
		pPlayer->setSpeed(pPlayer->getSpeed() * 1.1f); //? maybe need to adjust the speedup amount at a certain point to not increase so much
	}
}

void SpeedBuff::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::RectangleShape graphicsRect(getSize());
	graphicsRect.setFillColor(getColor());
	graphicsRect.setPosition(getPosition());
	target.draw(graphicsRect);
}
