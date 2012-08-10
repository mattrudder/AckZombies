/**
* @file ActorSpawnTrigger.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created May 9, 2006
*
* Contains the implementation of the CActorSpawnTrigger class
*/

#include "ActorSpawnTrigger.h"
#include "ObjectManager.h"
#include "Player.h"
#include "AIManager.h"

/**
* CActorSpawnTrigger::activateSpawns
* @date Modified May 9, 2006
*/
void CActorSpawnTrigger::activateSpawns(void)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();

	if (m_fCurrentTime < m_fWaitTime)
		return;

	if (m_nMaxTimes > 0)
		if (m_nNumTimes > m_nMaxTimes)
			return;

	// get the players to see if they are close enough
	CObjectManager::ObjectList loPlayers;
	poObjectManager->getObjects(OBJ_PLAYER, &loPlayers);
	CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();

	if (m_fDepth == 0.0f)
	{
		while (oPlayerIter != loPlayers.end())
		{
			// is the player close enough
			if (computeDistanceSquared(this->getPosition(), ((CPlayer*)(*oPlayerIter))->getPosition()) < (this->m_fRadius * this->m_fRadius))
			{
				// reset
				m_fCurrentTime = 0.0f;

				// activate the spawns
				std::list<CActorSpawn*>::iterator oSpawnIter = m_loSpawns.begin();
				while (oSpawnIter != m_loSpawns.end())
				{
					CActorSpawn* poSpawn = ((CActorSpawn*)(*oSpawnIter));
					if (!poSpawn->getUsed())
					{
						m_nNumTimes++;
						poSpawn->setUsed(true);
						CAIManager::getInstancePtr()->spawnGroup(poSpawn->getSpawnType(), poSpawn->getPosition(), poSpawn->getNumActors());
					}
					oSpawnIter++;
				}
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
					// reset
					m_fCurrentTime = 0.0f;

					// activate the spawns
					std::list<CActorSpawn*>::iterator oSpawnIter = m_loSpawns.begin();
					while (oSpawnIter != m_loSpawns.end())
					{
						CActorSpawn* poSpawn = ((CActorSpawn*)(*oSpawnIter));
						if (!poSpawn->getUsed())
						{
							m_nNumTimes++;
							poSpawn->setUsed(true);
							CAIManager::getInstancePtr()->spawnGroup(poSpawn->getSpawnType(), poSpawn->getPosition(), poSpawn->getNumActors());
						}
						oSpawnIter++;
					}
				}
			}
			oPlayerIter++;
		}
	}
}