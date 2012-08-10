/**
* @file Options.h
* @author Jordan Leckner
* @date Created March 29, 2006
*
* This file includes all of the Option Button classes used for the option menu
*/

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include "MenuButton.h"
#include "MenuManager.h"
#include "Singleton.h"
#include <string>
using std::string;
#include "ConfigFile.h"

#include "SoundManager.h"


enum EControlScheme
{
	CS_KEYBOARD,
	CS_XBOX360,
	CS_JOYPAD,
	MAX_SCHEMES
};

class CPlayer;

/**
* Declares the Options class
* used for the storing of all options
* 
* @author Jordan Leckner
* @date Created April 8, 2006
* @date Modified April 10, 2006
*/
class COptions : public CSingleton<COptions>
{
	friend class CSingleton<COptions>;

	friend class COptionsMenu;
	friend class COKButton;
	friend class CControlsMenu;
	friend class CMultiplayerMenu;
	friend class CControlsMenu;
private:

	//! Current Resolution Index
	int					m_nResolutionIndex;
	//! Sound Volume
	int					m_nSoundVolume;
	//! Music Volume
	int					m_nMusicVolume;
	//! Gamma Level
	int					m_nGammaLevel;

	//! Control Bindings
	string				m_ControlBindings[24];

	//! Control Pre-sets
	string				m_KeyboardPresets[12];
	string				m_XBOX360Presets[12];
	string				m_JoypadPresets[12];

	//! Where the keyboard avatar is right now
	int					m_nKeyboardPos;
	//! Where the XBOX360 controller is right now
	int					m_nXBOX360Pos;

	//! Which control scheme player 1 is set to.
	int					m_nControlSchemeP1;
	//! Which control scheme player 2 is set to.
	int					m_nControlSchemeP2;

	//! The old gamma level; from the start of the program
	D3DGAMMARAMP		m_oOldGammaLevel;


	//! Config File
	CConfigFile			m_oConfig;

public:

	/**
	* Constructor
	*/
	COptions();

	//! Accessors
	char* getControlBinding(int nIndex)
	{
		return const_cast<char*>(m_ControlBindings[nIndex].c_str());
	}
	char* getKeyboardPreset(int nIndex)
	{
		return const_cast<char*>(m_KeyboardPresets[nIndex].c_str());
	}
	char* getXBOX360Preset(int nIndex)
	{
		return const_cast<char*>(m_XBOX360Presets[nIndex].c_str());
	}
	char* getJoypadPreset(int nIndex)
	{
		return const_cast<char*>(m_JoypadPresets[nIndex].c_str());
	}
	int getControlScheme(bool bFirstPlayer)
	{
		if (bFirstPlayer)
			return m_nControlSchemeP1;
		else
			return m_nControlSchemeP2;
	}
	int getSoundVolume(void) { return m_nSoundVolume; }
	int getMusicVolume(void) { return m_nMusicVolume; }
	//*

	/**
	* Sets the controls for a player
	* 
	* @date Created May 11, 2006
	*/
	void setPlayerControls(CPlayer *poPlayer, bool bFirstPlayer);

	/**
	* saves the controls for the players
	* 
	* @date Created May 15, 2006
	*/
	void saveOptions();

	/**
	* resets the gamma to what it was before.
	* 
	* @date Created June 9, 2006
	*/
	void resetGamma();

private:

	//! Singleton stuff.
	//!@{
	COptions(const COptions&) {}
	const COptions &operator=(const COptions&) {}
	virtual ~COptions(void)
	{
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetGammaRamp(0, 
			D3DSGR_NO_CALIBRATION, &m_oOldGammaLevel);
		saveOptions();
	}
	//!@}
};



/**
* Declares the Options Button class
* used for the action of a Resolution Left Arrow Button.
* 
* @author Jordan Leckner
* @date Created March 29, 2006
* @date Modified March 29, 2006
*/
class COptionsResolutionLeftArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 29, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsResolutionLeftArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_RESOLUTION_LEFTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created March 29, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Options Button class
* used for the action of a Resolution Right Arrow Button.
* 
* @author Jordan Leckner
* @date Created March 29, 2006
* @date Modified March 29, 2006
*/
class COptionsResolutionRightArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created March 29, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsResolutionRightArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_RESOLUTION_RIGHTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created March 29, 2006
	*/
	virtual void doAction(void);
};


/**
* Declares the Options Button class
* used for the action of a Sound Volume Left Arrow Button.
* 
* @author Jordan Leckner
* @date Created April 4, 2006
* @date Modified April 4, 2006
*/
class COptionsSoundVolumeLeftArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 4, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsSoundVolumeLeftArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_SOUNDVOLUME_LEFTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created April 4, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Options Button class
* used for the action of a Sound Volume Right Arrow Button.
* 
* @author Jordan Leckner
* @date Created April 4, 2006
* @date Modified April 4, 2006
*/
class COptionsSoundVolumeRightArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 4, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsSoundVolumeRightArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_SOUNDVOLUME_RIGHTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created April 4, 2006
	*/
	virtual void doAction(void);
};


/**
* Declares the Options Button class
* used for the action of a Music Volume Left Arrow Button.
* 
* @author Jordan Leckner
* @date Created April 4, 2006
* @date Modified April 4, 2006
*/
class COptionsMusicVolumeLeftArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 4, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsMusicVolumeLeftArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_MUSICVOLUME_LEFTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created April 4, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Options Button class
* used for the action of a Music Volume Right Arrow Button.
* 
* @author Jordan Leckner
* @date Created April 4, 2006
* @date Modified April 4, 2006
*/
class COptionsMusicVolumeRightArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 4, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsMusicVolumeRightArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_MUSICVOLUME_RIGHTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created April 4, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Options Button class
* used for the action of a Gamma Level Left Arrow Button.
* 
* @author Jordan Leckner
* @date Created April 30, 2006
* @date Modified April 30, 2006
*/
class COptionsGammaLevelLeftArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 30, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsGammaLevelLeftArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_GAMMALEVEL_LEFTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created April 30, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Options Button class
* used for the action of a Gamma Level Right Arrow Button.
* 
* @author Jordan Leckner
* @date Created April 30, 2006
* @date Modified April 30, 2006
*/
class COptionsGammaLevelRightArrowButton : public CMenuButton
{
private:

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 30, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	COptionsGammaLevelRightArrowButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground,
		const char *szTextureBackgroundHover, 
		int nWidthTex, int nHeightTex, bool bCheckInput = true, CMenu* poParentMenu = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = BT_OPTIONS_GAMMALEVEL_RIGHTARROW;
	}

	/**
	* Process a specified action
	* 
	* @date Created April 30, 2006
	*/
	virtual void doAction(void);
};

#endif