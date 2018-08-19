#include "LevelManager.h"
#include "../TinyXML/tinyxml.h"
#include <sstream>
#include <cstdlib>
#include "GamePlayState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Tile.h"
#include "Player.h"
#include "Lever.h"
#include "PressurePoint.h"
#include "Door.h"
#include "Trap.h"
#include "EntityManager.h"

LevelManager::LevelManager()
{
	//m_hStairImg = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/stair_left.png");
}

LevelManager::~LevelManager()
{
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hStairImg);

	delete m_pLevel;
	m_pLevel = nullptr;

	for (unsigned int s = 0; s < m_vRooms.size(); s++)
	{
		delete m_vRooms[s];
		m_vRooms[s] = nullptr;
	}
	m_vRooms.clear();

	//for (unsigned int a = 0; a < m_vPuzzles.size(); a++)
	//{
	//	m_vPuzzles[a]->Release();
	//	//m_vPuzzles[a] = nullptr;
	//}
	//m_vPuzzles.clear();

	//for (unsigned int l = 0; l < m_vChallenges.size(); l++)
	//{
	//	m_vChallenges[l]->Release();
	//	//m_vChallenges[l] = nullptr;
	//}
	//m_vChallenges.clear();

}

void LevelManager::Update(float elapsedTime)
{
	for (unsigned int x = 0; x < m_vRooms.size(); x++)
	{
		if (m_vRooms[x]->GetRect().IsPointInRectangle(GamePlayState::GetInstance()->GetPlayer()->GetRect().ComputeCenter()))
		{
			m_vRooms[x]->Update(elapsedTime);
			m_vRooms[x]->SetPlayerHere(true);
			if (m_vRooms[3]->IsPlayerHere())
			{
				GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_TrapBox);
			}
		}
		else
			m_vRooms[x]->SetPlayerHere(false);
	}

	//for (unsigned int r = 0; r < m_vPuzzles.size(); r++)
	//{
	//	//if (m_vPuzzles[x]->GetRect().IsPointInRectangle(GamePlayState::GetInstance()->GetPlayer()->GetRect().ComputeCenter()))
	//	{
	//		m_vPuzzles[r]->Update(elapsedTime);
	//	}
	//}

	//for (unsigned int c = 0; c < m_vChallenges.size(); c++)
	//{
	//	//if (m_vPuzzles[x]->GetRect().IsPointInRectangle(GamePlayState::GetInstance()->GetPlayer()->GetRect().ComputeCenter()))
	//	{
	//		m_vChallenges[c]->Update(elapsedTime);
	//	}
	//}
}

void LevelManager::RenderEndPoint()
{
	/*SGD::Rectangle rect = { m_rEndPoint.x, m_rEndPoint.y-32, m_rEndPoint.x + 64, m_rEndPoint.y + 32 };

	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);*/

	//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hStairImg,
	//{ m_rEndPoint.x + Game::GetInstance()->GetWorldSpace().x, m_rEndPoint.y + Game::GetInstance()->GetWorldSpace().y });

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, { 255, 255, 255, 255 }, 5);
}

