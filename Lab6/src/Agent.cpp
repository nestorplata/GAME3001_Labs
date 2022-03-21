#include "Agent.h"

#include <iostream>

#include "Util.h"

Agent::Agent()
{
	for (int i = 0; i < 5; i++)
	{
		m_lineColour[i] = glm::vec4(0, 1, 0, 1);
		m_collisionWhiskers[i] = false;
	}

	m_whiskersangles = 45;

	m_LOSDistance = 300;
}

Agent::~Agent()
= default;

glm::vec2 Agent::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agent::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agent::getLOSDistance() const
{
	return m_LOSDistance;
}

bool Agent::hasLOS() const
{
	return m_hasLOS;
}

float Agent::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agent::getLOSColour() const
{
	return m_LOSColour;
}

glm::vec2 Agent::getLeftLOSEndPoint() const
{
	return m_leftLOSEndPoint;
}

glm::vec2 Agent::getMiddleLeftLOSEndPoint() const
{
	return m_middleleftLOSEndPoint;
}

glm::vec2 Agent::getMiddleLOSEndPoint() const
{
	return m_middleLOSEndPoint;
}

glm::vec2 Agent::getMiddleRightLOSEndPoint() const
{
	return m_middlerightLOSEndPoint;
}

glm::vec2 Agent::getRightLOSEndPoint() const
{
	return m_rightLOSEndPoint;
}

bool* Agent::getCollisionWhiskers()
{
	return m_collisionWhiskers;
}

glm::vec4 Agent::getLineColour(int index)
{
	return m_lineColour[index];
}

float Agent::getWhiskerAngle()
{
	return m_whiskersangles;
}

void Agent::setTargetPosition(const glm::vec2 new_position)
{
	m_targetPosition = new_position;
}

void Agent::setCurrentDirection(const glm::vec2 new_direction)
{
	m_currentDirection = new_direction;
}

void Agent::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agent::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setHasLOS(const bool state, const glm::vec4 colour)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? colour : glm::vec4(1, 0, 0, 1);
}

void Agent::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agent::setLOSColour(const glm::vec4 colour)
{
	m_LOSColour = colour;
}

void Agent::setLeftLOSEndPoint(glm::vec2 point)
{
	m_leftLOSEndPoint = point;
}

void Agent::setMiddleLeftLOSEndPoint(glm::vec2 point)
{
	m_middleleftLOSEndPoint = point;
}

void Agent::setMiddleLOSEndPoint(glm::vec2 point)
{
	m_middleLOSEndPoint = point;
}

void Agent::setMiddleRightLOSEndPoint(glm::vec2 point)
{
	m_middlerightLOSEndPoint = point;
}

void Agent::setRightLOSEndPoint(glm::vec2 point)
{
	m_rightLOSEndPoint = point;
}

void Agent::setLineColour(int index, glm::vec4 colour)
{
	m_lineColour[index] = colour;
}

void Agent::setWhiskerAngle(float angle)
{
	m_whiskersangles = angle;
}

void Agent::updateWhiskers(float angle)
{
	m_whiskersangles = angle;


	float x = sin((getCurrentHeading() - m_whiskersangles + 60) * Util::Deg2Rad);
	float y = cos((getCurrentHeading() - m_whiskersangles + 60) * Util::Deg2Rad);
	setLeftLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.5f);

	x = sin((getCurrentHeading() - m_whiskersangles + 105) * Util::Deg2Rad);
	y = cos((getCurrentHeading() - m_whiskersangles + 105) * Util::Deg2Rad);
	setMiddleLeftLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.75f);
	//std::cout << std::endl << "x= " << x << std::endl;
	//std::cout << "y= " << y << std::endl;

	setMiddleLOSEndPoint(getTransform()->position + getCurrentDirection() * getLOSDistance());

	x = sin((getCurrentHeading() + m_whiskersangles + 75) * Util::Deg2Rad);
	y = cos((getCurrentHeading() + m_whiskersangles + 75) * Util::Deg2Rad);
	setMiddleRightLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.75f);
	//std::cout << std::endl << "x= " << x << std::endl;
	//std::cout << "y= " << y << std::endl;

	x = sin((getCurrentHeading() + m_whiskersangles + 120) * Util::Deg2Rad);
	y = cos((getCurrentHeading() + m_whiskersangles + 120) * Util::Deg2Rad);
	setRightLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.5f);
	//std::cout << std::endl<<"x= "<<x << std::endl;
	//std::cout << "y= "<<y << std::endl;

}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}
