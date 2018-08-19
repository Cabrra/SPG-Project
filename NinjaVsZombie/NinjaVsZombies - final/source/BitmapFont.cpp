/***************************************************************
|	File:		BitmapFont.cpp
|	Author:		Jagoba Marcos
|	Course:
|	Purpose:	BitmapFont class draws text using an image
|				of fixed-size character glyphs
***************************************************************/

#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "../TinyXML/tinystr.h"
#include "../TinyXML/tinyxml.h"
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

void BitmapFont::Initialize(const char* filename, bool isUppercase)
{
	m_bOnlyUppercase = isUppercase;

	TiXmlDocument doc;
	if (doc.LoadFile(filename) == false)
		return;
	else
	{
		TiXmlElement* root = doc.RootElement();

		if (root == nullptr)
			return;
		int numAnimation;
		root->Attribute("NumberofAnimations", &numAnimation);
		//load map/level info
		char Name[20];
		TiXmlString Path;
		TiXmlString CharId;
		int numFrames, RectL, RectT, RectR, RectB, PosX, PosY;
		TiXmlElement* pAnimation = root->FirstChildElement("Animation");
		strcpy_s(Name, 20, pAnimation->Attribute("Name"));
		if (Name == nullptr)
			return;

		Path = pAnimation->Attribute("Path");

		Path = "resource/graphics/" + Path;
		// Load the image
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(Path.c_str());

		pAnimation->Attribute("NumberofFrames", &numFrames);
		TiXmlElement* pFrames = pAnimation->FirstChildElement("Frame");

		Char ch;
		for (int i = 0; i < numFrames; i++)
		{
			pFrames->Attribute("FrameRectX", &RectL);
			pFrames->Attribute("FrameRectY", &RectT);
			pFrames->Attribute("FrameRectWidth", &RectR);
			pFrames->Attribute("FrameRectHeight", &RectB);
			pFrames->Attribute("AnchorpointX", &PosX);
			pFrames->Attribute("AnchorpointY", &PosY);

			CharId = pFrames->Attribute("EventTrigger");

			ch.CharRect.left = (float)RectL;
			ch.CharRect.top = (float)RectT;
			ch.CharRect.right = (float)RectR + ch.CharRect.left;
			ch.CharRect.bottom = (float)RectB + ch.CharRect.top;
			ch.Anchor.x = (float)PosX;
			ch.Anchor.y = (float)PosY;
			ch.ID = CharId.c_str()[0];
			Characters.push_back(ch);
			pFrames = pFrames->NextSiblingElement("Frame");
		}
		m_nCharHeight = (int)(Characters[0].CharRect.bottom - Characters[0].CharRect.top);
		m_nCharWidth = (int)(Characters[0].CharRect.right - Characters[0].CharRect.left);
	}
}

/**************************************************************/
// Terminate
//	- clean up resources
void BitmapFont::Terminate(void)
{
	// Unload the image
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);

	Characters.clear();
}


/**************************************************************/
// Draw
//	- draw the text one character at a time,
//	  using the Cell Algorithm to calculate the source rect
//void BitmapFont::Draw( const char* output, SGD::Point position,
//			float scale, SGD::Color color ) const
//{
//	// Validate the image
//	assert( m_hImage != SGD::INVALID_HANDLE 
//			&& "BitmapFont::Draw - image was not loaded" );
//
//	// Validate the parameter
//	assert( output != nullptr 
//			&& "BitmapFont::Draw - string cannot be null" );
//	
//	
//	// Is this string invisible?
//	if( output[ 0 ] == '\0'			// string is empty?
//		|| scale == 0.0f			// scale is invisible?
//		|| color.alpha == 0 )		// color is invisible?
//		return;
//	
//	
//	// Store the starting X position for newlines
//	float colStart = position.x;
//
//	// Iterate through the characters in the string
//	for( int i = 0; output[ i ]; i++ )
//	{
//		// Get the current character
//		char ch = output[ i ];
//
//		// Check for whitespace
//		if( ch == ' ' )
//		{
//			// Move to the next position
//			position.x += m_nCharWidth * scale;
//			continue;
//		}
//		else if( ch == '\n' )
//		{
//			// Move to the next row
//			position.y += m_nCharHeight * scale;
//			position.x =  colStart;
//			continue;
//		}
//		else if( ch == '\t' )
//		{
//			// Calculate the number of pixels from the start
//			float pixels = position.x - colStart;
//
//			// Calculate the number of characters from the start
//			int chars = int(pixels / (m_nCharWidth * scale));
//
//			// Calculate the number of characters to add
//			// to get a 4-space alignment
//			int spaces = 4 - (chars%4);
//
//			
//			// Move to the next position
//			position.x += spaces * (m_nCharWidth * scale);
//			continue;
//		}
//
//
//		// Convert to uppercase?
//		if( m_bOnlyUppercase == true )
//			ch = toupper( ch );
//
//
//		// Calculate the tile ID for this character
//		int id = ch - m_cFirstChar;
//
//		// Calculate the source rect for that glyph
//		SGD::Rectangle cell;
//		cell.left	= float( (id % m_nNumCols) * m_nCharWidth  );
//		cell.top	= float( (id / m_nNumCols) * m_nCharHeight );
//		cell.right	= cell.left + m_nCharWidth;
//		cell.bottom	= cell.top  + m_nCharHeight;
//
//		// Draw the character
//		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
//			m_hImage, position, 
//			cell, 0.0f, {}, 
//			color, {scale, scale} );
//
//		
//		// Move to the next position
//		position.x += (int)(m_nCharWidth * scale);
//	}
//}

