/**
* @file ObjectiveTrigger.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
*
* Contains the implementation of the CObjectiveTrigger class
*/

#include "ObjectiveTrigger.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "Player.h"

/**
* CObjectiveTrigger::update
*
* @date Modified June 2, 2006
*/
void CObjectiveTrigger::update(void)
{
	// if this trigger isn't triggered just get out
	if (!m_bTriggered)
		return;

	// if the type is timed, update the timer
	if (m_eObjectiveType == OJT_TIMED)
		m_fCurrentTime -= CTimer::getInstance().getFrameTime();
}

/**
* CObjectiveTrigger::checkTriggerActivate
*
* @date Modified June 2, 2006
*/
bool CObjectiveTrigger::checkTriggerActivate(void)
{
	// get the players to see if they are close enough
	CObjectManager::ObjectList loPlayers;
	CObjectManager::getInstancePtr()->getObjects(OBJ_PLAYER, &loPlayers);
	CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();

	bool bActivate = true;

	if (m_fDepth == 0.0f)
	{
		while (oPlayerIter != loPlayers.end())
		{
			// is the player close to far away
			if (computeDistanceSquared(this->getPosition(), ((CPlayer*)(*oPlayerIter))->getPosition()) > (this->m_fRadius * this->m_fRadius))
			{
				bActivate = false;
				break;
			}
			oPlayerIter++;
		}
	}
	else
	{
		while (oPlayerIter != loPlayers.end())
		{
			// 2D AABB check
			D3DXVECTOR3 vPlayerPos = ((CPlayer*)(*oPlayerIter))->getPosition(), vTriggerPos = getPosition();
			float fWidth = m_fRadius * 0.5f;
			float fDepth = m_fDepth * 0.5f;
			if (vPlayerPos.x > (vTriggerPos.x - fWidth) && vPlayerPos.x < vTriggerPos.x + fWidth)
			{
				if (vPlayerPos.z > (vTriggerPos.z - fDepth) && vPlayerPos.z < (vTriggerPos.z + fDepth))
				{
				}
				else
				{
					bActivate = false;
					break;
				}
			}
			else
			{
				bActivate = false;
				break;
			}
			oPlayerIter++;
		}
	}

	if (!bActivate)
		return false;

	m_bTriggered = true;

	// switch on what type the trigger is to check for activation and then do different actions
	switch (m_eObjectiveType)
	{
	case OJT_GOTO:
		{
			break;
		}
	case OJT_TIMED:
		{
			std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_loBarriers.begin();
			while (oBarrierIter != m_loBarriers.end())
			{
				((CObjectiveBarrier*)(*oBarrierIter))->activate();
				((CObjectiveBarrier*)(*oBarrierIter))->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "roadBlockSign.x"));
				((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
				((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
				oBarrierIter++;
			}

			int nRand = rand() % 3;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
				break;
			case 2:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
				break;
			default:
				break;
			}

			m_fCurrentTime = m_fCountdownTime;

			break;
		}
	case OJT_DUDES:
		{
			std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_loBarriers.begin();
			while (oBarrierIter != m_loBarriers.end())
			{
				((CObjectiveBarrier*)(*oBarrierIter))->activate();
				((CObjectiveBarrier*)(*oBarrierIter))->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "roadBlockSign.x"));
				((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
				((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
				oBarrierIter++;
			}

			int nRand = rand() % 3;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
				break;
			case 2:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
				break;
			default:
				break;
			}


			m_unCurrentDudes = m_unNumDudes;

			break;
		}
	}

	return true;
}

/**
* CObjectiveTrigger::checkTriggerComplete
*
* @date Modified June 10, 2006
*/
bool CObjectiveTrigger::checkTriggerComplete(bool bForceComplete)
{
	// switch on what type the trigger is to check for activation and then do different actions
	switch (m_eObjectiveType)
	{
	case OJT_GOTO:
		{
			// there is nothing to check for, all the player had to do was get here
			// just deactivate the barriers
			std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_loBarriers.begin();
			while (oBarrierIter != m_loBarriers.end())
			{
				((CObjectiveBarrier*)(*oBarrierIter))->deactivate();
				((CObjectiveBarrier*)(*oBarrierIter))->setMesh(NULL);
				((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
				((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
				oBarrierIter++;
			}

			int nRand = rand() % 3;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
				break;
			case 2:
				CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
				break;
			default:
				break;
			}

			return true;
		}
	case OJT_TIMED:
		{
			// update the tooltip
			int nMinutes = ((int)(m_fCurrentTime / 60.0f));
			int nSeconds = (int)(m_fCurrentTime - (nMinutes * 60.0f));
			int nMilliSeconds = (int)((m_fCurrentTime - (int)m_fCurrentTime) * 100.0f);
			//if (fSeconds >= 10.0f)
				m_strToolTip.Format("Survive %i:%02i:%02i", nMinutes, nSeconds, nMilliSeconds);
			//else
			//	m_strToolTip.Format("Survive %i:0%2.1i", nMinutes, fSeconds, nMilliSeconds);

			// check for completion
			if (m_fCurrentTime <= 0.0f || bForceComplete)
			{
				std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_loBarriers.begin();
				while (oBarrierIter != m_loBarriers.end())
				{
					((CObjectiveBarrier*)(*oBarrierIter))->deactivate();
					((CObjectiveBarrier*)(*oBarrierIter))->setMesh(NULL);
					((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
					((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
					oBarrierIter++;
				}

				int nRand = rand() % 3;
				switch (nRand)
				{
				case 0:
					CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
					break;
				case 1:
					CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
					break;
				case 2:
					CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
					break;
				default:
					break;
				}

				return true;
			}
			else
				return false;
		}
	case OJT_DUDES:
		{
			// update the tooltip
			m_strToolTip.Format("Kill %i Zombies", m_unCurrentDudes);

			// check for completion
			if (m_unCurrentDudes <= 0 || bForceComplete)
			{
				std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_loBarriers.begin();
				while (oBarrierIter != m_loBarriers.end())
				{
					((CObjectiveBarrier*)(*oBarrierIter))->deactivate();
					((CObjectiveBarrier*)(*oBarrierIter))->setMesh(NULL);
					((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
					((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
					oBarrierIter++;
				}

				int nRand = rand() % 3;
				switch (nRand)
				{
				case 0:
					CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
					break;
				case 1:
					CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
					break;
				case 2:
					CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
					break;
				default:
					break;
				}

				return true;
			}
			else
				return false;
		}
	}

	// this is really bad
	return false;
}