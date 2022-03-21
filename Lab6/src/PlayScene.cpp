#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"


#include "Renderer.h"
#include "Util.h"
#include "Config.h"
#include <fstream>

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();	
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	m_checkAgentLOS(m_pSpaceShip, m_pTarget);
	switch (m_LOSMode)
	{
	case 0:
		m_checkAllNodesWithTarget(m_pTarget);
		break;
	case 1:
		m_checkAllNodesWithTarget(m_pSpaceShip);
		break;
	case 2:
		m_checkAllNodesWithBoth();
		break;
	}

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	//crate new obstacles from a file
	m_createObstaclesFromFile();
	//TODO: File Load Logic
	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_pTarget->getTransform()->position = glm::vec2(500.0f, 300.0f);
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
	addChild(m_pSpaceShip);

	//setup a few more fields
	m_LOSMode = 0;
	m_obstacleBuffer = 0;
	m_pathNodeLOSDistance = 1000;
	m_setPathNodeLOSDistance(m_pathNodeLOSDistance);

	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);

	//preloaded sounds
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "thunder", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::m_buildGrid()
{
	const auto tile_size = Config::TILE_SIZE;
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); //redraw the obstacle if the obstacle moves

	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			//determine which path_nodes to keep
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2((col * tile_size) + offset.x, (row * tile_size) + offset.y);
			bool keep_node = true;
			for (auto obstacle : m_pObstacles)
			{
				//determine which path_nodes to keep
				if (CollisionManager::AABBCheckWithBuffer(path_node, obstacle, m_obstacleBuffer))
				{
					keep_node = false;
				}
			}
			if (keep_node)
			{
				addChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}
	}
	//if my grid is supposed to be hidden - make it so
	m_toggleGrid(m_isGridEnabled);

}

void PlayScene::m_toggleGrid(bool state)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) 
{
	bool hasLOS = false;
	agent->setHasLOS(hasLOS);
	glm::vec4 LOSColour;

	const auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target_object);
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : getDisplayList())
		{
				//check if the display object is closer to the spaceship than the target 
				const auto AgentToObjectDistance = Util::getClosestEdge(m_pSpaceShip->getTransform()->position, display_object);
				if (AgentToObjectDistance <= AgentToTargetDistance) continue;

				if ((display_object->getType() != AGENT) && (display_object->getType() != PATH_NODE) && (display_object->getType() != TARGET))
				{
					contact_list.push_back(display_object);
				}

		}
		const glm::vec2 agentEndPoint = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		hasLOS = CollisionManager::LOSCheck(agent, agentEndPoint, contact_list, target_object);

		LOSColour = (target_object->getType() == AGENT) ? glm::vec4(0, 0, 1, 1) : glm::vec4(0, 1, 0, 1);
		agent->setHasLOS(hasLOS, LOSColour);
	}

	return hasLOS;
}

bool PlayScene::m_checkPathNodeLOS(PathNode* path_nade, DisplayObject* target_object)
{
	return false;
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object)
{
}

void PlayScene::m_checkAllNodesWithBoth()
{
}

void PlayScene::m_setPathNodeLOSDistance(int dist)
{
}



void PlayScene::m_clearNodes()
{
	m_pGrid.clear();
	for (auto display_object : getDisplayList())
	{
		if (display_object->getType() == PATH_NODE)
		{
			removeChild(display_object);
		}
	}
}

void PlayScene::m_createObstaclesFromFile()
{
	std::ifstream inFile("../Assets/data/obstacles.txt");
	while (!inFile.eof())
	{
		std::cout << "Obstacle" << std::endl;
		Obstacle* obstacle = new Obstacle();
		float x, y, w, h;
		inFile >> x >> y >> w >> h;
		obstacle->getTransform()->position = glm::vec2(x, y);
		obstacle->setWidth(w);
		obstacle->setHeight(h);
		addChild(obstacle);
		m_pObstacles.push_back(obstacle);
	}
}



void PlayScene::GUI_Function()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 6 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	if (ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();
	if (ImGui::Button("Node LOS Target", { 300, 20 }))
	{
		m_LOSMode = 0;
	}

	if (m_LOSMode ==0)
	{
		ImGui::SameLine();
		ImGui::Text("<Activate>");

	}

	if (ImGui::Button("Node LOS SpaceShip", { 300, 20 }))
	{
		m_LOSMode = 1;
	}

	if (m_LOSMode == 1)
	{
		ImGui::SameLine();
		ImGui::Text("<Activate>");

	}

	if (ImGui::Button("Node LOS Targe and SpaceShip", { 300, 20 }))
	{
		m_LOSMode = 2;
	}

	if (m_LOSMode == 2)
	{
		ImGui::SameLine();
		ImGui::Text("<Activate>");

	}

	if (ImGui::SliderInt("Path Node LOS Distance", &m_pathNodeLOSDistance, 0, 1000))
	{
		m_setPathNodeLOSDistance(m_pathNodeLOSDistance);
	}

	ImGui::Separator();


	// spaceship properties
	static int shipPosition[] = { m_pSpaceShip->getTransform()->position.x, m_pSpaceShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Start Position", shipPosition, 0,600))
	{
		m_pSpaceShip->getTransform()->position.x = shipPosition[0];
		m_pSpaceShip->getTransform()->position.y = shipPosition[1];

	}
	// allow the ship to rotate
	static int angle;
	if (ImGui::SliderInt("Ship Direction", &angle, -180, 180))
	{
		m_pSpaceShip->setCurrentHeading(angle);
	}

	ImGui::Separator();
	// target properties
	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderInt2("Goal Position", targetPosition, 0, 600)) 
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];

	}

	ImGui::Separator();

	//Add Obstacle position Control for all Obstacles

	for (unsigned i = 0; i < m_pObstacles.size(); ++i)
	{
		int obstaclePosition[] = { m_pObstacles[i]->getTransform()->position.x, m_pObstacles[i]->getTransform()->position.y };
		std::string label = "Obstacle" + std::to_string(i + 1) + " Position";
		if (ImGui::SliderInt2(label.c_str(), obstaclePosition, 0, 600))
		{
			m_pObstacles[i]->getTransform()->position.x = obstaclePosition[0];
			m_pObstacles[i]->getTransform()->position.y = obstaclePosition[1];
			m_buildGrid();
		}
	}
	ImGui::Separator();

	if (ImGui::SliderInt("Obstacle Buffer", &m_obstacleBuffer, 0, 100))
	{
		m_buildGrid();
	}
	ImGui::End();
}


//int PlayScene::m_obstacleBuffer;


