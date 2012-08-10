/**
* @file MenuManager.cpp
* @author Jordan Leckner
* @date Modified March 17, 2006
*
* This file includes the definition of the Menu Manager class
* used for holding and displaying of menus.
*/
#include "MenuManager.h"
#include "IntroMenu.h"
#include "OptionsMenu.h"
#include "CreditsMenu.h"
#include "OptionsMenu.h"
#include "ControlsMenu.h"
#include "MultiplayerMenu.h"
#include "HUD.h"
#include "ObjectiveHUD.h"

#include "DirectInputWrapper.h"
#include "BaseGameState.h"
#include "GameKernel.h"

/**
* CMenuManager::CMenuManager
* @date Modified March 06, 2006
*/
CMenuManager::CMenuManager()
{
	m_pMouseTexture = NULL;
	m_pMouseVertexBuffer = NULL;
	m_poCurrentMenu = NULL;
	m_poHUD = NULL;
	m_poActualHUD = NULL;
	m_poObjectiveHUD = NULL;
}

/**
* CMenuManager::~CMenuManager
* @date Modified April 6, 2006
*/
CMenuManager::~CMenuManager()
{
	for (unsigned int i = 0; i < m_Menus.size(); ++i)
	{
		// make sure it's a valid pointer
		if (m_Menus[i])
		{
			deleteMenu(m_Menus[i]);
		}
	}
	m_poHUD = NULL;
	SAFE_DELETE(m_poActualHUD);
	SAFE_DELETE(m_poObjectiveHUD);
	SAFE_RELEASE(m_pMouseTexture);
	SAFE_RELEASE(m_pMouseVertexBuffer);

	COptions::deleteInstance();
}

