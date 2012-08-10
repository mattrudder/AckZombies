/**
* @file MenuButton.cpp
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the Menu Button class's definition
* used for displaying a menu's Buttons and processing
* its behavior when pressed.
*/
#include "MenuButton.h"
#include "Menu.h"
#include "MenuManager.h"
#include "VendingMachineButtons.h"
#include "DirectInputWrapper.h"

/**
* CMenuButton::~CMenuButton
* @date Modified March 06, 2006
*/
CMenuButton::~CMenuButton()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pTextureBackground);
	SAFE_RELEASE(m_pTextureBackgroundHover);
	SAFE_RELEASE(m_pLine);
	SAFE_RELEASE(m_pFont);
	if (m_szText)
	{
		delete [] m_szText;
		m_szText = NULL;
	}
}

/**
* CMenuButton::CMenuButton
* @date Modified April 10, 2006
*/
CMenuButton::CMenuButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, const char *szTextureBackgroundHover,
						 int nWidthTex, 
						 int nHeightTex, bool bCheckInput, CMenu* poParentMenu, char *szFontFace, int nFontWidth,
						 int nFontHeight, char *szText)
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

	m_pTextureBackground = m_pTextureBackgroundHover = m_pFinalTexture = NULL;

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

	m_pFinalTexture = m_pTextureBackground;

	// make sure we have a texture
	if (szTextureBackgroundHover)
	{
		// Load in the texture and get the surface description for the width and height.
		D3DXCreateTextureFromFile(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(), 
			szTextureBackgroundHover, &m_pTextureBackgroundHover);

		// make sure it loaded correctly
		if (!m_pTextureBackgroundHover)
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
	// Color is intially set to white with full alpha.
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

	// fill out the bounding box
	m_rBoundingBox.left = (LONG)tMenuQuad[0].pos.x;
	m_rBoundingBox.bottom = (LONG)tMenuQuad[0].pos.y;
	m_rBoundingBox.right = (LONG)tMenuQuad[1].pos.x;
	m_rBoundingBox.top = (LONG)tMenuQuad[1].pos.y;

	// Create the vertex buffer that will hold the quad information
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->CreateVertexBuffer(6 * sizeof(SMenuFVF), 0, 
		SMenuFVF::FVF_Flags, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

	// Lock the vertex buffer so that we can copy the array of verts into it.
	void *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, tMenuQuad, sizeof(tMenuQuad));
	m_pVertexBuffer->Unlock();

	// If we have a parent menu set, set its child to us. (this)
	m_poParentMenu = poParentMenu;
	if (poParentMenu)
	{
		poParentMenu->getButtons().push_back(this);
	}

	// Create the Line object
	D3DXCreateLine(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(), &m_pLine);

	m_pFont = NULL;
	m_szText = NULL;
	if (szFontFace)
	{
		// Create the font
		D3DXCreateFont(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(),
			nFontHeight,
			nFontWidth,
			400,
			0,
			false,
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			PROOF_QUALITY,
			DEFAULT_PITCH,
			szFontFace,
			&m_pFont);

		if (szText)
		{
			m_szText = new char[128];
			strcpy(m_szText, szText);
		}
	}

	m_nPositionX = nPosX;
	m_nPositionY = nPosY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_bLeftMouseDownInBox = false;
	m_bLeftMouseButtonClicked = false;
	m_dwAlpha = 0;
	m_bCheckInput = bCheckInput;
	// no type yet
	m_unType = -1;
	m_dwFormat = DT_CENTER;
	m_dwColor = 0xffffffff;
	m_bEnter = false;
}

