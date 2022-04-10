#pragma once
#ifndef __RADIUS_CONDITION__
#define __RADIUS_CONDITION__

#include "ConditionNode.h"

class RadiusCondition : public ConditionNode
{
	RadiusCondition(bool LOS = false);
	virtual ~RadiusCondition();

	//getters and Setters
	void setIsWithinRadius(bool state);

	virtual bool Condition() override;

private:
	bool m_hasLOS;
};

#endif /* defined (__RADIUS_CONDITION__)*/