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
	if (m_shipIsMoving)
	{
		m_moveShip();
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

	m_buildGrid();

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_currentHeuristic = MANHATTAN;


	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	m_pTarget->getTransform()->position = m_getTile(11, 11)->getTransform()->position + offset;
	m_pTarget->setGridPosition(11.0f, 11.0f);
	m_getTile(11, 11)->setTileStatus(GOAL);

	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offset;
	m_pSpaceShip->setGridPosition(1.0f, 3.0f);
	m_getTile(1, 3)->setTileStatus(START);

	addChild(m_pSpaceShip);
	
	//preloaded sounds
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "thunder", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);

	m_computeTileCosts();

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::m_buildGrid()
{
	const auto tile_size = Config::TILE_SIZE;
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile();
			tile->getTransform()->position = glm::vec2(col* tile_size, row*tile_size);
			tile->setGridPosition(col, row); //grid position
			tile->setParent(this);
			addChild(tile);
			tile->addLabels();
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}

	//setup the neighbour references for each tile
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			//top
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row -1));
			}
			//Right
			if (col == Config::COL_NUM -1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col+1, row ));
			}

			//Bottom
			if (row == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row +1));
			}

			//Left
			if (col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col -1, row));
			}
		}
	}
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}

void PlayScene::m_setGridEnabled(bool state)
{
	m_isGridEnabled = state;

	for (auto tile : m_pGrid)
	{
		tile->setEnabled(m_isGridEnabled);
		tile->setLabelsEnabled(m_isGridEnabled);
	}
}

void PlayScene::m_computeTileCosts()
{
	float distance = 0.0f;
	float dx = 0.0f;
	float dy = 0.0f;

	//for each tile in the grid, loop
	for (auto tile : m_pGrid)
	{
		//calculate the distance from each tile to the goal tile]
		switch(m_currentHeuristic)
		{
		case MANHATTAN:
			dx = abs(tile->getGridPosition().x - m_pTarget->getGridPosition().x);
			dy = abs(tile->getGridPosition().y - m_pTarget->getGridPosition().y);
			distance = dx + dy;

				break;
		case EUCLIDEAN:
			// computes the eucliden distance ("as the crow flies") for each tile
			distance = Util::distance(tile->getGridPosition(), m_pTarget->getGridPosition());
			break;

		}

		tile->setTileCost(distance);
	}
}

void PlayScene::m_findShortestPath()
{
	if (m_pPathList.empty())
	{
		//step 1. Add Start Position
		Tile* start_tile = m_getTile(m_pSpaceShip->getGridPosition());
		start_tile->setTileStatus(OPEN);
		m_pOpenList.push_back(start_tile);
		bool goal_found = false;

		//step2. Loop until the OpenList is Empty or the Goal is found
		while (!m_pOpenList.empty() && !goal_found)
		{
			//step 2a - initialize variables for minimum distance
			auto min_distance = INFINITY;
			Tile* min_tile;
			int min_tile_index = 0;
			int neighbour_count = 0;
			std::vector<Tile*> neighbour_list;

			//step 2b - Check Valid Neighbours
			//loop through each neighbour in right-winding order (Top - Right - Bottom -Left)
			for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; ++index)
			{
				const auto neighbour = m_pOpenList[0]->getNeighbourTile(static_cast<NeighbourTile>(index));
				if (neighbour == nullptr || neighbour->getTileStatus() == IMPASSABLE)
				{
					continue;
				}
				neighbour_list.push_back(neighbour);
			}

			//step 2c - For every neighbour in the neighbour list - check if it has the min distance to the goal
			//or-- alternatively -- the nihgbour could be the goal
			for (auto neighbour : neighbour_list)
			{
				if (neighbour->getTileStatus() != GOAL)
				{
					//check if neighbour tile cost is less that the minimum found so far...
					if (neighbour->getTileCost() < min_distance)
					{
						min_distance = neighbour->getTileCost();
						min_tile = neighbour;
						min_tile_index = neighbour_count;
					}
					neighbour_count++;
				}
				else //neighbour is the Goal Tile
				{
					min_tile = neighbour;
					m_pPathList.push_back(min_tile);
					goal_found = true;
					break;
				}
			}

			//step 2d - add top tile of the open_list to the path_list
			m_pPathList.push_back(m_pOpenList[0]);
			m_pOpenList.pop_back();

			//step 2e - add the min_tile to the openlist
			m_pOpenList.push_back(min_tile);
			min_tile->setTileStatus(OPEN);
			neighbour_list.erase(neighbour_list.begin() + min_tile_index);

			//step 2f - push all remaining neighbours onto the closed list
			for (auto neighbour : neighbour_list)
			{
				if (neighbour->getTileStatus() == UNVISITED)
				{
					neighbour->setTileStatus(CLOSED);
					m_pClosedList.push_back(neighbour);
				}
			}
		}

		Tile* goal = m_pPathList.at(m_pPathList.size() - 2);
		m_pPathList.erase(m_pPathList.end() - 2);
		m_pPathList.push_back(goal);

		m_displayPathList();
	}
}

void PlayScene::m_displayPathList()
{
	for (auto tile : m_pPathList)
	{
		std::cout << "(" << tile->getGridPosition().x << ", " << tile->getGridPosition().y << ")" << std::endl;
	}
	std::cout << "Path Length: " << m_pPathList.size() << std::endl;
}

