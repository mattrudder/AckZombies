/**
* @file PauseState.h
* @author Jordan Leckner
* @date Created April 24, 2006
*
* This file includes the Menu game state
*/
#ifndef _PAUSESTATE_H_
#define _PAUSESTATE_H_

#include "Gamestate.h"
#include "MenuManager.h"
#include "TestState.h"
#include "OptionsMenu.h"
#include "HUD.h"

class CPauseState : public CGameState<CPauseState>
{
protected:

	//! The pause menu
	CMenu *m_poPauseMenu;

	//! Options Menu
	COptionsMenu *m_poOptionsMenu;

public:

	CPauseState() : m_poPauseMenu(NULL), m_poOptionsMenu(NULL)
	{
	}

	void setOptionsMenu(COptionsMenu *pOptionsMenu)
	{
		m_poOptionsMenu = pOptionsMenu;
	}

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created May 16, 2006
	*/
	virtual void onActivate(void)
	{
		static bool bFirstTime = true;
		if(bFirstTime)
		{
			CMenuManager *pMM = CMenuManager::getInstancePtr();
			m_poPauseMenu = new CMenu(0, 0, pMM->getWindowWidth(), pMM->getWindowHeight(), 
				"../Textures/Menu/Pause.tga", 1024, 768);
			m_poPauseMenu->setType(MT_PAUSE);

			CMenuButton *pButton;
			int nWindowWidth = pMM->getWindowWidth();
			int nWindowHeight = pMM->getWindowHeight();
			pButton = new CBackToGameButton((nWindowWidth >> 1) - 128, (nWindowHeight >> 1) - 64, 
				256, 32, "../Textures/Menu/backtogame.tga", "../Textures/Menu/backtogameHover.tga", 256, 32, true, m_poPauseMenu);

			pButton = new CMainMenuButton((nWindowWidth >> 1) - 128, (nWindowHeight >> 1), 
				256, 32, "../Textures/Menu/mainmenubutton.tga", "../Textures/Menu/mainmenubuttonHover.tga", 256, 32, true, m_poPauseMenu);

			pButton = new COptionsButton((nWindowWidth >> 1) - 128, (nWindowHeight >> 1) + 64, 
				256, 32, "../Textures/Menu/options.tga", "../Textures/Menu/optionsHover.tga", 256, 32, true, m_poPauseMenu);

			pButton = new CQuitButton((nWindowWidth >> 1) - 128, (nWindowHeight >> 1) + 128, 
				256, 32, "../Textures/Menu/quit.tga", "../Textures/Menu/quitHover.tga", 256, 32, true, m_poPauseMenu);
			
			bFirstTime = false;
		}
	}

	/**
	* Called by the kernel when the active state is switched away from this 
	* state. This allows the state to perform any cleanup needed from its 
	* onActivate call.
	*
	* @date Created May 16, 2006
	*/
	virtual void onDeactivate(void)
	{
	}

	/**
	* Called each frame within the kernel's tick function, while this is the 
	* active state.
	*
	* @date Created May 16, 2006
	*/
	virtual void tick(void)
	{
		if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
		{
			CMenuManager::getInstance().getHUD()->m_poHUDBackground->setAlpha(0);
			CMenu *pMenu = m_poPauseMenu->getChildMenu();

			if (pMenu)
				CMenuManager::getInstance().deleteMenu(pMenu);

			changeState(CTestState::getInstancePtr());
		}
	}

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created May 16, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice)
	{
		// draw the level geometry
		CTestState::getInstance().m_pMesh2->draw();
	}

	/**
	* Called each frame within the render system, just after world/actor 
	* geometry is rendered but before the scene is presented while this 
	* is the active state.
	*
	* @date Created May 16, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void postRender(CRenderDevice& oRenderDevice)
	{
		CMenuManager *pMM = CMenuManager::getInstancePtr();
		D3DXVECTOR2 mousePos = pMM->checkMouseInput((float)pMM->getMouseWidth(), (float)pMM->getMouseHeight(), true);
		// get the current cull mode and save it to restore later
		DWORD dwCullMode;
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pMM->getReadyToRender();

		CMenu *pMenu = m_poPauseMenu;

		while (pMenu)
		{
			if (pMenu)
				pMenu->drawMenu(&mousePos);

			if (pMenu->isDeleteMe())
			{
				pMenu->getParentMenu()->setChildMenu(NULL);
				pMM->deleteMenu(pMenu);
				break;
			}
			else
				pMenu = pMenu->getChildMenu();
		}

		pMM->canDisplayMouseCursor(true);
		pMM->displayMouseCursor();
		pMM->endRender();
		// restore cull mode
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, dwCullMode);
	}
};

#endif