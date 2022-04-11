#pragma once
#ifndef __RANGE_COMBAT_CONDITION__
#define __RANGE_COMBAT_CONDITION__

#include "ConditionNode.h"

class RangeCombatCondition : public ConditionNode
{
public:
	RangeCombatCondition(bool LOS = false);
	virtual ~RangeCombatCondition();

	//getters and Setters
	void setIsWithinCombatRange(bool state);

	virtual bool Condition() override;

private: 
	bool m_hasLOS;
};

#endif /* defined (__RANGE_COMBAT_CONDITION__)*/