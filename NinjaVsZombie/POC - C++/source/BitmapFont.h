/***************************************************************
|	File:		BitmapFont.h
|	Author:		Jagoba Marcos
|	Course:		 
|	Purpose:	BitmapFont class draws text using an image
|				of fixed-size character glyphs
***************************************************************/

#ifndef BITMAPFONT_H
#define BITMAPFONT_H


#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"


/**************************************************************/
// BitmapFont class
//	- displays text using an image of fixed-size characters
//	- image MUST be in ASCII order!
//	- image can be missing characters, as long as there is space reserved
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
class BitmapFont
{
public:
	/**********************************************************/
	// Constructor & Destructor
	BitmapFont( void )	= default;
	~BitmapFont( void )	= default;

	
	/**********************************************************/
	// Initialize & Terminate
	void Initialize(const char* filename, int width, int height, int rows, int col);	// should have parameters
	// should have parameters
	void Terminate ( void );

	
	/**********************************************************/
	// Draw
	void Draw( const char* output, SGD::Point position,
			   float scale, SGD::Color color ) const;
	
	void Draw( const wchar_t* output, SGD::Point position,
			   float scale, SGD::Color color ) const;

private:
	/**********************************************************/
	// image
	SGD::HTexture	m_hImage			= SGD::INVALID_HANDLE;

	// cell data
	int				m_nCharWidth		= 0;
	int				m_nCharHeight		= 0;
	int				m_nNumRows			= 0;
	int				m_nNumCols			= 0;

	// font info
	char			m_cFirstChar		= '\0';
	bool			m_bOnlyUppercase	= false;
};

#endif //BITMAPFONT_H
