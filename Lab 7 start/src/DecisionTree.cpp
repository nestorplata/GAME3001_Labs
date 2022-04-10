#include "DecisionTree.h"
#include <iostream>
DecisionTree::DecisionTree()
{
	m_buildTree();
}

DecisionTree::DecisionTree(Agent* agent)
{
	m_agent = agent;
	m_buildTree();
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

void DecisionTree::setAgent(Agent * agent)
{
	m_agent = agent;
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

void DecisionTree::m_buildTree()
{

	m_LOSNode = new LOSCondition();
	m_treeNodeList.push_back(m_LOSNode);

	m_RadiusNode = new RadiusCondition();
	AddNode(m_LOSNode, m_RadiusNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_RadiusNode);

	m_CloseCombatNode = new CloseCombatCondition();
	AddNode(m_LOSNode, m_CloseCombatNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_CloseCombatNode);

	//Actions

	TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
	static_cast<ActionNode*>(patrolNode)->setAgent(m_agent);
	m_treeNodeList.push_back(patrolNode);

	TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToLOSNode)->setAgent(m_agent);
	m_treeNodeList.push_back(moveToLOSNode);

	TreeNode* moveToPlayerNOde = AddNode(m_CloseCombatNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
	static_cast<ActionNode*>(moveToPlayerNOde)->setAgent(m_agent);
	m_treeNodeList.push_back(moveToPlayerNOde);

	TreeNode* attackNode = AddNode(m_CloseCombatNode, new AttackAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(attackNode)->setAgent(m_agent);
	m_treeNodeList.push_back(attackNode);
}
