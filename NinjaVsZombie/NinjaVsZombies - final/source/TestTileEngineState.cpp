///Used to test the tile engine

#include "TestTileEngineState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include <cassert>

#include "MainMenuState.h"
#include "LevelManager.h"
#include "Map.h"
#include "TileSet.h"

#include "Game.h"


/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ TestTileEngineState* TestTileEngineState::GetInstance(void)
{
	static TestTileEngineState s_Instance;

	return &s_Instance;
}

/*virtual*/ void TestTileEngineState::Enter(void)
{
	manager = new LevelManager();
	
	//use when checking for load
	assert(manager->LoadFile("resource/XML/testfile.xml") != false
		&& "Level Failed To Load!!!");
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void TestTileEngineState::Exit(void)
{
	delete manager;
	manager = nullptr;
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool TestTileEngineState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	}

	if (pInput->IsKeyPressed(SGD::Key::UpArrow) == true)
	{
		
	}

	if (pInput->IsKeyPressed(SGD::Key::DownArrow) == true)
	{
		
	}

	if (pInput->IsKeyPressed(SGD::Key::LeftArrow) == true)
	{
		Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x + Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetWorldSpace().y });
		Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left - Game::GetInstance()->GetScreenWidth(), 
			Game::GetInstance()->GetWorldCamera().top }, SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() } });
	}

	if (pInput->IsKeyPressed(SGD::Key::RightArrow) == true)
	{
		Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x - Game::GetInstance()->GetScreenWidth(), 
			Game::GetInstance()->GetWorldSpace().y });
		Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left + Game::GetInstance()->GetScreenWidth(), 
			Game::GetInstance()->GetWorldCamera().top }, SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() } });
	}

	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void TestTileEngineState::Update(float elapsedTime)
{
	manager->GetMap()->Update(elapsedTime);
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void TestTileEngineState::Render(void)
{
	//SGD::GraphicsManager::GetInstance()->DrawString("HEY MAN", SGD::Point(200, 200), SGD::Color(255, 255, 255));
	manager->GetMap()->Render();
}