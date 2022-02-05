#include "SpaceShip.h"
#include"TextureManager.h"
#include "Game.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance().load("../Assets/textures/ncl.png", "space");

	const auto size = TextureManager::Instance().getTextureSize("space");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(500.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(AGENT);

	m_maxSpeed = 5.0f;
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
	TextureManager::Instance().draw("space", x, y, getCurrentHeading(), 255, true);

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
	setDesiredVelocity(getTargetPosition());
	//setCurrentDirection(getDesiredVelocity());

	//auto target_direction = getTargetPosition() - getTransform()->position;

	const glm::vec2 steering_direction = getDesiredVelocity() - getCurrentDirection();

	lookWhereYourGoing(steering_direction);

	getRigidBody()->acceleration =getCurrentDirection() *getAccelerationRate();

}

void SpaceShip::lookWhereYourGoing(glm::vec2 target_direction)
{
	float target_rotation = Util::signedAngle(getCurrentDirection(), target_direction);

	float turn_sensitiviy = 5.0f;

	if (abs(target_rotation) > turn_sensitiviy)
	{
		if (target_rotation > 0.0f)
		{
			setCurrentHeading(getCurrentHeading() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setCurrentHeading(getCurrentHeading() - getTurnRate());
		}
	}

	const auto size = TextureManager::Instance().getTextureSize("space");
	//std::cout << "Sixe( width: " << size.x << ", height: " << size.y << " )" << std::endl;
}

void SpaceShip::m_move()
{
	Seek();

	const float dt = TheGame::Instance().getDeltaTime();

	const glm::vec2 initial_position = getTransform()->position;

	//const glm::vec2 velocity_term = (getRigidBody()->velocity + getCurrentDirection() * getMaxSpeed()) * 0.5f* dt;
	const glm::vec2 velocity_term = getRigidBody()->velocity* dt;
	
	//const glm::vec2 acceleration_term = (getRigidBody()->acceleration + getCurrentDirection()* getAccelerationRate())* 0.5f * dt * dt;
	const glm::vec2 acceleration_term = getRigidBody()->acceleration * 0.5f;

	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	getTransform()->position = final_position;

	//add our acceleration to velocity
	getRigidBody()->velocity += getRigidBody()->acceleration;

	//clamp our velocity at max speed;
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, getMaxSpeed());

}


