///**
//* @file MultiplayerMenu.cpp
//* @author Jordan Leckner
//* @date Created April 3, 2006
//*
//* This file includes the definition of the Multiplayer Menu class
//* used for displaying a Multiplayer menu and its buttons.
//*/
//#include "MultiplayerMenu.h"
//#include "DirectInputWrapper.h"
//
///**
//* CCreditsMenu::drawMenu
//* @date Modified April 10, 2006
//*/
//void CMultiplayerMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
//{
//	// Direct3d Device
//	static IDirect3DDevice9 *poDirect3DDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
//
//	poDirect3DDevice->SetTexture(0, m_pTextureBackground);
//	// Finally, draw the triangles of the quad.
//	poDirect3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SMenuFVF));
//	poDirect3DDevice->SetFVF(SMenuFVF::FVF_Flags);
//	poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
//	poDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 3, 1);
//	// Clear out the texture
//	poDirect3DDevice->SetTexture(0, 0);
//
//	// only draw the buttons if the child menu's texture is valid.
//	// this way we can save memory on the background.
//	if (!m_poChildMenu || (m_poChildMenu && m_poChildMenu->m_pTextureBackground))
//	{
//		// Go through all of the buttons for this menu and draw them in front
//		// of this menu.
//		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
//		{
//			// draw the first text and ok buttons
//			if (i < 4)
//			{
//				// Draw this button and check for input
//				if (m_Buttons[i]->drawButton(pMouseCursor))
//				{
//				}
//			}
//		}
//		// draw the 360 controllers if they're plugged in
//		if (CDirectInputWrapper::getInstance().m_oXInputWrapper.isConnected(0))
//			m_Buttons[4]->drawButton(NULL);
//		if (CDirectInputWrapper::getInstance().m_oXInputWrapper.isConnected(1))
//			m_Buttons[5]->drawButton(NULL);
//		// draw the joysticks if they're plugged in
//        if (CDirectInputWrapper::getInstance().m_pDIDJoysticks[0])
//			m_Buttons[6]->drawButton(NULL);
//		if (CDirectInputWrapper::getInstance().m_pDIDJoysticks[1])
//			m_Buttons[7]->drawButton(NULL);
//	}
//
//	bool bXBOX360InSpot = false, bKeyboardInSpot = false;
//	// move the avatars where they should be.
//	if (m_Buttons[3]->getPosition().x > m_nKeyboardPos)
//	{
//		if (m_Buttons[3]->getPosition().x - m_nKeyboardPos >= 32)
//			m_Buttons[3]->addToVertexData(-32);
//		bKeyboardInSpot = false;
//	}
//	else if (m_Buttons[3]->getPosition().x < m_nKeyboardPos)
//	{
//		if (m_nKeyboardPos - m_Buttons[3]->getPosition().x >= 32)
//			m_Buttons[3]->addToVertexData(32);
//		bKeyboardInSpot = false;
//	}
//	else
//	{
//		bKeyboardInSpot = true;
//	}
//	if (m_Buttons[4]->getPosition().x > m_nXBOX360Pos)
//	{
//		if (m_Buttons[4]->getPosition().x - m_nXBOX360Pos >= 32)
//			m_Buttons[4]->addToVertexData(-32);
//		bXBOX360InSpot = false;
//	}
//	else if (m_Buttons[4]->getPosition().x < m_nXBOX360Pos)
//	{
//		if (m_nXBOX360Pos - m_Buttons[4]->getPosition().x >= 32)
//			m_Buttons[4]->addToVertexData(32);
//		bXBOX360InSpot = false;
//	}
//	else
//	{
//		bXBOX360InSpot = true;
//	}
//
//	int nWindowHalf = CMenuManager::getInstance().getWindowWidth() >> 1;
//
//	int nPower2 = 1;
//	while (nPower2 < nWindowHalf)
//		nPower2 *= 2;
//	nWindowHalf = nPower2;
//
//	// catch input to move the keyboard/joystick avatars.
//	if (bKeyboardInSpot && CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_A, MENU_SYSTEM))
//	{
//		if (m_nKeyboardPos > nWindowHalf - 64)
//			m_nKeyboardPos = nWindowHalf - 64;
//		else if (m_nKeyboardPos == nWindowHalf - 64)
//			m_nKeyboardPos = m_nLeftPos;
//	}
//	if (bKeyboardInSpot && CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_D, MENU_SYSTEM))
//	{
//		if (m_nKeyboardPos < nWindowHalf - 64)
//			m_nKeyboardPos = nWindowHalf - 64;
//		else if (m_nKeyboardPos == nWindowHalf - 64)
//			m_nKeyboardPos = m_nRightPos;
//	}
//	if (bXBOX360InSpot && CDirectInputWrapper::getInstance().m_oXInputWrapper.getThumbstickDirection(0, 
//		XIA_LEFT_X_AXIS) < -0.75f)
//	{
//		if (m_nXBOX360Pos > nWindowHalf - 64)
//			m_nXBOX360Pos = nWindowHalf - 64;
//		else if (m_nXBOX360Pos == nWindowHalf - 64)
//			m_nXBOX360Pos = m_nLeftPos;
//	}
//	if (bXBOX360InSpot && CDirectInputWrapper::getInstance().m_oXInputWrapper.getThumbstickDirection(0, 
//		XIA_LEFT_X_AXIS) > 0.75f)
//	{
//		if (m_nXBOX360Pos < nWindowHalf - 64)
//			m_nXBOX360Pos = nWindowHalf - 64;
//		else if (m_nXBOX360Pos == nWindowHalf - 64)
//			m_nXBOX360Pos = m_nRightPos;
//	}
//
//	// Go back to the main menu if they press escape.
//	if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
//	{
//		m_bDeleteMe = true;
//		// reset the alpha
//		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
//			if (m_Buttons[i]->m_unType != BT_TEXT)
//				m_Buttons[i]->m_dwAlpha = 0;
//
//		// reset the parents alpha
//		for (unsigned int i = 0; i < m_poParentMenu->m_Buttons.size(); ++i)
//			if (m_poParentMenu->m_Buttons[i]->m_unType != BT_TEXT)
//				m_poParentMenu->m_Buttons[i]->m_dwAlpha = 0;
//	}
//}
//
//
