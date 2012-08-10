/**
* @file MenuManager.h
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the Menu Manager class
* used for holding and displaying of menus.
*/ 
#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

#include "Singleton.h"

// using vector for holding all the menus.
#include <vector>
using std::vector;

// use for FVF
#include "MenuButton.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// forward declare the menu class.
class CMenu;
class CHUDMenu;

#include "BaseGameState.h"

/**
* Declares the menu manager system
* used for holding and displaying of menus.
* 
* @author Jordan Leckner
* @date Created March 06, 2006
* @date Modified June 5, 2006
*/
class CMenuManager : public CSingleton<CMenuManager>
{
	friend class CSingleton<CMenuManager>;
private:

	//! The vector of all menus currently allocated.
	vector<CMenu*>				m_Menus;
	//! The current menu being displayed.
	CMenu*						m_poCurrentMenu;
	//! Vertex Buffer used for displaying of a quad for the mouse
	IDirect3DVertexBuffer9*		m_pMouseVertexBuffer;
	//! Vertices of the quad for the mouse
	SMenuFVF					m_tMouseQuad[6];
	//! Texture for the mouse pointer
	IDirect3DTexture9*			m_pMouseTexture;
	//! Mouse width used for moving the mouse around.
	int							m_nMouseWidth;
	//! Mouse height used for moving the mouse around.
	int							m_nMouseHeight;
	//! Whether or not to display the mouse cursor
	bool						m_bDisplayMouseCursor;
	//! HUD is actually just a menu with no buttons.
	CHUDMenu*					m_poHUD;

	//! Actual HUD
	CHUDMenu*					m_poActualHUD;
	//! Objective HUD
	CHUDMenu*					m_poObjectiveHUD;

	//! Intro Menus
	CMenu*						m_poIntro1;
	CMenu*						m_poIntro2;

	//! Matrix for keeping the old projection
	D3DXMATRIX					m_oldProjection;
	//! Matrix for keeping the old view
	D3DXMATRIX					m_oldView;
	//! Matrix for keeping the old world
	D3DXMATRIX					m_oldWorld;
	//! The Direct3D Device
	IDirect3DDevice9*			m_poDirect3DDevice;

	// Window Width
	int							m_nWindowWidth;
	// Window Height
	int							m_nWindowHeight;

	//! Button to tell the user to hit escape to go out of menu
	CBitmapFont*				m_pEscTextButton;

	/**
	* Constructs a MenuManager.
	* 
	* @date Created March 06, 2006	
	*/
	CMenuManager();
	//! Singleton stuff.
	//!@{
	CMenuManager(const CMenuManager&) {}
	operator=(const CMenuManager&){}
	//!@}

public:

	/**
	* Destructs a MenuManager. Deletes all allocated menus.
	* 
	* @date Created March 06, 2006	
	*/
	virtual ~CMenuManager();

	/**
	* Gets the current menu
	* 
	* @date Created March 06, 2006	
	*/
	inline CMenu* getCurrentMenu()
	{
		return m_poCurrentMenu;
	}

	/**
	* Sets the window width and height based on fullscreen or windowed
	* 
	* @date Created April 27, 2006	
	*/
	void	setWindowWidth(bool bWindowed = true);

	/**
	* Gets the window width
	* 
	* @date Created April 10, 2006	
	*/
	inline int getWindowWidth()
	{
		return m_nWindowWidth;
	}

	/**
	* Gets the mouse width
	* 
	* @date Created May 16, 2006	
	*/
	inline int getMouseWidth()
	{
		return m_nMouseWidth;
	}

	/**
	* Gets the mouse height
	* 
	* @date Created May 16, 2006	
	*/
	inline int getMouseHeight()
	{
		return m_nMouseHeight;
	}

	/**
	* Gets the window height
	* 
	* @date Created April 10, 2006	
	*/
	inline int getWindowHeight()
	{
		return m_nWindowHeight;
	}

	/**
	* Gets the current HUD
	* 
	* @date Created April 28, 2006	
	*/
	inline CHUDMenu* getHUD()
	{
		return m_poActualHUD;
	}

	/**
	* initializes a MenuManager.
	* 
	* @date Created March 10, 2006	
	* @param[in]	szMouseTexture			Filename of the texture to use for the background of the menu.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	bWindowed				Whether or not the game is in windowed mode
	*/
	bool initializeManager(const char *szMouseTexture, int nWidth, int nHeight, int nWidthTex, int nHeightTex,
		bool bWindowed = true);

	/**
	* Adds a menu to the manager and sets it as the current menu to display.
	* 
	* @date Created March 06, 2006
	* @param[in]	poMenu		Pointer to the Menu to add.	
	*/
	void addMenu(CMenu *poMenu);

	/**
	* Deletes a menu from the manager.
	* 
	* @date Created March 26, 2006
	* @param[in]	poMenu		Pointer to the Menu to delete.
	*/
	void deleteMenu(CMenu *poMenu);


	/**
	* Displays and Processes input for buttons on the current menu and its children.
	* 
	* @date Created March 06, 2006	
	*/
	void updateCurrentMenu(void);

	/**
	* Displays and updates the HUD
	* 
	* @date Created March 17, 2006	
	*/
	void updateHUD(void);

	/**
	* Checks mouse input and corrects it
	* 
	* @date Created April 6, 2006	
	* @return mouse position 
	*/
	D3DXVECTOR2 checkMouseInput(float fCursorWidth, float fCursorHeight, bool bForMenu = false);

	/**
	* Displays the mouse cursor
	* 
	* @date Created April 6, 2006	
	*/
	void displayMouseCursor(void);

	/**
	* Changes whether or not this mouse cursor can be displayed
	* 
	*/
	inline void canDisplayMouseCursor(bool bDisplay)
	{
		m_bDisplayMouseCursor = bDisplay;
	}

	/**
	* Sets the current menu
	* 
	* @param[in]	poMenu		Pointer to the Menu to set.
	* @date Created March 17, 2006
	*/
	inline void setCurrentMenu(CMenu *poMenu)
	{
		if (poMenu)
		{
			m_poCurrentMenu = poMenu;
		}
	}

	/**
	* Sets up the matrices for rendering
	* 
	* @date Created April 6 2006
	*/
	void getReadyToRender();

	/**
	* Puts back the old matrices after rendering
	* 
	* @date Created April 6 2006
	*/
	void endRender();
};

#endif