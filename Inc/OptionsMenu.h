/**
* @file OptionsMenu.h
* @author Jordan Leckner
* @date Created March 29, 2006
*
* This file includes the Options Menu class
* used for displaying an options menu and its buttons.
*/
#ifndef _OPTIONSMENU_H_
#define _OPTIONSMENU_H_

#include "Menu.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

#include "Options.h"

#include "MainButtons.h"

/**
* Declares the options menu class
* used for displaying an options menu and its buttons
* 
* @author Jordan Leckner
* @date Created March 29, 2006
* @date Modified April 30, 2006
*/
class COptionsMenu : public CMenu
{
public:

	//! Our list of possible resolutions
	vector<string>		m_Resolutions;
	//! Current Resolution Index
	int					m_nResolutionIndex;
	//! Sound Volume
	int					m_nSoundVolume;
	//! Music Volume
	int					m_nMusicVolume;
	//! Gamma Level
	int					m_nGammaLevel;

public:
	/**
	* Constructs a menu
	* 
	* @date Created March 29, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	iHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsMenu() : CMenu(0, 0, CMenuManager::getInstance().getWindowWidth(), 
		CMenuManager::getInstance().getWindowHeight(), NULL, 0, 0, NULL)
	{
		m_nType = MT_OPTIONS;
		CTextButton *poText;
		CMenuButton *poButton;
		string strVolume;

		// OK Button
		new COKButton((CMenuManager::getInstance().getWindowWidth() >> 1) - 257, 
			CMenuManager::getInstance().getWindowHeight() - 64, 256, 32, "../Textures/Menu/ok.tga", "../Textures/Menu/okHover.tga", 
			256, 32, true, this);

		// Resolution text
		poText = new CTextButton(192, 268, 256, 32, "../Textures/Menu/resolution.tga", NULL, 256, 32, false, this);
		poText->setType(8675309);
		// Resolution left arrow button
		poButton = new COptionsResolutionLeftArrowButton(480, 271, 32, 32, "../Textures/Menu/LeftArrow.tga", "../Textures/Menu/LeftArrowHover.tga", 
			32, 32, true, this);
		poButton->setType(8675309);
		// Resolution right arrow button
		poButton = new COptionsResolutionRightArrowButton(800, 271, 32, 32, "../Textures/Menu/RightArrow.tga", "../Textures/Menu/RightArrowHover.tga", 
			32, 32, true, this);
		poButton->setType(8675309);
		// add in all the possible strings for each resolution available
		string strResolution = "640x480";
		m_Resolutions.push_back(strResolution);
		strResolution = "800x600";
		m_Resolutions.push_back(strResolution);
		strResolution = "1024x768";
		m_Resolutions.push_back(strResolution);
		strResolution = "1280x1024";
		m_Resolutions.push_back(strResolution);
		strResolution = "1440x900";
		m_Resolutions.push_back(strResolution);

		m_nResolutionIndex = COptions::getInstance().m_nResolutionIndex;

		poText = new CTextButton(512, 248, 256, 48, NULL, NULL, 0, 0, false, this, "Arial", 24, 48, 
			const_cast<char*>(m_Resolutions[m_nResolutionIndex].c_str()));
		poText->setType(8675309);

		// Sound volume text
		poText = new CTextButton(192, 316, 256, 32, "../Textures/Menu/soundvolume.tga", NULL, 256, 32, false, this);
		// Sound volume left arrow button
		new COptionsSoundVolumeLeftArrowButton(480, 319, 32, 32, "../Textures/Menu/LeftArrow.tga", "../Textures/Menu/LeftArrowHover.tga",
			32, 32, true, this);
		// Sound volume right arrow button
		new COptionsSoundVolumeRightArrowButton(800, 319, 32, 32, "../Textures/Menu/RightArrow.tga", "../Textures/Menu/RightArrowHover.tga",
			32, 32, true, this);
		// Create the volume text
		poText = new CTextButton(550, 304, 256, 48, NULL, NULL, 0, 0, false, this, "Arial", 24, 48, NULL);
		poText->setFormat(DT_LEFT);
		m_nSoundVolume = COptions::getInstance().m_nSoundVolume;
		for (int i = 0; i < m_nSoundVolume; ++i)
		{
			strVolume += "¦";
		}
		poText->setText(const_cast<char*>(strVolume.c_str()));

		// Sound volume text
		poText = new CTextButton(192, 364, 256, 32, "../Textures/Menu/musicvolume.tga", NULL, 256, 32, false, this);
		// Sound volume left arrow button
		new COptionsMusicVolumeLeftArrowButton(480, 367, 32, 32, "../Textures/Menu/LeftArrow.tga", "../Textures/Menu/LeftArrowHover.tga",
			32, 32, true, this);
		// Sound volume right arrow button
		new COptionsMusicVolumeRightArrowButton(800, 367, 32, 32, "../Textures/Menu/RightArrow.tga", "../Textures/Menu/RightArrowHover.tga",
			32, 32, true, this);
		// Create the volume text
		poText = new CTextButton(550, 352, 256, 48, NULL, NULL, 0, 0, false, this, "Arial", 24, 48, NULL);
		poText->setFormat(DT_LEFT);
		m_nMusicVolume = COptions::getInstance().m_nMusicVolume;
		strVolume.clear();
		for (int i = 0; i < m_nMusicVolume; ++i)
		{
			strVolume += "¦";
		}
		poText->setText(const_cast<char*>(strVolume.c_str()));

		// Cancel Button
		new CCancelButton((CMenuManager::getInstance().getWindowWidth() >> 1) + 1,
			CMenuManager::getInstance().getWindowHeight() - 64, 256, 32, "../Textures/Menu/cancel.tga", "../Textures/Menu/cancelHover.tga",
			256, 32, true, this);

		// configure controls button
		new CControlsButton((CMenuManager::getInstance().getWindowWidth() >> 1) - 128, 512, 256, 32,
			"../Textures/Menu/bindcontrols.tga", "../Textures/Menu/bindcontrolsHover.tga", 256, 32, true, this);

		// Gamma level text
		poText = new CTextButton(192, 420, 256, 32, "../Textures/Menu/gammalevel.tga", NULL, 256, 32, false, this);
		// Gamma level left arrow button
		new COptionsGammaLevelLeftArrowButton(480, 413, 32, 32, "../Textures/Menu/LeftArrow.tga", "../Textures/Menu/LeftArrowHover.tga",
			32, 32, true, this);
		// Gamma level right arrow button
		new COptionsGammaLevelRightArrowButton(800, 413, 32, 32, "../Textures/Menu/RightArrow.tga", "../Textures/Menu/RightArrowHover.tga",
			32, 32, true, this);
		// Create the Gamma text
		poText = new CTextButton(550, 408, 256, 48, NULL, NULL, 0, 0, false, this, "Arial", 24, 48, NULL);
		poText->setFormat(DT_LEFT);
		m_nGammaLevel = COptions::getInstance().m_nGammaLevel;
		strVolume.clear();
		for (int i = 0; i < m_nGammaLevel; ++i)
		{
			strVolume += "¦";
		}
		poText->setText(const_cast<char*>(strVolume.c_str()));
	}

public:

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created March 29, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
};


#endif