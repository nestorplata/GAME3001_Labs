#include "RangeCombatCondition.h"

RangeCombatCondition::RangeCombatCondition(bool LOS)
{
	m_name = "Range Combat Condition";
	setIsWithinCombatRange(LOS);
}

RangeCombatCondition::~RangeCombatCondition()
= default;

void RangeCombatCondition::setIsWithinCombatRange(bool state)
{
	m_hasLOS = state;
}

bool RangeCombatCondition::Condition()
{
	return m_hasLOS;
}
