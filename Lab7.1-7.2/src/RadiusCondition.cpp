#include "RadiusCondition.h"

RadiusCondition::RadiusCondition(bool LOS)
{
	m_name = "Radius Condition";
	setIsWithinRadius(LOS);
}

RadiusCondition::~RadiusCondition()
= default;

void RadiusCondition::setIsWithinRadius(bool state)
{
	m_hasLOS = state;
}

bool RadiusCondition::Condition()
{
	return m_hasLOS;
}