/**
* CMenuButton::drawButton
* @date Modified March 13, 2006
*/
bool CMenuButton::drawButton(D3DXVECTOR2 *pMouseCursor)
{
	// alpha fading in
	if (m_dwAlpha <= 255)
	{
		static SMenuFVF tMenuQuad[6];
		void *pVertices;
		m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		memcpy(tMenuQuad, pVertices, sizeof(tMenuQuad));
		for (int i = 0; i < 6; ++i)
		{
			tMenuQuad[i].color &= ~ 0xff000000;
			tMenuQuad[i].color |= (m_dwAlpha << 24);
		}
		memcpy(pVertices, tMenuQuad, sizeof(tMenuQuad));
		m_pVertexBuffer->Unlock();

		if (m_dwAlpha == 248)
			m_dwAlpha += 7;
		else
			m_dwAlpha += 8;
	}

	// Direct3d Device
	static IDirect3DDevice9 *poDirect3DDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	// Set the button's texture to draw
	poDirect3DDevice->SetTexture(0, m_pFinalTexture);
	// Draw the actual quad
	poDirect3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SMenuFVF));
	poDirect3DDevice->SetFVF(SMenuFVF::FVF_Flags);
	poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 3, 1);
	// Reset texture to nothing
	poDirect3DDevice->SetTexture(0, 0);

	int nWindowWidth = CMenuManager::getInstance().getWindowWidth();
	int nWindowHeight = CMenuManager::getInstance().getWindowHeight();

	if (m_pFont && m_szText)
	{
		RECT rBox;
		rBox.left = m_nPositionX + (nWindowWidth >> 1); 
		rBox.right = rBox.left + m_nWidth;
		rBox.top = (nWindowHeight >> 1) - m_nPositionY - m_nHeight;
		rBox.bottom = rBox.top + m_nHeight;
		m_pFont->DrawTextA(NULL, m_szText, -1, &rBox, m_dwFormat, m_dwColor);
	}

	m_pLine->Begin();
	// Check to see if the mouse is hovering over the button
	if (m_bCheckInput && checkInput(pMouseCursor))
	{
		// Only draw the lines if the button is not a regular menu button.
		if (m_unType >= VMB_SHOTGUN && m_unType <= VMB_MAX)
		{
			// Draw the outline of the button
			RECT rBox = m_rBoundingBox;
			rBox.left += nWindowWidth >> 1;
			rBox.right += nWindowWidth >> 1;	
			rBox.top = (nWindowHeight >> 1) - rBox.top;
			rBox.bottom = (nWindowHeight >> 1) - rBox.bottom;
			D3DXVECTOR2 vLinePoints[5]; 
			vLinePoints[0] = D3DXVECTOR2((float)rBox.left, (float)rBox.top);
			vLinePoints[1] = D3DXVECTOR2((float)rBox.left, (float)rBox.bottom);
			vLinePoints[2] = D3DXVECTOR2((float)rBox.right, (float)rBox.bottom);
			vLinePoints[3] = D3DXVECTOR2((float)rBox.right, (float)rBox.top);
			vLinePoints[4] = D3DXVECTOR2((float)rBox.left, (float)rBox.top);
			// Finally draw the line
			m_pLine->Draw(vLinePoints, 5, 0xffffffff);
		}
		else if (m_unType == BT_TEXT)
		{
			setTextColor(D3DCOLOR_XRGB(255, 0, 0));
		}
		// End must be called if begin is called.
		m_pLine->End();
		if (m_pTextureBackgroundHover)
			m_pFinalTexture = m_pTextureBackgroundHover;
		return true;
	}
	else
	{
		if (m_unType == BT_TEXT)
		{
			setTextColor(D3DCOLOR_XRGB(255, 255, 255));
		}
		m_pFinalTexture = m_pTextureBackground;
	}

	m_pLine->End();

	return false;
}

/**
* CMenuButton::checkInput
* @date Modified May 9, 2006
*/
bool CMenuButton::checkInput(D3DXVECTOR2 *pMouseCursor)
{
	// Make sure we have an actual mouse cursor
	if (!pMouseCursor)
		return false;

	// Whether or not the mouse is hovering over the button
	bool bMouseHover = false;

	CDirectInputWrapper *pDI = CDirectInputWrapper::getInstancePtr();

	// Change format from windows to directx coordinates
	pMouseCursor->y = -pMouseCursor->y;

	if (pMouseCursor->x >= m_rBoundingBox.left && pMouseCursor->x <= m_rBoundingBox.right)   
	{
		if (pMouseCursor->y <= m_rBoundingBox.top && pMouseCursor->y >= m_rBoundingBox.bottom)
		{
			// Mouse is hovering over the button
			bMouseHover = true;

			// If the left mouse button is down
			if (pDI->isLeftMouseButtonDown())
			{
				m_bLeftMouseDownInBox = true;
			}
			else
			{
				// if the button was once down, it has now been clicked.
				if (m_bLeftMouseDownInBox)
				{
					m_bLeftMouseButtonClicked = true;
				}
				// Button is no longer pressed
				m_bLeftMouseDownInBox = false;
			}
		}
		else
		{
			bMouseHover = false; m_bLeftMouseDownInBox = false; m_bLeftMouseButtonClicked = false;
		}
	}
	else
	{
		bMouseHover = false; m_bLeftMouseDownInBox = false; m_bLeftMouseButtonClicked = false;
	}

	// we got a click so do this button's action.
	if ((m_bLeftMouseButtonClicked && bMouseHover) || m_bEnter)
	{
		m_bLeftMouseButtonClicked = false;
		m_bEnter = false;
		this->doAction();
	}

	// Change it back
	pMouseCursor->y = -pMouseCursor->y;

	// Return whether or not the mouse was hovering over this button
	return bMouseHover;
}

