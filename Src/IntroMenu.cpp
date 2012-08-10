/**
* @file IntroMenu.cpp
* @author Jordan Leckner
* @date Created June 5, 2006
*
* This file includes the definition of the Intro Menu class
* used for displaying an intro menu and its buttons.
*/
#include "IntroMenu.h"
#include "MenuManager.h"
#include "DirectInputWrapper.h"

/**
* CMenu::CIntroMenu
* @date Modified June 5, 2006
*/
CIntroMenu::CIntroMenu(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, int nWidthTex, int nHeightTex, CMenu* poParentMenu)
: CMenu(nPosX, nPosY, nWidth, nHeight, NULL, 0, 0, poParentMenu)
{
	m_pIntro = new CMenuButton(0, 0, CMenuManager::getInstance().getWindowWidth(), CMenuManager::getInstance().getWindowHeight(),
		szTextureBackground, NULL, 1024, 768, false);
}

CIntroMenu::~CIntroMenu()
{
	delete m_pIntro;
}

/**
* CIntroMenu::drawMenu
* @date Modified June 5, 2006
*/
void CIntroMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
{
	// draw the intro
	m_pIntro->drawButton(NULL);

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



