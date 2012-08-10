/**
* @file ControlsMenu.cpp
* @author Jordan Leckner
* @date Created April 6, 2006
*
* This file includes the definition of the Controls Menu class
* used for displaying a Controls menu and its buttons.
*/
#include "ControlsMenu.h"

/**
* CControlsMenu::drawMenu
* @date Modified May 10, 2006
*/
void CControlsMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
{
	// Direct3d Device
	static IDirect3DDevice9 *poDirect3DDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	poDirect3DDevice->SetTexture(0, m_pTextureBackground);
	// Finally, draw the triangles of the quad.
	poDirect3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SMenuFVF));
	poDirect3DDevice->SetFVF(SMenuFVF::FVF_Flags);
	poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 3, 1);
	// Clear out the texture
	poDirect3DDevice->SetTexture(0, 0);

	int nWindowWidth = CMenuManager::getInstance().getWindowWidth();
	int nWindowHeight = CMenuManager::getInstance().getWindowHeight();

	// only draw the buttons if the child menu's texture is valid.
	// this way we can save memory on the background.
	if (!m_poChildMenu || (m_poChildMenu && m_poChildMenu->getTextureBackground()))
	{
		CMenu::catchInput();
		// Go through all of the buttons for this menu and draw them in front
		// of this menu.
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			// draw the captions
			if (i == 2)
			{
				int nWidthBefore = ((CTextButton*)m_Buttons[i])->m_nPositionX;
				// draw the player 1 and player 2 captions
				((CTextButton*)m_Buttons[i])->setText("Player 1");
				((CTextButton*)m_Buttons[i])->m_nPositionY = (nWindowHeight >> 1) - 192;
				((CTextButton*)m_Buttons[i])->m_nPositionX = -(nWindowWidth >> 1) + 64;
				((CTextButton*)m_Buttons[i])->drawButton(NULL);
				((CTextButton*)m_Buttons[i])->setText("Player 2");
				((CTextButton*)m_Buttons[i])->m_nPositionX = (nWindowWidth >> 1) - 256 - 32;
				((CTextButton*)m_Buttons[i])->drawButton(NULL);
				// reset it
				((CTextButton*)m_Buttons[i])->m_nPositionX = nWidthBefore;

				unsigned int j = 0;

				if (m_nControlSchemeP1 == CS_XBOX360 || m_nControlSchemeP1 == CS_JOYPAD)
					j = 4;
				for (; j < m_ControlNames.size(); ++j)
				{
					((CTextButton*)m_Buttons[i])->setText(const_cast<char*>(m_ControlNames[j].c_str()));
					((CTextButton*)m_Buttons[i])->m_nPositionY = (int)((CTextButton*)m_Buttons[i+1])->getPosition().y
						- (nWindowHeight >> 1) - (j * 24) - 2;
					((CTextButton*)m_Buttons[i])->drawButton(NULL);
				}

				// do player 2
				((CTextButton*)m_Buttons[i])->m_nPositionX = (nWindowWidth >> 1) - 256 - 256;

				j = 0;
				if (m_nControlSchemeP2 == CS_XBOX360 || m_nControlSchemeP2 == CS_JOYPAD)
					j = 4;

				for (; j < m_ControlNames.size(); ++j)
				{
					((CTextButton*)m_Buttons[i])->setText(const_cast<char*>(m_ControlNames[j].c_str()));
					((CTextButton*)m_Buttons[i])->m_nPositionY = (int)((CTextButton*)m_Buttons[i+1])->getPosition().y
						- (nWindowHeight >> 1) - (j * 24);
					((CTextButton*)m_Buttons[i])->drawButton(NULL);
				}
				
				// reset it back
				((CTextButton*)m_Buttons[i])->m_nPositionX = nWidthBefore;
			}
			else
			{
			//	// Draw this button and check for input
			//	if (m_Buttons[i]->drawButton(pMouseCursor))
			//	{
			//	}
				if (i > 26 || i < 2)
					m_Buttons[i]->drawButton(pMouseCursor);
			}
			if (i > 1 && i < 27 && ((CTextButton*)m_Buttons[i])->m_bProcessActionEveryFrame)
				((CTextButton*)m_Buttons[i])->doAction();
		}

		// Player 1
		if (m_nControlSchemeP1 == CS_KEYBOARD)
		{
			for (int i = 3; i < 15; ++i)
			{
				if (m_Buttons[i]->drawButton(pMouseCursor))
				{
					if (i != m_nHoverIndex)
					{
						CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
						m_nHoverIndex = i;
					}
				}
			}
		}
		else if (m_nControlSchemeP1 == CS_XBOX360)
		{
			for (int i = 7; i < 15; ++i)
				if (m_Buttons[i]->drawButton(pMouseCursor))
				{
					if (i != m_nHoverIndex)
					{
						CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
						m_nHoverIndex = i;
					}
				}
		}
		else
		{
			for (int i = 7; i < 15; ++i)
				if (m_Buttons[i]->drawButton(pMouseCursor))
				{
					if (i != m_nHoverIndex)
					{
						CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
						m_nHoverIndex = i;
					}
				}
		}

		// Player 2
		if (m_nControlSchemeP2 == CS_KEYBOARD)
		{
			for (int i = 15; i < 27; ++i)
				if (m_Buttons[i]->drawButton(pMouseCursor))
				{
					if (i != m_nHoverIndex)
					{
						CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
						m_nHoverIndex = i;
					}
				}
		}
		else if (m_nControlSchemeP2 == CS_XBOX360)
		{
			for (int i = 19; i < 27; ++i)
				if (m_Buttons[i]->drawButton(pMouseCursor))
				{
					if (i != m_nHoverIndex)
					{
						CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
						m_nHoverIndex = i;
					}
				}
		}
		else
		{
			for (int i = 19; i < 27; ++i)
				if (m_Buttons[i]->drawButton(pMouseCursor))
				{
					if (i != m_nHoverIndex)
					{
						CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
						m_nHoverIndex = i;
					}
				}
		}
	}

	// set the text for both player's control scheme
	m_pPlayer1Scheme->setText(const_cast<char*>(m_ControlSchemes[m_nControlSchemeP1].c_str()));
	m_pPlayer1Scheme->drawButton(NULL);
	m_pPlayer2Scheme->setText(const_cast<char*>(m_ControlSchemes[m_nControlSchemeP2].c_str()));
	m_pPlayer2Scheme->drawButton(NULL);

	// Go back to the main menu if they press escape.
	if (!m_poChildMenu && CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
	{
		m_bDeleteMe = true;
		// reset the alpha
		for (unsigned int i = 0; i < getButtons().size(); ++i)
			if (m_Buttons[i]->getType() != BT_TEXT)
				m_Buttons[i]->setAlpha(0);

		// reset the parents alpha
		for (unsigned int i = 0; i < m_poParentMenu->getButtons().size(); ++i)
			if (m_poParentMenu->getButtons()[i]->getType() != BT_TEXT)
				m_poParentMenu->getButtons()[i]->setAlpha(0);
	}
}


