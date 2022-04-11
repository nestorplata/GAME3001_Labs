#include "CloseCombatCondition.h"

CloseCombatCondition::CloseCombatCondition(bool LOS)
{
	m_name = "Close Combat Condition";
	setIsWithinCombatRange(LOS);
}

CloseCombatCondition::~CloseCombatCondition()
= default;

void CloseCombatCondition::setIsWithinCombatRange(bool state)
{
	m_hasLOS = state;
}

bool CloseCombatCondition::Condition()
{
	return m_hasLOS;
}
