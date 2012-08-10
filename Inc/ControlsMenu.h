/**
* @file ControlsMenu.h
* @author Jordan Leckner
* @date Created April 5, 2006
*
* This file includes the Controls Menu class
* used for displaying an Controls menu and its buttons.
*/
#ifndef _CONTROLSMENU_H_
#define _CONTROLSMENU_H_

#include "Menu.h"
#include "MenuManager.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "Options.h"

#include "MainButtons.h"
#include "DirectInputWrapper.h"



/**
* 
* @author Jordan Leckner
* @date Created May 10, 2006
* @date Modified May 10, 2006
*/
class CControlsPlayer1LeftArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a button
	* 
	* @date Created May 10, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CControlsPlayer1LeftArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover,
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CONTROLS_PLAYER1_LEFTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created May 10, 2006
	*/
	virtual void doAction(void);
};

/**
* 
* @author Jordan Leckner
* @date Created May 10, 2006
* @date Modified May 10, 2006
*/
class CControlsPlayer1RightArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a button
	* 
	* @date Created May 10, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CControlsPlayer1RightArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover,
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CONTROLS_PLAYER1_RIGHTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created May 10, 2006
	*/
	virtual void doAction(void);
};


/**
* 
* @author Jordan Leckner
* @date Created May 10, 2006
* @date Modified May 10, 2006
*/
class CControlsPlayer2LeftArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a button
	* 
	* @date Created May 10, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CControlsPlayer2LeftArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover,
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CONTROLS_PLAYER2_LEFTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created May 10, 2006
	*/
	virtual void doAction(void);
};

/**
* 
* @author Jordan Leckner
* @date Created May 10, 2006
* @date Modified May 10, 2006
*/
class CControlsPlayer2RightArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a button
	* 
	* @date Created May 10, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CControlsPlayer2RightArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover,
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_CONTROLS_PLAYER2_RIGHTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created May 10, 2006
	*/
	virtual void doAction(void);
};



/**
* Declares the Controls menu class
* used for displaying a Controls menu and its buttons
* 
* @author Jordan Leckner
* @date Created April 5, 2006
* @date Modified April 10, 2006
*/
class CControlsMenu : public CMenu
{
	friend class CControlsPlayer1LeftArrowButton;
	friend class CControlsPlayer1RightArrowButton;
	friend class CControlsPlayer2LeftArrowButton;
	friend class CControlsPlayer2RightArrowButton;
	friend class COKButton;
public:

	//! list of all of the control buttons/movements/etc.
	vector<string>		m_ControlNames;

	//! Which control scheme player 1 is set to.
	int m_nControlSchemeP1;
	//! Which control scheme player 2 is set to.
	int m_nControlSchemeP2;

	//! the 3 different control schemes
	string m_ControlSchemes[3];

	//! player 1's control scheme
	CTextButton* m_pPlayer1Scheme;

	//! player 2's control scheme
	CTextButton* m_pPlayer2Scheme;

