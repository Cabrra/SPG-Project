#include "Puzzle.h"
#include "Game.h"
#include "PressurePoint.h"
#include "Unbreakable.h"
#include "Trap.h"
#include "Door.h"

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>

#include "BitmapFont.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

Puzzle::Puzzle(std::vector<Entity*>	locker, float timer, bool restart, std::vector<PressurePoint*> pressureList, std::vector<Lever*> leverList,
	bool order) : Listener(this)
{
	Listener::RegisterForEvent("PUZZLE_OBJECT_ACTIVATED");
	Listener::RegisterForEvent("PRESSURE_COMPRESSED");
	Listener::RegisterForEvent("PRESSURE_DECOMPRESSED");
	for (unsigned int i = 0; i < pressureList.size(); i++)
		m_PressurePointList.push_back(pressureList[i]);

	for (unsigned int i = 0; i < leverList.size(); i++)
		m_LeverList.push_back(leverList[i]);
	m_Timer = timer;
	m_HasOrder = order;
	m_CurrTime = 0.0f;
	m_IsSolved = false;
	SetSize({ 5, 5 }); // to have GetRect
	SetRestart(restart);
	for (unsigned int i = 0; i < locker.size(); i++)
		AddLocker(locker[i]);

	dooropensnd = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Dooropen.wav");
	doorclosesnd = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Doorclose.wav");
}


Puzzle::~Puzzle()
{
	m_PressurePointList.clear();
	m_LeverList.clear();
	m_CurrentLeverList.clear();
	m_CurrentPressurePointList.clear();
	for (unsigned int i = 0; i < m_lockedVector.size(); i++)
		m_lockedVector[i]->Release();
	m_lockedVector.clear();

	SGD::AudioManager::GetInstance()->UnloadAudio(dooropensnd);
	SGD::AudioManager::GetInstance()->UnloadAudio(doorclosesnd);
}

void Puzzle::Render()
{
	if (m_IsSolved == true)
	{
		const BitmapFont* pFont = Game::GetInstance()->GetFont();

		/*SGD::Rectangle myRect = SGD::Rectangle(
			(Game::GetInstance()->GetWorldCamera().left + 140),
			(Game::GetInstance()->GetWorldCamera().top + 70),
			(Game::GetInstance()->GetWorldCamera().right - 240),
			(Game::GetInstance()->GetWorldCamera().bottom - 100));*/

		HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

		char buf[255];

		LoadString(HInstance, IDS_PUZZLESOLVED, buf, 255);

		pFont->Draw(buf, { 150, 50 }, 0.8f, { 0, 0, 0 });
		pFont->Draw(buf, { 150, 53 }, 0.8f, { 255, 255, 255 });
	}
}
void Puzzle::AddLocker(Entity* newLocker)
{
	if (newLocker != nullptr)
	{
		m_lockedVector.push_back(newLocker);
		newLocker->AddRef();
	}

}

/*virtual*/ void Puzzle::Update(float elapsedTime)		/*override*/
{
	if (m_bRestart == true)
	{
		if (m_CurrTime > 0)
		{
			m_CurrTime -= elapsedTime;

			if (m_CurrTime < 0)
			{
				if (m_isBeenRestarted == false)
				{

					m_isBeenRestarted = true;
					m_CurrTime = m_Timer;
					for (unsigned int i = 0; i < m_CurrentLeverList.size(); i++)
					{
						bool act = m_CurrentLeverList[i]->GetActive();
						m_CurrentLeverList[i]->SetActive(!act);
					}
					m_CurrentLeverList.clear();
					for (unsigned int i = 0; i < m_CurrentPressurePointList.size(); i++)
						m_CurrentPressurePointList[i]->SetCompressed(false);
					m_CurrentPressurePointList.clear();

					if (m_IsSolved == true)
					{
						m_IsSolved = false;
						for (unsigned int i = 0; i < m_lockedVector.size(); i++)
						{
							if (m_lockedVector[i]->GetType() == ENT_TRAP || m_lockedVector[i]->GetType() == ENT_FIRETRAP || m_lockedVector[i]->GetType() == ENT_SPIKETRAP
								|| m_lockedVector[i]->GetType() == ENT_ELECTRICTRAP || m_lockedVector[i]->GetType() == ENT_SPAWNTRAP)
							{
								Trap* mytr = (Trap*)m_lockedVector[i];
								bool act = mytr->GetActive();
								mytr->SetActive(!act);
							}
							else if (m_lockedVector[i]->GetType() == Entity::ENT_DOOR)
							{
								Door* mydur = (Door*)m_lockedVector[i];

								bool act = mydur->GetUnlocked();
								mydur->SetUnlocked(!act);

								if (mydur->GetUnlocked() == true)
									SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
								else
									SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);

							}
						}
						m_CurrentLeverList.clear();
					}
				}
			}
		}
	}
}

