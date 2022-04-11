#include "RangedCombatEnemy.h"
#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

// Action includes
#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToRangeAction.h"
#include "PatrolAction.h"

// New for Lab7c
#include "FleeAction.h"
#include "MoveToCoverAction.h"
#include "WaitBehindCoverAction.h"

RangedCombatEnemy::RangedCombatEnemy()
{
	TextureManager::Instance().load("../Assets/textures/reliant_small.png", "ranged_enemy");

	const auto size = TextureManager::Instance().getTextureSize("ranged_enemy");
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

	// set patrol
	m_patrol.push_back(glm::vec2(760, 40)); // Top Right
	m_patrol.push_back(glm::vec2(760, 560)); // Bot Right
	m_patrol.push_back(glm::vec2(40, 560)); // Bot Left
	m_patrol.push_back(glm::vec2(40, 40)); // Top Left
	m_waypoint = 0;

	setTargetPosition(m_patrol[m_waypoint]);
	setType(AGENT);

	// Create Decision Tree
	m_tree = new DecisionTree(this); // overloaded constructor
	m_buildTree();
	m_tree->Display();
}

RangedCombatEnemy::~RangedCombatEnemy() 
= default;

void RangedCombatEnemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance().draw("ranged_enemy", x, y, getCurrentHeading(), 255, isCentered());

	// draw LOS
	if(EventManager::Instance().isIMGUIActive())
	{
		Util::DrawLine(getTransform()->position, getMiddleLOSEndPoint(), getLOSColour());
	}
}

void RangedCombatEnemy::update()
{
	// Determine which action to perform
	m_tree->MakeDecision();
}

void RangedCombatEnemy::clean()
{
}

float RangedCombatEnemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

float RangedCombatEnemy::getTurnRate() const
{
	return m_turnRate;
}

float RangedCombatEnemy::getAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 RangedCombatEnemy::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

void RangedCombatEnemy::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void RangedCombatEnemy::setTurnRate(const float angle)
{
	m_turnRate = angle;
}

void RangedCombatEnemy::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void RangedCombatEnemy::setDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position);
}

void RangedCombatEnemy::Seek()
{
	// Find next waypoint:
	if(Util::distance(m_patrol[m_waypoint], getTransform()->position) < 10)
	{
		// if moved to last waypoint go back to the start
		if(++m_waypoint == m_patrol.size())
		{
			m_waypoint = 0;
		}
		setTargetPosition(m_patrol[m_waypoint]);
	}

	setDesiredVelocity(getTargetPosition());

	const glm::vec2 steering_direction = getDesiredVelocity() - getCurrentDirection();

	LookWhereYoureGoing(steering_direction);

	getRigidBody()->acceleration = getCurrentDirection() * getAccelerationRate();
}

void RangedCombatEnemy::LookWhereYoureGoing(const glm::vec2 target_direction)
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

void RangedCombatEnemy::Flee()
{
	ActionState action = FLEE;
	if(getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	// action...
}

void RangedCombatEnemy::MoveToLOS()
{
	ActionState action = MOVE_TO_LOS;
	if (getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	// action...
}

void RangedCombatEnemy::WaitBehindCover()
{
	ActionState action = WAIT_BEHIND_COVER;
	if (getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	// action...
}

void RangedCombatEnemy::MoveToCover()
{
	ActionState action = MOVE_TO_COVER;
	if (getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	// action...
}

void RangedCombatEnemy::Attack()
{
	ActionState action = ATTACK;
	if (getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	// action...
}

void RangedCombatEnemy::Patrol()
{
	ActionState action = PATROL;
	if (getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	m_move();
}

void RangedCombatEnemy::MoveToRange()
{
	ActionState action = MOVE_TO_RANGE;
	if (getActionState() != action)
	{
		// Initialize
		setActionState(action);
	}
	// action...
}

const DecisionTree* RangedCombatEnemy::getTree() const
{
	return m_tree;
}

void RangedCombatEnemy::m_move()
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

void RangedCombatEnemy::m_buildTree()
{
	// Create and add root node.
	m_tree->setEnemyHealthNode(new EnemyHealthCondition());
	m_tree->getTree().push_back(m_tree->getEnemyHealthNode());

	// add the Flee Action to the Left Subtree
	TreeNode* fleeNode = m_tree->AddNode(m_tree->getEnemyHealthNode(), new FleeAction(), LEFT_TREE_NODE);
	fleeNode->setAgent(this);
	m_tree->getTree().push_back(fleeNode);

	m_tree->setEnemyHitNode(new EnemyHitCondition());
	m_tree->AddNode(m_tree->getEnemyHealthNode(), m_tree->getEnemyHitNode(), RIGHT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getEnemyHitNode());

	m_tree->setPlayerDetectedNode(new PlayerDetectedCondition());
	m_tree->AddNode(m_tree->getEnemyHitNode(), m_tree->getPlayerDetectedNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getPlayerDetectedNode());

	LOSCondition* losNodeRight = new LOSCondition();
	m_tree->AddNode(m_tree->getEnemyHitNode(), losNodeRight, RIGHT_TREE_NODE);
	losNodeRight->setAgent(this);
	m_tree->getTree().push_back(losNodeRight);

	TreeNode* patrolNode = m_tree->AddNode(m_tree->getPlayerDetectedNode(), new PatrolAction(), LEFT_TREE_NODE);
	patrolNode->setAgent(this);
	m_tree->getTree().push_back(patrolNode);

	LOSCondition* losNodeLeft = new LOSCondition();
	m_tree->AddNode(m_tree->getPlayerDetectedNode(), losNodeLeft, RIGHT_TREE_NODE);
	losNodeLeft->setAgent(this);
	m_tree->getTree().push_back(losNodeLeft);

	TreeNode* waitBehindCoverNode = m_tree->AddNode(losNodeRight, new WaitBehindCoverAction(), LEFT_TREE_NODE);
	waitBehindCoverNode->setAgent(this);
	m_tree->getTree().push_back(waitBehindCoverNode);

	TreeNode* moveToCoverNode = m_tree->AddNode(losNodeRight, new MoveToCoverAction(), RIGHT_TREE_NODE);
	moveToCoverNode->setAgent(this);
	m_tree->getTree().push_back(moveToCoverNode);

	TreeNode* moveToLOSNode = m_tree->AddNode(losNodeLeft, new MoveToLOSAction(), LEFT_TREE_NODE);
	moveToLOSNode->setAgent(this);
	m_tree->getTree().push_back(moveToLOSNode);

	m_tree->setRangedCombatNode(new RangedCombatCondition());
	m_tree->AddNode(losNodeLeft, m_tree->getRangedCombatNode(), RIGHT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getRangedCombatNode());

	TreeNode* moveToRangeNode = m_tree->AddNode(m_tree->getRangedCombatNode(), new MoveToRangeAction(), LEFT_TREE_NODE);
	moveToRangeNode->setAgent(this);
	m_tree->getTree().push_back(moveToRangeNode);

	TreeNode* attackNode = m_tree->AddNode(m_tree->getRangedCombatNode(), new AttackAction(), RIGHT_TREE_NODE);
	attackNode->setAgent(this);
	m_tree->getTree().push_back(attackNode);
}
