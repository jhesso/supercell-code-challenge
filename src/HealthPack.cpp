#include "HealthPack.h"
#include "Constants.h"
#include "Game.h"
#include "Player.h"

HealthPack::HealthPack(Game* game, sf::Vector2f position) :
	Rectangle(sf::Vector2f(PowerUpWidth, PowerUpHeight)),
	m_pGame(game)
{
	setPosition(position);
	setOrigin(sf::Vector2f(0.0f, 0.0f));
	setIsPickedUp(false);
	setColor(sf::Color::Green);
}

void HealthPack::update(float deltaTime) //? delta time not needed??
{
	if (m_isPickedUp)
		return;

	Player* pPlayer = m_pGame->getPlayer();

	if (collidesWith(pPlayer))
	{
		setIsPickedUp(true);
		pPlayer->setHp(pPlayer->getHp() + 20);
	}
}

void HealthPack::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::RectangleShape graphicsRect(getSize());
	graphicsRect.setFillColor(getColor());
	graphicsRect.setPosition(getPosition());
	target.draw(graphicsRect);
}