	/**
	* Constructs a menu
	* 
	* @date Created April 5, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	iHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CControlsMenu() : CMenu(0, 0, CMenuManager::getInstance().getWindowWidth(), 
		CMenuManager::getInstance().getWindowHeight(), NULL, 0, 0, NULL)
	{
		m_nType = MT_CONTROLS;
		CTextButton *poText;
		COptions *poOptions = COptions::getInstancePtr();
		int nWindowWidth = CMenuManager::getInstance().getWindowWidth();
		int nWindowHeight = CMenuManager::getInstance().getWindowHeight();

		m_nControlSchemeP1 = COptions::getInstance().m_nControlSchemeP1;
		m_nControlSchemeP2 = COptions::getInstance().m_nControlSchemeP2;

		// OK Button
		new COKButton((nWindowWidth >> 1) - 257, nWindowHeight - 64, 256, 32, "../Textures/Menu/ok.tga", "../Textures/Menu/okHover.tga",
			256, 32, true, this);
		// Cancel Button
		new CCancelButton((nWindowWidth >> 1) + 1, nWindowHeight - 64, 256, 32, "../Textures/Menu/cancel.tga", "../Textures/Menu/cancelHover.tga",
			256, 32, true, this);

		// Control text
		poText = new CTextButton(16, 256, 256, 24, NULL, NULL, 0, 0, false, this, "Arial", 8, 16, NULL);
		poText->m_dwFormat = DT_LEFT;

		// Fill out the list of controls
		string strControl;
		strControl = "Move Left";
		m_ControlNames.push_back(strControl);
		strControl = "Move Right";
		m_ControlNames.push_back(strControl);
		strControl = "Move Up";
		m_ControlNames.push_back(strControl);
		strControl = "Move Down";
		m_ControlNames.push_back(strControl);
		strControl = "Fire";
		m_ControlNames.push_back(strControl);
		strControl = "Action";
		m_ControlNames.push_back(strControl);
		strControl = "Previous Weapon";
		m_ControlNames.push_back(strControl);
		strControl = "Next Weapon";
		m_ControlNames.push_back(strControl);
		strControl = "Rotate Camera L";
		m_ControlNames.push_back(strControl);
		strControl = "Rotate Camera R";
		m_ControlNames.push_back(strControl);
		strControl = "Use Gadget";
		m_ControlNames.push_back(strControl);
		strControl = "Menu/Pause";
		m_ControlNames.push_back(strControl);

		int nBoxWidth = 192;

		// PLAYER 1
		// Create all of the bind-able buttons
		poText = new CTextButton(256, 256, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[0].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVELEFT_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[0].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[0].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[0].c_str());
		
		poText = new CTextButton(256, 280, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[1].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVERIGHT_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[1].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[1].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[1].c_str());
		
		poText = new CTextButton(256, 304, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[2].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVEUP_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[2].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[2].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[2].c_str());
		
		poText = new CTextButton(256, 328, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[3].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVEDOWN_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[3].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[3].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[3].c_str());
		
		poText = new CTextButton(256, 352, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[4].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_FIRE_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[4].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[4].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[4].c_str());
		
		poText = new CTextButton(256, 376, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[5].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_ACTION_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[5].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[5].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[5].c_str());
		
		poText = new CTextButton(256, 400, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[6].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_PREVIOUSWEAPON_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[6].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[6].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[6].c_str());
		
		poText = new CTextButton(256, 424, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[7].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_NEXTWEAPON_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[7].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[7].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[7].c_str());
		
		poText = new CTextButton(256, 448, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[8].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_ROTATECAMERALEFT_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[8].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[8].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[8].c_str());
		
		poText = new CTextButton(256, 472, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[9].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_ROTATECAMERARIGHT_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[9].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[9].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[9].c_str());
		
		poText = new CTextButton(256, 496, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[10].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_USEGADGET_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[10].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[10].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[10].c_str());
		
		poText = new CTextButton(256, 520, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[11].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_PAUSE_P1;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP1 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[11].c_str());
		else if (m_nControlSchemeP1 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[11].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[11].c_str());


		int nP2 = 256;
		// PLAYER 2
		// Create all of the bind-able buttons
		poText = new CTextButton(nWindowWidth - nP2, 256, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[12].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVELEFT_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[12].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[12].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[12].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 280, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[13].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVERIGHT_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[13].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[13].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[13].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 304, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[14].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVEUP_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[14].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[14].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[14].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 328, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[15].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_MOVEDOWN_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[15].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[15].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[15].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 352, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[16].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_FIRE_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[16].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[16].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[16].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 376, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[17].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_ACTION_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[17].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[17].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[17].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 400, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[18].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_PREVIOUSWEAPON_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[18].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[18].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[18].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 424, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[19].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_NEXTWEAPON_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[19].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[19].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[19].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 448, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[20].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_ROTATECAMERALEFT_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[20].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[20].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[20].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 472, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[21].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_ROTATECAMERARIGHT_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[21].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[21].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[21].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 496, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[22].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_USEGADGET_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[22].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[22].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[22].c_str());
		
		poText = new CTextButton(nWindowWidth - nP2, 520, nBoxWidth, 16, NULL, NULL, 0, 0, true, this, 
			"Arial", 8, 16, const_cast<char*>(poOptions->m_ControlBindings[23].c_str()));
		poText->m_nSecondaryType = ST_CONTROL_PAUSE_P2;
		poText->m_dwFormat = DT_LEFT;
		if (m_nControlSchemeP2 == CS_KEYBOARD)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoDIK(poOptions->m_ControlBindings[23].c_str());
		else if (m_nControlSchemeP2 == CS_XBOX360)
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringtoXInput(poOptions->m_ControlBindings[23].c_str());
		else
			poText->m_dwKey = CDirectInputWrapper::getInstance().convertStringToJoypad(poOptions->m_ControlBindings[23].c_str());


		int nP1LeftArrowX = 64;
		int nP1LeftArrowY = nWindowHeight - 128;

		int nP2LeftArrowX = nWindowWidth - nP2;
		int nP2LeftArrowY = nWindowHeight - 128;

		new CControlsPlayer1LeftArrowButton(nP1LeftArrowX, nP1LeftArrowY, 32, 32, "../Textures/Menu/LeftArrow.tga", "../Textures/Menu/LeftArrowHover.tga",
			32, 32, true, this);
		new CControlsPlayer1RightArrowButton(nP1LeftArrowX + 192, nP1LeftArrowY, 32, 32, "../Textures/Menu/RightArrow.tga", "../Textures/Menu/RightArrowHover.tga",
			32, 32, true, this);

		new CControlsPlayer2LeftArrowButton(nP2LeftArrowX, nP1LeftArrowY, 32, 32, "../Textures/Menu/LeftArrow.tga", "../Textures/Menu/LeftArrowHover.tga",
			32, 32, true, this);
		new CControlsPlayer2RightArrowButton(nP2LeftArrowX + 192, nP1LeftArrowY, 32, 32, "../Textures/Menu/RightArrow.tga", "../Textures/Menu/RightArrowHover.tga",
			32, 32, true, this);

		m_ControlSchemes[0] = "Keyboard";
		m_ControlSchemes[1] = "XBOX360";
		m_ControlSchemes[2] = "Joypad";

		m_pPlayer1Scheme = new CTextButton(nP1LeftArrowX + 48, nP1LeftArrowY, 128, 32, NULL, NULL, 0, 0, false, this, "Arial", 12, 24);

		m_pPlayer2Scheme = new CTextButton(nP2LeftArrowX + 48, nP1LeftArrowY, 128, 32, NULL, NULL, 0, 0, false, this, "Arial", 12, 24);

		m_nControlSchemeP1 = COptions::getInstance().m_nControlSchemeP1;
		m_nControlSchemeP2 = COptions::getInstance().m_nControlSchemeP2;
	}

public:

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created April 5, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
};


#endif