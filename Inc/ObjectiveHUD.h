/**
* @file ObjectiveHUD.h
* @author Jordan Leckner
* @date Created June 5, 2006
*
* This file includes the ObjectiveHUD class
* used for displaying n ObjectiveHUD and its buttons.
*/
#ifndef _OBJECTIVEHUDMENU_H_
#define _OBJECTIVEHUDMENU_H_

#include "MenuManager.h"
#include "HUD.h"
#include "MainButtons.h"
#include "objectmanager.h"

/**
* Declares the ObjectiveHUD menu class
* used for displaying a ObjectiveHUD menu and its buttons
* 
* @author Jordan Leckner
* @date Created June 5, 2006
* @date Modified June 10, 2006
*/
class CObjectiveHUDMenu : public CHUDMenu
{
private:

	//! Background for the objectives
	CMenuButton*	m_poObjectivesBG;
	//! Hold on to the list of objectives
	vector<CString> m_vstrObjectives;
	//! list of corresponding order numbers
	vector<unsigned int>	m_vunOrderNumbers;

public:
	/**
	* Constructs a menu
	* 
	* @date Created April 12, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	iHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CObjectiveHUDMenu();

	~CObjectiveHUDMenu()
	{
		delete m_poObjectivesBG;
	}

	// Accessor
	CMenuButton* getBG(void) { return m_poObjectivesBG; }

public:

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created		June 5, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
};


#endif