/**
* CMenuManager::initializeManager
* @date Modified March 10, 2006
*/
bool CMenuManager::initializeManager(const char *szMouseTexture, int nWidth, int nHeight, int nWidthTex, int nHeightTex,
									 bool bWindowed)
{
	m_poDirect3DDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	if (!m_poDirect3DDevice)
		return false;

	setWindowWidth(CRenderSystem::getInstance().isWindowed());

	// clear out quad
	memset(&m_tMouseQuad, 0, sizeof(m_tMouseQuad));
	// Load up the mouse texture
	int nPosX = 0, nPosY = 0;
	m_nMouseWidth = nWidth;
	m_nMouseHeight = nHeight;

	// Surface Description used for getting the width and height of the texture
	D3DSURFACE_DESC SurfaceDesc;

	// make sure we have a texture
	if (!szMouseTexture)
		return false;

	// Load in the texture and get the surface description for the width and height.
	D3DXCreateTextureFromFile(m_poDirect3DDevice, szMouseTexture, &m_pMouseTexture);

	// make sure it loaded correctly
	if (!m_pMouseTexture)
		return false;

	m_pMouseTexture->GetLevelDesc(0, &SurfaceDesc);

	// Set the positions, texture coordinates, and color of each vert. (6 of them)
	// The positions will be in the distance from the top left corner of the screen.
	// The u,v's will be a ratio of the actual image in the texture to the texture width and height.
	// Color is initially set to white with full alpha.
	m_tMouseQuad[0].pos.x = (float)nPosX;
	m_tMouseQuad[0].pos.y = (float)nPosY;
	m_tMouseQuad[0].u = 0;
	m_tMouseQuad[0].v = (float)nHeightTex / (float)SurfaceDesc.Height;
	m_tMouseQuad[0].color = 0xffffffff;
	m_tMouseQuad[1].pos.x = (float)nPosX + (float)nWidth;
	m_tMouseQuad[1].pos.y = (float)nPosY + (float)nHeight;
	m_tMouseQuad[1].u = (float)nWidthTex / (float)SurfaceDesc.Width;
	m_tMouseQuad[1].v = 0;
	m_tMouseQuad[1].color = 0xffffffff;
	m_tMouseQuad[2].pos.x = (float)nPosX;
	m_tMouseQuad[2].pos.y = (float)nPosY + (float)nHeight;
	m_tMouseQuad[2].u = 0;
	m_tMouseQuad[2].v = 0;
	m_tMouseQuad[2].color = 0xffffffff;
	m_tMouseQuad[3].pos.x = (float)nPosX + (float)nWidth;
	m_tMouseQuad[3].pos.y = (float)nPosY + (float)nHeight;
	m_tMouseQuad[3].u = (float)nWidthTex / (float)SurfaceDesc.Width;
	m_tMouseQuad[3].v = 0;
	m_tMouseQuad[3].color = 0xffffffff;
	m_tMouseQuad[4].pos.x = (float)nPosX;
	m_tMouseQuad[4].pos.y = (float)nPosY;
	m_tMouseQuad[4].u = 0;
	m_tMouseQuad[4].v = (float)nHeightTex / (float)SurfaceDesc.Height;
	m_tMouseQuad[4].color = 0xffffffff;
	m_tMouseQuad[5].pos.x = (float)nPosX + (float)nWidth;
	m_tMouseQuad[5].pos.y = (float)nPosY;
	m_tMouseQuad[5].u = (float)nWidthTex / (float)SurfaceDesc.Width;
	m_tMouseQuad[5].v = (float)nHeightTex / (float)SurfaceDesc.Height;
	m_tMouseQuad[5].color = 0xffffffff;

	// Create the vertex buffer that will hold the quad information
	m_poDirect3DDevice->CreateVertexBuffer(6 * sizeof(SMenuFVF), D3DUSAGE_DYNAMIC, SMenuFVF::FVF_Flags, D3DPOOL_DEFAULT, &m_pMouseVertexBuffer, NULL);

	// Lock the vertex buffer so that we can copy the array of verts into it.
	void *pVertices;
	m_pMouseVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, m_tMouseQuad, sizeof(m_tMouseQuad));
	m_pMouseVertexBuffer->Unlock();

	int nWindowCenter = (m_nWindowHeight >> 1) + 128;

	// Create the initial menus
	CMenu *poMainMenu = new CMenu(0, 0, m_nWindowWidth, m_nWindowHeight, "../Textures/Menu/MainMenu.tga", 1024, 768);
	new CSinglePlayerButton((m_nWindowWidth >> 1) - 128, nWindowCenter - 192, 256, 32, "../Textures/Menu/singleplayer.tga",
		"../Textures/Menu/singleplayerHover.tga", 256, 32, true, poMainMenu);
	new CMultiplayerButton((m_nWindowWidth >> 1) - 128, nWindowCenter - 159, 256, 32, "../Textures/Menu/multiplayer.tga", 
		"../Textures/Menu/multiplayerHover.tga", 256, 32, true, poMainMenu);
	new COptionsButton((m_nWindowWidth >> 1) - 128, nWindowCenter - 126, 256, 32, "../Textures/Menu/options.tga", 
		"../Textures/Menu/optionsHover.tga", 256, 32, true, poMainMenu);
	new CCreditsButton((m_nWindowWidth >> 1) - 128, nWindowCenter - 93, 256, 32, "../Textures/Menu/credits.tga", 
		"../Textures/Menu/creditsHover.tga", 256, 32, true, poMainMenu);
	new CHighScoreButton((m_nWindowWidth >> 1) - 128, nWindowCenter - 60, 256, 32, "../Textures/Menu/HighScore.tga", 
		"../Textures/Menu/HighScoreHover.tga", 256, 32, true, poMainMenu);
	new CQuitButton((m_nWindowWidth >> 1) - 128, nWindowCenter - 27, 256, 32, "../Textures/Menu/quit.tga", 
		"../Textures/Menu/quitHover.tga", 256, 32, true, poMainMenu);

	addMenu(poMainMenu);

	// make intro menus
	m_poIntro2 = new CIntroMenu(0, 0, m_nWindowWidth, m_nWindowHeight, "../Textures/Menu/CypherEdgeInteractive.tga", 1024, 768);
	m_poIntro2->setSecondsToShow(10.0f);
	m_poIntro2->setType(MT_INTRO2);

	m_poIntro1 = new CIntroMenu(0, 0, m_nWindowWidth, m_nWindowHeight, "../Textures/Menu/TemporalShift.tga", 1024, 768);
	m_poIntro1->setSecondsToShow(4.5f);
	m_poIntro1->setType(MT_INTRO1);

	setCurrentMenu(poMainMenu);

	m_bDisplayMouseCursor = false;

	// initialize the HUD(s)
	m_poActualHUD = new CHUDMenu;
	m_poObjectiveHUD = new CObjectiveHUDMenu;

	// Set up the options menu by initing it.
	COptions::getInstance();

	m_pEscTextButton = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "HandelGothic.ini");

	return true;
}

