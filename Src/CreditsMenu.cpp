/**
* @file CreditsMenu.cpp
* @author Jordan Leckner
* @date Created March 31, 2006
*
* This file includes the definition of the Credits Menu class
* used for displaying a Credit menu and its buttons.
*/
#include "CreditsMenu.h"
#include "DirectInputWrapper.h"
#include "MenuManager.h"
#include "SoundManager.h"

/**
* CCreditsMenu::drawMenu
* @date Modified April 10, 2006
*/
void CCreditsMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
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

	bool bTick = m_oTick.tick();

	CMenu::catchInput();

	for (unsigned int i = 0; i < m_Credits.size(); ++i)
	{
		// we have only 1 button.
		((CTextButton*)m_Buttons[0])->setText(const_cast<char*>(m_Credits[i].name.c_str()));
		((CTextButton*)m_Buttons[0])->setPositionX((int)m_Credits[i].position.x);
		((CTextButton*)m_Buttons[0])->setPositionY((int)m_Credits[i].position.y);

		// Draw this button
		m_Buttons[0]->drawButton(NULL);

		// change to the job
		((CTextButton*)m_Buttons[0])->setText(const_cast<char*>(m_Credits[i].job.c_str()));
		
		// move the position for the job.
		((CTextButton*)m_Buttons[0])->setPositionX(((CTextButton*)m_Buttons[0])->getPositionX() + 32);
		((CTextButton*)m_Buttons[0])->setPositionY(((CTextButton*)m_Buttons[0])->getPositionY() - 48);

		// Draw this button
		m_Buttons[0]->drawButton(NULL);

		// put them back
		((CTextButton*)m_Buttons[0])->setPositionX(((CTextButton*)m_Buttons[0])->getPositionX() - 32);
		((CTextButton*)m_Buttons[0])->setPositionY(((CTextButton*)m_Buttons[0])->getPositionY() + 48);

		if (bTick)
			m_Credits[i].position.y += 1;//CTimer::getInstance().getFrameTime() * 100.0f;
	}

	// Go back to the main menu if they press escape.
	if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
	{
		m_bDeleteMe = true;
		CMenuManager::getInstance().canDisplayMouseCursor(true);
	}

	// Go back to the main menu if the credits are done.
	if (m_Credits[m_Credits.size() - 1].position.y > (CMenuManager::getInstance().getWindowHeight() >> 1))
	{
		m_bDeleteMe = true;
		CMenuManager::getInstance().canDisplayMouseCursor(true);
	}

	// draw the OK Button
	if (m_pOKButton->drawButton(pMouseCursor))
	{
		if ((int)m_pOKButton != m_nHoverIndex)
		{
			CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
			m_nHoverIndex = (int)m_pOKButton;
		}
	}
	else
		m_nHoverIndex = 1;
}


