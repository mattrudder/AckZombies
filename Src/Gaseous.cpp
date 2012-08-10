/**
* @file Gaseous.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CGaseous class
*/

#include "Gaseous.h"
#include "ObjectManager.h"
#include "Math3D.h"
#include "AIGroup.h"
#include "Player.h"
#include "SoundManager.h"

/**
* CGaseous::CGaseous
* @date Modified June 7, 2006
*/
CGaseous::CGaseous(void) : CEnemy(OBJ_ENEMY_GASEOUSCLAY), m_fFlashTimer(0), m_bUpdate(true)
{
	m_dwOldColor = getColor();
	m_dwRedColor = D3DCOLOR_XRGB(128,0,0);

	// Create the explosion particle effect
	m_pDeathParticle = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pDeathParticle->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "Explosion.xml"));
}

CGaseous::~CGaseous(void)
{
}

/**
* CGaseous::update
* @date Modified June 7, 2006
*/
void CGaseous::update(void)
{
	// get the list of players
	CObjectManager::ObjectList lPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
	bool bEnemyClose = false;

	// we're dead.
	if (m_nHealth <= 0)
	{
		// first time
		if (m_bUpdate)
		{
			m_fFlashTimer = CTimer::getInstance().getTime();
			m_bUpdate = false;
			((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Idle");
		}
	}
	else
	{
		CPlayer *poPlayer;
		float fDistance;
		// go through the player list and do a distance check
		for (CObjectManager::ObjectList::iterator iter = lPlayers.begin(); iter != lPlayers.end(); ++iter)
		{
			poPlayer = (CPlayer*)*iter;

			fDistance = computeDistanceSquared(getBV().centerPt, poPlayer->getBV().centerPt);
			// 25 squared
			if (fDistance <= 625.0f)
				bEnemyClose = true;
		}
		if (!bEnemyClose)
			setColor(m_dwOldColor);
	}

	if (!m_bUpdate || bEnemyClose)
	{
		// flash for a few.
		if (getColor() == m_dwOldColor)
			setColor(m_dwRedColor);
		else
			setColor(m_dwOldColor);
	}
	if (!m_bUpdate)
	{
		CSoundManager::getInstance().playSound(SND_EFFECT_GASEOUS_READY);
		// flash for 1 second, then explode/die.
		if (CTimer::getInstance().getTime() - m_fFlashTimer >= 1.0f)
		{
			m_bUpdate = true;
			doDamage();
		}
	}

	// call the base class update
	if (m_bUpdate)
		CCharacter::update();

	// update the AI
	m_oAI.update(this);

	// do this if gaseous was killed
	if(isDeleted())
	{
		// and he is in a group
		if (m_oAI.getGroup() != NULL)
		{
			// remove myself
			m_oAI.getGroup()->remove(this);
		}
	}

	m_bSelected = false;
}

/**
* CGaseous::doDamage
* @date Modified May 4, 2006
*/
void CGaseous::doDamage(void)
{
	// get all the characters so i can do damage to them
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CObjectManager::ObjectList loObjects;
	poObjectManager->getObjects(OBJ_CHARACTER, &loObjects);
	CObjectManager::ObjectList::iterator oObjIter = loObjects.begin();

	float fDist = 0.0f;
	float fDamage = 0.0f;

	// iterate all of the characters and check for damage
	while (oObjIter != loObjects.end())
	{
		// see how far away the character is
		fDist = computeDistanceSquared(((CCharacter*)(*oObjIter))->getPosition(), this->getPosition());

		// 50.0f squared
		if (fDist > 2500.0f)
		{
			oObjIter++;
			continue;
		}

		// based on the distance deal damage
		fDamage = (2500.0f / fDist);

		((CCharacter*)(*oObjIter))->setHealth(((CCharacter*)(*oObjIter))->getHealth() - ((int)(fDamage)));
		oObjIter++;
	}

	poObjectManager->removeObject(this);
}

/**
* CGaseous::create
* @date Modified April 24, 2006
*/
CObject* CGaseous::create()
{
	CGaseous* poZombie = new CGaseous;
	poZombie->m_pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "IceCreamZombie.x");
	return poZombie;
}

/**
* CGaseous::init
* @date Modified June 7, 2006
*/
void CGaseous::init(void)
{
	CEnemy::init();
	setColor(D3DCOLOR_XRGB(0,0,0));
	m_bUpdate = true;
	setHealth(50);
	((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Walk");
}