/**
* CMenuManager::addMenu
* @date Modified March 17, 2006
*/
void CMenuManager::addMenu(CMenu *poMenu)
{
	// Make sure it's a valid pointer
	if (!poMenu)
		return;

	// make sure its not already there
	for (unsigned int i = 0; i < m_Menus.size(); ++i)
	{
		if (m_Menus[i] == poMenu)
			return;
	}
	// Push back the menu on the vector
	m_Menus.push_back(poMenu);
}

/**
* CMenuManager::deleteMenu
* @date Modified April 30, 2006
*/
void CMenuManager::deleteMenu(CMenu *poMenu)
{
	if (!poMenu)
		return;

	if (m_poCurrentMenu == poMenu)
		m_poCurrentMenu = NULL;

	// delete all child menus
	if (poMenu->getChildMenu())
	{
		deleteMenu(poMenu->getChildMenu());
	}
	// remove all pointers to this menu
	if (poMenu->getParentMenu())
	{
		poMenu->getParentMenu()->setChildMenu(NULL);
	}

	// now delete the menu passed in
	for (vector<CMenu*>::iterator iter = m_Menus.begin(); iter != m_Menus.end(); ++iter)
	{
		if (*iter == poMenu)
		{
			if (!poMenu->isSingleton())
			{
				delete *iter;
				*iter = NULL;
			}
			m_Menus.erase(iter);
			break;
		}
	}
}

/**
* CMenuManager::updateCurrentMenu
* @date Modified March 06, 2006
*/
void CMenuManager::updateCurrentMenu()
{
	// get the current cull mode and save it to restore later
	static DWORD dwCullMode;
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// make a new pointer to hold on to the current menu
	// and then go through its children.
	CMenu *poMenuToDraw = m_poCurrentMenu;

	// Get ready to render
	getReadyToRender();

	D3DXVECTOR2 MousePos = checkMouseInput((float)m_nMouseWidth, (float)m_nMouseHeight, true);

	bool bEscapableMenu = false;

	// Draw the intro menus
	if (m_poIntro1)
	{
		m_poIntro1->drawMenu(NULL);
		if (m_poIntro1->isDeleteMe())
			SAFE_DELETE(m_poIntro1);
	}
	if (!m_poIntro1 && m_poIntro2)
	{
		m_poIntro2->drawMenu(NULL);
		if (m_poIntro2->isDeleteMe())
			SAFE_DELETE(m_poIntro2);
	}

	// only do the menus if the intros are gone.
	if (!m_poIntro1 && !m_poIntro2)
	{
		// loop through all of its children
		while (poMenuToDraw)
		{
			if (poMenuToDraw)
			{
				// make sure we're supposed to draw it.
				if (poMenuToDraw->isDisplay())
				{
					// if this is the menu up front, then process the input.
					if (poMenuToDraw->getChildMenu() == NULL)
					{
						// Draw this menu with the mouse cursor position
						poMenuToDraw->drawMenu(&MousePos);
					}
					else
					{
						// draw this menu without the mouse cursor.
						poMenuToDraw->drawMenu(NULL);
					}
				}

				if (poMenuToDraw->getType() == MT_OPTIONS || poMenuToDraw->getType() == MT_CREDITS || poMenuToDraw->getType() == MT_HIGHSCORE
					|| poMenuToDraw->getType() == MT_CONTROLS)
				{
					bEscapableMenu = true;
				}
			}

			if (poMenuToDraw->isDeleteMe())
			{
				// remove pointers
				poMenuToDraw->getParentMenu()->setChildMenu(NULL);
				// delete menu
				deleteMenu(poMenuToDraw);
				break;
			}
			else
			{
				// Go to the next child.
				poMenuToDraw = poMenuToDraw->getChildMenu();
			}
		}

		displayMouseCursor();
	}

	// end rendering
	endRender();

	// Draw the text button to show the user to hit ESC to go out of menu.
	static CString str = "Press ESC to go back";
	if (bEscapableMenu)
		m_pEscTextButton->drawString(str, 10, (float)getWindowHeight() - 30, 0xffffffff);

	// restore cull mode
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, dwCullMode);
}

