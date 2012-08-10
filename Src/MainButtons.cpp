/**
* @file MainButtons.cpp
* @author Jordan Leckner
* @date Created March 30, 2006
*
* This file includes all the Main Buttons classes definitions
*/
#include "MainButtons.h"
#include "Menu.h"
#include "MenuManager.h"
#include "OptionsMenu.h"
#include "CreditsMenu.h"
#include "MultiplayerMenu.h"
#include "HighScoreMenu.h"
#include "ControlsMenu.h"
#include "DirectInputWrapper.h"
#include "ObjectManager.h"
#include "Player.h"

#include "TestState.h"
#include "SoundManager.h"
#include "MenuState.h"
#include "PauseState.h"

/**
* CBackToGameButton::doAction
* @date Modified May 16, 2006
*/
void CBackToGameButton::doAction(void)
{
	CGameKernel::getInstance().changeState(CTestState::getInstancePtr());
}

/**
* CContinueButton::doAction
* @date Modified May 18, 2006
*/
void CContinueButton::doAction(void)
{
	CObjectManager::ObjectList loPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);

	if (loPlayers.empty())
		return;

	unsigned int nMoney = ((CPlayer*)(loPlayers.front()))->getAmtMoney();
	
	nMoney -= 50000;

	((CPlayer*)(loPlayers.front()))->setAmtMoney(nMoney);
	CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();
	while (oPlayerIter != loPlayers.end())
	{
		((CPlayer*)(*oPlayerIter))->activate();
		((CPlayer*)(*oPlayerIter))->setMesh((CAnimatedMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Johnny.x"));
		((CPlayer*)(*oPlayerIter))->setHealth(100);
		oPlayerIter++;
	}

	CGameKernel::getInstance().changeState(CTestState::getInstancePtr());
}

/**
* CMainMenuButton::doAction
* @date Modified May 16, 2006
*/
void CMainMenuButton::doAction(void)
{
	CGameKernel::getInstance().changeState(CMenuState::getInstancePtr());
}

/**
* CSinglePlayerButton::doAction
* @date Modified April 29, 2006
*/
void CSinglePlayerButton::doAction(void)
{
	// Clear out the second player, if one.
	CObjectManager::ObjectList lPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
	if (lPlayers.size() > 1)
	{
		CObjectManager::ObjectList::iterator iter = lPlayers.begin();
		iter++;
		CPlayer *poPlayer = (CPlayer*)*iter;
		CObjectManager::getInstance().removeObjectImmediate(poPlayer);
	}	

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);

	CMenuState::getInstance().setLoadingState(true);

	CLoadState::getInstance().setTwoPlayers(false);
}

