/**
* @file MainButtons.h
* @author Jordan Leckner
* @date Created March 30, 2006
*
* This file includes all the Main Buttons classes
*/

#ifndef _MAINBUTTONS_H_
#define _MAINBUTTONS_H_

#include "MenuButton.h"

/**
* Declares the Single Player Button class
* used for the action of an Single Player Button.
* 
* @author Jordan Leckner
* @date Created March 17, 2006
* @date Modified April 10, 2006
*/
class CSinglePlayerButton : public CMenuButton
{
private:
	

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 17, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CSinglePlayerButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_SINGLEPLAYER;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created March 17, 2006
	*/
	virtual void doAction(void);
};


/**
* Declares the Multiplayer Button class
* used for the action of an Multiplayer Button.
* 
* @author Jordan Leckner
* @date Created March 17, 2006
* @date Modified March 17, 2006
*/
class CMultiplayerButton : public CMenuButton
{
private:
	

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 17, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CMultiplayerButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_MULTIPLAYER;
	}

	/**
	* Process a specified action
	* Multiplayer Button will close the window and save changes
	* to the menus.
	* 
	* @date Created March 17, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Options Button class
* used for the action of an Options Button.
* 
* @author Jordan Leckner
* @date Created March 17, 2006
* @date Modified March 17, 2006
*/
class COptionsButton : public CMenuButton
{
private:
	

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 17, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS;
	}

	/**
	* Process a specified action
	* Options Button will close the window and save changes
	* to the menus.
	* 
	* @date Created March 17, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the OK Button class
* used for the action of an OK Button.
* 
* @author Jordan Leckner
* @date Created March 06, 2006
* @date Modified March 06, 2006
*/
class COKButton : public CMenuButton
{
private:
	

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 06, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COKButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OK;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created March 06, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Cancel Button class
* used for the action of an Cancel Button.
* 
* @author Jordan Leckner
* @date Created April 5, 2006
* @date Modified April 5, 2006
*/
class CCancelButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created April 5, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CCancelButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CANCEL;
	}

	/**
	* Process a specified action
	* Cancel Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 5, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Controls Button class
* used for the action of an Controls Button.
* 
* @author Jordan Leckner
* @date Created April 5, 2006
* @date Modified April 5, 2006
*/
class CControlsButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created April 5, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CControlsButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CONTROLS;
	}

	/**
	* Process a specified action
	* Controls Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 5, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Credits Button class
* used for the action of a Credits Button.
* 
* @author Jordan Leckner
* @date Created March 31, 2006
* @date Modified March 31, 2006
*/
class CCreditsButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created March 31, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CCreditsButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CREDITS;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created March 31, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the HighScore Button class
* used for the action of a HighScore Button.
* 
* @author Jordan Leckner
* @date Created April 5, 2006
* @date Modified April 5, 2006
*/
class CHighScoreButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created April 5, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CHighScoreButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_HIGHSCORE;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 5, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Quit Button class
* used for the action of a Quit Button.
* 
* @author Jordan Leckner
* @date Created March 13, 2006
* @date Modified March 13, 2006
*/
class CQuitButton : public CMenuButton
{
private:
	

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 06, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CQuitButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_QUIT;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created March 06, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Back to game Button class
* used for the action of a Back to game Button.
* 
* @author Jordan Leckner
* @date Created May 16, 2006
* @date Modified May 16, 2006
*/
class CBackToGameButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created May 16, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CBackToGameButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_BACKTOGAME;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created May 16, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Back to game Button class
* used for the action of a Back to game Button.
* 
* @author Jordan Leckner
* @date Created May 16, 2006
* @date Modified May 16, 2006
*/
class CContinueButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created May 16, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CContinueButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CONTINUE;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created May 16, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Main Menu Button class
* used for the action of a Main Menu Button.
* 
* @author Jordan Leckner
* @date Created May 16, 2006
* @date Modified May 16, 2006
*/
class CMainMenuButton : public CMenuButton
{
private:


public:

	/**
	* Constructs a menu
	* 
	* @date Created May 16, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CMainMenuButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_MAINMENU;
	}

	/**
	* Process a specified action
	* OK Button will close the window and save changes
	* to the menus.
	* 
	* @date Created May 16, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Text Button class
* used for the action of an Text Button.
* 
* @author Jordan Leckner
* @date Created March 28, 2006
* @date Modified May 9, 2006
*/
class CTextButton : public CMenuButton
{
	friend class CMenuButton;
	friend class CControlsMenu;
private:
	//! Secondary Type for the text button
	int					m_nSecondaryType;
	//! Should we process the action every frame
	bool				m_bProcessActionEveryFrame;
	//! Whether or not the left mouse button is down
	bool				m_bLeftMouseButtonDown;
	//! Whether or not the right mouse button is down
	bool				m_bRightMouseButtonDown;
	//! hold on to the key associated with this button for key binding
	DWORD				m_dwKey;

public:

	/**
	* Constructs a Text Button
	* 
	* @date Created March 28, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	bCheckInput				Whether or not to check input on this button
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	* @param[in]	szFontFace				Font face type
	* @param[in]	nFontWidth				Width of the font
	* @param[in]	nFontHeight				Height of the font
	* @param[in]	szText					The text of the button, if any
	*/
	CTextButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, char *szFontFace = 0, int nFontWidth = 0, 
		int nFontHeight = 0, char *szText = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu,
		szFontFace, nFontWidth, nFontHeight, szText)
	{
		m_unType = BT_TEXT;
		m_nSecondaryType = - 1;
		m_bProcessActionEveryFrame = false;
		m_dwAlpha = 256;
		m_bLeftMouseButtonDown = false;
		m_bRightMouseButtonDown = false;
		m_dwKey = -1;
	}

	//! Accessors
	int getSecondaryType() { return m_nSecondaryType; }

	/**
	* Process a specified action
	* 
	* @date Created March 28, 2006
	*/
	virtual void doAction(void);

	/**
	* Sets the text of the button
	* 
	* @date Created March 29, 2006
	*/
	inline void setText(char *szText)
	{
		if (szText)
		{
			if (!m_szText)
			{
				m_szText = new char[128];
			}
			memset(m_szText, 0, sizeof(m_szText));
			strcpy(m_szText, szText);
		}
	}

	/**
	* gets the text of the button
	* 
	* @date Created May 9, 2006
	*/
	inline char * getText()
	{
		return m_szText;
	}
};

#endif