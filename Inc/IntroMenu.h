/**
* @file IntroMenu.h
* @author Jordan Leckner
* @date Created June 5, 2006
*
* This file includes the IntroMenu class
* used for displaying an Intromenu and its buttons.
*/
#ifndef _INTOMENU_H_
#define _INTROMENU_H_

#include "Menu.h"
#include "MenuButton.h"


/**
* Declares the IntroMenu class
* used for displaying an Intromenu and its buttons
* 
* @author Jordan Leckner
* @date Created June 5, 2006
* @date Modified June 5, 2006
*/
class CIntroMenu : public CMenu
{
protected:			

	//! Intro screen
	CMenuButton* m_pIntro;

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 06, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	iHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CIntroMenu(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, int nWidthTex, int nHeightTex, 
		CMenu* poParentMenu = 0);
	
	/**
	* Destructs an IntroMenu.
	* 
	* @date Created June 5, 2006
	*/
	~CIntroMenu();

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created June 5, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
};

#endif