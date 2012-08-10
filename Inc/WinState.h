/**
* @file WinState.h
* @author Jordan Leckner
* @date Created June 2, 2006
*
* This file includes the win state
*/
#ifndef _WINSTATE_H_
#define _WINSTATE_H_

#define COLOR 0xFFFFFFFF
#define MAXNAMEENTRY 8

#include "Gamestate.h"
#include "MenuManager.h"
#include "Menu.h"
#include "HighScoreMenu.h"

class CWinState : public CGameState<CWinState>
{
protected:

	//! Display Font
	CBitmapFont* m_fntDina;

	//! The gameover menu
	CMenu *m_poWin;

	//! used to access the stats and money
	CPlayer* m_poPlayer;

	//! timing variable
	double m_dTiming;

	//! currently incrementing stat
	int m_nCurrentStat;

	//! incrementation
	unsigned int m_unIncrement;

	//! Whether or not we should insert to the high score table
	bool m_bInsert;

	//! Whether or not to display to the user to type their name in
	bool m_bEnterName;

	//! Name of the player for high score
	char m_szName[32];

public:

	CWinState() : m_poWin(NULL), m_bInsert(true)
	{
		memset(m_szName, 0, 32);
	}

	virtual ~CWinState(void)
	{
		if (m_poWin)
			delete m_poWin;

		m_poWin = NULL;
	}

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created May 16, 2006
	*/
	virtual void onActivate(void)
	{
		m_dTiming = 0.0;
		m_nCurrentStat = 0;
		m_unIncrement = 0;
		m_bInsert = true;
		m_bEnterName = true;
		memset(m_szName, 0, 32);

		CMenuManager *pMM = CMenuManager::getInstancePtr();

		static bool bFirstTime = true;
		if (bFirstTime)
		{
			m_fntDina = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "HandelGothic.ini");

			m_poWin = new CMenu(0, 0, pMM->getWindowWidth(), pMM->getWindowHeight(), 
				"../Textures/Menu/youwin.tga", 1024, 768);
			m_poWin->setType(MT_GAMEOVER);

			CMenuButton *pButton;
			int nWindowWidth = pMM->getWindowWidth();
			int nWindowHeight = pMM->getWindowHeight();

			// standard buttons
			pButton = new CMainMenuButton((nWindowWidth >> 1) - 128, (nWindowHeight >> 1) + 165, 
				256, 32, "../Textures/Menu/mainmenubutton.tga", "../Textures/Menu/mainmenubuttonHover.tga", 256, 32, true, m_poWin);

			pButton = new CQuitButton((nWindowWidth >> 1) - 128, (nWindowHeight >> 1) + 202, 
				256, 32, "../Textures/Menu/quit.tga", "../Textures/Menu/quitHover.tga", 256, 32, true, m_poWin);

			bFirstTime = false;
		}

		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		m_poPlayer = ((CPlayer*)(loPlayers.front()));
		m_poPlayer->m_oStats.setParTime(300.0f);
		m_poPlayer->m_oStats.setTimeTaken(m_poPlayer->m_fTime);
		if(loPlayers.size() > 1)
		{
			CPlayer* playerTwo = (CPlayer*)loPlayers.back();
			m_poPlayer->m_oStats.addEnemiesKilled(playerTwo->m_oStats.getEnemiesKilled());
			m_poPlayer->m_oStats.addTotalMoneyEarned(playerTwo->m_oStats.getTotalMoneyEarned());
		}
		m_poPlayer->m_oStats.computeAccuracy();
		m_poPlayer->m_oStats.computeFinalGrade(false);
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
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *poPlayer = (CPlayer*)lPlayers.front();
		// get the current cull mode and save it to restore later
		DWORD dwCullMode;
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pMM->getReadyToRender();

		CMenu *pMenu = m_poWin;

		pMenu->drawMenu(&mousePos);

