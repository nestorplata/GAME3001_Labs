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
	if (getAgent()->getActionState() != MOVE_TO_PLAYEr)
	{
		std::cout << "Performing " << m_name << std::endl;

	}
	getAgent()->MoveToPlayer();
}
