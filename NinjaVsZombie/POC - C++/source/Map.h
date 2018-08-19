#pragma once
#include "Tile.h"
#include "TileSet.h"
#include "IEntity.h"

class Map
{
	//cell location
	int m_nCellX;
	int m_nCellY;

	//grid info
	int m_nGridRows;
	int m_nGridCols;
	int m_nGridWidth;
	int m_nGridHeight;
	
	//tiles in map
	std::vector<Tile*> m_pTiles;

	//tileset
	TileSet* m_pTileset;

public:
	Map(int rows,int cols,int width,int height);
	~Map();

	void InitializeMap(); //sets up the map with a grid size and number of rows/cols and blank tiles
	void InitializeTileset(const char* image,int cols, int rows, int width, int height); //sets up the tileset with image,size and tile ids

	//cell location
	int GetCellX(){ return m_nCellX; }
	int GetCellY(){ return m_nCellY; }

	//grid info
	int GetGridColumnNum(){ return m_nGridCols; }
	int GetGridRowNum(){ return m_nGridRows; }
	int GetGridWidth(){ return m_nGridWidth; }
	int GetGridHeight(){ return m_nGridHeight; }

	void SetGridColumnNum(int cols){ m_nGridCols = cols; }
	void SetGridRowNum(int rows){ m_nGridRows = rows; }
	void SetGridWidth(int width){ m_nGridWidth = width; }
	void SetGridHeight(int height){ m_nGridHeight = height; }

	std::vector<Tile*> GetTiles(){ return m_pTiles; }
	void SetTile(int col, int row,int id,Tile::m_eTiletype type);

	void Update(float elapsedTime);
	void Render();

	void CheckCollision(const IEntity* pOther); // checks collisions for all tiles in map
};