/**
* CMultiplayerButton::doAction
* @date Modified April 29, 2006
*/
void CMultiplayerButton::doAction(void)
{
	CMenuState::getInstance().setLoadingState(true);

	CLoadState::getInstance().setTwoPlayers(true);

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COptionsButton::doAction
* @date Modified April 29, 2006
*/
void COptionsButton::doAction(void)
{
	COptionsMenu *poOptionsMenu = new COptionsMenu;

	if (m_poParentMenu->getType() == MT_PAUSE)
	{
		CPauseState::getInstance().setOptionsMenu(poOptionsMenu);
	}

	CMenuManager::getInstance().addMenu(poOptionsMenu);

	poOptionsMenu->setParentMenu(m_poParentMenu);
	m_poParentMenu->setChildMenu(poOptionsMenu);

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* COKButton::doAction
* @date Modified April 29, 2006
*/
void COKButton::doAction(void)
{
	m_poParentMenu->deleteMe();

	// reset the alpha
	for (unsigned int i = 0; i < m_poParentMenu->getButtons().size(); ++i)
		if (m_poParentMenu->getButtons()[i]->getType() != BT_TEXT)
			m_poParentMenu->getButtons()[i]->setAlpha(0);

	// reset the parents alpha
	if (m_poParentMenu->getParentMenu())
		for (unsigned int i = 0; i < m_poParentMenu->getParentMenu()->getButtons().size(); ++i)
			if (m_poParentMenu->getParentMenu()->getButtons()[i]->getType() != BT_TEXT)
				m_poParentMenu->getParentMenu()->getButtons()[i]->setAlpha(0);

	if (m_poParentMenu->getType() == MT_OPTIONS)
	{
		COptionsMenu *poOptionsMenu = (COptionsMenu*)m_poParentMenu;

		COptions::getInstance().m_nResolutionIndex = poOptionsMenu->m_nResolutionIndex;
		COptions::getInstance().m_nSoundVolume = poOptionsMenu->m_nSoundVolume;
		COptions::getInstance().m_nMusicVolume = poOptionsMenu->m_nMusicVolume;
		COptions::getInstance().m_nGammaLevel = poOptionsMenu->m_nGammaLevel;
	}
	else if (m_poParentMenu->getType() == MT_CONTROLS)
	{
		CControlsMenu *poControlMenu = (CControlsMenu*)m_poParentMenu;
		COptions *poOptions = COptions::getInstancePtr();

		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CDirectInputWrapper *pDI = CDirectInputWrapper::getInstancePtr();
		int nCounter = 0;

		int nPlayerScheme = ((CControlsMenu*)m_poParentMenu)->m_nControlSchemeP1;
		int nPlayerScheme2 = ((CControlsMenu*)m_poParentMenu)->m_nControlSchemeP2;

		// If the list is empty, then there are no players yet, so just set the options
		if (lPlayers.empty())
		{
			for (int i = 0; i < 24; ++i)
			{
				poOptions->m_ControlBindings[i] = ((CTextButton*)poControlMenu->m_Buttons[3 + i])->getText();
				poOptions->m_nControlSchemeP1 = nPlayerScheme;
				poOptions->m_nControlSchemeP2 = nPlayerScheme2;
			}
		}

		// set all of the controls for the players
		for (CObjectManager::ObjectList::iterator iter = lPlayers.begin(); iter != lPlayers.end(); ++iter)
		{
			CPlayer *poPlayer = (CPlayer*)*iter;
		
			if (nPlayerScheme == CS_KEYBOARD)
			{
				poPlayer->setControls(
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  7])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) + 13])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  11])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  12])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  9])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  10])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  8])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  3])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  4])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  5])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  6])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  14])->getText())
				);

				// first time through
				if (!nCounter)
				{
					for (int i = 0; i < 12; ++i)
					{
						CTextButton *poTextButton = ((CTextButton*)poControlMenu->getButtons()[i + 3]);
						poOptions->m_ControlBindings[i] = poTextButton->getText();
					}
				}
				else
				{
					for (int i = 0; i < 12; ++i)
					{
						CTextButton *poTextButton = ((CTextButton*)poControlMenu->getButtons()[i + 15]);
						poOptions->m_ControlBindings[i + 12] = poTextButton->getText();
					}
				}

				poPlayer->setInputType(PIT_KEYBOARDMOUSE);
			}
			else if (nPlayerScheme == CS_XBOX360)
			{
				poPlayer->setControls(
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  7])->getText()),
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) + 13])->getText()),
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  11])->getText()),
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  12])->getText()),
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  9])->getText()),
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  10])->getText()),
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  8])->getText()),
					// Zero for the movement, we don't want to set the movement controls
					0, 0, 0, 0,
					pDI->convertStringtoXInput(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  14])->getText())
				);

				// first time through
				if (!nCounter)
				{
					for (int i = 0; i < 12; ++i)
					{
						CTextButton *poTextButton = ((CTextButton*)poControlMenu->getButtons()[i + 3]);
						poOptions->m_ControlBindings[i] = poTextButton->getText();
					}
				}
				else
				{
					for (int i = 0; i < 12; ++i)
					{
						CTextButton *poTextButton = ((CTextButton*)poControlMenu->getButtons()[i + 15]);
						poOptions->m_ControlBindings[i + 12] = poTextButton->getText();
					}
				}

				// Player 1
				if (!nCounter)
					poPlayer->setInputType(PIT_1ST_XBOX360PAD);
				else
					poPlayer->setInputType(PIT_2ND_XBOX360PAD);
			}
			else if (nPlayerScheme == CS_JOYPAD)
			{
				poPlayer->setControls(
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  7])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) + 13])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  11])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  12])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  9])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  10])->getText()),
					pDI->convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  8])->getText()),
					// Zero for the movement, we don't want to set the movement controls
					0, 0, 0, 0,
					pDI->convertStringToJoypad(((CTextButton*)poControlMenu->getButtons()[(nCounter*12) +  14])->getText())
				);

				// first time through
				if (!nCounter)
				{
					for (int i = 0; i < 12; ++i)
					{
						CTextButton *poTextButton = ((CTextButton*)poControlMenu->getButtons()[i + 3]);
						poOptions->m_ControlBindings[i] = poTextButton->getText();
					}
				}
				else
				{
					for (int i = 0; i < 12; ++i)
					{
						CTextButton *poTextButton = ((CTextButton*)poControlMenu->getButtons()[i + 15]);
						poOptions->m_ControlBindings[i + 12] = poTextButton->getText();
					}
				}

				// Player 1
				if (!nCounter)
					poPlayer->setInputType(PIT_1ST_GAMEPAD);
				else
					poPlayer->setInputType(PIT_2ND_GAMEPAD);
			}

			nCounter++;

			nPlayerScheme = ((CControlsMenu*)m_poParentMenu)->m_nControlSchemeP2;
		}
		COptions::getInstance().m_nControlSchemeP1 = ((CControlsMenu*)m_poParentMenu)->m_nControlSchemeP1;
		COptions::getInstance().m_nControlSchemeP2 = ((CControlsMenu*)m_poParentMenu)->m_nControlSchemeP2;
	}

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* CCancelButton::doAction
* @date Modified April 29, 2006
*/
void CCancelButton::doAction(void)
{
	m_poParentMenu->deleteMe();

	// reset the alpha
	for (unsigned int i = 0; i < m_poParentMenu->getButtons().size(); ++i)
		if (m_poParentMenu->getButtons()[i]->getType() != BT_TEXT)
			m_poParentMenu->getButtons()[i]->setAlpha(0);

	// reset the parents alpha
	if (m_poParentMenu->getParentMenu())
		for (unsigned int i = 0; i < m_poParentMenu->getParentMenu()->getButtons().size(); ++i)
			if (m_poParentMenu->getParentMenu()->getButtons()[i]->getType() != BT_TEXT)
				m_poParentMenu->getParentMenu()->getButtons()[i]->setAlpha(0);

	if (m_poParentMenu->getType() == MT_OPTIONS)
	{
		CSoundManager::getInstance().setEffectVolume(((float)COptions::getInstance().getSoundVolume()) / 15.0f);
		CSoundManager::getInstance().setMusicVolume(((float)COptions::getInstance().getMusicVolume()) / 15.0f);
		COptions::getInstance().resetGamma();
	}

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCANCEL);
}

