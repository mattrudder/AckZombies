/**
* @file HUD.h
* @author Jordan Leckner
* @date Created April 12, 2006
*
* This file includes the HUD class
* used for displaying n HUD and its buttons.
*/

#ifndef _HUDMENU_H_
#define _HUDMENU_H_

#include "MenuManager.h"
#include "Menu.h"
#include "MainButtons.h"
#include "objectmanager.h"
#include "player.h"

#define MAX_AWESOME 512
#define MAX_AWESOME_TEX 327
#define MAX_AWESOME_BUFFER 2

/**
* Declares the HUD menu class
* used for displaying a HUD menu and its buttons
* 
* @author Jordan Leckner
* @date Created April 12, 2006
* @date Modified June 5, 2006
*/
class CHUDMenu : public CMenu
{
	friend class CObjectiveHUDMenu;
	friend class CMenuManager;
	friend class CPauseState;
private:

	//! HUD Background
	CMenuButton *m_poHUDBackground;

	//! Player one's health
	CTextButton *m_poPlayer1Health;
	//! Player one's ammo
	CTextButton *m_poPlayer1Ammo;

	//! Player two's health
	CTextButton *m_poPlayer2Health;
	//! Player two's ammo
	CTextButton *m_poPlayer2Ammo;

	//! Players' score
	CTextButton *m_poPlayerScore;

	//! Awesome meter
	CMenuButton *m_poAwesomeMeter;

	//! Current Awesome Meter Width
	int			m_nAwesomeMeterWidth;

	//! User reticles
	CMenuButton	*m_poReticleP1;
	CMenuButton	*m_poReticleP2;
	//! Width and Height of Reticle
	D3DXVECTOR2 m_ReticleSize;

	//! Player weapon model
	CMesh		*m_poWeaponMesh;

	//! Player weapon model
	CMesh		*m_poGrenadeMesh;
	//! Color for the grenade
	D3DCOLOR	m_dwgrenadeColor;

	//! Player one's grenade ammo
	CTextButton *m_poPlayer1GrenadeAmmo;
	//! Player two's grenade ammo
	CTextButton *m_poPlayer2GrenadeAmmo;

	//! Temporary AwesomeMeterBuffer
	int			m_nAwesomeMeterBuffer;

	//! the font to draw the objective description
	CBitmapFont* m_poObjectiveFont;

	//! Arrow to point to the next objective
	CMesh		*m_pObjectiveArrow;

	//! Background for the objective text
	CMenuButton*	m_poObjectiveTextBG;

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
	CHUDMenu();

	~CHUDMenu()
	{
		delete m_poHUDBackground;
		delete m_poReticleP1;
		delete m_poReticleP2;
		CMenu::~CMenu();
	}

public:

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created		April 12, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);

	/**
	* Adds width to the awesome meter
	* 
	* @date Created		April 28, 2006
	* @param[in]	nNum		Number to increase by
	*/
	void addToAwesomeMeter(int nNum);
};


#endif