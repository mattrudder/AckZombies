/**
* @file Zombie.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CZombie class
*/

#include "Zombie.h"
#include "AIGroup.h"
#include "ObjectManager.h"
#include "Player.h"
#include "SoundManager.h"

/**
* CZombie::CZombie
* @date Modified April 28, 2006
*/
CZombie::CZombie(void) : CEnemy(OBJ_ENEMY_ZOMBIECITIZEN)
{
	calcPlaySoundTime();
	
	m_fSoundTimer = CTimer::getInstance().getTime();
}

CZombie::~CZombie(void)
{
}

/**
* CZombie::calcPlaySoundTime
* @date Modified June 8, 2006
*/
void CZombie::calcPlaySoundTime()
{
	int nRand = rand() % 5;
	switch (nRand)
	{
	case 0:
		m_nPlaySoundTime = 5;
		break;
	case 1:
		m_nPlaySoundTime = 10;
		break;
	case 2:
		m_nPlaySoundTime = 15;
		break;
	case 3:
		m_nPlaySoundTime = 20;
		break;
	case 4:
		m_nPlaySoundTime = 25;
		break;
	default:
		break;
	}
}

/**
* CZombie::update
* @date Modified June 1, 2006
*/
void CZombie::update(void)
{
	// call the base class update
	CCharacter::update();

	// if this is dead
	if (isDeleted())
	{
		// and he is in a group
		if (m_oAI.getGroup() != NULL)
		{
			// remove myself
			m_oAI.getGroup()->remove(this);
		}

		// spawn some coins
		spawnPickup(150, 10.0f, 20.0f); 
	}

	// check to for self deletion
	if (m_tSelfDelete.tick())
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();
		while (oPlayerIter != loPlayers.end())
		{
			// is the player too far away
			if (computeDistanceSquared(((CPlayer*)(*oPlayerIter))->getPosition(), getPosition()) > 62500.0f) // 250 squared
			{
				// set it to be deleted by the objectmanager
				CObjectManager::getInstance().removeObject(this);

				// if i am a member of a group
				if (m_oAI.getGroup() != NULL)
				{
					// i need to get out of it
					m_oAI.getGroup()->remove(this);
				}
				// no need to update AI for a now dead zombie
				return;
			}
			oPlayerIter++;
		}
		
	}

	// update the AI
	m_oAI.update(this);
	m_bSelected = false;

	// Should we play a sound?
	if (CTimer::getInstance().getTime() - m_fSoundTimer >= m_nPlaySoundTime)
	{
		// what sound should we play?
		int nRand = rand() % 2;
		switch (nRand)
		{
		case 0:
			CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEBRAINS);
			break;
		case 1:
			CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEBRAINS);
			break;
		default:
			break;
		}

		m_fSoundTimer = CTimer::getInstance().getTime();
		calcPlaySoundTime();
	}
}


/**
* CZombie::create
* @date Modified April 24, 2006
*/
CObject* CZombie::create()
{
	CZombie* poZombie = new CZombie;
	// test code to load mesh
	poZombie->m_pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "citizenZombie.x");
	return poZombie;
}

/**
* CZombie::init
* @date Modified May 11, 2006
*/
void CZombie::init(void)
{
	CEnemy::init();

	setHealth(100);
	((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Walk");
	m_tSelfDelete.setInterval(5.0f);
}