		pMM->canDisplayMouseCursor(true);
		pMM->displayMouseCursor();
		pMM->endRender();
		// restore cull mode
		CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, dwCullMode);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int nWindowWidth = pMM->getWindowWidth();
		int nWindowHeight = pMM->getWindowHeight();

		// this needs to print out the stats
		CString string;

		switch (m_nCurrentStat)
		{
		case 0:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					int nIncrease = (unsigned int)(m_poPlayer->m_oStats.getEnemiesSpawned() * 0.01f);
					nIncrease < 1 ? nIncrease = 1 : nIncrease;
					m_unIncrement += nIncrease;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getEnemiesKilled())
				{
					m_unIncrement = m_poPlayer->m_oStats.getEnemiesKilled();
					m_nCurrentStat = 1;
					m_dTiming = 0.0;
					bNext = true;
				}

				string.Format("Enemies Killed/Spawned");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 5)), COLOR);
				string.Format("%i / %i", m_unIncrement, m_poPlayer->m_oStats.getEnemiesSpawned());
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 5)), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
			case 1:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					int nIncrease = (unsigned int)(m_poPlayer->m_oStats.getShotsFired() * 0.01f);
					nIncrease < 1 ? nIncrease = 1 : nIncrease;
					m_unIncrement += nIncrease;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getShotsHit())
				{
					m_unIncrement = m_poPlayer->m_oStats.getShotsHit();
					m_nCurrentStat = 2;
					m_dTiming = 0.0;
					bNext = true;
				}

				string.Format("Shots Hit/Fired");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 4)), COLOR);
				string.Format("%i / %i", m_unIncrement, m_poPlayer->m_oStats.getShotsFired());
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 4)), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
			case 2:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					int nIncrease = (unsigned int)(m_poPlayer->m_oStats.getAwesomeEnemiesSpawned() * 0.01f);
					nIncrease < 1 ? nIncrease = 1 : nIncrease;
					m_unIncrement += nIncrease;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getAwesomeKills())
				{
					m_unIncrement = m_poPlayer->m_oStats.getAwesomeKills();
					m_nCurrentStat = 3;
					m_dTiming = 0.0;
					bNext = true;
				}

				string.Format("Awesome Time Kills/Max");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 3)), COLOR);
				string.Format("%i / %i", m_unIncrement, m_poPlayer->m_oStats.getAwesomeEnemiesSpawned());
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 3)), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
			case 3:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					int nIncrease = (unsigned int)(m_poPlayer->m_oStats.getAwesomeTime() * 0.1f);
					nIncrease < 1 ? nIncrease = 1 : nIncrease;
					m_unIncrement += nIncrease;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getAwesomeTime())
				{
					m_unIncrement = (unsigned int)m_poPlayer->m_oStats.getAwesomeTime();
					m_nCurrentStat = 4;
					m_dTiming = 0.0;
					bNext = true;
				}

				CString time1, time2;
				int nMinutes = ((int)((float)m_unIncrement / 60.0f));
				float fSeconds = (float)m_unIncrement - (nMinutes * 60.0f);
				if (fSeconds >= 10.0f)
					time1.Format("%i:%.2g", nMinutes, fSeconds);
				else
					time1.Format("%i:0%.1g", nMinutes, fSeconds);

				nMinutes = ((int)((m_poPlayer->m_oStats.getNumAwesomes() * 45.0f) / 60.0f));
				fSeconds = (m_poPlayer->m_oStats.getNumAwesomes() * 45.0f) - (nMinutes * 60.0f);
				if (fSeconds >= 10.0f)
					time2.Format("%i:%.2g", nMinutes, fSeconds);
				else
					time2.Format("%i:0%.1g", nMinutes, fSeconds);

				string.Format("Awesome Time Spent/Max");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 2)), COLOR);
				string.Format("%s / %s", time1.GetBuffer(), time2.GetBuffer());
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 2)), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
			case 4:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					int nIncrease = (unsigned int)(m_poPlayer->m_oStats.getTotalMoneyEarned() * 0.01f);
					nIncrease < 1 ? nIncrease = 1 : nIncrease;
					m_unIncrement += nIncrease;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getTotalMoneyEarned())
				{
					m_unIncrement = m_poPlayer->m_oStats.getTotalMoneyEarned();
					m_nCurrentStat = 5;
					m_dTiming = 0.0;
					bNext = true;
				}

				string.Format("Money Earned/Max");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight())), COLOR);
				string.Format("%i / %i", m_unIncrement, m_poPlayer->m_oStats.getMoneySpawned());
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight())), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
			case 5:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					int nIncrease = (unsigned int)(m_poPlayer->m_oStats.getTimeTaken() * 0.01f);
					nIncrease < 1 ? nIncrease = 1 : nIncrease;
					m_unIncrement += nIncrease;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getTimeTaken())
				{
					m_unIncrement = (unsigned int)m_poPlayer->m_oStats.getTimeTaken();
					m_nCurrentStat = 6;
					m_dTiming = 0.0;
					bNext = true;
				}

				CString time1, time2;
				int nMinutes = ((int)(m_unIncrement / 60.0f));
				float fSeconds = m_unIncrement - (nMinutes * 60.0f);
				if (fSeconds >= 10.0f)
					time1.Format("%i:%.2g", nMinutes, fSeconds);
				else
					time1.Format("%i:0%.1g", nMinutes, fSeconds);

				nMinutes = ((int)(m_poPlayer->m_oStats.getParTime() / 60.0f));
				fSeconds = m_poPlayer->m_oStats.getParTime() - (nMinutes * 60.0f);
				if (fSeconds >= 10.0f)
					time2.Format("%i:%.2g", nMinutes, m_poPlayer->m_oStats.getParTime() - (nMinutes * 60.0f));
				else
					time2.Format("%i:0%g", nMinutes, m_poPlayer->m_oStats.getParTime() - (nMinutes * 60.0f));

				string.Format("Time Taken/Par");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)(nWindowHeight >> 1), COLOR);
				string.Format("%s / %s", time1.GetBuffer(), time2.GetBuffer());
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)(nWindowHeight >> 1), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
			case 6:
			{
				m_dTiming += (double)CTimer::getInstance().getFrameTime();
				if (m_dTiming > 0.00000001)
				{
					m_unIncrement += 1;
					m_dTiming = 0.0;
				}

				bool bNext = false;
				if (m_unIncrement >= m_poPlayer->m_oStats.getFinalGrade())
				{
					m_unIncrement = (unsigned int)m_poPlayer->m_oStats.getFinalGrade();
					m_nCurrentStat = 7;
					m_dTiming = 0.0;
					bNext = true;
				}

				string.Format("Final Grade");
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) + (m_fntDina->getLineHeight())), COLOR);
				string.Format("%i", m_unIncrement);
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) + (m_fntDina->getLineHeight())), COLOR);

				if (bNext)
					m_unIncrement = 0;
				break;
			}
		}

		if (m_nCurrentStat > 0)
		{
			string.Format("Enemies Killed/Spawned");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 5)), COLOR);
			string.Format("%i / %i", m_poPlayer->m_oStats.getEnemiesKilled(), m_poPlayer->m_oStats.getEnemiesSpawned());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 5)), COLOR);
		}

		if (m_nCurrentStat > 1)
		{
			string.Format("Shots Hit/Fired");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 4)), COLOR);
			string.Format("%i / %i    %g%%", m_poPlayer->m_oStats.getShotsHit(), m_poPlayer->m_oStats.getShotsFired(), m_poPlayer->m_oStats.getAccuracy());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 4)), COLOR);
		}

		if (m_nCurrentStat > 2)
		{
			string.Format("Awesome Time Kills/Max");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 3)), COLOR);
			string.Format("%i / %i", m_poPlayer->m_oStats.getAwesomeKills(), m_poPlayer->m_oStats.getAwesomeEnemiesSpawned());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 3)), COLOR);
		}

		if (m_nCurrentStat > 3)
		{
			CString time1, time2;
			int nMinutes = ((int)(m_poPlayer->m_oStats.getAwesomeTime() / 60.0f));
			float fSeconds = m_poPlayer->m_oStats.getAwesomeTime() - (nMinutes * 60.0f);
			if (fSeconds >= 10.0f)
				time1.Format("%i:%.2g", nMinutes, fSeconds);
			else
				time1.Format("%i:0%.1g", nMinutes, fSeconds);

			nMinutes = ((int)((m_poPlayer->m_oStats.getNumAwesomes() * 45.0f) / 60.0f));
			fSeconds = (m_poPlayer->m_oStats.getNumAwesomes() * 45.0f) - (nMinutes * 60.0f);
			if (fSeconds >= 10.0f)
				time2.Format("%i:%.2g", nMinutes, fSeconds);
			else
				time2.Format("%i:0%.1g", nMinutes, fSeconds);

			string.Format("Awesome Time Spent/Max");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 2)), COLOR);
			string.Format("%s / %s", time1.GetBuffer(), time2.GetBuffer());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight() * 2)), COLOR);
		}

		if (m_nCurrentStat > 4)
		{
			string.Format("Money Earned/Max");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight())), COLOR);
			string.Format("%i / %i", m_poPlayer->m_oStats.getTotalMoneyEarned(), m_poPlayer->m_oStats.getMoneySpawned());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) - (m_fntDina->getLineHeight())), COLOR);
		}

		if (m_nCurrentStat > 5)
		{
			CString time1, time2;
			int nMinutes = ((int)(m_poPlayer->m_oStats.getTimeTaken() / 60.0f));
			float fSeconds = m_poPlayer->m_oStats.getTimeTaken() - (nMinutes * 60.0f);
			if (fSeconds >= 10.0f)
				time1.Format("%i:%.2g", nMinutes, fSeconds);
			else
				time1.Format("%i:0%.1g", nMinutes, fSeconds);

			nMinutes = ((int)(m_poPlayer->m_oStats.getParTime() / 60.0f));
			fSeconds = m_poPlayer->m_oStats.getParTime() - (nMinutes * 60.0f);
			if (fSeconds >= 10.0f)
				time2.Format("%i:%.2g", nMinutes, m_poPlayer->m_oStats.getParTime() - (nMinutes * 60.0f));
			else
				time2.Format("%i:0%g", nMinutes, m_poPlayer->m_oStats.getParTime() - (nMinutes * 60.0f));

			string.Format("Time Taken/Par");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)(nWindowHeight >> 1), COLOR);
			string.Format("%s / %s", time1.GetBuffer(), time2.GetBuffer());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)(nWindowHeight >> 1), COLOR);
		}

		if (m_nCurrentStat > 6)
		{
			string.Format("Final Grade");
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 400), (float)((nWindowHeight >> 1) + (m_fntDina->getLineHeight())), COLOR);
			string.Format("%g%%", m_poPlayer->m_oStats.getFinalGrade());
			m_fntDina->drawString(string, (float)((nWindowWidth >> 1) + 100), (float)((nWindowHeight >> 1) + (m_fntDina->getLineHeight())), COLOR);

			float fGrade = m_poPlayer->m_oStats.getFinalGrade();
			if (fGrade >= 97.0f){string.Format("OWNED");}
			else if (fGrade >= 90.0f){string.Format("A");}
			else if (fGrade >= 80.0f){string.Format("B");}
			else if (fGrade >= 70.0f){string.Format("C");}
			else if (fGrade >= 60.0f){string.Format("D");}
			else
				string.Format("F");

			m_fntDina->drawString(string, (float)((nWindowWidth >> 1)), (float)((nWindowHeight >> 1) + (m_fntDina->getLineHeight() * 2)), COLOR);

			if (m_bEnterName && CHighScoreTable::getInstance().getLowestScore() < fGrade)
			{
				// prompt the user to enter their name.
				string.Format("Enter Name and Press Enter:   %s", m_szName);
				m_fntDina->drawString(string, (float)((nWindowWidth >> 1) - 256), (float)((nWindowHeight >> 1) + (m_fntDina->getLineHeight() * 3)), COLOR);

				// get the data from the user for their name
				DWORD dwKey;
				if (CDirectInputWrapper::getInstance().getKey(dwKey))
				{
					if (dwKey == DIK_BACKSPACE)
					{
						int i = 0;
						while (true)
						{
							if (i && !m_szName[i])
							{
								m_szName[i-1] = 0;
								break;
							}
							++i;
							if (i > MAXNAMEENTRY + 1)
								break;
						}
					}
					else
					{
						int i = 0;
						while (true)
						{
							if (!m_szName[i])
							{
								// only get alpha-numeric keys
								if (CDirectInputWrapper::getInstance().isAlphaNumeric(dwKey))
								{
									if (dwKey != DIK_RETURN)
										m_szName[i] = *CDirectInputWrapper::getInstance().convertDIKtoString(dwKey);
									break;
								}
							}
							++i;
							if (i > MAXNAMEENTRY)
								break;
						}
					}
				}
			}

			// if they hit enter, save their score
			if (CDirectInputWrapper::getInstance().isKeyDownBuffered(DIK_RETURN, MENU_SYSTEM))
				m_bEnterName = false;

			// save the score to the high score table if we can.
			if (m_bInsert && !m_bEnterName)
			{
				CHighScoreTable::getInstance().insertScore(m_szName, fGrade);
				m_bInsert = false;
				memset(m_szName, 0, 32);
			}
		}
	}
};

#endif