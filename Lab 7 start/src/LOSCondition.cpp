#include "LOSCondition.h"

LOSCondition::LOSCondition(bool LOS)
{
	m_name = "LOS Condition";
	setLOS(LOS);
}

LOSCondition::~LOSCondition()
= default;

void LOSCondition::setLOS(bool state)
{
	m_hasLOS = state;
}

bool LOSCondition::Condition()
{
	return m_hasLOS;
}
