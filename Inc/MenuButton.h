/**
* @file MenuButton.h
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the Menu Button class
* used for displaying a menu's Buttons and processing
* its behavior when pressed.
*/

#ifndef _MENUBUTTON_H_
#define _MENUBUTTON_H_

#include "RenderSystem.h"

#include "d3dx9.h"

// Custom FVF with position, color, and texture coords
struct SMenuFVF
{
	D3DXVECTOR3 pos;	// position
	D3DCOLOR    color;	// color
	float		u;		// u texture coord
	float		v;		// v texture coord

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
	};
};

enum EButtonType
{
	BT_SINGLEPLAYER = 128,
	BT_MULTIPLAYER,
	BT_OPTIONS,
	BT_CREDITS,
	BT_HIGHSCORE,
	BT_QUIT,
	BT_OK,
	BT_CANCEL,
	BT_CONTROLS,
	BT_TEXT,
	BT_BACKTOGAME,
	BT_CONTINUE,
	BT_MAINMENU,
	BT_OPTIONS_RESOLUTION_LEFTARROW,
	BT_OPTIONS_RESOLUTION_RIGHTARROW,
	BT_OPTIONS_SOUNDVOLUME_LEFTARROW,
	BT_OPTIONS_SOUNDVOLUME_RIGHTARROW,
	BT_OPTIONS_MUSICVOLUME_LEFTARROW,
	BT_OPTIONS_MUSICVOLUME_RIGHTARROW,
	BT_OPTIONS_GAMMALEVEL_LEFTARROW,
	BT_OPTIONS_GAMMALEVEL_RIGHTARROW,
	BT_CONTROLS_PLAYER1_LEFTARROW,
	BT_CONTROLS_PLAYER1_RIGHTARROW,
	BT_CONTROLS_PLAYER2_LEFTARROW,
	BT_CONTROLS_PLAYER2_RIGHTARROW,
	BT_MAX
};

enum ESecondaryType
{
	ST_CONTROL_MOVELEFT_P1,
	ST_CONTROL_MOVERIGHT_P1,
	ST_CONTROL_MOVEUP_P1,
	ST_CONTROL_MOVEDOWN_P1,
	ST_CONTROL_FIRE_P1,
	ST_CONTROL_ACTION_P1,
	ST_CONTROL_PREVIOUSWEAPON_P1,
	ST_CONTROL_NEXTWEAPON_P1,
	ST_CONTROL_ROTATECAMERALEFT_P1,
	ST_CONTROL_ROTATECAMERARIGHT_P1,
	ST_CONTROL_USEGADGET_P1,
	ST_CONTROL_PAUSE_P1,
	ST_CONTROL_MOVELEFT_P2,
	ST_CONTROL_MOVERIGHT_P2,
	ST_CONTROL_MOVEUP_P2,
	ST_CONTROL_MOVEDOWN_P2,
	ST_CONTROL_FIRE_P2,
	ST_CONTROL_ACTION_P2,
	ST_CONTROL_PREVIOUSWEAPON_P2,
	ST_CONTROL_NEXTWEAPON_P2,
	ST_CONTROL_ROTATECAMERALEFT_P2,
	ST_CONTROL_ROTATECAMERARIGHT_P2,
	ST_CONTROL_USEGADGET_P2,
	ST_CONTROL_PAUSE_P2,
	ST_MAX
};

// Forward declare the Menu class
class CMenu;

/**
* Declares the Menu Button class
* used for displaying a menu and its buttons
* 
* @author Jordan Leckner
* @date Created March 06, 2006
* @date Modified April 10, 2006
*/
class CMenuButton
{
	friend class CMenu;
protected:

