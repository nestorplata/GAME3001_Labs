#pragma once
#ifndef __TORPEDOK__
#define __TORPEDOK__

#include "TorpedoAnimationState.h"
#include "Torpedo.h"

class TorpedoK final : public virtual Torpedo
{
public:
	
	TorpedoK(float speed = 0.0f, glm::vec2 direction = { 0,0 });
	~TorpedoK();

	
};

#endif /* defined (__TORPEDOK__) */