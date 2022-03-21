#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"

#include "Heuristic.h"
#include "Target.h"
#include "SpaceShip.h"
//#include "Tile.h"
#include "Obstacle.h"
#include "PathNode.h"
class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;
	bool m_isShortestPathEnabled =false;

	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;

	std::vector<PathNode*> m_pGrid;
	PathNode* shortestNode;

	std::vector<Obstacle*> m_pObstacles;	

	void m_buildGrid();

	void m_toggleGrid(const bool state);
	bool m_checkAgentLOS(Agent* agent, DisplayObject* target_object);
	bool m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object);
	void m_checkAllNodesWithTarget(DisplayObject* target_object);
	void m_checkAllNodesWithBoth();
	void m_setPathNodeLOSDistance(int dist);
	int m_LOSMode;

	
	void m_clearNodes();
	void m_createObstaclesFromFile();

	int m_obstacleBuffer;
	int m_pathNodeLOSDistance;

	glm::vec2 ShortestpathNode;
};

#endif /* defined (__PLAY_SCENE__) */