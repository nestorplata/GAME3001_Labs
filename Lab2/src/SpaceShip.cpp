#include "SpaceShip.h"
#include"TextureManager.h"
#include "Game.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance().load("../Assets/textures/ncl.png", "space");

	const auto size = TextureManager::Instance().getTextureSize("space");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(300.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(AGENT);

	m_maxSpeed = 1.0f;
	m_turnRate = 5.0f;
	m_accelerationRate = 1.0f;


}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the SpaceShip
	TextureManager::Instance().draw("space", x, y, 0, 255, true);

}

void SpaceShip::update()
{
	m_move();
}

void SpaceShip::clean()
{
}

float SpaceShip::getMaxSpeed() const
{
	return m_maxSpeed;
}

float SpaceShip::getTurnRate() const
{
	return m_turnRate;
}

float SpaceShip::getAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 SpaceShip::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

void SpaceShip::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void SpaceShip::setTurnRate(const float angle)
{
	m_turnRate = angle;
}

void SpaceShip::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void SpaceShip::setDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position);
}

void SpaceShip::Seek()
{
}

void SpaceShip::lookWhereYourGoing()
{
}

void SpaceShip::m_move()
{
	const float dt = TheGame::Instance().getDeltaTime();

	const glm::vec2 initial_position = getTransform()->position;

	//auto velocity_plus_acceleration = getRigidBody()->velocity + getRigidBody()->acceleration;
	//std::cout << "Combined :(" << velocity_plus_acceleration.x << ", " << velocity_plus_acceleration.y << ")"<< std::endl;

	const glm::vec2 velocity_term = getRigidBody()->velocity * dt;
	
	const glm::vec2 acceleration_term = getRigidBody()->acceleration * 0.5f * dt * dt;

	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	getTransform()->position = final_position;

	getRigidBody()->velocity += getRigidBody()->acceleration;


	std::cout << "Velocity: (" << getRigidBody()->velocity.x << ", " << getRigidBody()->velocity.y << ")" << std::endl;
	std::cout << "Acceleration: (" << getRigidBody()->acceleration.x << ", " << getRigidBody()->acceleration.y << ")" << std::endl;

}


