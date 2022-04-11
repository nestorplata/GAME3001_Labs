#include "DecisionTree.h"
#include <iostream>
DecisionTree::DecisionTree()
{
	//m_buildTree();
}

DecisionTree::DecisionTree(Agent* agent)
{
	m_agent = agent;
	//m_buildTree();
}

DecisionTree::~DecisionTree()
= default;

Agent* DecisionTree::getAgent() const
{
	return m_agent;
}

LOSCondition* DecisionTree::getLOSNode() const
{
	return m_LOSNode;
}

RadiusCondition* DecisionTree::getRadiusNode() const
{
	return m_RadiusNode;
}

CloseCombatCondition* DecisionTree::getCloseCombatNode() const
{
	return m_CloseCombatNode;
}

RangeCombatCondition* DecisionTree::getRangedCombatNode() const
{
	return m_RangeCombatNode;
}

std::vector<TreeNode*>& DecisionTree::getTreee()
{
	return m_treeNodeList;
}

void DecisionTree::setAgent(Agent * agent)
{
	m_agent = agent;
}

void DecisionTree::setLOSNode(LOSCondition* node)
{
	m_LOSNode = node;
}

void DecisionTree::setRadiusNode(RadiusCondition* node)
{
	m_RadiusNode = node;
}

void DecisionTree::setCloseCombatNode(CloseCombatCondition* node)
{
	m_CloseCombatNode = node;
}

void DecisionTree::setRangedCombatNode(RangeCombatCondition* node)
{
	m_RangeCombatNode = node;
}

TreeNode* DecisionTree::AddNode(TreeNode * parent, TreeNode * child_node, TreeNodeType type)
{
	switch (type)
	{
	case LEFT_TREE_NODE:
		parent->m_pLeft = child_node;
		break;

	case RIGHT_TREE_NODE:
		parent->m_pRight = child_node;
		break;
	}
	return child_node;
}

void DecisionTree::Display()
{
	for (auto node : m_treeNodeList)
	{
	std::cout << node->m_name << std::endl;
}
}

void DecisionTree::update()
{
	//TBD
}

void DecisionTree::Clean()
{
	for (auto node : m_treeNodeList)
	{
		delete node;
		node = nullptr;
	}
	m_treeNodeList.clear();
	m_treeNodeList.shrink_to_fit();

	m_LOSNode = nullptr;
	m_RadiusNode = nullptr;
	m_CloseCombatNode = nullptr;
}

void DecisionTree::MakeDecision()
{
	auto currentNode = m_treeNodeList[0];
	while(!currentNode->m_isLeaf)
	{
		currentNode = dynamic_cast<ConditionNode*>(currentNode)->Condition() ?
			(currentNode->m_pRight) :
			(currentNode->m_pLeft);
	}
	dynamic_cast<ActionNode*>(currentNode)->Action();
}

//void DecisionTree::m_buildTree()
//{

//}
