#include "LevelManager.h"
#include "../TinyXML/tinyxml.h"
#include <sstream>
#include <cstdlib>

#include "Tile.h"

LevelManager::LevelManager()
{
}

LevelManager::~LevelManager()
{
	delete m_pLevel;
	m_pLevel = nullptr;
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

		char levelName[12];
		int numRooms;
		char bossName[16];
		int levelrows, levelcols, levelwidth, levelheight;

		strcpy_s(levelName, 12, pLevel->Attribute("Name"));

		if (levelName == nullptr)
			return false;

		strcpy_s(bossName, 16, pLevel->Attribute("Boss"));

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
		
		//load tile info
		TiXmlElement* pTilelist = root->FirstChildElement("Tile_List");

		TiXmlElement* pTile = pTilelist->FirstChildElement("Tile");

		int rowcount = 0, colcount = 0;

		while (pTile != nullptr)
		{

			if (colcount >= m_pLevel->GetGridColumnNum())
			{
				rowcount++;
				colcount = 0;
			}

			int id;
			int type;

			pTile->Attribute("Id", &id);
			pTile->Attribute("Type", &type);
			if (type < 1 || type > 4)
				return false;

			m_pLevel->SetTile(colcount, rowcount, id, (Tile::m_eTiletype)type);

			colcount++;
			pTile = pTile->NextSiblingElement("Tile");
		}
	}

	return true;
}

void LevelManager::SaveFile()
{
	//create tinyXML Document
	TiXmlDocument doc;

	//create a tinyXML Declaration
	TiXmlDeclaration* pDecl = new TiXmlDeclaration{ "1.0", "utf-8", "" };

	//link the declaration to the doc
	doc.LinkEndChild(pDecl);

	//all the levels
	TiXmlElement* pRoot = new TiXmlElement{ "Level_Info" };

	//link the root to the document
	doc.LinkEndChild(pRoot);

	TiXmlElement* pLevel = new TiXmlElement{ "Level" };
	pRoot->LinkEndChild(pLevel);

	pLevel->SetAttribute("Name", "Test Level");
	pLevel->SetAttribute("NumOfRooms", 1);
	pLevel->SetAttribute("Boss", "None");
	pLevel->SetAttribute("Rows", 5);
	pLevel->SetAttribute("Cols", 5);
	pLevel->SetAttribute("Width", 32);
	pLevel->SetAttribute("Height", 32);

	TiXmlElement* pTileset = new TiXmlElement{ "Tileset" };
	pRoot->LinkEndChild(pTileset);

	pTileset->SetAttribute("Path", "resource/Graphics/test_tileset.png");
	pTileset->SetAttribute("Rows", 15);
	pTileset->SetAttribute("Cols", 15);
	pTileset->SetAttribute("Width", 32);
	pTileset->SetAttribute("Height", 32);

	TiXmlElement* pTilelist = new TiXmlElement{ "Tile_List" };
	pRoot->LinkEndChild(pTilelist);

	for (int i = 0; i < 25; i++)
	{
		int id = rand() % 20;
		TiXmlElement* pTile = new TiXmlElement{ "Tile" };
		pTilelist->LinkEndChild(pTile);
		pTile->SetAttribute("Id", id);
		pTile->SetAttribute("Type", 1);
	}

	doc.SaveFile("resource/XML/testfile.xml");
}