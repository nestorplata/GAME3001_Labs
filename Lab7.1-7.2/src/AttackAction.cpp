#include "AttackAction.h"

AttackAction::AttackAction()
{
	m_name = "Attack Action";
}

AttackAction::~AttackAction()
{
}

void AttackAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	getAgent()->setActionState(PATROL);
}