/**
* CControlsPlayer1LeftArrowButton::doAction
* @date Modified May 11, 2006
*/
void CControlsPlayer1LeftArrowButton::doAction(void)
{
	CControlsMenu *poControlsMenu = (CControlsMenu*)m_poParentMenu;
	COptions* poOptions = COptions::getInstancePtr();
	CTextButton *poText;

	poControlsMenu->m_nControlSchemeP1--;

	if (poControlsMenu->m_nControlSchemeP1 < CS_KEYBOARD)
		poControlsMenu->m_nControlSchemeP1 = CS_JOYPAD;

	if (poControlsMenu->m_nControlSchemeP1 == CS_KEYBOARD)
	{
		for (int i = 3; i < 15; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getKeyboardPreset(i-3));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP1 == CS_XBOX360)
	{
		for (int i = 3; i < 15; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getXBOX360Preset(i-3));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP1 == CS_JOYPAD)
	{
		for (int i = 3; i < 15; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getJoypadPreset(i-3));
		}
	}
}

/**
* CControlsPlayer1RightArrowButton::doAction
* @date Modified May 11, 2006
*/
void CControlsPlayer1RightArrowButton::doAction(void)
{
	CControlsMenu *poControlsMenu = (CControlsMenu*)m_poParentMenu;
	COptions* poOptions = COptions::getInstancePtr();
	CTextButton *poText;

	poControlsMenu->m_nControlSchemeP1++;

	if (poControlsMenu->m_nControlSchemeP1 > CS_JOYPAD)
		poControlsMenu->m_nControlSchemeP1 = CS_KEYBOARD;

	if (poControlsMenu->m_nControlSchemeP1 == CS_KEYBOARD)
	{
		for (int i = 3; i < 15; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getKeyboardPreset(i-3));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP1 == CS_XBOX360)
	{
		for (int i = 3; i < 15; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getXBOX360Preset(i-3));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP1 == CS_JOYPAD)
	{
		for (int i = 3; i < 15; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getJoypadPreset(i-3));
		}
	}
}

/**
* CControlsPlayer2LeftArrowButton::doAction
* @date Modified May 11, 2006
*/
void CControlsPlayer2LeftArrowButton::doAction(void)
{
	CControlsMenu *poControlsMenu = (CControlsMenu*)m_poParentMenu;
	COptions* poOptions = COptions::getInstancePtr();
	CTextButton *poText;

	poControlsMenu->m_nControlSchemeP2--;

	if (poControlsMenu->m_nControlSchemeP2 < CS_KEYBOARD)
		poControlsMenu->m_nControlSchemeP2 = CS_JOYPAD;

	if (poControlsMenu->m_nControlSchemeP2 == CS_KEYBOARD)
	{
		for (int i = 15; i < 27; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getKeyboardPreset(i-15));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP2 == CS_XBOX360)
	{
		for (int i = 15; i < 27; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getXBOX360Preset(i-15));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP2 == CS_JOYPAD)
	{
		for (int i = 15; i < 27; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getJoypadPreset(i-15));
		}
	}
}

/**
* CControlsPlayer2RightArrowButton::doAction
* @date Modified May 11, 2006
*/
void CControlsPlayer2RightArrowButton::doAction(void)
{
	CControlsMenu *poControlsMenu = (CControlsMenu*)m_poParentMenu;
	COptions* poOptions = COptions::getInstancePtr();
	CTextButton *poText;

	poControlsMenu->m_nControlSchemeP2++;

	if (poControlsMenu->m_nControlSchemeP2 > CS_JOYPAD)
		poControlsMenu->m_nControlSchemeP2 = CS_KEYBOARD;

	if (poControlsMenu->m_nControlSchemeP2 == CS_KEYBOARD)
	{
		for (int i = 15; i < 27; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getKeyboardPreset(i-15));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP2 == CS_XBOX360)
	{
		for (int i = 15; i < 27; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getXBOX360Preset(i-15));
		}
	}
	else if (poControlsMenu->m_nControlSchemeP2 == CS_JOYPAD)
	{
		for (int i = 15; i < 27; ++i)
		{
			poText = (CTextButton*)poControlsMenu->m_Buttons[i];
			poText->setText(poOptions->getJoypadPreset(i-15));
		}
	}
}


