#pragma once
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Map.h"

class LevelManager
{
private:
	Map* m_pLevel;

public:
	LevelManager();
	~LevelManager();

	bool LoadFile(const char* file);
	void SaveFile();

	Map* GetMap(){ return m_pLevel; }
};