void BitmapFont::Draw(const char* output, SGD::Point position,
	float scale, SGD::Color color) const
{
	// Validate the image
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	// Validate the parameter
	assert(output != nullptr
		&& "BitmapFont::Draw - string cannot be null");


	// Is this string invisible?
	if (output[0] == '\0'			// string is empty?
		|| scale == 0.0f			// scale is invisible?
		|| color.alpha == 0)		// color is invisible?
		return;


	// Store the starting X position for newlines
	float colStart = position.x;

	// Iterate through the characters in the string
	for (int i = 0; output[i]; i++)
	{
		// Get the current character
		char ch = output[i];

		// Check for whitespace
		if (ch == ' ')
		{
			// Move to the next position
			position.x += m_nCharWidth * scale;
			continue;
		}
		else if (ch == '\n')
		{
			// Move to the next row
			position.y += m_nCharHeight * scale;
			position.x = colStart;
			continue;
		}
		else if (ch == '\t')
		{
			// Calculate the number of pixels from the start
			float pixels = position.x - colStart;

			// Calculate the number of characters from the start
			int chars = int(pixels / (m_nCharWidth * scale));

			// Calculate the number of characters to add
			// to get a 4-space alignment
			int spaces = 4 - (chars % 4);


			// Move to the next position
			position.x += spaces * (m_nCharWidth * scale);
			continue;
		}


		// Convert to uppercase?
		if (m_bOnlyUppercase == true)
			ch = toupper(ch);


		// Calculate the tile ID for this character
		int id = ch - m_cFirstChar;

		// Calculate the source rect for that glyph
		SGD::Rectangle cell;

		for (size_t i = 0; i < Characters.size(); i++)
		{
			if (Characters[i].ID == ch)
			{
				cell.left = Characters[i].CharRect.left;
				cell.top = Characters[i].CharRect.top;
				cell.right = Characters[i].CharRect.right;
				cell.bottom = Characters[i].CharRect.bottom;

				// Draw the character
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(
					m_hImage, { position.x, position.y - (Characters[i].Anchor.y - Characters[i].CharRect.top)*scale },
					cell, 0.0f, {},
					color, { scale, scale });

				// Move to the next position
				position.x += (int)((Characters[i].CharRect.right - Characters[i].CharRect.left) * scale);
				break;
			}
		}
	}
}

