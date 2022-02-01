#include "StarShip.h"
#include"TextureManager.h"

StarShip::StarShip()
{
	TextureManager::Instance().load("../Assets/textures/ncl.png", "Star_Ship");

	const auto size = TextureManager::Instance().getTextureSize("Star_Ship");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(STAR_SHIP);
}

StarShip::~StarShip()
= default;

void StarShip::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the StarShip
	TextureManager::Instance().draw("Star_Ship", x, y, 0, 255, true);
}

void StarShip::update()
{
	m_move();
	m_checkBounds();
}

void StarShip::clean()
{
}

void StarShip::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
}

void StarShip::m_checkBounds()
{
}

void StarShip::m_reset()
{
}
