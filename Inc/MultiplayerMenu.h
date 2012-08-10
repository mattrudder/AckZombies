///**
//* @file MultiplayerMenu.h
//* @author Jordan Leckner
//* @date Created April 3, 2006
//*
//* This file includes the Multiplayer Menu class
//* used for displaying a Multiplayer menu and its buttons.
//*/
//#ifndef _MULTIPLAYERMENU_H_
//#define _MULTIPLAYERMENU_H_
//
//#include "Menu.h"
//#include "MenuManager.h"
//
//#include "MainButtons.h"
//
//#include "Options.h"
//
///**
//* Declares the Multiplayer menu class
//* used for displaying a Multiplayer menu and its buttons
//* 
//* @author Jordan Leckner
//* @date Created April 3, 2006
//* @date Modified April 10, 2006
//*/
//class CMultiplayerMenu : public CMenu
//{
//	friend class COKButton;
//private:
//
//	//! Left position of where the avatars should be
//	int		m_nLeftPos;
//	//! Right position of where the avatars should be
//	int		m_nRightPos;
//
//	//! Where the Keyboard is right now
//	int		m_nKeyboardPos;
//	//! Where the XBOX360 controller is right now
//	int		m_nXBOX360Pos;
//
//public:
//	/**
//	* Constructs a menu
//	* 
//	* @date Created April 3, 2006
//	* @param[in]	nPosX					Position X in screen space where the menu resides.
//	* @param[in]	nPosY					Position Y in screen space where the menu resides.
//	* @param[in]	nWidth					Width in screen space how wide the menu is.
//	* @param[in]	iHeight					Height in screen space how wide the menu is.
//	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
//	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
//	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
//	* @param[in]	poParentMenu			Pointer to parent Menu if available.
//	*/
//	CMultiplayerMenu() : CMenu(0, 0, CMenuManager::getInstance().getWindowWidth(), 
//		CMenuManager::getInstance().getWindowHeight(), NULL, 0, 0, NULL)
//	{
//		m_nType = MT_MULTIPLAYER;
//
//		int nWindowHalf = CMenuManager::getInstance().getWindowWidth() >> 1;
//		int nPower2 = 1;
//		while (nPower2 < nWindowHalf)
//			nPower2 *= 2;
//		nWindowHalf = nPower2;
//		int nWindowQuarter = nWindowHalf >> 1;
//		m_nLeftPos = nWindowHalf - nWindowQuarter - 64;
//		m_nRightPos = nWindowHalf + nWindowQuarter - 64;
//		m_nKeyboardPos = COptions::getInstance().m_nKeyboardPos;
//		m_nXBOX360Pos = COptions::getInstance().m_nXBOX360Pos;
//
//		// Create the player1 and player2 text
//		new CTextButton(m_nLeftPos, 192, 128, 32, NULL, 0, 0, false, this,
//			"Arial", 16, 32, "Player 1");
//
//		new CTextButton(m_nRightPos, 192, 128, 32, NULL, 0, 0, false, this,
//			"Arial", 16, 32, "Player 2");
//
//		new COKButton(nWindowHalf - 64, CMenuManager::getInstance().getWindowHeight() - 128, 256, 32, "../Textures/Menu/ok.tga",
//			256, 32, true, this);
// 
//		new CMenuButton(nWindowHalf - 64, 256, 128, 74, "../Textures/Menu/keyboard.tga", 256, 148, false, this);
//
//		new CMenuButton(nWindowHalf - 64, 352, 128, 90, "../Textures/Menu/360controller.tga", 256, 180, false, this);
//		new CMenuButton(nWindowHalf - 64, 448, 128, 90, "../Textures/Menu/360controller.tga", 256, 180, false, this);
//
//		new CMenuButton(nWindowHalf - 64, 544, 128, 90, "../Textures/Menu/joypad.tga", 160, 133, false, this);
//		new CMenuButton(nWindowHalf - 64, 640, 128, 90, "../Textures/Menu/joypad.tga", 160, 133, false, this);
//	}
//
//public:
//
//	/**
//	* Draws a menu, its buttons, and its children menus.
//	* 
//	* @date Created April 3, 2006
//	* @param[in]	pMouseCursor			Mouse cursor position
//	*/
//	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
//};
//
//
//#endif