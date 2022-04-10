#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
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

	void setAgent(Agent* agent);

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

	std::vector<TreeNode*> m_treeNodeList;

	void m_buildTree();
};

#endif /* defined (__DECISION_TREE__)*/