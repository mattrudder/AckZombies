/**
* @file Options.cpp
* @author Jordan Leckner
* @date Created March 29, 2006
*
* This file includes the definitions of all of the Option Button
* classes used for the option menu
*/

#include "Options.h"

#include "OptionsMenu.h"
#include "SoundManager.h"
#include "DirectInputWrapper.h"
#include "Player.h"

/**
* COptions::COptions
* @date Modified June 9, 2006
*/
COptions::COptions() : m_nResolutionIndex(0), m_nSoundVolume(15), m_nMusicVolume(15), m_nGammaLevel(15),
m_nKeyboardPos(CMenuManager::getInstance().getWindowWidth() >> 1), 
m_nXBOX360Pos(CMenuManager::getInstance().getWindowWidth() >> 1)
{
	m_oConfig.LoadConfig("AckZombies.ini");

	m_KeyboardPresets[0] = "A";
	m_KeyboardPresets[1] = "D";
	m_KeyboardPresets[2] = "W";
	m_KeyboardPresets[3] = "S";
	m_KeyboardPresets[4] = "LMOUSE";
	m_KeyboardPresets[5] = "F";
	m_KeyboardPresets[6] = "CAPS";
	m_KeyboardPresets[7] = "TAB";
	m_KeyboardPresets[8] = "Q";
	m_KeyboardPresets[9] = "E";
	m_KeyboardPresets[10] = "R";
	m_KeyboardPresets[11] = "ESC";

	m_ControlBindings[0] = m_oConfig.GetValue("bindings", "P1MoveLeft");
	m_ControlBindings[1] = m_oConfig.GetValue("bindings", "P1MoveRight");
	m_ControlBindings[2] = m_oConfig.GetValue("bindings", "P1MoveUp");
	m_ControlBindings[3] = m_oConfig.GetValue("bindings", "P1MoveDown");
	m_ControlBindings[4] = m_oConfig.GetValue("bindings", "P1Fire");
	m_ControlBindings[5] = m_oConfig.GetValue("bindings", "P1Action");
	m_ControlBindings[6] = m_oConfig.GetValue("bindings", "P1PrevWeapon");
	m_ControlBindings[7] = m_oConfig.GetValue("bindings", "P1NextWeapon");
	m_ControlBindings[8] = m_oConfig.GetValue("bindings", "P1CamLeft");
	m_ControlBindings[9] = m_oConfig.GetValue("bindings", "P1CamRight");
	m_ControlBindings[10] = m_oConfig.GetValue("bindings", "P1UseGadget");
	m_ControlBindings[11] = m_oConfig.GetValue("bindings", "P1Pause");

	m_XBOX360Presets[0] = "0";
	m_XBOX360Presets[1] = "0";
	m_XBOX360Presets[2] = "0";
	m_XBOX360Presets[3] = "0";
	m_XBOX360Presets[4] = "XBOX360_RTRIGGER";
	m_XBOX360Presets[5] = "XBOX360_A";
	m_XBOX360Presets[6] = "XBOX360_Y";
	m_XBOX360Presets[7] = "XBOX360_X";
	m_XBOX360Presets[8] = "XBOX360_LSHOULDER";
	m_XBOX360Presets[9] = "XBOX360_RSHOULDER";
	m_XBOX360Presets[10] = "XBOX360_LTRIGGER";
	m_XBOX360Presets[11] = "XBOX360_START";

	m_ControlBindings[12] = m_oConfig.GetValue("bindings", "P2MoveLeft");
	m_ControlBindings[13] = m_oConfig.GetValue("bindings", "P2MoveRight");
	m_ControlBindings[14] = m_oConfig.GetValue("bindings", "P2MoveUp");
	m_ControlBindings[15] = m_oConfig.GetValue("bindings", "P2MoveDown");
	m_ControlBindings[16] = m_oConfig.GetValue("bindings", "P2Fire");
	m_ControlBindings[17] = m_oConfig.GetValue("bindings", "P2Action");
	m_ControlBindings[18] = m_oConfig.GetValue("bindings", "P2PrevWeapon");
	m_ControlBindings[19] = m_oConfig.GetValue("bindings", "P2NextWeapon");
	m_ControlBindings[20] = m_oConfig.GetValue("bindings", "P2CamLeft");
	m_ControlBindings[21] = m_oConfig.GetValue("bindings", "P2CamRight");
	m_ControlBindings[22] = m_oConfig.GetValue("bindings", "P2UseGadget");
	m_ControlBindings[23] = m_oConfig.GetValue("bindings", "P2Pause");

	m_JoypadPresets[0] = "JOYPAD_BUTTON0";
	m_JoypadPresets[1] = "JOYPAD_BUTTON1";
	m_JoypadPresets[2] = "JOYPAD_BUTTON2";
	m_JoypadPresets[3] = "JOYPAD_BUTTON3";
	m_JoypadPresets[4] = "JOYPAD_X";
	m_JoypadPresets[5] = "JOYPAD_Y";
	m_JoypadPresets[6] = "JOYPAD_Z";
	m_JoypadPresets[7] = "JOYPAD_BUTTON4";
	m_JoypadPresets[8] = "JOYPAD_BUTTON5";
	m_JoypadPresets[9] = "JOYPAD_BUTTON6";
	m_JoypadPresets[10] = "JOYPAD_BUTTON7";
	m_JoypadPresets[11] = "JOYPAD_BUTTON8";

	// Control Schemes
	CString str;
	str = m_oConfig.GetValue("bindings", "P1ControlScheme");
	m_nControlSchemeP1 = str.ToInt();
	str = m_oConfig.GetValue("bindings", "P2ControlScheme");
	m_nControlSchemeP2 = str.ToInt();

	str = m_oConfig.GetValue("options", "SoundVolume");
	m_nSoundVolume = str.ToInt();
	if (m_nSoundVolume > 15)
		m_nSoundVolume = 15;
	if (m_nSoundVolume < 1)
		m_nSoundVolume = 1;

	str = m_oConfig.GetValue("options", "MusicVolume");
	m_nMusicVolume = str.ToInt();
	if (m_nMusicVolume > 15)
		m_nMusicVolume = 15;
	if (m_nMusicVolume < 1)
		m_nMusicVolume = 1;

	str = m_oConfig.GetValue("options", "GammaLevel");
	m_nGammaLevel = str.ToInt();
	if (m_nGammaLevel > 15)
		m_nGammaLevel = 15;
	if (m_nGammaLevel < 1)
		m_nGammaLevel = 1;

	// set the sound options
	CSoundManager::getInstance().setEffectVolume((float)m_nSoundVolume / 15.0f);
	CSoundManager::getInstance().setMusicVolume((float)m_nMusicVolume / 15.0f);

	int nPower2 = 1;
	while (nPower2 < m_nKeyboardPos)
		nPower2 *= 2;
	m_nKeyboardPos = nPower2 - 64;
	m_nXBOX360Pos = nPower2 - 64;

	// Get the original gamma level
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetGammaRamp(0, &m_oOldGammaLevel);
	resetGamma();
}

