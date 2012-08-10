/**
* @file OptionsMenu.cpp
* @author Jordan Leckner
* @date Created March 29, 2006
*
* This file includes the definition of the Options Menu class
* used for displaying an option menu and its buttons.
*/
#include "OptionsMenu.h"
#include "DirectInputWrapper.h"

/**
* COptionsMenu::drawMenu
* @date Modified April 10, 2006
*/
void COptionsMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
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

	// only draw the buttons if the child menu's texture is valid.
	// this way we can save memory on the background.
	if (!m_poChildMenu || (m_poChildMenu && m_poChildMenu->getTextureBackground()))
	{
		CMenu::catchInput();
		// Go through all of the buttons for this menu and draw them in front
		// of this menu.
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			if (m_Buttons[i]->getType() == 8675309)
				continue;
			// Draw this button and check for input
			if (m_Buttons[i]->drawButton(pMouseCursor))
			{
				// Mouse is hovering over the button
				if (i != m_nHoverIndex)
				{
					CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
					m_nHoverIndex = i;
				}
			}
		}
	}

	// Go back to the main menu if they press escape.
	if (!m_poChildMenu && CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
	{
		m_bDeleteMe = true;
		// reset the alpha
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
			if (m_Buttons[i]->getType() != BT_TEXT)
				m_Buttons[i]->setAlpha(0);

		// reset the parents alpha
		for (unsigned int i = 0; i < m_poParentMenu->getButtons().size(); ++i)
			if (m_poParentMenu->getButtons()[i]->getType() != BT_TEXT)
				m_poParentMenu->getButtons()[i]->setAlpha(0);

		CSoundManager::getInstance().setEffectVolume(((float)COptions::getInstance().getSoundVolume()) / 15.0f);
		CSoundManager::getInstance().setMusicVolume(((float)COptions::getInstance().getMusicVolume()) / 15.0f);
		COptions::getInstance().resetGamma();
	}
}