bool LevelManager::LoadFile(const char* file)
{
	TiXmlDocument doc;

	if (doc.LoadFile(file) == false)
		return false;
	else
	{
		TiXmlElement* root = doc.RootElement();

		if (root == nullptr)
			return false;

		//load map/level info
		TiXmlElement* pLevel = root->FirstChildElement("Level");

		char levelName[34];
		int numRooms;
		char bossName[34];
		int levelrows, levelcols, levelwidth, levelheight;

		strcpy_s(levelName, 34, pLevel->Attribute("Name"));

		if (levelName == nullptr)
			return false;

		strcpy_s(bossName, 34, pLevel->Attribute("Boss"));

		if (bossName == nullptr)
			return false;

		pLevel->Attribute("NumOfRooms", &numRooms);
		pLevel->Attribute("Rows", &levelrows);
		pLevel->Attribute("Cols", &levelcols);
		pLevel->Attribute("Width", &levelwidth);
		pLevel->Attribute("Height", &levelheight);

		if (numRooms <= 0 || levelrows <= 0 || levelcols <= 0 || levelwidth <= 0 || levelheight <= 0)
			return false;

		m_pLevel = new Map(levelrows, levelcols, levelwidth, levelheight);

		//load tileset info
		char tilesetPath[100];
		int tilesetrows, tilesetcols, tilesetwidth, tilesetheight;

		TiXmlElement* pTileset = root->FirstChildElement("Tileset");

		strcpy_s(tilesetPath, 100, pTileset->Attribute("Path"));

		if (tilesetPath == nullptr)
			return false;

		pTileset->Attribute("Rows", &tilesetrows);
		pTileset->Attribute("Cols", &tilesetcols);
		pTileset->Attribute("Width", &tilesetwidth);
		pTileset->Attribute("Height", &tilesetheight);

		if (tilesetrows <= 0 || tilesetcols <= 0 || tilesetwidth <= 0 || tilesetheight <= 0)
			return false;

		//initialize map and tile set
		m_pLevel->InitializeTileset(tilesetPath, tilesetcols, tilesetrows, tilesetwidth, tilesetheight);
		m_pLevel->InitializeMap();

		//load rooms
		TiXmlElement* pRoomlist = root->FirstChildElement("Room_List");
		TiXmlElement* pRoom = pRoomlist->FirstChildElement("Room");

		while (pRoom != nullptr)
		{
			Room* rm = new Room();

			char roomName[64];
			int roomUnlocked, roomNum, roomX, roomY, roomWidth, roomHeight;


			strcpy_s(roomName, 64, pRoom->Attribute("Name"));
			pRoom->Attribute("Unlocked", &roomUnlocked);
			pRoom->Attribute("RoomNum", &roomNum);
			pRoom->Attribute("PosX", &roomX);
			pRoom->Attribute("PosY", &roomY);
			pRoom->Attribute("Width", &roomWidth);
			pRoom->Attribute("Height", &roomHeight);

			rm->SetRoomNum(roomNum);

			if (roomUnlocked == 1)
				rm->SetUnlocked(true);
			else
				rm->SetUnlocked(false);

			rm->SetPosition({ (float)roomX, (float)roomY });
			rm->SetSize({ (float)roomWidth, (float)roomHeight });

			//load tile info
			TiXmlElement* pTilelist = pRoom->FirstChildElement("Tile_List");

			TiXmlElement* pTile = pTilelist->FirstChildElement("Tile");

			//int rowcount = 0, colcount = 0;

			while (pTile != nullptr)
			{

				/*if (colcount >= m_pLevel->GetGridColumnNum())
				{
					rowcount++;
					colcount = 0;
				}*/
				int tX;
				int tY;
				int id;
				int type;

				pTile->Attribute("PosX", &tX);
				pTile->Attribute("PosY", &tY);
				pTile->Attribute("Id", &id);
				pTile->Attribute("Type", &type);
				if (type < 1 || type > 10)
					return false;

				m_pLevel->SetTile(tX, tY, id, (Tile::m_eTiletype)type);

				Tile* tile = m_pLevel->GetTiles()[((tY*m_pLevel->GetGridColumnNum()) + tX)];

				rm->AddTile(tile);
				tile->Release();

				//colcount++;
				pTile = pTile->NextSiblingElement("Tile");
			}

			//load respawn
			TiXmlElement* pRespawnlist = pRoom->FirstChildElement("Respawn_List");

			TiXmlElement* pRespawn = pRespawnlist->FirstChildElement("Respawn");

			while (pRespawn != nullptr)
			{
				int posX, posY;

				pRespawn->Attribute("PosX", &posX);
				pRespawn->Attribute("PosY", &posY);

				rm->SetRespawnpoint(SGD::Point((float)posX, (float)posY));
				pRespawn = pRespawn->NextSiblingElement("Respawn");
			}

			//load escapes
			TiXmlElement* pEscapelist = pRoom->FirstChildElement("Escapepoint_List");

			TiXmlElement* pEscapepoint = pEscapelist->FirstChildElement("EscapePoint");

			while (pEscapepoint != nullptr)
			{
				int posX, posY;

				pEscapepoint->Attribute("PosX", &posX);
				pEscapepoint->Attribute("PosY", &posY);

				rm->AddEscapepoint(SGD::Point((float)posX, (float)posY));
				pEscapepoint = pEscapepoint->NextSiblingElement("EscapePoint");
			}

			//load spawns
			TiXmlElement* pSpawnlist = pRoom->FirstChildElement("Spawn_List");

			TiXmlElement* pSpawn = pSpawnlist->FirstChildElement("Spawn");

			while (pSpawn != nullptr)
			{
				SpawnPoint* sp = new SpawnPoint();

				//char name[64];
				int posX, posY, spawnAmount;
				double spawnRate;
				int type;

				//strcpy_s(name, 64, pSpawn->Attribute("Name"));
				pSpawn->Attribute("Type", &type);
				pSpawn->Attribute("SpawnRate", &spawnRate);
				pSpawn->Attribute("SpawnAmount", &spawnAmount);
				pSpawn->Attribute("PosX", &posX);
				pSpawn->Attribute("PosY", &posY);

				sp->SetPosition(SGD::Point((float)posX, (float)posY));
				sp->SetSpawnTimer((float)spawnRate);
				sp->SetMaxEnemies(spawnAmount);
				sp->SetType((SpawnPoint::m_eObjectType)type);

				//load waypoints
				TiXmlElement* pWaypointlist = pSpawn->FirstChildElement("Waypoint_List");

				TiXmlElement* pWaypoint = pWaypointlist->FirstChildElement("Waypoint");

				while (pWaypoint != nullptr)
				{
					SGD::Point wp;
					int wX, wY;

					pWaypoint->Attribute("PosX", &wX);
					pWaypoint->Attribute("PosY", &wY);

					wp = SGD::Point((float)wX, (float)wY);
					sp->AddWaypoint(wp);
					pWaypoint = pWaypoint->NextSiblingElement("Waypoint");
				}

				rm->AddSpawnPoint(sp);

				pSpawn = pSpawn->NextSiblingElement("Spawn");
			}

			m_vRooms.push_back(rm);
			pRoom = pRoom->NextSiblingElement("Room");
		}

		TiXmlElement* pPuzzlelist = root->FirstChildElement("Puzzle_List");
		TiXmlElement* pPuzzle = pPuzzlelist->FirstChildElement("Puzzle");

		while( pPuzzle != nullptr)
		{
			//Puzzle* pz = new Puzzle();
			//char[64] puzzleName;
			int puzzposX,puzzposY,puzzord,puzzres;
			double puzztimer;
			bool bOrd;
			bool bRes;

			//strcpy_s(puzzleName, 64, pPuzzle->Attribute("Name"));

			pPuzzle->Attribute("PosX",&puzzposX);
			pPuzzle->Attribute("PosY",&puzzposY);
			pPuzzle->Attribute("Ordered",&puzzord);
			pPuzzle->Attribute("Restart",&puzzres);
			pPuzzle->Attribute("Timer",&puzztimer);

			//pz->SetPosition({posX,posY});

			std::vector<Entity*> lockerlst;

			TiXmlElement* pDoorpzlist = pPuzzle->FirstChildElement("Door_List");
			TiXmlElement* pDoorpz = pDoorpzlist->FirstChildElement("Door");


			while( pDoorpz != nullptr)
			{

				int posX, posY, vert, unl;

				 pDoorpz->Attribute("PosX",&posX);
				 pDoorpz->Attribute("PosY",&posY);
				 pDoorpz->Attribute("Vertical",&vert);
				 pDoorpz->Attribute("Unlocked",&unl);

				 Entity* dr;
				 SpawnPoint* sp = new SpawnPoint();
				 if (vert == 1)
				 {
					 sp->SetPosition({ (float)posX, (float)posY });
					 dr = GamePlayState::GetInstance()->CreateVDoor(sp);
				 }
				 else
				 {
					 sp->SetPosition({ (float)posX, (float)posY });
					 dr = GamePlayState::GetInstance()->CreateHDoor(sp);
				 }

				 if (unl == 1)
					 ((Door*)dr)->SetUnlocked(true);
				 else
					 ((Door*)dr)->SetUnlocked(false);

				 lockerlst.push_back(dr);
				 dr->Release();
				 delete sp;
				 pDoorpz = pDoorpz->NextSiblingElement("Door");
			}

			TiXmlElement* pTraplist = pPuzzle->FirstChildElement("Trap_List");
			TiXmlElement* pTrap = pTraplist->FirstChildElement("Trap");

			while(pTrap != nullptr)
			{
				//Trap* tr = new Tile();

				int posX, posY, type;

				pTrap->Attribute("PosX",&posX);
				pTrap->Attribute("PosY",&posY);
				//pTrap->Attribute("Id",&id);
				pTrap->Attribute("Type",&type);

				if (type == Trap::TrapType::TRAP_FIRE)
				{
					Tile* sp = new Tile();
					sp->SetPosition({ (float)posX*m_pLevel->GetGridWidth(), (float)posY*m_pLevel->GetGridHeight() });
					sp->SetSize({ 32, 32 });
					Entity* tr = GamePlayState::GetInstance()->CreateFireTrap(sp);
					lockerlst.push_back(tr);
					delete sp;
					tr->Release();
				}
				else if (type == Trap::TrapType::TRAP_LIGHTING)
				{
					Tile* sp = new Tile();
					sp->SetPosition({ (float)posX*m_pLevel->GetGridWidth(), (float)posY*m_pLevel->GetGridHeight() });
					sp->SetSize({32,32});
					Entity* tr = GamePlayState::GetInstance()->CreateElectricTrap(sp);
					lockerlst.push_back(tr);
					delete sp;
					tr->Release();
				}
				else if (type == Trap::TrapType::TRAP_SPIKE)
				{
					Tile* sp = new Tile();
					sp->SetPosition({ (float)posX*m_pLevel->GetGridWidth(), (float)posY*m_pLevel->GetGridHeight() });
					sp->SetSize({ 32, 32 });
					Entity* tr = GamePlayState::GetInstance()->CreateSpikeTrap(sp);
					lockerlst.push_back(tr);
					delete sp;
					tr->Release();
				}

				pTrap = pTrap->NextSiblingElement("Trap");
			}

			TiXmlElement* pLeverlist = pPuzzle->FirstChildElement("Lever_List");
			TiXmlElement* pLever = pLeverlist->FirstChildElement("Lever");

			std::vector<Lever*> leverlst;

			while(pLever != nullptr)
			{
				int posX, posY, on;


				pLever->Attribute("PosX",&posX);
				pLever->Attribute("PosY",&posY);
				pLever->Attribute("IsOn",&on);

				SGD::Point pt;
				pt = { (float)posX, (float)posY };
				Lever* lr = GamePlayState::GetInstance()->CreateLever(pt, nullptr);
				
				if (on == 1)
					lr->TurnOn();
				else
					lr->TurnOff();

				leverlst.push_back(lr);
				lr->Release();

				pLever = pLever->NextSiblingElement("Lever");
			}

			TiXmlElement* pPPlist = pPuzzle->FirstChildElement("PressurePoint_List");
			TiXmlElement* pPP = pPPlist->FirstChildElement("PressurePoint");
			
			std::vector<PressurePoint*> pplst;

			while(pPP != nullptr)
			{
				int posX, posY;

				pPP->Attribute("PosX",&posX);
				pPP->Attribute("PosY",&posY);

				SGD::Point pt;
				pt = { (float)posX, (float)posY };
				PressurePoint* pp = GamePlayState::GetInstance()->CreatePressurePoint(pt);
				pplst.push_back(pp);
				pp->Release();

				pPP = pPP->NextSiblingElement("PressurePoint");
			}

			if (puzzord == 1)
				bOrd = true;
			else
				bOrd = false;

			if (puzzres == 1)
				bRes = true;
			else
				bRes = false;

			//pz->SetTimer((float)timer);

			Puzzle* pz = new Puzzle(lockerlst, (float)puzztimer,bRes,pplst,leverlst,bOrd );
			pz->SetPosition({ (float)puzzposX, (float)puzzposY });
			GamePlayState::GetInstance()->GetEntityManager()->AddEntity(pz, GamePlayState::Bucket::BUCKET_PUZZLES);
			pz->Release();

			pPuzzle = pPuzzle->NextSiblingElement("Puzzle");
		}

		TiXmlElement* pChallengelist = root->FirstChildElement("Challenge_List");
		TiXmlElement* pChallenge = pChallengelist->FirstChildElement("Challenge");

		while(pChallenge != nullptr)
		{
			int posX, posY,numNeut,numZom;
			double timer;
			//pChallenge->Attribute("Name");
			pChallenge->Attribute("PosX",&posX);
			pChallenge->Attribute("PosY",&posY);
			pChallenge->Attribute("NumOfNeutral",&numNeut);
			pChallenge->Attribute("NumOfZombies",&numZom);
			pChallenge->Attribute("Timer",&timer);


			TiXmlElement* pDoorchlist = pChallenge->FirstChildElement("Door_List");
			TiXmlElement* pDoorch = pDoorchlist->FirstChildElement("Door");

			//while(pDoorch != nullptr)
			//{
			//Door dr = new Door();


			if (pDoorch != nullptr)
			{
				Entity* dr;

				int posDoorX, posDoorY, DoorVert, DoorUnl;

				pDoorch->Attribute("PosX", &posDoorX);
				pDoorch->Attribute("PosY", &posDoorY);
				pDoorch->Attribute("Vertical", &DoorVert);
				pDoorch->Attribute("Unlocked", &DoorUnl);

				if (DoorVert == 1)
				{
					SpawnPoint* sp = new SpawnPoint();
					sp->SetPosition({ (float)posDoorX, (float)posDoorY });
					dr = GamePlayState::GetInstance()->CreateVDoor(sp);
					delete sp;
				}
				else
				{
					SpawnPoint* sp = new SpawnPoint();
					sp->SetPosition({ (float)posDoorX, (float)posDoorY });
					dr = GamePlayState::GetInstance()->CreateHDoor(sp);
					delete sp;
				}

				if (DoorUnl == 1)
					((Door*)dr)->SetUnlocked(true);
				else
					((Door*)dr)->SetUnlocked(false);

				Challenge* ch = new Challenge(dr, numZom, numNeut, { (float)posX, (float)posY }, (float)timer);
				GamePlayState::GetInstance()->GetEntityManager()->AddEntity(ch, GamePlayState::Bucket::BUCKET_PUZZLES);
				ch->Release();
				dr->Release();

			}
			//}
			/*else
			{
				Challenge* ch = new Challenge(nullptr, numZom, numNeut, { (float)posX, (float)posY }, (float)timer);
				GamePlayState::GetInstance()->GetEntityManager()->AddEntity(ch, GamePlayState::Bucket::BUCKET_PUZZLES);
				ch->Release();
			}
			*/
			//dr->Release();

			pChallenge = pChallenge->NextSiblingElement("Challenge");
		}

		/*for (unsigned int st = 0; st < m_vRooms.size(); st++)
		{
		if (m_vRooms[st]->GetRoomNum() == 1)
		Game::GetInstance()->SetWorldCamera(m_vRooms[st]->GetRect());
		}*/
	}

	return true;
}