/**
* COptions::resetGamma
* @date Modified June 9, 2006
*/
void COptions::resetGamma()
{	
	// set the gamma level
	D3DGAMMARAMP oGamma;
	float fGamma = (float)m_nGammaLevel;
	fGamma /= 7.5f;

	for (unsigned int i = 0; i < 256; ++i)
	{
		oGamma.red[i] = oGamma.green[i] = oGamma.blue[i] = 
			unsigned short(min(255, i * (fGamma))) << 8;
	}

	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &oGamma);
}


/**
* COptionsResolutionLeftArrowButton::doAction
* @date Modified March 29, 2006
*/
void COptionsResolutionLeftArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	// get the next item down
	poOptionsMenu->m_nResolutionIndex--;
	if (poOptionsMenu->m_nResolutionIndex < 0)
	{
		poOptionsMenu->m_nResolutionIndex = int(poOptionsMenu->m_Resolutions.size()) - 1;
	}
	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[4];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(poOptionsMenu->m_Resolutions[poOptionsMenu->m_nResolutionIndex].c_str()));

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsResolutionRightArrowButton::doAction
* @date Modified March 29, 2006
*/
void COptionsResolutionRightArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	// get the next item down
	poOptionsMenu->m_nResolutionIndex++;
	if (poOptionsMenu->m_nResolutionIndex >= int(poOptionsMenu->m_Resolutions.size()))
	{
		poOptionsMenu->m_nResolutionIndex = 0;
	}
	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[4];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(poOptionsMenu->m_Resolutions[poOptionsMenu->m_nResolutionIndex].c_str()));

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsSoundVolumeLeftArrowButton::doAction
* @date Modified April 4, 2006
*/
void COptionsSoundVolumeLeftArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	poOptionsMenu->m_nSoundVolume--;

	if (poOptionsMenu->m_nSoundVolume < 1)
	{
		poOptionsMenu->m_nSoundVolume = 1;
	}

	string strVolume;

	for (int i = 0; i < poOptionsMenu->m_nSoundVolume; ++i)
	{
		strVolume += "¦";
	}

	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[8];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(strVolume.c_str()));

	// Set the volume
	CSoundManager::getInstance().setEffectVolume(((float)poOptionsMenu->m_nSoundVolume) / 15.0f);
	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsSoundVolumeRightArrowButton::doAction
* @date Modified April 4, 2006
*/
void COptionsSoundVolumeRightArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	poOptionsMenu->m_nSoundVolume++;

	if (poOptionsMenu->m_nSoundVolume > 15)
	{
		poOptionsMenu->m_nSoundVolume = 15;
	}

	string strVolume;

	for (int i = 0; i < poOptionsMenu->m_nSoundVolume; ++i)
	{
		strVolume += "¦";
	}

	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[8];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(strVolume.c_str()));

	// Set the volume
	CSoundManager::getInstance().setEffectVolume(poOptionsMenu->m_nSoundVolume / 15.0f);
	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsMusicVolumeLeftArrowButton::doAction
* @date Modified April 4, 2006
*/
void COptionsMusicVolumeLeftArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	poOptionsMenu->m_nMusicVolume--;

	if (poOptionsMenu->m_nMusicVolume < 1)
	{
		poOptionsMenu->m_nMusicVolume = 1;
	}

	string strVolume;

	for (int i = 0; i < poOptionsMenu->m_nMusicVolume; ++i)
	{
		strVolume += "¦";
	}

	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[12];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(strVolume.c_str()));

	// Set the volume
	CSoundManager::getInstance().setMusicVolume(poOptionsMenu->m_nMusicVolume / 15.0f);
	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsMusicVolumeRightArrowButton::doAction
* @date Modified April 4, 2006
*/
void COptionsMusicVolumeRightArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	poOptionsMenu->m_nMusicVolume++;

	if (poOptionsMenu->m_nMusicVolume > 15)
	{
		poOptionsMenu->m_nMusicVolume = 15;
	}

	string strVolume;

	for (int i = 0; i < poOptionsMenu->m_nMusicVolume; ++i)
	{
		strVolume += "¦";
	}

	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[12];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(strVolume.c_str()));

	// Set the volume
	CSoundManager::getInstance().setMusicVolume(poOptionsMenu->m_nMusicVolume / 15.0f);
	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsGammaLevelLeftArrowButton::doAction
* @date Modified April 30, 2006
*/
void COptionsGammaLevelLeftArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	poOptionsMenu->m_nGammaLevel--;

	if (poOptionsMenu->m_nGammaLevel < 1)
	{
		poOptionsMenu->m_nGammaLevel = 1;
	}

	string strVolume;

	for (int i = 0; i < poOptionsMenu->m_nGammaLevel; ++i)
	{
		strVolume += "¦";
	}

	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[18];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(strVolume.c_str()));

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);

	// set the gamma level
	D3DGAMMARAMP oGamma;
	float fGamma = (float)poOptionsMenu->m_nGammaLevel;
	fGamma /= 7.5f;

	for (unsigned int i = 0; i < 256; ++i)
	{
		oGamma.red[i] = oGamma.green[i] = oGamma.blue[i] = 
			unsigned short(min(255, i * (fGamma))) << 8;
	}

	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &oGamma);
}

