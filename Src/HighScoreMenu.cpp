/**
* @file HighScoreMenu.cpp
* @author Jordan Leckner
* @date Created April 5, 2006
*
* This file includes the definition of the HighScore Menu class
* used for displaying a HighScore menu and its buttons.
*/
#include "DirectInputWrapper.h"
#include "HighScoreMenu.h"
#include "SoundManager.h"
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios_base;

/**
* CHighScoreMenu::drawMenu
* @date Modified May 21, 2006
*/
void CHighScoreMenu::drawMenu(D3DXVECTOR2 *pMouseCursor)
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

	CMenu::catchInput();

	int nWindowHeight = CMenuManager::getInstance().getWindowHeight();
	int nWindowQuarter = nWindowHeight >> 1;

	static priority_queue<SHighScore> tempQueue;
	tempQueue = *CHighScoreTable::getInstance().getHighScoreList();
	static char szBuffer[32];
	memset(szBuffer, 0, 32);
	for (int i = 0; i < 10; ++i)
	{
		SHighScore tScore = tempQueue.top();
		// we have only 1 button.
		// draw the name
		((CTextButton*)m_Buttons[0])->setText(tScore.szName);
		((CTextButton*)m_Buttons[0])->setPositionX(-192);
		((CTextButton*)m_Buttons[0])->setPositionY(- i * 32);

		//// Draw this button
		m_Buttons[0]->drawButton(NULL);

		// change to the score
		sprintf(szBuffer, "%.2f%%", tScore.fScore);
		((CTextButton*)m_Buttons[0])->setText(szBuffer);

		// move the position for the score.
		((CTextButton*)m_Buttons[0])->setPositionX(((CTextButton*)m_Buttons[0])->getPositionX() + 256);

		// Draw this button
		m_Buttons[0]->drawButton(NULL);

		tempQueue.pop();
	}

	// draw the ok button
	if (m_Buttons[1]->drawButton(pMouseCursor))
	{
		if (1 != m_nHoverIndex)
		{
			CSoundManager::getInstance().playSound(SND_EFFECT_MENUHOVER);
			m_nHoverIndex = 1;
		}
	}
	else
		m_nHoverIndex = -1;

	// Go back to the main menu if they press escape.
	if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_ESCAPE, MENU_SYSTEM))
	{
		m_bDeleteMe = true;
	}
}

/**
* CHighScoreMenu::loadHighScoreTable
* @date Modified May 21, 2006
*/
void CHighScoreTable::loadHighScoreTable(char *szFileName)
{
	if (!szFileName)
		return;

	static bool bOnce = true;

	ifstream ifl(szFileName, ios_base::binary);
	SHighScore tScore;

	for (int i = 0; i < 10; ++i)
	{
		// read in a score and push it in the queue
		ifl.read((char*)&tScore, sizeof(tScore));
		m_HighScoreList.push(tScore);
		
		if (bOnce)
		{
			m_fLowestScore = tScore.fScore;
			bOnce = false;
		}
		if (tScore.fScore < m_fLowestScore)
			m_fLowestScore = tScore.fScore;
	}

	bOnce = true;
	ifl.close();
}

/**
* CHighScoreMenu::saveHighScoreTable
* @date Modified May 21, 2006
*/
void CHighScoreTable::saveHighScoreTable(char *szFileName)
{
	if (!szFileName)
		return;

	ofstream ofl(szFileName, ios_base::binary);
	SHighScore tScore;
	priority_queue<SHighScore> tempQueue = m_HighScoreList;

	for (int i = 0; i < 10; ++i)
	{
		tScore = tempQueue.top();

		ofl.write((char*)&tScore, sizeof(tScore));

		tempQueue.pop();
	}

	ofl.close();
}

/**
* CHighScoreMenu::insertScore
* @date Modified May 21, 2006
*/
bool CHighScoreTable::insertScore(SHighScore tHighScore)
{
	if (tHighScore.fScore < m_fLowestScore)
		return false;

	m_HighScoreList.push(tHighScore);

	// get the lowest score
	priority_queue<SHighScore> tempQueue = m_HighScoreList;
	float fLowestScore = 0;
	for (int i = 0; i < 10; ++i)
	{
		SHighScore score = tempQueue.top();

		fLowestScore = score.fScore;

		if (fLowestScore < m_fLowestScore)
			m_fLowestScore = fLowestScore;

		tempQueue.pop();
	}

	return true;
}

/**
* CHighScoreMenu::insertScore
* @date Modified May 21, 2006
*/
bool CHighScoreTable::insertScore(char *szName, float fScore)
{
	if (!szName)
		return false;

	SHighScore tHighScore;
	strcpy(tHighScore.szName, szName);
	tHighScore.fScore = fScore;

	if (tHighScore.fScore < m_fLowestScore)
		return false;

	m_HighScoreList.push(tHighScore);

	// get the lowest score
	priority_queue<SHighScore> tempQueue = m_HighScoreList;
	float fLowestScore = 0;
	for (int i = 0; i < 10; ++i)
	{
		SHighScore score = tempQueue.top();

		fLowestScore = score.fScore;

		if (fLowestScore < m_fLowestScore)
			m_fLowestScore = fLowestScore;

		tempQueue.pop();
	}

	return true;
}




