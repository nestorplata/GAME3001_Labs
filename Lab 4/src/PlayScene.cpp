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
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "thunder", SOUND_SFX);
	m_bDebugView = false;

	m_pTarget = new Target(); // instantiating a new Target object - allocating memory on the Heap
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	addChild(m_pSpaceShip);

	m_pTile = new Tile();
	m_pTile->getTransform()->position = glm::vec2(400.0f, 300.0f);
	m_pTile->setParent(this);
	m_pTile->addLabels();
	addChild(m_pTile);
	m_pTile->setLabelsEnabled(true);

	
	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);


	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 4 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	static bool toggleDebug = false;
	if (ImGui::Checkbox("Toggle Debug", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}
	
	ImGui::Separator();
	// spaceship properties
	static float start_position[2] = { m_pSpaceShip->getTransform()->position.x, m_pSpaceShip->getTransform()->position.y };
	if (ImGui::SliderFloat2("Start Position", start_position, 0.0f, 600.0f))
	{
		m_pSpaceShip->getTransform()->position = glm::vec2(start_position[0], start_position[1]);
	}
	// target properties
	
	static float goal[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Goal Position", goal, 0.0f, 600.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(goal[0], goal[1]);
	}

	ImGui::End();
}
