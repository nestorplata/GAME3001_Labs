#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
#include "RangeCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNodeType.h"
#include "TreeNodeType.h"
#include "ActionNode.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "AttackAction.h"

class DecisionTree
{
public:
	DecisionTree();
	DecisionTree(Agent* agent);

	~DecisionTree();

	Agent* getAgent() const;
	LOSCondition* getLOSNode() const;
	RadiusCondition* getRadiusNode() const;
	CloseCombatCondition* getCloseCombatNode() const;
	RangeCombatCondition* getRangedCombatNode() const;
	std::vector<TreeNode*>& getTreee();

	void setAgent(Agent* agent);
	void setLOSNode(LOSCondition* node);
	void setRadiusNode(RadiusCondition* node);
	void setCloseCombatNode(CloseCombatCondition* node);
	void setRangedCombatNode(RangeCombatCondition* node);

	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void update();
	void Clean();

	void MakeDecision();

private:
	Agent* m_agent;
	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;
	RangeCombatCondition* m_RangeCombatNode;

	std::vector<TreeNode*> m_treeNodeList;
};

#endif /* defined (__DECISION_TREE__)*/