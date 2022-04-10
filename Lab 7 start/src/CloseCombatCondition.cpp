#include "CloseCombatCondition.h"

CloseCombatCondition::CloseCombatCondition(bool LOS)
{
	m_name = "LOS Condition";
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
