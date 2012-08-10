/**
* @file Menu.h
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the Menu class
* used for displaying a menu and its buttons.
*/
#ifndef _MENU_H_
#define _MENU_H_

#include "MenuButton.h"
// used for holding the buttons
#include <vector>
using std::vector;

#include "ticktimer.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

enum EMenuType
{
	MT_INTRO1 = 0,
	MT_INTRO2,
	MT_SINGLEPLAYER,
	MT_MULTIPLAYER,
	MT_OPTIONS,
	MT_VENDINGMACHINE,
	MT_CONTROLS,
	MT_CREDITS,
	MT_HIGHSCORE,
	MT_QUIT,
	MT_HUD,
	MT_OBJECTIVEHUD,
	MT_LOADING,
	MT_PAUSE,
	MT_GAMEOVER,
	MT_WIN,
	MT_MAX
};

#define THUMBSTICKTHRESHOLD 0.2f

/**
* Declares the menu class
* used for displaying a menu and its buttons
* 
* @author Jordan Leckner
* @date Created March 06, 2006
* @date Modified April 30, 2006
*/
class CMenu
{
protected:			

	//! Type of the Menu
	int							m_nType;
	//! Vertex Buffer used for displaying of a quad for the menu
	IDirect3DVertexBuffer9*		m_pVertexBuffer;
	//! Texture for the background of the menu
	IDirect3DTexture9*			m_pTextureBackground;
	//! Vector of buttons to display on the menu
	vector<CMenuButton*>		m_Buttons;
	//! Pointer to the child menu, if one
	CMenu*						m_poChildMenu;
	//! Pointer to the parent of this menu
	CMenu*						m_poParentMenu;
	//! Whether or not to display this menu
	bool						m_bDisplay;
	//! Box to display next to current selection
	ID3DXMesh*					m_pBoxSelection;
	//! Rotation variable for box to spin on
	float						m_fBoxRotationY;
	//! Whether or not this menu should be deleted
	bool						m_bDeleteMe;
	//! Timer for the intro menus
	//CTickTimer					m_Timer;
	float						m_fTimer;
	//! Whether or not this class is a singleton
	bool						m_bSingleton;
	//! time to draw menu
	float						m_fTimeToShowMenu;

	//! index of which button was last hovered over
	int							m_nHoverIndex;
	//! index of which button is selected
	int							m_nSelected;
	//! Timer for the 360 thumbstick selection
	float						m_fThumbStickTimer;

	// Default constructor
	CMenu() {}

public:

	//! Accessors
	IDirect3DTexture9* getTextureBackground() { return m_pTextureBackground; }
	vector<CMenuButton*>& getButtons() { return m_Buttons; }
	CMenu* getChildMenu() { return m_poChildMenu; }
	CMenu* getParentMenu() { return m_poParentMenu; }
	bool isDeleteMe() { return m_bDeleteMe; }
	bool isDisplay() { return m_bDisplay; }
	int getType() { return m_nType; }
	bool isSingleton() { return m_bSingleton; }

	//! Mutators
	void setChildMenu(CMenu* poMenu) { m_poChildMenu = poMenu; }
	void setParentMenu(CMenu* poMenu) { m_poParentMenu = poMenu; }
	void deleteMe(bool bDelete = true) { m_bDeleteMe = bDelete; }

	/**
	* Set time to show
	* @param[in]	dwSeconds		seconds to show the menu for
	*/
	inline void setSecondsToShow(float fSeconds)
	{
		//m_Timer.setInterval(fSeconds);
		m_fTimeToShowMenu = fSeconds;
		m_fTimer = CTimer::getInstance().getTime();
	}

	/**
	* Set type of menu
	* @param[in]	nType		the type of menu
	*/
	inline void setType(int nType)
	{
		m_nType = nType;
	}

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
	CMenu(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, int nWidthTex, int nHeightTex, 
		CMenu* poParentMenu = 0);
	
	/**
	* Destructs a Menu.
	* 
	* @date Created March 06, 2006
	*/
	virtual ~CMenu();

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created March 06, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);

	/**
	* Changes whether or not this menu can be displayed
	* 
	* @date Created March 06, 2006
	* @param[in]	bDisplay			Whether or not this menu can be displayed
	*/
	inline void canDisplay(bool bDisplay)
	{
		m_bDisplay = bDisplay;
	}

	/**
	* gets input from keyboard/controllers for menu
	* 
	* @date Created June 18, 2006
	*/
	void catchInput();
};

#endif