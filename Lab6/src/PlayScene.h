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

	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;

	//create grid of pathnodes
	std::vector<PathNode*> m_pGrid;

	void m_buildGrid();

};

#endif /* defined (__PLAY_SCENE__) */