/**
* CMenuButton::doAction
* @date Modified April 3, 2006
*/
void CMenuButton::doAction()
{
}

/**
* CMenuButton::copyVertexData
* @date Modified April 3, 2006
*/
void CMenuButton::addToVertexData(int nX, int nY, bool bWidth)
{
	SMenuFVF tMenuQuad[6];
	void *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(tMenuQuad, pVertices, sizeof(tMenuQuad));

	if (!bWidth)
	{
		for (int i = 0; i < 6; ++i)
		{
			tMenuQuad[i].pos.x += (float)nX;
			tMenuQuad[i].pos.y += (float)nY;
		}
	}
	else
	{
		tMenuQuad[1].pos.x += nX;
		tMenuQuad[3].pos.x += nX;
		tMenuQuad[5].pos.x += nX;
		
		tMenuQuad[1].pos.y += nY;
		tMenuQuad[2].pos.y += nY;
		tMenuQuad[3].pos.y += nY;
	}

	memcpy(pVertices, tMenuQuad, sizeof(tMenuQuad));
	m_pVertexBuffer->Unlock();

	if (!bWidth)
	{
		// update the member position
		m_nPositionX += nX;
		m_nPositionY += nY;
	}
}

/**
* CMenuButton::copyVertexData
* @date Modified May 26, 2006
*/
void CMenuButton::addToVertexDataUV(float fU, float fV, bool bWidth)
{
	SMenuFVF tMenuQuad[6];
	void *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(tMenuQuad, pVertices, sizeof(tMenuQuad));

	if (!bWidth)
	{
		for (int i = 0; i < 6; ++i)
		{
			tMenuQuad[i].u += fU;
			tMenuQuad[i].v += fV;
		}
	}
	else
	{
		tMenuQuad[1].u += fU;
		tMenuQuad[3].u += fU;
		tMenuQuad[5].u += fU;
		
		tMenuQuad[1].v += fV;
		tMenuQuad[2].v += fV;
		tMenuQuad[3].v += fV;
	}

	memcpy(pVertices, tMenuQuad, sizeof(tMenuQuad));
	m_pVertexBuffer->Unlock();
}

/**
* CMenuButton::setVertexData
* @date Modified May 9, 2006
*/
void CMenuButton::setVertexData(D3DXVECTOR2 *pPos)
{
	SMenuFVF tMenuQuad[6];
	void *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(tMenuQuad, pVertices, sizeof(tMenuQuad));

	D3DXVECTOR3 vec3Mouse;
	vec3Mouse.x = pPos->x;
	vec3Mouse.y = pPos->y;
	vec3Mouse.z = 0;

	tMenuQuad[0].pos = vec3Mouse;
	tMenuQuad[1].pos = vec3Mouse;
	tMenuQuad[2].pos = vec3Mouse;
	tMenuQuad[3].pos = vec3Mouse;
	tMenuQuad[4].pos = vec3Mouse;
	tMenuQuad[5].pos = vec3Mouse;
	tMenuQuad[1].pos.x += m_nWidth;
	tMenuQuad[3].pos.x += m_nWidth;
	tMenuQuad[5].pos.x += m_nWidth;
	tMenuQuad[1].pos.y += m_nHeight;
	tMenuQuad[2].pos.y += m_nHeight;
	tMenuQuad[3].pos.y += m_nHeight;

	memcpy(pVertices, tMenuQuad, sizeof(tMenuQuad));
	m_pVertexBuffer->Unlock();
}

/**
* CMenuButton::getPosition
* @date Modified April 3, 2006
*/
D3DXVECTOR2 CMenuButton::getPosition()
{
	return D3DXVECTOR2((float)(m_nPositionX + (CMenuManager::getInstance().getWindowWidth() >> 1)), 
		(float)(m_nPositionY + (CMenuManager::getInstance().getWindowHeight() >> 1)));
}

/**
* CMenuButton::getCenterPosition
* @date Modified June 17, 2006
*/
D3DXVECTOR2 CMenuButton::getCenterPosition()
{
	float x, y;
	x = (float)(((m_rBoundingBox.right - m_rBoundingBox.left) >> 1) + m_rBoundingBox.left);
	y = (float)(((m_rBoundingBox.top - m_rBoundingBox.bottom) >> 1) - m_rBoundingBox.top);
	return D3DXVECTOR2(x, y);
}



