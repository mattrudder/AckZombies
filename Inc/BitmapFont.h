/**
* @file BitmapFont.h
* @author Matt Rudder
* @date Created Mar 28, 2006
*
* Handles displaying of text to the screen, using a bitmap font.
*/

#ifndef _BITMAPFONT_H_
#define _BITMAPFONT_H_

// Engine includes
#include "AcespaceEngine.h"
#include "Texture2D.h"
#include "BaseResource.h"

// System includes
#include <map>

/**
* Handles displaying of text to the screen, using a bitmap font
*
* @author Matt Rudder
* @date Created Mar 28, 2006
* @date Modified Mar 28, 2006
*/
class CBitmapFont : public CBaseResource
{
protected:
	//! The texture used by the font.
	CTexture2D* m_poTexture;
	//! Collection of char/size mappings
	std::map<char, char> m_mCharMap;
	//! Size of each tile
	unsigned char m_cTileSize;
	//! Tiles per row/col
	unsigned char m_cTilesAcross;
	//! Height of a line
	unsigned char m_cLineHeight;

	/**
	* Helper function that provides a rectangle containing the requested char.
	*
	* @date Created Mar 29, 2006
	* @param[in]	cChar	The character to find the bounds of.
	* @param[in]	pRect	The rectangle to fill out with the bounds.
	*/
	void getCharRect(char cChar, RECT* pRect);

	/**
	* Helper function that provides a rectangle, showing the size of the requested string.
	*
	* @date Created Apr 04, 2006
	* @param[in]	sString		The string to measure.
	* @param[out]	pRect		The rectangle to store the result in; NULL if the bounds aren't required.
	* @return The width of the string.
	*/
	unsigned int measureString(CString sString, RECT* pRect = NULL);

	/**
	* Sprite handler functions
	* @{
	*/
	void beginSprite(void);
	void endSprite(void);
	//!@}

public:
	CBitmapFont(void);
	virtual ~CBitmapFont(void);

	unsigned char getLineHeight(void) { return m_cLineHeight; }

	/**
	* Loads a font image and definition file from disk.
	*
	* @date Created Mar 28, 2006
	* @param[in]	sFilename	The font to load
	* @return true if the file and texture were loaded, false otherwise
	*/
	static CBaseResource* createFontFromFile(const CString sFilename);

	/**
	* Draws a string a text using the currently loaded font.
	*
	* @date Created Mar 29, 2006
	* @param[in]	str		The string to print.
	* @param[in]	fX		The x position the box of text should start.
	* @param[in]	fY		The y position the box of text should start.
	* @param[in]	dwColor	The color to draw the entire string as.
	* @param[in]	bHandleSprite	Determines if D3DX Sprite handling needs to be taken care of.
	*/
	void drawString(CString str, float fX, float fY, D3DCOLOR dwColor = 0xFFFFFFFF, bool bHandleSprite = true);

	/**
	* Draws a string a text using the currently loaded font. Supports embedded color
	* changes.
	*
	* @date Created Mar 29, 2006
	* @param[in]	str		The string to print.
	* @param[in]	fX		The x position the box of text should start.
	* @param[in]	fY		The y position the box of text should start.
	* @param[in]	dwColor	The color to begin drawing as.
	* @param[in]	bHandleSprite	Determines if D3DX Sprite handling needs to be taken care of.
	* @remark In order to change the text color mid-string, use the following 
	*		  token: {hexcolor} 
	*		  For example, to change the color to red, use "{0xFFFF0000}"
	*/
	void drawStringMarkup(CString str, float fX, float fY, D3DCOLOR dwColor = 0xFFFFFFFF, bool bHandleSprite = true);

	/**
	* Draws a scrollable view of a extra-long string. Useful for windows of text
	*
	* @date Created Apr 4, 2006
	* @param[in]	str			The string to print
	* @param[in]	rView		Rectangle defining the view to render into.
	* @param[in]	unOffset	The line number, starting from the bottom to render.
	* @param[in]	dwColor		The color to begin drawing as.
	* @param[in]	bHandleSprite	Determines if D3DX Sprite handling needs to be taken care of.
	* @remark Uses drawStringMarkup syntax for actual string rendering. See 
	*		  drawStringMarkup for markup format.
	*/
	void drawStringView(CString str, RECT rView, unsigned int unOffset = 0, D3DCOLOR dwColor = 0xFFFFFFFF, bool bHandleSprite = true);
};

#endif //_BITMAPFONT_H_