/*virtual*/ void Puzzle::HandleEvent(const SGD::Event* pEvent) /*override;*/
{
	if (pEvent->GetEventID() == "PUZZLE_OBJECT_ACTIVATED")
	{
		bool found = false;
		unsigned int pos = 0;

		Lever* myLev = (Lever*)pEvent->GetSender(); // only levers can send this event
		m_isBeenRestarted = false;
		m_CurrTime = m_Timer;

		for (unsigned int i = 0; i < m_LeverList.size(); i++) //check if this puzzle has the lever
		{
			if (m_LeverList[i] == myLev)
			{
				found = true;
				pos = i;
				break;
			}
		}

		if (m_IsSolved == false) //if is not solved yet
		{
			if (found == true && myLev->GetActive() == true)
			{
				m_CurrentLeverList.push_back(myLev);

				if (m_HasOrder == false) //*******************ORDERLESS
				{
					if (CheckLevers() == true)
					{
						m_IsSolved = true;
						for (unsigned int i = 0; i < m_lockedVector.size(); i++)
						{
							if (m_lockedVector[i]->GetType() == ENT_TRAP || m_lockedVector[i]->GetType() == ENT_FIRETRAP || m_lockedVector[i]->GetType() == ENT_SPIKETRAP
								|| m_lockedVector[i]->GetType() == ENT_ELECTRICTRAP || m_lockedVector[i]->GetType() == ENT_SPAWNTRAP)
							{
								Trap* mytr = (Trap*)m_lockedVector[i];
								bool act = mytr->GetActive();
								mytr->SetActive(!act);
							}
							else if (m_lockedVector[i]->GetType() == Entity::ENT_DOOR)
							{
								Door* mydur = (Door*)m_lockedVector[i];

								bool act = mydur->GetUnlocked();
								mydur->SetUnlocked(!act);

								if (mydur->GetUnlocked() == true)
									SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
								else
									SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);

							}
						}
						//m_CurrentLeverList.clear();
					}
				}
				else //***************************************ORDERED
				{
					if (CheckOrderedLevers() == 1) //solved
					{
						m_IsSolved = true;
						for (unsigned int i = 0; i < m_lockedVector.size(); i++)
						{
							if (m_lockedVector[i]->GetType() == ENT_TRAP || m_lockedVector[i]->GetType() == ENT_FIRETRAP || m_lockedVector[i]->GetType() == ENT_SPIKETRAP
								|| m_lockedVector[i]->GetType() == ENT_ELECTRICTRAP || m_lockedVector[i]->GetType() == ENT_SPAWNTRAP)
							{
								Trap* mytr = (Trap*)m_lockedVector[i];
								bool act = mytr->GetActive();
								mytr->SetActive(!act);
							}
							else if (m_lockedVector[i]->GetType() == Entity::ENT_DOOR)
							{
								Door* mydur = (Door*)m_lockedVector[i];

								bool act = mydur->GetUnlocked();
								mydur->SetUnlocked(!act);

								if (mydur->GetUnlocked() == true)
									SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
								else
									SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);
							}
						}
						m_CurrentLeverList.clear();
					}
					else if (CheckOrderedLevers() == -1) //Wrong order
					{
						for (unsigned int i = 0; i < m_CurrentLeverList.size(); i++)
							m_CurrentLeverList[i]->SetActive(false);
						m_CurrentLeverList.clear();
					}
				}
			}
		}
		if (found == true && myLev->GetActive() == false) //DEACTIVATE
		{
			if (m_IsSolved == true)
			{
				m_IsSolved = false;
				for (unsigned int i = 0; i < m_lockedVector.size(); i++)
				{
					if (m_lockedVector[i]->GetType() == ENT_TRAP || m_lockedVector[i]->GetType() == ENT_FIRETRAP || m_lockedVector[i]->GetType() == ENT_SPIKETRAP
						|| m_lockedVector[i]->GetType() == ENT_ELECTRICTRAP || m_lockedVector[i]->GetType() == ENT_SPAWNTRAP)
					{
						Trap* mytr = (Trap*)m_lockedVector[i];
						bool act = mytr->GetActive();
						mytr->SetActive(!act);
					}
					else if (m_lockedVector[i]->GetType() == Entity::ENT_DOOR)
					{
						Door* mydur = (Door*)m_lockedVector[i];

						bool act = mydur->GetUnlocked();
						mydur->SetUnlocked(!act);

						if (mydur->GetUnlocked() == true)
							SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
						else
							SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);
					}
				}
			}

			if (m_HasOrder == false) //ORDERLESS
			{
				if (m_IsSolved == true)
					m_IsSolved = false;

				if (m_CurrentLeverList.size() > 0)
					m_CurrentLeverList.erase(m_CurrentLeverList.begin() + pos);
			}
			else //ORDERED
			{
				for (unsigned int i = 0; i < m_CurrentLeverList.size(); i++)
					m_CurrentLeverList[i]->SetActive(false);
				m_CurrentLeverList.clear();
			}
		}
	}

	else if (pEvent->GetEventID() == "PRESSURE_COMPRESSED")
	{
		bool found = false;

		PressurePoint* myPres = (PressurePoint*)pEvent->GetSender();

		for (unsigned int i = 0; i < m_PressurePointList.size(); i++)
		{
			if (m_PressurePointList[i] == myPres)
			{
				found = true;
				break;
			}
		}
		for (unsigned int i = 0; i < m_CurrentPressurePointList.size(); i++)
		{
			if (m_CurrentPressurePointList[i] == myPres)
			{
				found = false;
				break;
			}
		}
		if (found == true && m_IsSolved == false)
		{
			m_CurrentPressurePointList.push_back(myPres);

			if (CheckPressurePoints() == true)
			{
				m_IsSolved = true;
				for (unsigned int i = 0; i < m_lockedVector.size(); i++)
				{
					if (m_lockedVector[i]->GetType() == ENT_TRAP || m_lockedVector[i]->GetType() == ENT_FIRETRAP || m_lockedVector[i]->GetType() == ENT_SPIKETRAP
						|| m_lockedVector[i]->GetType() == ENT_ELECTRICTRAP || m_lockedVector[i]->GetType() == ENT_SPAWNTRAP)
					{
						Trap* mytr = (Trap*)m_lockedVector[i];
						bool act = mytr->GetActive();
						mytr->SetActive(!act);
					}
					else if (m_lockedVector[i]->GetType() == Entity::ENT_DOOR)
					{
						Door* mydur = (Door*)m_lockedVector[i];

						if (mydur->GetUnlocked() == true)
							SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
						else
							SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);

						bool act = mydur->GetUnlocked();
						mydur->SetUnlocked(!act);

					}
				}
			}
		}
	}

	else if (pEvent->GetEventID() == "PRESSURE_DECOMPRESSED")
	{
		unsigned int pos = 0;
		bool found = false;
		PressurePoint* myPres = (PressurePoint*)pEvent->GetSender();

		for (unsigned int i = 0; i < m_PressurePointList.size(); i++)
		{
			if (m_PressurePointList[i] == myPres)
			{
				found = true;
				pos = i;
				break;
			}
		}
		for (unsigned int i = 0; i < m_CurrentPressurePointList.size(); i++)
		{
			if (m_CurrentPressurePointList[i] == myPres)
			{
				found = true;
				pos = i;
				break;
			}
		}

		if (m_IsSolved == true && found == true)
		{
			m_IsSolved = false;
			for (unsigned int i = 0; i < m_lockedVector.size(); i++)
			{
				if (m_lockedVector[i]->GetType() == ENT_TRAP || m_lockedVector[i]->GetType() == ENT_FIRETRAP || m_lockedVector[i]->GetType() == ENT_SPIKETRAP
					|| m_lockedVector[i]->GetType() == ENT_ELECTRICTRAP || m_lockedVector[i]->GetType() == ENT_SPAWNTRAP)
				{
					Trap* mytr = (Trap*)m_lockedVector[i];
					bool act = mytr->GetActive();
					mytr->SetActive(!act);
				}
				else if (m_lockedVector[i]->GetType() == Entity::ENT_DOOR)
				{
					Door* mydur = (Door*)m_lockedVector[i];
					bool act = mydur->GetUnlocked();
					mydur->SetUnlocked(!act);

					if (mydur->GetUnlocked() == true)
						SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
					else
						SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);
					m_CurrentPressurePointList.clear();
				}

			}
		}
		/*if (found == true && m_CurrentPressurePointList.size() > 0)
		{
			m_CurrentPressurePointList.erase(m_CurrentPressurePointList.begin() + pos);
		}*/
	}
}

