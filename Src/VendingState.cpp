/**
* @file VendingState.cpp
* @author Jordan Leckner
* @date Created May 18, 2006
*
* the Vending state
*/

#include "VendingState.h"
#include "DirectInputWrapper.h"
#include "TestState.h"

CVendingState::~CVendingState()
{
	SAFE_DELETE(m_poVendingMenu);
	SAFE_DELETE(m_poPlayerMoneyText);
}

/**
* CVendingState::onActivate
* @date Modified May 18, 2006
*/
void CVendingState::onActivate(void)
{
	++m_unActivateTimes;
	static bool bFirstTime = true;
	if(bFirstTime)
	{
		CMenuManager *pMM = CMenuManager::getInstancePtr();
		m_poVendingMenu = new CMenu(0, 0, pMM->getWindowWidth(), pMM->getWindowHeight(), 
			"../Textures/Menu/VendingMachine.png", 1024, 768);
		m_poVendingMenu->setType(MT_VENDINGMACHINE);

		CMenuButton *pButton;
		int nWindowWidth = pMM->getWindowWidth();
		int nWindowHeight = pMM->getWindowHeight();

		pButton = new COKButton((nWindowWidth >> 1) - 128, (nWindowHeight) - 64, 
			256, 32, "../Textures/Menu/ok.tga", "../Textures/Menu/okHover.tga", 256, 32, true, m_poVendingMenu);

		m_poPlayerMoneyText = new CTextButton((nWindowWidth >> 1) - 64, 20, 256, 32, NULL, NULL, 0, 0, false, NULL, "Arial Black", 
			12, 18);
		m_poPlayerMoneyText->setFormat(DT_LEFT);

		bFirstTime = false;
	}


	vector<CMenuButton*>::iterator iter = m_poVendingMenu->getButtons().begin();
	// move past the ok button
	++iter;

	while (iter != m_poVendingMenu->getButtons().end())
	{
		m_poVendingMenu->getButtons().erase(iter);
		--iter;
		++iter;
	}

	bool bShotgunIn = false;
	// Add all of the vending machine buttons
	for (unsigned int i = 0; i < m_poVendingMachine->m_VendingButtons.size(); ++i)
	{
		if (m_poVendingMachine->m_VendingButtons[i]->getType() == VMB_SHOTGUN || 
			m_poVendingMachine->m_VendingButtons[i]->getType() == VMB_TRIPLEBARRELSHOTGUN)
			bShotgunIn = true;
		m_poVendingMenu->getButtons().push_back(m_poVendingMachine->m_VendingButtons[i]);
	}
	
	if (bShotgunIn)
		CSoundManager::getInstance().playSound(SND_EFFECT_SODAWITHSHOTGUN);
}

/**
* CVendingState::onDeactivate
* @date Modified May 18, 2006
*/
void CVendingState::onDeactivate(void)
{
}

/**
* CVendingState::tick
* @date Modified May 18, 2006
*/
void CVendingState::tick(void)
{
	if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
	{
		changeState(CTestState::getInstancePtr());
	}
}

/**
* CVendingState::preRender
* @date Modified May 18, 2006
*/
void CVendingState::preRender(CRenderDevice& oRenderDevice)
{
	if (m_poVendingMenu->isDeleteMe())
	{
		m_poVendingMenu->deleteMe(false);
		m_poVendingMachine->setPlayer(NULL);
		changeState(CTestState::getInstancePtr());
	}
}

/**
* CVendingState::postRender
* @date Modified May 18, 2006
*/
void CVendingState::postRender(CRenderDevice& oRenderDevice)
{
	CMenuManager *pMM = CMenuManager::getInstancePtr();
	D3DXVECTOR2 mousePos = pMM->checkMouseInput((float)pMM->getMouseWidth(), (float)pMM->getMouseHeight(), true);
	CObjectManager::ObjectList lPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
	CPlayer *poPlayer = (CPlayer*)lPlayers.front();
	// get the current cull mode and save it to restore later
	DWORD dwCullMode;
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pMM->getReadyToRender();

	CMenu *pMenu = m_poVendingMenu;

	pMenu->drawMenu(&mousePos);

	// Show the player their money
	char szBuffer[128] = {0};
	sprintf(szBuffer, "Money: %d", poPlayer->getAmtMoney());
	m_poPlayerMoneyText->setText(szBuffer);
	m_poPlayerMoneyText->drawButton(NULL);

	pMM->canDisplayMouseCursor(true);
	pMM->displayMouseCursor();
	pMM->endRender();
	// restore cull mode
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, dwCullMode);
}




