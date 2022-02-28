#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Heuristic.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Tile.h"

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

	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;

	//PathFinding Objects and Functions
	std::vector<Tile*> m_pGrid;
	bool m_isGridEnabled;

	//create grid
	void m_buildGrid();
	bool m_getGridEnabled() const;
	void m_setGridEnabled(bool state);
	void m_computeTileCosts();

	// Calculate the chortest path
	void m_findShortestPath();
	void m_displayPathList();
	void m_resetPathFinding();
	void m_resetSimulation();

	std::vector<Tile*> m_OpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;
	
	// convinience functions to convert world to grid space
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	//heuristics
	Heuristic m_currentHeuristic;

	int m_moveCounter = 0;
	bool m_shipIsMoving = false;
	void m_moveShip();

	static int start_position[2];
	static int goal_position[2];
};

#endif /* defined (__PLAY_SCENE__) */