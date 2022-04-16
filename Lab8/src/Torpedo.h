#pragma once
#ifndef __TORPEDO__
#define __TORPEDO__

#include "TorpedoAnimationState.h"
#include "Sprite.h"

class Torpedo : public Sprite
{
public:
	Torpedo();
	~Torpedo();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void CreateTorpedo(float speed, glm::vec2 direction,
		std::string txtfile, std::string pngfile, std::string name);
	// setters
	void setAnimationState(TorpedoAnimationState new_state);

private:
	void m_buildAnimations();

	TorpedoAnimationState m_currentAnimationState;

	float m_speed;
	glm::vec2 m_direction;
	std::string m_ShipName;
};

#endif /* defined (__TORPEDO__) */