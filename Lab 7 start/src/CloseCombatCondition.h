#pragma once
#ifndef __CLOSE_COMBAT_CONDITION__
#define __CLOSE_COMBAT_CONDITION__

#include "ConditionNode.h"

class CloseCombatCondition : public ConditionNode
{
	CloseCombatCondition(bool LOS = false);
	virtual ~CloseCombatCondition();

	//getters and Setters
	void setIsWithinCombatRange(bool state);

	virtual bool Condition() override;

private: 
	bool m_hasLOS;
};

#endif /* defined (__CLOSE_COMBAT_CONDITION__)*/