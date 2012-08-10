/**
* @file QBZombie.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CQBZombie class
*/

#include "QBZombie.h"
#include "AIGroup.h"

/**
* CQBZombie::CQBZombie
* @date Modified April 7, 2006
*/
CQBZombie::CQBZombie(void) : CEnemy(OBJ_ENEMY_QUARTERBACK), m_dCircleTime(0.0), m_fAttackDist(0.0f), m_bWhichWay(false)
{
}

CQBZombie::~CQBZombie(void)
{
}

/**
* CQBZombie::update
* @date Modified April 12, 2006
*/
void CQBZombie::update(void)
{
	// call the base class update
	CCharacter::update();

	// do this if the qb was killed
	if(isDeleted())
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

	// update the AI
	m_oAI.update(this);
	m_bSelected = false;
}

/**
* CQBZombie::collided
* @date Modified May 9, 2006
*/
void CQBZombie::collided(CActor* pActor)
{
	if (pActor == NULL)
	{
		this->setWay(!this->getWay());
		this->getAI()->update(this);
	}
}

/**
* CQBZombie::create
* @date Modified April 24, 2006
*/
CObject* CQBZombie::create()
{
	CQBZombie* poZombie = new CQBZombie;
	poZombie->m_pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "IceCreamZombie.x");
	return poZombie;
}

/**
* CQBZombie::init
* @date Modified May 11, 2006
*/
void CQBZombie::init(void)
{
	CEnemy::init();

	setHealth(3000);
	((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Walk");
}