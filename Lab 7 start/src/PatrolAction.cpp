#include "PatrolAction.h"
#include <iostream>
PatrolAction::PatrolAction()
{
	m_name = "Patrol Action";
}

PatrolAction::~PatrolAction()
{
}

void PatrolAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	getAgent()->setActionState(PATROL);
}
