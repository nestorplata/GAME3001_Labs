#include "Agent.h"

#include <iostream>

#include "Util.h"

Agent::Agent()
{
	m_lineColour[0] = glm::vec4(0, 1, 0, 1);
	m_lineColour[1] = glm::vec4(0, 1, 0, 1);
	m_lineColour[2] = glm::vec4(0, 1, 0, 1);

	m_collisionWhiskers[0] = false;
	m_collisionWhiskers[0] = false;
	m_collisionWhiskers[0] = false;

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

glm::vec2 Agent::getMiddleLosEndPoint() const
{
	return m_middleLOSEndPoint;
}

glm::vec2 Agent::getRightLOSEEndPoint() const
{
	return m_rightLOSEendPoint;
}

bool* Agent::getCollisionWhiskers()
{
	return m_collisionWhiskers;
}

glm::vec4 Agent::getLineColour(int index)
{
	return m_lineColour[index];
}

float Agent::getWhiskersAngle()
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

void Agent::setMiddleLOSEndPoint(glm::vec2 point)
{
	m_middleLOSEndPoint = point;
}

void Agent::setRightLOSEndPoint(glm::vec2 point)
{
	m_rightLOSEendPoint = point;
}

void Agent::setLineColour(int index, glm::vec4 colour)
{
	m_lineColour[index] = colour;
}

void Agent::setWhiskersAngle(float angle)
{
	m_whiskersangles = angle;
}

void Agent::updateWhiskers(float angle)
{
	m_whiskersangles = angle;
	setMiddleLOSEndPoint(getTransform()->position + getCurrentDirection() * getLOSDistance());

	float x = sin((getCurrentHeading() + m_whiskersangles + 90) * Util::Deg2Rad);
	float y = cos((getCurrentHeading() + m_whiskersangles + 90) * Util::Deg2Rad);
	setLeftLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.75f);

	x = sin((getCurrentHeading() + m_whiskersangles + 90) * Util::Deg2Rad);
	y = cos((getCurrentHeading() + m_whiskersangles + 90) * Util::Deg2Rad);
	setRightLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.75f);
	std::cout << std::endl<<"x= "<<x << std::endl;
	std::cout << "y= "<<y << std::endl;


}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}