bool Puzzle::CheckLevers()
{
	bool check = true;

	bool inner = false;

	for (unsigned int loop = 0; loop < m_LeverList.size(); loop++)
	{
		for (unsigned int i = 0; i < m_CurrentLeverList.size(); i++)
		{
			if (m_LeverList[loop] == m_CurrentLeverList[i])
			{
				inner = true;
				break;
			}
			else if (i == m_CurrentLeverList.size() - 1)
			{
				inner = false;
				break;
			}
		}
		if (inner == false) //not active
		{
			check = false;
			break;
		}
	}

	return check;
}

bool Puzzle::CheckPressurePoints()
{
	bool check = true;
	bool inner = false;

	for (unsigned int loop = 0; loop < m_PressurePointList.size(); loop++)
	{
		for (unsigned int i = 0; i < m_CurrentPressurePointList.size(); i++)
		{
			if (m_PressurePointList[loop] == m_CurrentPressurePointList[i])
			{
				inner = true;
				break;
			}
			else if (i == m_CurrentPressurePointList.size() - 1)
			{
				inner = false;
				break;
			}
		}
		if (inner == false) //not found
		{
			check = false;
			break;
		}
	}

	return check;
}

int Puzzle::CheckOrderedLevers()
{
	// -1 = wrong order
	// 0. = right order by now
	// 1 = solved
	int  check = -1;
	bool inner = false;
	unsigned int i = 0;
	for (; i < m_CurrentLeverList.size(); i++)
	{
		inner = false;
		if (m_LeverList[i] == m_CurrentLeverList[i])
		{
			inner = true;
			continue;
		}
		else
		{
			break;
		}
	}

	if (i == m_CurrentLeverList.size() && m_CurrentLeverList.size() == m_LeverList.size() && inner == true)
	{
		//m_CurrentLeverList.clear();
		check = 1;
	}
	else if (i == m_CurrentLeverList.size() && m_CurrentLeverList.size() < m_LeverList.size() && inner == true)
		check = 0;


	return check;
}

int Puzzle::CheckOrderedPressurePoints()
{
	// -1 = wrong order
	// 0. = right order by now
	// 1 = solved
	int  check = -1;
	bool inner = false;
	unsigned int i = 0;
	for (; i < m_CurrentPressurePointList.size(); i++)
	{
		inner = false;
		if (m_PressurePointList[i] == m_CurrentPressurePointList[i])
		{
			inner = true;
			continue;
		}
		else
		{
			break;
		}
	}

	if (i == m_CurrentPressurePointList.size() - 1 && m_CurrentPressurePointList.size() == m_PressurePointList.size() && inner == true)
	{
		//m_CurrentPressurePointList.clear();
		check = 1;
	}
	else if (i == m_CurrentPressurePointList.size() - 1 && m_CurrentPressurePointList.size() < m_PressurePointList.size() && inner == true)
		check = 0;


	return check;
}