/**
* CControlsButton::doAction
* @date Modified April 29, 2006
*/
void CControlsButton::doAction(void)
{
	CControlsMenu *poMenu = new CControlsMenu;

	if (m_poParentMenu->getParentMenu()->getType() == MT_PAUSE)
	{

	}
	else
	{
		CMenuManager::getInstance().addMenu(poMenu);
	}
	
	poMenu->setParentMenu(m_poParentMenu);
	m_poParentMenu->setChildMenu(poMenu);

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* CCreditsButton::doAction
* @date Modified April 29, 2006
*/
void CCreditsButton::doAction(void)
{
	CCreditsMenu *poMenu = new CCreditsMenu;
	CMenuManager::getInstance().addMenu(poMenu);
	poMenu->setParentMenu(m_poParentMenu);
	m_poParentMenu->setChildMenu(poMenu);

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* CHighScoreButton::doAction
* @date Modified April 29, 2006
*/
void CHighScoreButton::doAction(void)
{
	CHighScoreMenu *poHighScoreMenu = new CHighScoreMenu;
	CMenuManager::getInstance().addMenu(poHighScoreMenu);
	poHighScoreMenu->setParentMenu(m_poParentMenu);
	m_poParentMenu->setChildMenu(poHighScoreMenu);

	CSoundManager::getInstance().playSound(SND_EFFECT_MENUCLICK);
}

/**
* CTextButton::doAction
* @date Modified May 9, 2006
*/
void CTextButton::doAction(void)
{
	if (m_nSecondaryType == -1)
		return;

	CControlsMenu *poControlMenu = (CControlsMenu*)m_poParentMenu;

	// Control scheme for this button
	int nPlayer, nControlScheme;
	if (m_nSecondaryType < ST_CONTROL_MOVELEFT_P2)
		nPlayer = 1;
	else
		nPlayer = 2;

	if (nPlayer == 1)
		nControlScheme = poControlMenu->m_nControlSchemeP1;
	else
		nControlScheme = poControlMenu->m_nControlSchemeP2;

	bool bKeyboardKey = false, bXInputKey = false;
	static bool bPressAnotherKey = false;
	// get keyboard input
	DWORD dwKey = m_dwKey;
	if (nControlScheme == CS_KEYBOARD && CDirectInputWrapper::getInstance().getKey(dwKey))
	{
		bool bNotInList = true;
		if (nPlayer == 1)
		{
			for (int i = ST_CONTROL_MOVELEFT_P1; i < ST_CONTROL_MOVELEFT_P2; ++i)
			{
				if (CDirectInputWrapper::getInstance().convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[i + 3])->getText())
					== dwKey)
				{
					bNotInList = false;
				}
			}
		}
		else
		{
			for (int i = ST_CONTROL_MOVELEFT_P2; i < ST_MAX; ++i)
			{
				if (CDirectInputWrapper::getInstance().convertStringtoDIK(((CTextButton*)poControlMenu->getButtons()[i + 3])->getText())
					== dwKey)
				{
					bNotInList = false;
				}
			}
		}

		if (!bNotInList)
		{
			bPressAnotherKey = true;
			m_bProcessActionEveryFrame = true;
			bKeyboardKey = false;
			dwKey = m_dwKey;
		}
		else
		{
			bPressAnotherKey = false;
			bKeyboardKey = true;
			m_bProcessActionEveryFrame = false;
		}

	}
	else
		m_bProcessActionEveryFrame = true;

	// get X Input button
	if (nControlScheme == CS_XBOX360 && m_bProcessActionEveryFrame)
		if (CDirectInputWrapper::getInstance().getXInputButton(0, dwKey))
		{
			bool bXinputNotIn = true;

			if (nPlayer == 1)
			{
				for (int i = ST_CONTROL_MOVELEFT_P1; i < ST_CONTROL_MOVELEFT_P2; ++i)
				{
					if (CDirectInputWrapper::getInstance().convertStringtoXInput(
						((CTextButton*)poControlMenu->getButtons()[i + 3])->getText()) == dwKey)
					{
						bXinputNotIn = false;
					}
				}
			}
			else
			{
				for (int i = ST_CONTROL_MOVELEFT_P2; i < ST_MAX; ++i)
				{
					if (CDirectInputWrapper::getInstance().convertStringtoXInput(
						((CTextButton*)poControlMenu->getButtons()[i + 3])->getText()) == dwKey)
					{
						bXinputNotIn = false;
					}
				}
			}

			if (bXinputNotIn)
			{
				bPressAnotherKey = true;
				bXInputKey = true;
				m_bProcessActionEveryFrame = false;
			}
			else
			{
				bPressAnotherKey = false;
				bXInputKey = false;
				m_bProcessActionEveryFrame = true;
				dwKey = m_dwKey;
			}
		}

	// get Joystick button
	if (nControlScheme == CS_JOYPAD && m_bProcessActionEveryFrame)
		if (CDirectInputWrapper::getInstance().getJoypadButton(0, dwKey))
		{
			bool bJoystickNotIn = true;

			if (nPlayer == 1)
			{
				for (int i = ST_CONTROL_MOVELEFT_P1; i < ST_CONTROL_MOVELEFT_P2; ++i)
				{
					if (CDirectInputWrapper::getInstance().convertStringToJoypad(
						((CTextButton*)poControlMenu->getButtons()[i + 3])->getText()) == dwKey)
					{
						bJoystickNotIn = false;
					}
				}
			}
			else
			{
				for (int i = ST_CONTROL_MOVELEFT_P2; i < ST_MAX; ++i)
				{
					if (CDirectInputWrapper::getInstance().convertStringToJoypad(
						((CTextButton*)poControlMenu->getButtons()[i + 3])->getText()) == dwKey)
					{
						bJoystickNotIn = false;
					}
				}
			}

			if (bJoystickNotIn)
			{
				bPressAnotherKey = true;
				m_bProcessActionEveryFrame = false;
			}
			else
			{
				bPressAnotherKey = false;
				m_bProcessActionEveryFrame = true;
				dwKey = m_dwKey;
			}
		}

	bool bLeftMouseButton = false, bRightMouseButton = false, bMouseInside = false;
	// check for both mouse buttons
	if (CDirectInputWrapper::getInstance().isLeftMouseButtonDown())
	{
		m_bLeftMouseButtonDown = true;
	}
	else
	{
		if (m_bLeftMouseButtonDown)
		{
			m_bLeftMouseButtonDown = false;
			bLeftMouseButton = true;
		}
	}
	if (CDirectInputWrapper::getInstance().isRightMouseButtonDown())
	{
		bRightMouseButton = true;
	}
	else
	{
		if (m_bRightMouseButtonDown)
		{
			m_bRightMouseButtonDown = false;
			bRightMouseButton = true;
		}
	}

	// make sure the mouse is down inside the button
	if (checkInput(CDirectInputWrapper::getInstance().getMousePositionPtr()))
		bMouseInside = true;
	else
	{
		bMouseInside = false;
		if (m_bLeftMouseButtonDown)
		{
			m_bProcessActionEveryFrame = false;
			bKeyboardKey = true;
		}
	}

	// set the text of the button to whatever was pressed.
	for (int i = ST_CONTROL_MOVELEFT_P1; i < ST_MAX; ++i)
	{
		if (m_nSecondaryType == i)
		{
			if (m_bProcessActionEveryFrame)
			{
				if (bLeftMouseButton && bMouseInside)
				{
					// make sure left mouse is not already in the list
					bool bMouseIn = true;
					for (int j = ST_CONTROL_MOVELEFT_P1; j < ST_MAX; ++j)
					{
						if (!strcmp(((CTextButton*)poControlMenu->getButtons()[j + 3])->getText(), "LMOUSE"))
							bMouseIn = false;
					}
					if (bMouseIn)
					{
						((CTextButton*)poControlMenu->getButtons()[i + 3])->setText("LMOUSE");
						m_bProcessActionEveryFrame = false;
						bPressAnotherKey = false;
					}
					else
					{
						bPressAnotherKey = true;
					}
				}
				else if (bRightMouseButton && bMouseInside)
				{
					// make sure right mouse is not already in the list
					bool bMouseIn = true;
					for (int j = ST_CONTROL_MOVELEFT_P1; j < ST_MAX; ++j)
					{
						if (!strcmp(((CTextButton*)poControlMenu->getButtons()[j + 3])->getText(), "RMOUSE"))
							bMouseIn = false;
					}
					if (bMouseIn)
					{
						((CTextButton*)poControlMenu->getButtons()[i + 3])->setText("RMOUSE");
						m_bProcessActionEveryFrame = false;
						bPressAnotherKey = false;
					}
					else
					{
						bPressAnotherKey = true;
					}
				}
				else
				{
					if (bPressAnotherKey)
						((CTextButton*)poControlMenu->getButtons()[i + 3])->setText("Press another key..");
					else
						((CTextButton*)poControlMenu->getButtons()[i + 3])->setText("Press a key..");
				}

			}
			else
			{
				if (bKeyboardKey)
					((CTextButton*)poControlMenu->getButtons()[i + 3])->setText(const_cast<char*>
						(CDirectInputWrapper::getInstance().convertDIKtoString(dwKey)));
				else if (bXInputKey)
					((CTextButton*)poControlMenu->getButtons()[i + 3])->setText(const_cast<char*>
						(CDirectInputWrapper::getInstance().convertDIKtoStringXInput(dwKey)));
				else
					((CTextButton*)poControlMenu->getButtons()[i + 3])->setText(const_cast<char*>
						(CDirectInputWrapper::getInstance().convertDIKtoStringJoypad(dwKey)));
			}
		}
	}
}


/**
* CQuitButton::doAction
* @date Modified April 29, 2006
*/
void CQuitButton::doAction(void)
{
	PostQuitMessage(0);
}