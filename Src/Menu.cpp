/**
* @file Menu.cpp
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the definition of the Menu class
* used for displaying a menu and its buttons.
*/
#include "Menu.h"
#include "MenuManager.h"
#include "DirectInputWrapper.h"
#include "SoundManager.h"
#include "MainButtons.h"

/**
* CMenu::CMenu
* @date Modified April 10, 2006
*/
CMenu::CMenu(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, int nWidthTex, int nHeightTex, CMenu* poParentMenu)
{
	// convert the positions into coordinates so that 0,0 is at the top left of the screen.
	// If you don't do this, 0,0 will be in the center.
	nPosX -= CMenuManager::getInstance().getWindowWidth() >> 1;
	nPosY = (CMenuManager::getInstance().getWindowHeight() >> 1) - nHeight - nPosY;

	// Create the quad and zero it out
	SMenuFVF tMenuQuad[6];
	memset(&tMenuQuad, 0, sizeof(tMenuQuad));

	// Surface Description used for getting the width and height of the texture
	D3DSURFACE_DESC SurfaceDesc;
	m_pTextureBackground = NULL;

	// make sure we have a texture
	if (szTextureBackground)
	{
		// Load in the texture and get the surface description for the width and height.
		D3DXCreateTextureFromFile(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(), 
			szTextureBackground, &m_pTextureBackground);

		// make sure it loaded correctly
		if (!m_pTextureBackground)
			return;
	}

	memset(&SurfaceDesc, 0, sizeof(SurfaceDesc));
	if (szTextureBackground)
	{
		m_pTextureBackground->GetLevelDesc(0, &SurfaceDesc);
	}
	else
	{
		// prevent divide by zero
		SurfaceDesc.Height = SurfaceDesc.Width = 1;
	}

	// Set the positions, texture coordinates, and color of each vert. (6 of them)
	// The positions will be in the distance from the top left corner of the screen.
	// The u,v's will be a ratio of the actual image in the texture to the texture width and height.
	// Color is initially set to white with zero alpha.
	tMenuQuad[0].pos.x = (float)nPosX;
	tMenuQuad[0].pos.y = (float)nPosY;
	tMenuQuad[0].u = 0;
	tMenuQuad[0].v = (float)nHeightTex / (float)SurfaceDesc.Height;
	tMenuQuad[0].color = 0x00ffffff;
	tMenuQuad[1].pos.x = (float)nPosX + (float)nWidth;
	tMenuQuad[1].pos.y = (float)nPosY + (float)nHeight;
	tMenuQuad[1].u = (float)nWidthTex / (float)SurfaceDesc.Width;
	tMenuQuad[1].v = 0;
	tMenuQuad[1].color = 0x00ffffff;
	tMenuQuad[2].pos.x = (float)nPosX;
	tMenuQuad[2].pos.y = (float)nPosY + (float)nHeight;
	tMenuQuad[2].u = 0;
	tMenuQuad[2].v = 0;
	tMenuQuad[2].color = 0x00ffffff;
	tMenuQuad[3].pos.x = (float)nPosX + (float)nWidth;
	tMenuQuad[3].pos.y = (float)nPosY + (float)nHeight;
	tMenuQuad[3].u = (float)nWidthTex / (float)SurfaceDesc.Width;
	tMenuQuad[3].v = 0;
	tMenuQuad[3].color = 0x00ffffff;
	tMenuQuad[4].pos.x = (float)nPosX;
	tMenuQuad[4].pos.y = (float)nPosY;
	tMenuQuad[4].u = 0;
	tMenuQuad[4].v = (float)nHeightTex / (float)SurfaceDesc.Height;
	tMenuQuad[4].color = 0x00ffffff;
	tMenuQuad[5].pos.x = (float)nPosX + (float)nWidth;
	tMenuQuad[5].pos.y = (float)nPosY;
	tMenuQuad[5].u = (float)nWidthTex / (float)SurfaceDesc.Width;
	tMenuQuad[5].v = (float)nHeightTex / (float)SurfaceDesc.Height;
	tMenuQuad[5].color = 0x00ffffff;

	// Put alpha in if we have a texture background
	if (szTextureBackground)
	{
		for (int i = 0; i < 6; ++i)
		{
			tMenuQuad[i].color |= 0xff000000;
		}
	}

	// Create the vertex buffer that will hold the quad information
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->CreateVertexBuffer(6 * sizeof(SMenuFVF), 
		D3DUSAGE_DYNAMIC, SMenuFVF::FVF_Flags, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

	// Lock the vertex buffer so that we can copy the array of verts into it.
	void *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, tMenuQuad, sizeof(tMenuQuad));
	m_pVertexBuffer->Unlock();

	// child windows do not get set until another one is made.
	m_poChildMenu = 0;
	// If we have a parent menu set, set its child to us. (this)
	m_poParentMenu = poParentMenu;
	if (poParentMenu)
	{
		poParentMenu->m_poChildMenu = this;
	}
	m_bDisplay = true;

	D3DXCreateBox(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(), 50, 50, 50, &m_pBoxSelection, 0);

	m_fBoxRotationY = 0;
	m_bDeleteMe = false;
	// not a real menu yet
	m_nType = -1;
	//m_Timer.setInterval(-1);
	m_fTimer = 0;
	m_bSingleton = false;
	m_nHoverIndex = -1;
	m_nSelected = 0;
	m_fThumbStickTimer = CTimer::getInstance().getTime();
}

