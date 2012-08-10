/**
* @file Acidic.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAcidic class
*/

#include "Acidic.h"
#include "AIGroup.h"

/**
* CAcidic::CAcidic
* @date Modified April 7, 2006
*/
CAcidic::CAcidic(void) : CEnemy(OBJ_ENEMY_ICECREAMMAN), m_dCircleTime(0.0), m_fAttackDist(0.0f), m_bWhichWay(false)
{
}

CAcidic::~CAcidic(void)
{
}

/**
* CZombie::update
* @date Modified April 12, 2006
*/
void CAcidic::update(void)
{
	// call the base class update
	CCharacter::update();

	// do this if acidic was killed
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
* CAcidic::collided
* @date Modified May 9, 2006
*/
void CAcidic::collided(CActor* pActor)
{
	if (pActor == NULL)
	{
		this->setWay(!this->getWay());
		this->getAI()->update(this);
	}
}

/**
* CAcidic::create
* @date Modified April 24, 2006
*/
CObject* CAcidic::create()
{
	CAcidic* poZombie = new CAcidic;
	poZombie->m_pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "IceCreamZombie.x");
	return poZombie;
}

/**
* CAcidic::init
* @date Modified May 11, 2006
*/
void CAcidic::init(void)
{
	CEnemy::init();

	setColor(0xff000000);
	setHealth(2500);
	((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Walk");
}