#include "MoveToPlayerAction.h"
#include <iostream>
MoveToPlayerAction::MoveToPlayerAction()
{
	m_name = "Move To Player Action";
}

MoveToPlayerAction::~MoveToPlayerAction()
{
}

void MoveToPlayerAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	getAgent()->setActionState(PATROL);
}
