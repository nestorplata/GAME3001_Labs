#pragma once
#ifndef __STAR_SHIP__
#define __STAR_SHIP__

#include "DisplayObject.h"

class StarShip final : public DisplayObject {
public:
	StarShip();
	~StarShip();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__STAR_SHIP__) */