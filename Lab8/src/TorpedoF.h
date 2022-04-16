#pragma once
#ifndef __TORPEDOF__
#define __TORPEDOF__

#include "TorpedoAnimationState.h"
#include "Torpedo.h"

class TorpedoF final : public virtual Torpedo
{
public:

	TorpedoF(float speed = 0.0f, glm::vec2 direction = { 0,0 });
	~TorpedoF();


};

#endif /* defined (__TORPEDOF__) */