///**************************************************************/
//// Draw
////	- draw the text one character at a time,
////	  using the Cell Algorithm to calculate the source rect
//void BitmapFont::Draw(const wchar_t* output, SGD::Point position,
//	float scale, SGD::Color color) const
//{
//	// Validate the image
//	assert(m_hImage != SGD::INVALID_HANDLE
//		&& "BitmapFont::Draw - image was not loaded");
//
//	// Validate the parameter
//	assert(output != nullptr
//		&& "BitmapFont::Draw - string cannot be null");
//
//
//	// Is this string invisible?
//	if (output[0] == L'\0'		// string is empty?
//		|| scale == 0.0f			// scale is invisible?
//		|| color.alpha == 0)		// color is invisible?
//		return;
//
//
//	// Store the starting X position for newlines
//	float colStart = position.x;
//
//	// Iterate through the characters in the string
//	for (int i = 0; output[i]; i++)
//	{
//		// Get the current character (narrowed to ASCII)
//		char ch = (char)output[i];
//
//
//		// Check for whitespace
//		if (ch == ' ')
//		{
//			// Move to the next position
//			position.x += m_nCharWidth * scale;
//			continue;
//		}
//		else if (ch == '\n')
//		{
//			// Move to the next row
//			position.y += m_nCharHeight * scale;
//			position.x = colStart;
//			continue;
//		}
//		else if (ch == '\t')
//		{
//			// Calculate the number of pixels from the start
//			float pixels = position.x - colStart;
//
//			// Calculate the number of characters from the start
//			int chars = int(pixels / (m_nCharWidth * scale));
//
//			// Calculate the number of characters to add
//			// to get a 4-space alignment
//			int spaces = 4 - (chars % 4);
//
//
//			// Move to the next position
//			position.x += spaces * (m_nCharWidth * scale);
//			continue;
//		}
//
//
//		// Convert to uppercase?
//		if (m_bOnlyUppercase == true)
//			ch = toupper(ch);
//
//
//		// Calculate the tile ID for this character
//		int id = ch - m_cFirstChar;
//
//		// Calculate the source rect for that glyph
//		SGD::Rectangle cell;
//		cell.left = float((id % m_nNumCols) * m_nCharWidth);
//		cell.top = float((id / m_nNumCols) * m_nCharHeight);
//		cell.right = cell.left + m_nCharWidth;
//		cell.bottom = cell.top + m_nCharHeight;
//
//		// Draw the character
//		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
//			m_hImage, position,
//			cell, 0.0f, {},
//			color, { scale, scale });
//
//
//		// Move to the next position
//		position.x += (int)(m_nCharWidth * scale);
//	}
//}

void BitmapFont::Draw(const wchar_t* output, SGD::Point position,
	float scale, SGD::Color color) const
{
	// Validate the image
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	// Validate the parameter
	assert(output != nullptr
		&& "BitmapFont::Draw - string cannot be null");


	// Is this string invisible?
	if (output[0] == '\0'			// string is empty?
		|| scale == 0.0f			// scale is invisible?
		|| color.alpha == 0)		// color is invisible?
		return;


	// Store the starting X position for newlines
	float colStart = position.x;

	// Iterate through the characters in the string
	for (int i = 0; output[i]; i++)
	{
		// Get the current character
		char ch = (char)output[i];

		// Check for whitespace
		if (ch == ' ')
		{
			// Move to the next position
			position.x += m_nCharWidth * scale;
			continue;
		}
		else if (ch == '\n')
		{
			// Move to the next row
			position.y += m_nCharHeight * scale;
			position.x = colStart;
			continue;
		}
		else if (ch == '\t')
		{
			// Calculate the number of pixels from the start
			float pixels = position.x - colStart;

			// Calculate the number of characters from the start
			int chars = int(pixels / (m_nCharWidth * scale));

			// Calculate the number of characters to add
			// to get a 4-space alignment
			int spaces = 4 - (chars % 4);


			// Move to the next position
			position.x += spaces * (m_nCharWidth * scale);
			continue;
		}


		// Convert to uppercase?
		if (m_bOnlyUppercase == true)
			ch = toupper(ch);


		// Calculate the tile ID for this character
		int id = ch - m_cFirstChar;

		// Calculate the source rect for that glyph
		SGD::Rectangle cell;

		for (size_t i = 0; i < Characters.size(); i++)
		{
			if (Characters[i].ID == ch)
			{
				cell.left = Characters[i].CharRect.left;
				cell.top = Characters[i].CharRect.top;
				cell.right = Characters[i].CharRect.right;
				cell.bottom = Characters[i].CharRect.bottom;

				// Draw the character
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(
					m_hImage, { position.x,position.y -(Characters[i].Anchor.y - Characters[i].CharRect.top)* scale },
					cell, 0.0f, {},
					color, { scale, scale });

				// Move to the next position
				position.x += (int)((Characters[i].CharRect.right - Characters[i].CharRect.left) * scale);
				break;
			}
		}		
	}
}