#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

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

	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_pTarget->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(200.0f, 300.0f);
	addChild(m_pSpaceShip);

	m_isGridEnabled = false;
	// TODO: m_storeObstacles();
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

	//TODO: m_clearNodes(); //redraw the obstacle if the obstacle moves

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

void PlayScene::m_checkShipLOS(DisplayObject* target_object)
{
}

void PlayScene::m_storeObstacles()
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



void PlayScene::GUI_Function()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 4 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	if (ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();

	// spaceship properties
	static int shipPosition[] = { m_pSpaceShip->getTransform()->position.x, m_pSpaceShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Start Position", shipPosition, 0,600))
	{
		m_pSpaceShip->getTransform()->position.x = shipPosition[0];
		m_pSpaceShip->getTransform()->position.y = shipPosition[1];

	}

	// target properties
	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderInt2("Goal Position", targetPosition, 0, 600)) 
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];

	}

	ImGui::End();
}


int PlayScene::m_obstacleBuffer;