/**
* CMenu::~CMenu
* @date Modified March 08, 2006
*/
CMenu::~CMenu()
{
	// delete all the allocated buttons
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		if (m_Buttons[i])
		{
			delete m_Buttons[i];
			m_Buttons[i] = 0;
		}
	}

	if (m_pBoxSelection)
	{
		SAFE_RELEASE(m_pBoxSelection);
		m_pBoxSelection = 0;
	}

	SAFE_RELEASE(m_pTextureBackground);
	SAFE_RELEASE(m_pVertexBuffer);
}

/**
* CMenu::drawMenu
* @date Modified March 06, 2006
*/
void CMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
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
	if (!m_poChildMenu || (m_poChildMenu && m_poChildMenu->m_pTextureBackground))
	{
		catchInput();
		// Go through all of the buttons for this menu and draw them in front
		// of this menu.
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			// Draw this button and check for input
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

    static bool bIntro1First = false;
	// If our alloted time is up, go to the next menu.
	if (m_fTimer && CTimer::getInstance().getTime() - m_fTimer >= m_fTimeToShowMenu)
	{
		m_bDeleteMe = true;
		// if we're done with the intro menus, then we can display the mouse cursor now.
		if (m_nType == MT_INTRO2)
			CMenuManager::getInstance().canDisplayMouseCursor(true);
		else if (m_nType == MT_INTRO1)
			bIntro1First = true;
	}
	// If they hit escape, skip the intro screens.
	if (m_nType == MT_INTRO1)
	{
		if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
		{
			m_bDeleteMe = true;
			bIntro1First = true;
		}
	}
	else if (bIntro1First && m_nType == MT_INTRO2)
	{
		if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
		{
			m_bDeleteMe = true;
			CMenuManager::getInstance().canDisplayMouseCursor(true);
		}
	}
}