/**
* COptionsGammaLevelRightArrowButton::doAction
* @date Modified April 30, 2006
*/
void COptionsGammaLevelRightArrowButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

	poOptionsMenu->m_nGammaLevel++;

	if (poOptionsMenu->m_nGammaLevel > 15)
	{
		poOptionsMenu->m_nGammaLevel = 15;
	}

	string strVolume;

	for (int i = 0; i < poOptionsMenu->m_nGammaLevel; ++i)
	{
		strVolume += "¦";
	}

	// get the text box
	CTextButton *pTextButton = (CTextButton*)poOptionsMenu->getButtons()[18];
	// set the new text for the new resolution
	pTextButton->setText(const_cast<char*>(strVolume.c_str()));

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);

	// set the gamma level
	D3DGAMMARAMP oGamma;
	float fGamma = (float)poOptionsMenu->m_nGammaLevel;
	fGamma /= 7.5f;

	for (unsigned int i = 0; i < 256; ++i)
	{
		oGamma.red[i] = oGamma.green[i] = oGamma.blue[i] = 
			unsigned short(min(255, i * (fGamma))) << 8;
	}

	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &oGamma);
}

/**
* COptions::setPlayerControls
* @date Modified May 11, 2006
*/
void COptions::setPlayerControls(CPlayer *poPlayer, bool bFirstPlayer)
{
		if (!poPlayer)
			return;

		CDirectInputWrapper *pDI = CDirectInputWrapper::getInstancePtr();

		if (bFirstPlayer)
		{
			if (m_nControlSchemeP1 == CS_KEYBOARD)
			{
				poPlayer->setControls(
					pDI->convertStringtoDIK(m_ControlBindings[4].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[10].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[8].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[9].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[6].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[7].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[5].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[0].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[1].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[2].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[3].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[11].c_str())
				);

				poPlayer->setInputType(PIT_KEYBOARDMOUSE);
			}
			else if (m_nControlSchemeP1 == CS_XBOX360)
			{
				poPlayer->setControls(
					pDI->convertStringtoXInput(m_ControlBindings[4].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[10].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[8].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[9].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[6].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[7].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[5].c_str()),
					0,
					0,
					0,
					0,
					pDI->convertStringtoXInput(m_ControlBindings[11].c_str())
				);

				poPlayer->setInputType(PIT_1ST_XBOX360PAD);
			}
			else // joypad
			{
				poPlayer->setControls(
					pDI->convertStringToJoypad(m_ControlBindings[4].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[10].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[8].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[9].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[6].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[7].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[5].c_str()),
					0,
					0,
					0,
					0,
					pDI->convertStringToJoypad(m_ControlBindings[11].c_str())
				);

				poPlayer->setInputType(PIT_1ST_GAMEPAD);
			}
		}
		else // player 2
		{
			if (m_nControlSchemeP2 == CS_KEYBOARD)
			{
				poPlayer->setControls(
					pDI->convertStringtoDIK(m_ControlBindings[16].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[22].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[20].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[21].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[18].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[19].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[17].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[12].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[13].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[14].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[15].c_str()),
					pDI->convertStringtoDIK(m_ControlBindings[23].c_str())
				);

				poPlayer->setInputType(PIT_KEYBOARDMOUSE);
			}
			else if (m_nControlSchemeP2 == CS_XBOX360)
			{
				poPlayer->setControls(
					pDI->convertStringtoXInput(m_ControlBindings[16].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[22].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[20].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[21].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[18].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[19].c_str()),
					pDI->convertStringtoXInput(m_ControlBindings[17].c_str()),
					0,
					0,
					0,
					0,
					pDI->convertStringtoXInput(m_ControlBindings[23].c_str())
				);

				poPlayer->setInputType(PIT_2ND_XBOX360PAD);
			}
			else // joypad
			{
				poPlayer->setControls(
					pDI->convertStringToJoypad(m_ControlBindings[16].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[22].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[20].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[21].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[18].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[19].c_str()),
					pDI->convertStringToJoypad(m_ControlBindings[17].c_str()),
					0,
					0,
					0,
					0,
					pDI->convertStringToJoypad(m_ControlBindings[23].c_str())
				);

				poPlayer->setInputType(PIT_2ND_GAMEPAD);
			}
		}
}


