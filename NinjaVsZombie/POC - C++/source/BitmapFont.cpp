/***************************************************************
|	File:		BitmapFont.cpp
|	Author:		Jagoba Marcos
|	Course:		 
|	Purpose:	BitmapFont class draws text using an image
|				of fixed-size character glyphs
***************************************************************/

#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <ctype.h>
#include <cassert>


/**************************************************************/
// Initialize
//	- configure the font for "SGD_Font_Glow.png"
//	- probably should have parameters / config file
void BitmapFont::Initialize(const char* filename, int width, int height, int rows, int col)
{
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		filename);

	m_nCharWidth = width;
	m_nCharHeight = height;
	m_nNumRows = rows;
	m_nNumCols = col;

	m_cFirstChar = ' ';		// space
	m_bOnlyUppercase = false;
}


/**************************************************************/
// Terminate
//	- clean up resources
void BitmapFont::Terminate( void )
{
	// Unload the image
	SGD::GraphicsManager::GetInstance()->UnloadTexture( m_hImage );
}


/**************************************************************/
// Draw
//	- draw the text one character at a time,
//	  using the Cell Algorithm to calculate the source rect
void BitmapFont::Draw( const char* output, SGD::Point position,
			float scale, SGD::Color color ) const
{
	// Validate the image
	assert( m_hImage != SGD::INVALID_HANDLE 
			&& "BitmapFont::Draw - image was not loaded" );

	// Validate the parameter
	assert( output != nullptr 
			&& "BitmapFont::Draw - string cannot be null" );
	
	
	// Is this string invisible?
	if( output[ 0 ] == '\0'			// string is empty?
		|| scale == 0.0f			// scale is invisible?
		|| color.alpha == 0 )		// color is invisible?
		return;
	
	
	// Store the starting X position for newlines
	float colStart = position.x;

	// Iterate through the characters in the string
	for( int i = 0; output[ i ]; i++ )
	{
		// Get the current character
		char ch = output[ i ];

		// Check for whitespace
		if( ch == ' ' )
		{
			// Move to the next position
			position.x += m_nCharWidth * scale;
			continue;
		}
		else if( ch == '\n' )
		{
			// Move to the next row
			position.y += m_nCharHeight * scale;
			position.x =  colStart;
			continue;
		}
		else if( ch == '\t' )
		{
			// Calculate the number of pixels from the start
			float pixels = position.x - colStart;

			// Calculate the number of characters from the start
			int chars = int(pixels / (m_nCharWidth * scale));

			// Calculate the number of characters to add
			// to get a 4-space alignment
			int spaces = 4 - (chars%4);

			
			// Move to the next position
			position.x += spaces * (m_nCharWidth * scale);
			continue;
		}


		// Convert to uppercase?
		if( m_bOnlyUppercase == true )
			ch = toupper( ch );


		// Calculate the tile ID for this character
		int id = ch - m_cFirstChar;

		// Calculate the source rect for that glyph
		SGD::Rectangle cell;
		cell.left	= float( (id % m_nNumCols) * m_nCharWidth  );
		cell.top	= float( (id / m_nNumCols) * m_nCharHeight );
		cell.right	= cell.left + m_nCharWidth;
		cell.bottom	= cell.top  + m_nCharHeight;

		// Draw the character
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage, position, 
			cell, 0.0f, {}, 
			color, {scale, scale} );

		
		// Move to the next position
		position.x += (int)(m_nCharWidth * scale);
	}
}

	
/**************************************************************/
// Draw
//	- draw the text one character at a time,
//	  using the Cell Algorithm to calculate the source rect
void BitmapFont::Draw( const wchar_t* output, SGD::Point position,
			float scale, SGD::Color color ) const
{
		// Validate the image
	assert( m_hImage != SGD::INVALID_HANDLE 
			&& "BitmapFont::Draw - image was not loaded" );

	// Validate the parameter
	assert( output != nullptr 
			&& "BitmapFont::Draw - string cannot be null" );
	
	
	// Is this string invisible?
	if( output[ 0 ] == L'\0'		// string is empty?
		|| scale == 0.0f			// scale is invisible?
		|| color.alpha == 0 )		// color is invisible?
		return;


	// Store the starting X position for newlines
	float colStart = position.x;

	// Iterate through the characters in the string
	for( int i = 0; output[ i ]; i++ )
	{
		// Get the current character (narrowed to ASCII)
		char ch = (char)output[ i ];


		// Check for whitespace
		if( ch == ' ' )
		{
			// Move to the next position
			position.x += m_nCharWidth * scale;
			continue;
		}
		else if( ch == '\n' )
		{
			// Move to the next row
			position.y += m_nCharHeight * scale;
			position.x =  colStart;
			continue;
		}
		else if( ch == '\t' )
		{
			// Calculate the number of pixels from the start
			float pixels = position.x - colStart;

			// Calculate the number of characters from the start
			int chars = int(pixels / (m_nCharWidth * scale));

			// Calculate the number of characters to add
			// to get a 4-space alignment
			int spaces = 4 - (chars%4);

			
			// Move to the next position
			position.x += spaces * (m_nCharWidth * scale);
			continue;
		}


		// Convert to uppercase?
		if( m_bOnlyUppercase == true )
			ch = toupper( ch );


		// Calculate the tile ID for this character
		int id = ch - m_cFirstChar;

		// Calculate the source rect for that glyph
		SGD::Rectangle cell;
		cell.left	= float( (id % m_nNumCols) * m_nCharWidth  );
		cell.top	= float( (id / m_nNumCols) * m_nCharHeight );
		cell.right	= cell.left + m_nCharWidth;
		cell.bottom	= cell.top  + m_nCharHeight;

		// Draw the character
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage, position, 
			cell, 0.0f, {}, 
			color, {scale, scale} );

		
		// Move to the next position
		position.x += (int)(m_nCharWidth * scale);
	}
}
