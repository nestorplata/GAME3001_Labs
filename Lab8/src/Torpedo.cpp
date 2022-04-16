#include "Torpedo.h"
#include "TextureManager.h"

void Torpedo::CreateTorpedo(float speed, glm::vec2 direction,
	std::string txtfile, std::string pngfile, std::string name)
{
	m_currentAnimationState=FIRED;

	TextureManager::Instance().loadSpriteSheet(txtfile, pngfile, name);

	setSpriteSheet(TextureManager::Instance().getSpriteSheet(name));
	
	// set frame width
	setWidth(64);

	// set frame height
	setHeight(64);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PROJECTILE);

	m_speed = speed;
	m_direction = { direction.x * speed, direction.y * speed };
	m_ShipName = name;

	m_buildAnimations();
}
Torpedo::Torpedo() 
= default;

Torpedo::~Torpedo()
= default;

void Torpedo::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case FIRED:
		TextureManager::Instance().playAnimation(m_ShipName, getAnimation("fired"),
			x, y, 5.0f, 0, 255, true);
		break;
	default:
		break;
	}
	
}

void Torpedo::update()
{
	this->getTransform()->position += m_direction;
}

void Torpedo::clean(){}

void Torpedo::setAnimationState(const TorpedoAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Torpedo::m_buildAnimations()
{
	Animation firedAnimation = Animation();

	firedAnimation.name = "fired";
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired1"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired2"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired3"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired4"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired5"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired6"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired7"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired8"));

	setAnimation(firedAnimation);
}
