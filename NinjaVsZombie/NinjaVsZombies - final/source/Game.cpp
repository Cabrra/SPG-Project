/***************************************************************
|	File:		Game.cpp
|	Author:		Jagoba Macos
|	Course:		 
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "ParticleManager.h"
#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"
#include "GamePlayState.h"

#include <ctime>
#include <cstdlib>
#include <cassert>
#include <Shlobj.h>
#include "AnimationManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


/**************************************************************/
// Singleton
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance
//	- allocate the ONE instance & return it
/*static*/ Game* Game::GetInstance( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the ONE instance
/*static*/ void Game::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}



/**************************************************************/
// Initialize
//	- initialize the SGD wrappers
//	- enter the Main Menu state
bool Game::Initialize( float width, float height )
{
	// Seed First!
	srand( (unsigned int)time( nullptr ) );
	rand();

	ShowCursor(false);

	HRESULT hr;
	ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &wszPath);

	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);

	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	string pathtowrite = stringstream.str();

	// Add the company and game information
	m_FilePath = pathtowrite + "\\DONTASKQUESTIONS\\NINJAVSZOMBIES\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, m_FilePath.c_str(), 0);



	// Store the size parameters
	m_fScreenWidth = width;
	m_fScreenHeight = height;

	//SetWorldSize({ 800, 600 });

	// Local pointers to the SGD singletons
	SGD::AudioManager*	  pAudio = SGD::AudioManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager*	  pInput = SGD::InputManager::GetInstance();
	SGD::EventManager::GetInstance()->Initialize();
	
	// Initialize the wrappers
	if (pAudio->Initialize() == false
		|| pGraphics->Initialize(false) == false
		|| pInput->Initialize() == false)
		return false;

	// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize("resource/XML/testFont.xml", false);

	//***********************DO NOT CHANGE THIS ORDER!!
	//load all emitters
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/mainBlossom.xml");	//0
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/mainCloudsR.xml");	//1
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/mainCloudsL.xml");	//2
	//PowerUps
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/speed.xml");			//3
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/strength.xml");		//4
	//Smokebomb/Traps/ blood
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/smokebomb.xml");		//5
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/fire.xml");			//6
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/electric.xml");		//7
	ParticleManager::GetParticleInstance()->LoadEmiter("resource/XML/Particles/lilBomb.xml");		//8
	//blood

	//***********************DO NOT CHANGE THIS ORDER!! END ***************************************************

	SGD::GraphicsManager::GetInstance()->Resize({ m_fScreenWidth, m_fScreenHeight }, false);
	m_gFullScreen = false;


	m_hButtonbar = pGraphics->LoadTexture("resource/Graphics/buttonbar.png");
	m_hButtonball = pGraphics->LoadTexture("resource/Graphics/buttonball.png");

	// Start the game in the Main Menu state
	ChangeState( MainMenuState::GetInstance() );
	

	// Store the current time (in milliseconds)
	m_ulGameTime	= GetTickCount();

	m_fWorldCamera = { 0, 0, m_fScreenWidth, m_fScreenHeight - HUDHEIGHT };
	m_WorldSpace = { 0, 0 };


	return true;	// success!
}


/**************************************************************/
// Main
//	- update the SGD wrappers
//	- run the current state
int Game::Update( void )
{
	// Update the wrappers
	if( SGD::AudioManager::GetInstance()->Update() == false 
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false)
		return -10;		// exit FAILURE!
	
	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();					// current time in milliseconds
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;	// convert to fraction of a second
	m_ulGameTime = now;									// store the current time

	// Cap the elapsed time to 1/8th of a second
	if( elapsedTime > 0.125f )
		elapsedTime = 0.125f;
	
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyDown(SGD::Key::Enter) == true)
	{
		if (m_gFullScreen == true)
		{
			SGD::GraphicsManager::GetInstance()->Resize({ m_fScreenWidth, m_fScreenHeight }, false);
			m_gFullScreen = false;
		}
		else
		{
			SGD::GraphicsManager::GetInstance()->Resize({ m_fScreenWidth, m_fScreenHeight }, true);
			m_gFullScreen = true;
		}
		return 0;
	}

	// Let the current state handle input
	IGameState* pCurrent = m_pCurrState;

	if (m_pCurrState->Input() == false)
		return 1;	// exit success! 

	// Update & render the current state if it was not changed
	if( m_pCurrState == pCurrent )
	{
		m_pCurrState->Update( elapsedTime );
		m_pCurrState->Render();
	}

	return 0;		// keep playing!
}


/**************************************************************/
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate( void )
{
	// Exit the current state
	ChangeState( nullptr );

	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hButtonbar);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hButtonball);

	ParticleManager::GetParticleInstance()->RemoveAll();
	ParticleManager::GetParticleInstance()->DeleteParticleInstance();

	AnimationManager::GetInstance()->Terminate();

	// Terminate the core SGD wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();

	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
}


/**************************************************************/
// ChangeState
//	- exit the current state to release resources
//	- enter the new state to allocate resources
//	- DANGER! Exiting the current state can CRASH the program!
//	  The state can ONLY be exited from the
//	  Input, Update, and Render methods!!!
void Game::ChangeState( IGameState* pNewState )
{
	// Exit the old state
	if( m_pCurrState != nullptr )
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNewState;

	// Enter the new state
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();
}
