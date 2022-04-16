#include "TorpedoK.h"
#include "TextureManager.h"

TorpedoK::TorpedoK(float speed, glm::vec2 direction)
{
	CreateTorpedo(speed, direction,
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo_k.png", 
		"torpedok_SpriteSheet");
}

TorpedoK::~TorpedoK()
= default;
