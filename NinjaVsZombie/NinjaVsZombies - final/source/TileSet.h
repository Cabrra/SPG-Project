#pragma once
#include "Tile.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include <vector>

class TileSet
{
	std::vector<Tile*> m_pTiles; //tiles arranged across the tileset image
	SGD::HTexture m_htTileSetImage = SGD::INVALID_HANDLE;

	int	m_nTileWidth = 0;
	int	m_nTileHeight = 0;
	int	m_nNumRows = 0;
	int	m_nNumCols = 0;

public:
	TileSet(const char* image,int cols,int rows,int width,int height);
	~TileSet();

	int GetTileColumnNum()const{ return m_nNumCols; }
	int GetTileRowNum()const{ return m_nNumRows; }
	int GetTileWidth()const{ return m_nTileWidth; }
	int GetTileHeight()const{ return m_nTileHeight; }
	SGD::HTexture GetImage()const{ return m_htTileSetImage; }

	void SetTileColumnNum(int cols){ m_nNumCols = cols; }
	void SetTileRowNum(int rows){ m_nNumRows = rows; }
	void SetTileWidth(int width){ m_nTileWidth = width; }
	void SetTileHeight(int height){ m_nTileHeight = height; }

	void SetTileSetIds(int tilecount); // sets all the tiles in tileset to an id number
	void DrawTile(int id,SGD::Point position);//draw the tile id at a position
};

