#include "CloseCombatEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"
#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "PatrolAction.h"

CloseCombatEnemy::CloseCombatEnemy()
{
	TextureManager::Instance().load("../Assets/textures/d7_small.png", "close_enemy");

	const auto size = TextureManager::Instance().getTextureSize("close_enemy");
	setWidth(size.x);
	setHeight(size.y);
	
	getRigidBody()->bounds = glm::vec2(getWidth(), getHeight());
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	// starting motion properties
	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_maxSpeed = 10.0f; // a maximum number of pixels moved per frame
	m_turnRate = 5.0f; // a maximum number of degrees to turn each time-step
	m_accelerationRate = 2.0f; // a maximum number of pixels to add to the velocity each frame
	
	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	// Fill in action state and patrol code
	setActionState(NO_ACTION);

	//Set patrol Path
	m_patrolPath.push_back(glm::vec2(760, 40));
	m_patrolPath.push_back(glm::vec2(760, 560));
	m_patrolPath.push_back(glm::vec2(40, 560));
	m_patrolPath.push_back(glm::vec2(40, 40));
	m_waypoint = 0; 

	setTargetPosition(m_patrolPath[m_waypoint]);
	setType(AGENT);

	m_tree = new DecisionTree(this);
	m_BuildTree();
	m_tree->Display();
}

CloseCombatEnemy::~CloseCombatEnemy() 
= default;

void CloseCombatEnemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance().draw("close_enemy", x, y, getCurrentHeading(), 255, isCentered());

	// draw LOS
	if (EventManager::Instance().isIMGUIActive()) {
		Util::DrawLine(getTransform()->position, getMiddleLOSEndPoint(), getLOSColour());
	}
}

void CloseCombatEnemy::update()
{
	// Determine which action to perform
	m_tree->MakeDecision();
}

void CloseCombatEnemy::clean()
{
}

float CloseCombatEnemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

float CloseCombatEnemy::getTurnRate() const
{
	return m_turnRate;
}

float CloseCombatEnemy::getAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 CloseCombatEnemy::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

void CloseCombatEnemy::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void CloseCombatEnemy::setTurnRate(const float angle)
{
	m_turnRate = angle;
}

void CloseCombatEnemy::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void CloseCombatEnemy::setDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position);
}

void CloseCombatEnemy::Seek()
{
	// Find next waypoint:
	if (Util::distance(m_patrolPath[m_waypoint], getTransform()->position) < 10)
	{
		if (++m_waypoint == m_patrolPath.size())
		{
			m_waypoint = 0;
		}
		setTargetPosition(m_patrolPath[m_waypoint]);
	}

	setDesiredVelocity(getTargetPosition());

	const glm::vec2 steering_direction = getDesiredVelocity() - getCurrentDirection();

	LookWhereYoureGoing(steering_direction);

	getRigidBody()->acceleration = getCurrentDirection() * getAccelerationRate();
}

void CloseCombatEnemy::LookWhereYoureGoing(const glm::vec2 target_direction)
{
	float target_rotation = Util::signedAngle(getCurrentDirection(), target_direction) - 90;

	const float turn_sensitivity = 3.0f;

	if (getCollisionWhiskers()[0])
	{
		setCurrentHeading(getCurrentHeading() + getTurnRate());
	}
	else if (getCollisionWhiskers()[2])
	{
		setCurrentHeading(getCurrentHeading() - getTurnRate());
	}
	else if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setCurrentHeading(getCurrentHeading() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setCurrentHeading(getCurrentHeading() - getTurnRate());
		}
	}

	updateWhiskers(getWhiskerAngle());
}

void CloseCombatEnemy::Patrol()
{
	if (getActionState() != PATROL)
	{
		setActionState(PATROL);

	}
	m_move();
}

void CloseCombatEnemy::MoveToPlayer()
{
	if (getActionState() != MOVE_TO_PLAYER)
	{
		setActionState(MOVE_TO_PLAYER);

	}
	//m_move();
}



const DecisionTree* CloseCombatEnemy::getTree()
{
	return m_tree;
}

void CloseCombatEnemy::m_move()
{
	Seek();
	
	//                                   final Position     position term    velocity term     acceleration term
	// kinematic equation for motion --> Pf            =      Pi     +     Vi*(time)    +   (0.5)*Ai*(time * time)

	const float dt = TheGame::Instance().getDeltaTime();

	// compute the position term
	const glm::vec2 initial_position = getTransform()->position;

	// compute the velocity term
	const glm::vec2 velocity_term = getRigidBody()->velocity * dt;

	// compute the acceleration term
	const glm::vec2 acceleration_term = getRigidBody()->acceleration * 0.5f;// *dt;
	
	
	// compute the new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	getTransform()->position = final_position;

	// add our acceleration to velocity
	getRigidBody()->velocity += getRigidBody()->acceleration;

	// clamp our velocity at max speed
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, getMaxSpeed());
}

void CloseCombatEnemy::m_BuildTree()
{
	
	m_tree->setLOSNode(new LOSCondition()) ;
	m_tree->getTreee().push_back(m_tree->getLOSNode());

	m_tree->setRadiusNode(new RadiusCondition());
	m_tree->AddNode(m_tree->getLOSNode(), m_tree->getRadiusNode(), LEFT_TREE_NODE);
	m_tree->getTreee().push_back(m_tree->getRadiusNode());

	m_tree->setCloseCombatNode(new CloseCombatCondition());
	m_tree->AddNode(m_tree->getLOSNode(), m_tree->getCloseCombatNode(), RIGHT_TREE_NODE);
	m_tree->getTreee().push_back(m_tree->getCloseCombatNode());

	//Actions

	TreeNode* patrolNode = m_tree->AddNode(m_tree->getRadiusNode(), new PatrolAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(patrolNode)->setAgent(this);
	m_tree->getTreee().push_back(patrolNode);

	TreeNode* moveToLOSNode = m_tree->AddNode(m_tree->getRadiusNode(), new MoveToLOSAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToLOSNode)->setAgent(this);
	m_tree->getTreee().push_back(moveToLOSNode);

	TreeNode* moveToPlayerNOde = m_tree->AddNode(m_tree->getCloseCombatNode(), new MoveToPlayerAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToPlayerNOde)->setAgent(this);
	m_tree->getTreee().push_back(moveToPlayerNOde);

	TreeNode* attackNode = m_tree->AddNode(m_tree->getCloseCombatNode(), new AttackAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(attackNode)->setAgent(this);
	m_tree->getTreee().push_back(attackNode);
}
