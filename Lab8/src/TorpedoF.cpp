#include "TorpedoF.h"
#include "TextureManager.h"

TorpedoF::TorpedoF(float speed, glm::vec2 direction)
{
	CreateTorpedo(speed, direction,
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo.png", 
		"torpedo_SpriteSheet");
}

TorpedoF::~TorpedoF()
= default;