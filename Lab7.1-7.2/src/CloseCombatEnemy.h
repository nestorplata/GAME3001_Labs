#pragma once
#ifndef __CLOSE_COMBAT_ENEMY__
#define __CLOSE_COMBAT_ENEMY__

#include "Agent.h"
#include "DecisionTree.h"
class CloseCombatEnemy final : public Agent
{
public:
	CloseCombatEnemy();
	~CloseCombatEnemy();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// getters and setters (accessors and mutators)
	float getMaxSpeed() const;
	float getTurnRate() const;
	float getAccelerationRate() const;
	glm::vec2 getDesiredVelocity() const;

	void setMaxSpeed(float speed);
	void setTurnRate(float angle);
	void setAccelerationRate(float rate);
	void setDesiredVelocity(glm::vec2 target_position);

	// public functions
	void Seek();
	void LookWhereYoureGoing(glm::vec2 target_direction);

	void Patrol() override;
	void MoveToPlayer();

	const DecisionTree* getTree();

private:
	// private movement variables
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	// new for Lab 7
	std::vector<glm::vec2> m_patrolPath;
	int m_waypoint;

	// where we want to go
	glm::vec2 m_desiredVelocity;

	// private functions
	void m_move();

	//DecisionTree
	DecisionTree* m_tree;
	void m_BuildTree();
};


#endif /* defined (__CLOSE_COMBAT_ENEMY__) */