/**
* CMenuManager::checkMouseInput
* @date Modified April 6, 2006
*/
D3DXVECTOR2 CMenuManager::checkMouseInput(float fCursorWidth, float fCursorHeight, bool bForMenu)
{
	// Update the mouse cursor quad position
	D3DXVECTOR2 *mousePos = CDirectInputWrapper::getInstancePtr()->getMousePositionPtr();

	// lock the mouse to the window bounds.
	if (mousePos->x < -m_nWindowWidth >> 1)
	{
		mousePos->x = (float)(-m_nWindowWidth >> 1);
	}
	if (mousePos->x > (m_nWindowWidth >> 1) - fCursorWidth)
	{
		mousePos->x = (float)(m_nWindowWidth >> 1) - fCursorWidth;
	}

	if (mousePos->y < -m_nWindowHeight >> 1)
	{
		mousePos->y = (float)(-m_nWindowHeight >> 1);
	}
	if (mousePos->y > (m_nWindowHeight >> 1) - fCursorHeight)
	{
		mousePos->y = (float)(m_nWindowHeight >> 1) - fCursorHeight;
	}

	if (bForMenu)
	{
		m_tMouseQuad[0].pos.x = mousePos->x;
		m_tMouseQuad[1].pos.x = mousePos->x + fCursorWidth;
		m_tMouseQuad[2].pos.x = mousePos->x;
		m_tMouseQuad[3].pos.x = mousePos->x + fCursorWidth;
		m_tMouseQuad[4].pos.x = mousePos->x;
		m_tMouseQuad[5].pos.x = mousePos->x + fCursorWidth;

		m_tMouseQuad[0].pos.y = -mousePos->y - fCursorHeight;
		m_tMouseQuad[1].pos.y = -mousePos->y;
		m_tMouseQuad[2].pos.y = -mousePos->y;
		m_tMouseQuad[3].pos.y = -mousePos->y;
		m_tMouseQuad[4].pos.y = -mousePos->y - fCursorHeight;
		m_tMouseQuad[5].pos.y = -mousePos->y - fCursorHeight;

		// Lock the vertex buffer so that we can copy the array of verts into it.
		void *pVertices;
		m_pMouseVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
		memcpy(pVertices, m_tMouseQuad, sizeof(m_tMouseQuad));
		m_pMouseVertexBuffer->Unlock();
	}

	return *mousePos;
}

/**
* CMenuManager::displayMouseCursor
* @date Modified April 6, 2006
*/
void CMenuManager::displayMouseCursor()
{
	if (m_bDisplayMouseCursor)
	{
		// set up our mouse texture to draw
		m_poDirect3DDevice->SetTexture(0, m_pMouseTexture);
		// Draw the mouse cursor
		m_poDirect3DDevice->SetStreamSource(0, m_pMouseVertexBuffer, 0, sizeof(SMenuFVF));
		m_poDirect3DDevice->SetFVF(SMenuFVF::FVF_Flags);
		m_poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		m_poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 3, 1);
		// Reset texture to nothing
		m_poDirect3DDevice->SetTexture(0, 0);
	}
}