Tile* PlayScene::m_getTile(const int col, const int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(const glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_getTile(col, row);

}

void PlayScene::m_resetPathFinding()
{
	m_pPathList.clear();
	m_pPathList.shrink_to_fit();
	m_pOpenList.clear();
	m_pOpenList.shrink_to_fit();
	m_pClosedList.clear();
	m_pClosedList.shrink_to_fit();

	//reset tile statuses
	for (auto tile : m_pGrid)
	{
		tile->setTileStatus(UNVISITED);
	}
	m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
	goal_position[0] = m_pTarget->getGridPosition().x;
	goal_position[1] = m_pTarget->getGridPosition().y;

	m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(START);
	start_position[0] = m_pSpaceShip->getGridPosition().x;
	start_position[1] = m_pSpaceShip->getGridPosition().y;

	m_moveCounter = 0;
	m_shipIsMoving = false;
}

void PlayScene::m_resetSimulation()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_resetPathFinding();
	// clear current status of ship and target tiles
	m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
	m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(UNVISITED);

	m_pTarget->getTransform()->position = m_getTile(11, 11)->getTransform()->position + offset;
	m_pTarget->setGridPosition(11.0f, 11.0f);
	m_getTile(11, 11)->setTileStatus(GOAL);
	goal_position[0] = m_pTarget->getGridPosition().x;
	goal_position[1] = m_pTarget->getGridPosition().y;

	m_pSpaceShip->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offset;
	m_pSpaceShip->setGridPosition(1.0f, 3.0f);
	m_getTile(1, 3)->setTileStatus(START);
	start_position[0] = m_pSpaceShip->getGridPosition().x;
	start_position[1] = m_pSpaceShip->getGridPosition().y;


}

void PlayScene::m_moveShip()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	if (m_moveCounter < m_pPathList.size())
	{
		auto pathTile_gridPosition = m_pPathList[m_moveCounter]->getGridPosition();
		m_pSpaceShip->getTransform()->position = m_getTile(pathTile_gridPosition)->getTransform()->position + offset;
		m_pSpaceShip->setGridPosition(pathTile_gridPosition.x, pathTile_gridPosition.y);
		if (Game::Instance().getFrames() % 20 == 0)
		{
			m_moveCounter++;
		}
	}
	else
	{
		m_shipIsMoving = false;
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

	static bool toggleGrid = false;
	if (ImGui::Checkbox("Toggle Grid", &toggleGrid))
	{
		m_isGridEnabled = toggleGrid;
		m_setGridEnabled(m_isGridEnabled);
	}
	
	ImGui::Separator();
	// Heuristic selection

	static int radio = m_currentHeuristic;
	ImGui::Text("Heuristic Type");
	ImGui::RadioButton("Manhattan", &radio, MANHATTAN);
	ImGui::SameLine();
	ImGui::RadioButton("Euclidean", &radio, EUCLIDEAN);

	if (m_currentHeuristic != radio)
	{
		m_currentHeuristic = static_cast<Heuristic>(radio);
		m_computeTileCosts();
	}

	ImGui::Separator();
	if (ImGui::Button("Find Shortest Path"))
	{
		m_findShortestPath();
	}

	ImGui::Separator();
	if (ImGui::Button("Start"))
	{
		if (!m_shipIsMoving)
		{
			m_shipIsMoving = true;
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Reset PathFinding"))
	{
		m_resetPathFinding();
	}

	ImGui::SameLine();
	if (ImGui::Button("Reset Simulation"))
	{
		m_resetSimulation();
	}


	ImGui::Separator();

	// spaceship properties
	start_position[0] = m_pSpaceShip->getGridPosition().x;
	start_position[1] = m_pSpaceShip->getGridPosition().y;
	if (ImGui::SliderInt2("Start Position", start_position, 0,Config::COL_NUM -1))
	{
		if (start_position[1] > Config::ROW_NUM - 1)
		{
			start_position[1] = Config::ROW_NUM - 1;
		}
		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(UNVISITED);
		m_pSpaceShip->getTransform()->position = m_getTile(start_position[0],
			start_position[1])->getTransform()->position + offset;
		m_pSpaceShip->setGridPosition(start_position[0], start_position[1]);
		m_getTile(m_pSpaceShip->getGridPosition())->setTileStatus(START);
		m_resetPathFinding();
	}
	// target properties
	
	goal_position[0] =  m_pTarget->getGridPosition().x;
	goal_position[1] = m_pTarget->getGridPosition().y;

	if(ImGui::SliderInt2("Goal Position", goal_position, 0, Config::COL_NUM - 1))
	{
		if (goal_position[1] > Config::ROW_NUM - 1)
		{
			goal_position[1] = Config::ROW_NUM - 1;
		}
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
		m_pTarget->getTransform()->position = m_getTile(goal_position[0],
			goal_position[1])->getTransform()->position + offset;
		m_pTarget->setGridPosition(goal_position[0], goal_position[1]);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(START);
		m_computeTileCosts();
		m_resetPathFinding();

	}

	ImGui::End();
}

//for reset.

int PlayScene::start_position[2];
int PlayScene::goal_position[2];