	//! Type of the button
	unsigned int				m_unType;
	//! Position X of the button
	int							m_nPositionX;
	//! Position Y of the button
	int							m_nPositionY;
	//! Width of the button
	int							m_nWidth;
	//! Height of the button
	int							m_nHeight;
	//! Vertex buffer used for the quad which will be drawn for the button
	IDirect3DVertexBuffer9*		m_pVertexBuffer;		
	//! Texture for the button
	IDirect3DTexture9*			m_pTextureBackground;
	//! Texture for the button hover
	IDirect3DTexture9*			m_pTextureBackgroundHover;
	//! Texture to use for drawing.
	IDirect3DTexture9*			m_pFinalTexture;
	//! Pointer to the menu which this button resides on
	CMenu*						m_poParentMenu;
	//! Bounding Rect for this button
	RECT						m_rBoundingBox;
	//! Whether or not the left mouse button is down inside the box
	bool						m_bLeftMouseDownInBox;
	//! Whether or not the left mouse button has been clicked
	bool						m_bLeftMouseButtonClicked;
	//! Line to draw the outline of the button
	ID3DXLine					*m_pLine;
	//! Alpha value used for fading
	DWORD						m_dwAlpha;
	//! Font used for text on the menu
	ID3DXFont					*m_pFont;
	//! Whether or not to check input on this button
	bool						m_bCheckInput;
	//! Text to use for the button
	char						*m_szText;
	//! Format of the text
	DWORD						m_dwFormat;
	//! Text color
	DWORD						m_dwColor;
	//! Keyboard/joystick input
	bool						m_bEnter;

public:
	/**
	* Constructs a menu
	* 
	* @date Created March 06, 2006
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
	CMenuButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, const char *szTextureBackgroundHover,
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0, char *szFontFace = 0, int nFontWidth = 0, 
		int nFontHeight = 0, char *szText = 0);
	
	//! Accessors
	unsigned int getType() { return m_unType; }
	DWORD getAlpha() { return m_dwAlpha; }
	int getPositionX() { return m_nPositionX; }
	int getPositionY() { return m_nPositionY; }

	//! Mutators
	void setType(unsigned int unType) { m_unType = unType; }
	void setFormat(DWORD dwFormat) { m_dwFormat = dwFormat; }
	void setAlpha(DWORD dwAlpha) { m_dwAlpha = dwAlpha; }
	void setPositionX(int nPos) { m_nPositionX = nPos; }
	void setPositionY(int nPos) { m_nPositionY = nPos; }

	/**
	* Destructs a menu
	* 
	* @date Created March 06, 2006
	*/
	virtual ~CMenuButton();

	/**
	* Draws a button on a menu
	* 
	* @date Created March 06, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	bool drawButton(D3DXVECTOR2 *pMouseCursor);

	/**
	* Process a specified action
	* Will be implmented when derived.
	* 
	* @date Created March 06, 2006
	*/
	virtual void doAction(void);

	/**
	* Checks input for button hover and click
	* 
	* @date Created March 10, 2006
	*/
	bool checkInput(D3DXVECTOR2 *pMouseCursor);

	/**
	* Adds to vertex data
	* 
	* @date Created April 3, 2006
	*/
	void addToVertexData(int nX, int nY = 0, bool bWidth = false);

	/**
	* Adds to vertex data UV's
	* 
	* @date Created May 26, 2006
	*/
	void addToVertexDataUV(float fU, float fV, bool bWidth);

	/**
	* sets the vertex data
	* 
	* @date Created May 9, 2006
	*/
	void setVertexData(D3DXVECTOR2 *pPos);

	/**
	* sets the text color
	* 
	* @date Created May 15, 2006
	*/
	inline void setTextColor(DWORD dwColor)
	{
		m_dwColor = dwColor;
	}

	/**
	* Gets the position of the button
	* 
	* @date Created April 3, 2006
	*/
	D3DXVECTOR2 getPosition();

	/**
	* Gets the center position of the button
	* 
	* @date Created June 17, 2006
	*/
	D3DXVECTOR2 getCenterPosition();

	/**
	* emulates a mouse click
	* 
	* @date Created June 17, 2006
	*/
	inline void enter() { m_bEnter = true; }

};

#endif