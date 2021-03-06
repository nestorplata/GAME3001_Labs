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

	if (m_bDebugView)
	{
		Util::DrawCircle(m_pTarget->getTransform()->position, m_pTarget->getWidth() * 0.5f);

		if (m_pSpaceShip->isEnabled())
		{
			Util::DrawCircle(m_pSpaceShip->getTransform()->position, Util::max(m_pSpaceShip->getWidth() * 0.5f, m_pSpaceShip->getHeight() * 0.5f));

		}
	}

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	CollisionManager::squaredRadiusCheck(m_pSpaceShip, m_pTarget);
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	//if (SDL_NumJoysticks() > 0)
	//{
	//	if (EventManager::Instance().getGameController(0) != nullptr)
	//	{
	//		const auto deadZone = 10000;
	//		if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
	//		{
	//			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
	//			m_playerFacingRight = true;
	//		}
	//		else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
	//		{
	//			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
	//			m_playerFacingRight = false;
	//		}
	//		else
	//		{
	//			if (m_playerFacingRight)
	//			{
	//				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
	//			}
	//			else
	//			{
	//				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
	//			}
	//		}
	//	}
	//}


	// handle player movement if no Game Controllers found
	//if (SDL_NumJoysticks() < 1)
	//{
	//	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	//	{
	//		m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
	//		m_playerFacingRight = false;
	//	}
	//	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	//	{
	//		m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
	//		m_playerFacingRight = true;
	//	}
	//	else
	//	{
	//		if (m_playerFacingRight)
	//		{
	//			m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
	//		}
	//		else
	//		{
	//			m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
	//		}
	//	}
	//}
	

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
	m_bDebugView = false;

	//Target Sprie
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(300.0f, 300.0f);
	addChild(m_pTarget);

	//Star_Ship
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.0f);

	m_pSpaceShip->setCurrentHeading(0.0f);
	m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() * 
		m_pSpaceShip->getAccelerationRate();
	m_pSpaceShip->setEnabled(false);
	addChild(m_pSpaceShip);

	// Back Button
	//m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	//m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	//m_pBackButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pBackButton->setActive(false);
	//	TheGame::Instance().changeSceneState(START_SCENE);
	//});

	//m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pBackButton->setAlpha(128);
	//});

	//m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pBackButton->setAlpha(255);
	//});
	//addChild(m_pBackButton);

	//// Next Button
	//m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	//m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	//m_pNextButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pNextButton->setActive(false);
	//	TheGame::Instance().changeSceneState(END_SCENE);
	//});

	//m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pNextButton->setAlpha(128);
	//});

	//m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pNextButton->setAlpha(255);
	//});

	//addChild(m_pNextButton);

	///* Instructions Label */
	//m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	//m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	//addChild(m_pInstructionsLabel);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Target Movement", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	static bool toggleDebug = false;
	if (ImGui::Checkbox("Toggle Debug", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}

	ImGui::Separator();

	static float position[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target Position", position, 0.0f, 600.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(position[0], position[1]);
		m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	}
	ImGui::Separator();

	 //Spaceshp properties
	static bool toggleSeek = m_pSpaceShip->isEnabled();
	if (ImGui::Checkbox("Toggle Seek", &toggleSeek))
	{
		m_pSpaceShip->setEnabled(toggleSeek);
	}

	static float speed = m_pSpaceShip->getMaxSpeed();
	if (ImGui::SliderFloat("Max Speed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
		//m_pSpaceShip->getRigidBody()->velocity = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getMaxSpeed();

	}

	static float acceleration = m_pSpaceShip->getAccelerationRate();
	if (ImGui::SliderFloat("acceleration", &acceleration, 0.0f, 50.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration);
		m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() * m_pSpaceShip->getAccelerationRate();

	}

	static float turn_rate = m_pSpaceShip->getTurnRate();
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);

	}

	if (ImGui::Button("Reset"))
	{
		//reset ship's position
		m_pSpaceShip->getTransform()->position = glm::vec2 (100.0, 100.0);

		//Reset target's position
		m_pTarget->getTransform()->position = glm::vec2(300.0, 300.0);


		m_pSpaceShip->setCurrentHeading(0.0f);
		m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0, 0.0);
		m_pSpaceShip->getRigidBody()->acceleration = m_pSpaceShip->getCurrentDirection() *
			m_pSpaceShip->getAccelerationRate();

		m_pSpaceShip->setTargetPosition(m_pTarget->getTransform()->position);
	}

	ImGui::End();
}
