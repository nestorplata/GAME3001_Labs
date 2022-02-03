#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__

#include "Agent.h"
#include "Util.h"

class SpaceShip final : public Agent 
{
public:
	SpaceShip();
	~SpaceShip();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	float getMaxSpeed() const;
	float getTurnRate() const;
	float getAccelerationRate() const;
	glm::vec2 getDesiredVelocity() const;

	void setMaxSpeed(float speed);
	void setTurnRate(float angle);
	void setAccelerationRate(float rate);
	void setDesiredVelocity(glm::vec2 target_position);

	void Seek();
	void lookWhereYourGoing();


private:

	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	glm::vec2 m_desiredVelocity;

	void m_move();
};


#endif /* defined (__SPACE_SHIP__) */