/**
* COptions::saveOptions
* @date Modified May 15, 2006
*/
void COptions::saveOptions()
{
	CString str;
	str.Format("%d", m_nControlSchemeP1);
	m_oConfig.SetValue("bindings", "P1ControlScheme", str);
	str.Format("%d", m_nControlSchemeP2);
	m_oConfig.SetValue("bindings", "P2ControlScheme", str);

	m_oConfig.SetValue("bindings", "P1MoveLeft", CString(m_ControlBindings[0].c_str()));
	m_oConfig.SetValue("bindings", "P1MoveRight", CString(m_ControlBindings[1].c_str()));
	m_oConfig.SetValue("bindings", "P1MoveUp", CString(m_ControlBindings[2].c_str()));
	m_oConfig.SetValue("bindings", "P1MoveDown", CString(m_ControlBindings[3].c_str()));
	m_oConfig.SetValue("bindings", "P1Fire", CString(m_ControlBindings[4].c_str()));
	m_oConfig.SetValue("bindings", "P1Action", CString(m_ControlBindings[5].c_str()));
	m_oConfig.SetValue("bindings", "P1PrevWeapon", CString(m_ControlBindings[6].c_str()));
	m_oConfig.SetValue("bindings", "P1NextWeapon", CString(m_ControlBindings[7].c_str()));
	m_oConfig.SetValue("bindings", "P1CamLeft", CString(m_ControlBindings[8].c_str()));
	m_oConfig.SetValue("bindings", "P1CamRight", CString(m_ControlBindings[9].c_str()));
	m_oConfig.SetValue("bindings", "P1UseGadget", CString(m_ControlBindings[10].c_str()));
	m_oConfig.SetValue("bindings", "P1Pause", CString(m_ControlBindings[11].c_str()));
	m_oConfig.SetValue("bindings", "P2MoveLeft", CString(m_ControlBindings[12].c_str()));
	m_oConfig.SetValue("bindings", "P2MoveRight", CString(m_ControlBindings[13].c_str()));
	m_oConfig.SetValue("bindings", "P2MoveUp", CString(m_ControlBindings[14].c_str()));
	m_oConfig.SetValue("bindings", "P2MoveDown", CString(m_ControlBindings[15].c_str()));
	m_oConfig.SetValue("bindings", "P2Fire", CString(m_ControlBindings[16].c_str()));
	m_oConfig.SetValue("bindings", "P2Action", CString(m_ControlBindings[17].c_str()));
	m_oConfig.SetValue("bindings", "P2PrevWeapon", CString(m_ControlBindings[18].c_str()));
	m_oConfig.SetValue("bindings", "P2NextWeapon", CString(m_ControlBindings[19].c_str()));
	m_oConfig.SetValue("bindings", "P2CamLeft", CString(m_ControlBindings[20].c_str()));
	m_oConfig.SetValue("bindings", "P2CamRight", CString(m_ControlBindings[21].c_str()));
	m_oConfig.SetValue("bindings", "P2UseGadget", CString(m_ControlBindings[22].c_str()));
	m_oConfig.SetValue("bindings", "P2Pause", CString(m_ControlBindings[23].c_str()));

	str.Format("%d", m_nSoundVolume);
	// options
	m_oConfig.SetValue("options", "SoundVolume", str);
	str.Format("%d", m_nMusicVolume);
	m_oConfig.SetValue("options", "MusicVolume", str);
	str.Format("%d", m_nGammaLevel);
	m_oConfig.SetValue("options", "GammaLevel", str);

	m_oConfig.SaveConfig("AckZombies.ini");
}



