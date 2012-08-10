/**
* @file ObjectiveHUD.cpp
* @author Jordan Leckner
* @date Created June 5, 2006
*
* This file includes the definition of the ObjectiveHUD Menu class
* used for displaying a ObjectiveHUD menu and its buttons.
*/
#include "ObjectiveHUD.h"
#include "AIManager.h"

/**
* CObjectiveHUDMenu::CObjectiveHUDMenu
* @date Modified June 10, 2006
*/
CObjectiveHUDMenu::CObjectiveHUDMenu()
{
	m_nType = MT_OBJECTIVEHUD;

	// get the objective font B
	m_poObjectiveFont = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "HandelGothic.ini");

	int nWindowWidth = CMenuManager::getInstance().getWindowWidth();
	int nWindowHeight = CMenuManager::getInstance().getWindowHeight();

	m_poObjectivesBG = new CMenuButton((nWindowWidth >> 1) - 256, (nWindowHeight >> 1) - 256, 512, 512, "../Textures/ObjectivesBG.tga",
		NULL, 512, 512, false);
}

/**
* CObjectiveHUDMenu::drawMenu
* @date Modified June 10, 2006
*/
void CObjectiveHUDMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
{
	static bool bFirstTime = true;
	if (bFirstTime)
	{
		CObjectManager::ObjectList lObjectiveList;
		CObjectManager::getInstance().getObjects(OBJ_OBJECTIVE_TRIGGER, &lObjectiveList);
		lObjectiveList.sort(CObjectiveTrigger::SortObjectiveTrigger);
		CObjectiveTrigger *pTrigger;
		for (CObjectManager::ObjectList::iterator iter = lObjectiveList.begin(); iter != lObjectiveList.end(); ++iter)
		{
			pTrigger = (CObjectiveTrigger*)*iter;
			m_vstrObjectives.push_back(pTrigger->m_strToolTip);
			m_vunOrderNumbers.push_back(pTrigger->m_unOrderNumber);
		}

		bFirstTime = false;
	}
	// get the current cull mode and save it to restore later
	DWORD dwCullMode;
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_poObjectivesBG->drawButton(NULL);

	// restore cull mode
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, dwCullMode);
	
	
	int i = 0;
	float x, y;
	CString output;

	output = "-Objectives-";
	x = (float)((CMenuManager::getInstance().getWindowWidth() >> 1) - 232);
	y = (float)((CMenuManager::getInstance().getWindowHeight() >> 1) - 164 + (-1.0f * 50.0f));
	m_poObjectiveFont->drawString(output, x, y);

	for (unsigned int i = 0; i < m_vstrObjectives.size(); ++i)
	{
		if (m_vunOrderNumbers[i] < CAIManager::getInstance().getCurrentObjective()->m_unOrderNumber)
			output = "+";
		else
			output = "-";

		output += m_vstrObjectives[i].GetBuffer();
		x = (float)((CMenuManager::getInstance().getWindowWidth() >> 1) - 232);
		y = (float)((CMenuManager::getInstance().getWindowHeight() >> 1) - 164 + ((float)i * 25.0f));
		m_poObjectiveFont->drawString(output, x, y);
	}
}






