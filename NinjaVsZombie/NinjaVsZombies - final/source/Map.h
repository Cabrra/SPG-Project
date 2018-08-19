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
	Map(int rows, int cols, int width, int height);
	~Map();

	void InitializeMap(); //sets up the map with a grid size and number of rows/cols and blank tiles
	void InitializeTileset(const char* image, int cols, int rows, int width, int height); //sets up the tileset with image,size and tile ids

	//cell location
	int GetCellX()const{ return m_nCellX; }
	int GetCellY()const{ return m_nCellY; }

	//grid info
	int GetGridColumnNum()const{ return m_nGridCols; }
	int GetGridRowNum()const{ return m_nGridRows; }
	int GetGridWidth()const{ return m_nGridWidth; }
	int GetGridHeight()const{ return m_nGridHeight; }

	void SetGridColumnNum(int cols){ m_nGridCols = cols; }
	void SetGridRowNum(int rows){ m_nGridRows = rows; }
	void SetGridWidth(int width){ m_nGridWidth = width; }
	void SetGridHeight(int height){ m_nGridHeight = height; }



	void CheckCollision(const IEntity* pOther);

	bool CheckCollision(SGD::Point point, const IEntity* pOther);

	std::vector<Tile*> GetTiles()const { return m_pTiles; }
	void SetTile(int col, int row, int id, Tile::m_eTiletype type);

	void Update(float elapsedTime);
	void Render();
};