/**
* CMenu::catchInput
* @date Modified June 18, 2006
*/
void CMenu::catchInput()
{
	// get the number of clickable buttons
	int nButtonSize = 0;
	CMenuButton* pButton;
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		pButton = m_Buttons[i];
		if (pButton->getType() != BT_TEXT && pButton->getType() != 8675309)
			nButtonSize++;
	}

	if (!nButtonSize)
		return;

	float fVal;
	// xbox360 controller(s)
	if (fVal = CDirectInputWrapper::getInstance().getXInputDevice()
		.getThumbstickDirection(0, XIA_LEFT_Y_AXIS))
	{
		if (CTimer::getInstance().getTime() - m_fThumbStickTimer >= THUMBSTICKTHRESHOLD)
		{
			if (fVal > 0.0f)
			{
				m_nSelected--;
				if (m_nSelected < 0)
					m_nSelected = (int)m_Buttons.size() - 1;

				pButton = m_Buttons[m_nSelected];

				while (pButton->getType() == BT_TEXT || pButton->getType() == 8675309)
				{
					if (pButton->getType() == BT_TEXT && ((CTextButton*)pButton)->getSecondaryType() != -1)
						break;
					pButton = m_Buttons[--m_nSelected];
					if (m_nSelected < 0)
					{
						m_nSelected = (int)m_Buttons.size() - 1;
						pButton = m_Buttons[m_nSelected];
					}
				}

				D3DXVECTOR2 cursorPos = m_Buttons[m_nSelected]->getCenterPosition();
				CDirectInputWrapper::getInstance().setMousePosition(cursorPos);
			}
			else if (fVal < 0.0f)
			{
				m_nSelected++;
				if (m_nSelected > (int)m_Buttons.size() - 1)
					m_nSelected = 0;

				pButton = m_Buttons[m_nSelected];

				while (pButton->getType() == BT_TEXT || pButton->getType() == 8675309)
				{
					if (pButton->getType() == BT_TEXT && ((CTextButton*)pButton)->getSecondaryType() != -1)
						break;
					pButton = m_Buttons[++m_nSelected];

					if (m_nSelected > (int)m_Buttons.size() - 1)
					{
						m_nSelected = 0;
						pButton = m_Buttons[m_nSelected];
					}
				}

				D3DXVECTOR2 cursorPos = m_Buttons[m_nSelected]->getCenterPosition();
				CDirectInputWrapper::getInstance().setMousePosition(cursorPos);
			}

			m_fThumbStickTimer = CTimer::getInstance().getTime();
		}
	}
	if (CDirectInputWrapper::getInstance().getXInputDevice().isButtonDownBuffered(0, XINPUT_GAMEPAD_A))
	{
		m_Buttons[m_nSelected]->enter();
	}
	if (fVal = CDirectInputWrapper::getInstance().getXInputDevice()
		.getThumbstickDirection(1, XIA_LEFT_Y_AXIS))
	{
		if (CTimer::getInstance().getTime() - m_fThumbStickTimer >= THUMBSTICKTHRESHOLD)
		{
			if (fVal > 0.0f)
			{
				m_nSelected--;
				if (m_nSelected < 0)
					m_nSelected = (int)m_Buttons.size() - 1;

				pButton = m_Buttons[m_nSelected];

				while (pButton->getType() == BT_TEXT || pButton->getType() == 8675309)
				{
					if (pButton->getType() == BT_TEXT && ((CTextButton*)pButton)->getSecondaryType() != -1)
						break;
					pButton = m_Buttons[--m_nSelected];
					if (m_nSelected < 0)
					{
						m_nSelected = (int)m_Buttons.size() - 1;
						pButton = m_Buttons[m_nSelected];
					}
				}

				D3DXVECTOR2 cursorPos = m_Buttons[m_nSelected]->getCenterPosition();
				CDirectInputWrapper::getInstance().setMousePosition(cursorPos);
			}
			else
			{
				m_nSelected++;
				if (m_nSelected > (int)m_Buttons.size() - 1)
					m_nSelected = 0;

				pButton = m_Buttons[m_nSelected];

				while (pButton->getType() == BT_TEXT || pButton->getType() == 8675309)
				{
					if (pButton->getType() == BT_TEXT && ((CTextButton*)pButton)->getSecondaryType() != -1)
						break;
					pButton = m_Buttons[++m_nSelected];

					if (m_nSelected > (int)m_Buttons.size() - 1)
					{
						m_nSelected = 0;
						pButton = m_Buttons[m_nSelected];
					}
				}

				D3DXVECTOR2 cursorPos = m_Buttons[m_nSelected]->getCenterPosition();
				CDirectInputWrapper::getInstance().setMousePosition(cursorPos);
			}
		}
	}
	if (CDirectInputWrapper::getInstance().getXInputDevice().isButtonDownBuffered(1, XINPUT_GAMEPAD_A))
	{
		m_Buttons[m_nSelected]->enter();
	}

}