/**
* CMenuManager::updateHUD
* @date Modified June 5, 2006
*/
void CMenuManager::updateHUD()
{
	// Get ready to render
	getReadyToRender();

	if (CDirectInputWrapper::getInstance().isKeyDown(DIK_F1))
	{
		m_poHUD = m_poObjectiveHUD;
		m_poActualHUD->m_poHUDBackground->setAlpha(0);
	}
	else
	{
		((CObjectiveHUDMenu*)m_poObjectiveHUD)->getBG()->setAlpha(0);
		m_poHUD = m_poActualHUD;
	}

	if (m_poHUD)
	{
		D3DXVECTOR2 *mousePos = CDirectInputWrapper::getInstance().getMousePositionPtr();
		// draw the HUD
		m_poHUD->drawMenu(mousePos);
	}

	// end rendering
	endRender();
}

/**
* CMenuManager::getReadyToRender
* @date Modified April 10, 2006
*/
void CMenuManager::getReadyToRender()
{
	// Matrices for projection, view, and world, as well as the old ones.
	D3DXMATRIX  matView, matOrtho;
	D3DXMatrixIdentity(&matView);

	// Get the transforms currently set so that we can set them back to their original state when we're done.
	m_poDirect3DDevice->GetTransform(D3DTS_PROJECTION, &m_oldProjection);
	m_poDirect3DDevice->GetTransform(D3DTS_VIEW, &m_oldView);
	m_poDirect3DDevice->GetTransform(D3DTS_WORLD, &m_oldWorld);
	// Create the orthogonal projection matrix that fits with the screen width and height.
	D3DXMatrixOrthoLH(&matOrtho, (float)m_nWindowWidth, (float)m_nWindowHeight, -100.0f, 100.0f);
	// set the new projection matrix as well as the identity for the world and view so that 
	// things get rendered right in front of the screen.
	m_poDirect3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	m_poDirect3DDevice->SetTransform(D3DTS_WORLD, &matView);
	m_poDirect3DDevice->SetTransform(D3DTS_VIEW, &matView);
	// Turn off lighting, we don't need it for the menus
	m_poDirect3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	// Turn on alpha blending so that we can use the alpha of the vertex as well as the texture.
	m_poDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	// set up the final alpha to use the texture alpha and the vertex alpha and then 
	// modulate them together to get the final alpha value.
	m_poDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_poDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_poDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// Change the blending states so that transparency works correctly.
	m_poDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_poDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

/**
* CMenuManager::endRender
* @date Modified April 6, 2006
*/
void CMenuManager::endRender()
{
	// Set back all states to what they were before.
	m_poDirect3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_poDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_poDirect3DDevice->SetTransform(D3DTS_PROJECTION, &m_oldProjection);
	m_poDirect3DDevice->SetTransform(D3DTS_WORLD, &m_oldWorld);
	m_poDirect3DDevice->SetTransform(D3DTS_VIEW, &m_oldView);
}

/**
* CMenuManager::setWindowWidth
* @date Modified April 27, 2006	
*/
void CMenuManager::setWindowWidth(bool bWindowed)
{
	RECT rClient;
	GetClientRect(CRenderSystem::getInstance().getRenderWindow(), &rClient);
	m_nWindowWidth = rClient.right - rClient.left;
	m_nWindowHeight = rClient.bottom - rClient.top;

	if (!bWindowed)
	{
		// array of possible widths and heights to use
		const int nWidthsAvail[]  = { 640, 800, 1024, 1280, 1440, 1600 };
		const int nHeightsAvail[] = { 480, 600, 768, 1024, 900, 1200 };

		// make the width and height a multiple of 2
		int n = 0, i = 0;
		while (true)
		{
			if ((m_nWindowWidth + n) > nWidthsAvail[i])
				++i;
			
			if ((m_nWindowWidth + n) - nWidthsAvail[i] == 0)
				break;
			++n;
		}
		m_nWindowWidth += n;
		n = i = 0;

		while (true)
		{
			if ((m_nWindowHeight + n) > nHeightsAvail[i])
				++i;

			if ((m_nWindowHeight + n) - nHeightsAvail[i] == 0)
				break;
			++n;
		}
		m_nWindowHeight += n;
	}
}


