/**
* @file Character.cpp
* @author Tom Williams
* @date Modified Apr 6, 2006
*
* definition for any interactive character within the game.
*/

#include "character.h"
#include "ObjectManager.h"
#include "MenuManager.h"
#include "AIManager.h"
#include "HUD.h"
#include "Player.h"
#include "SoundManager.h"

/**
* CCharacter::CCharacter()
* @date Modified Mar 27, 2006
*/
CCharacter::CCharacter(EObjectType eType) : CActor(eType), m_nHealth(0), m_vTempVel(0,0,0)
{
	m_pDeathParticle = NULL;
}

/**
* CCharacter::collided()
* @date Modified April 23, 2006
*/
void CCharacter::collided(CActor *poActor)
{
}

/**
* CCharacter::collided()
* @date Modified June 7, 2006
*/
void CCharacter::update()
{
	// only use for enemies
	if (getType() != OBJ_PLAYER)
	{
		CAnimatedMesh *pAnimMesh = (CAnimatedMesh*)getMesh();

		// if the enemy is shot, set back to walking if the animation has played once.
		if (pAnimMesh->getLastAnimationName() == (CString)"Shot")
		{
			// Don't move the enemy if he gets shot.
			setVelocity(D3DXVECTOR3(0,0,0));
			// check if the animation is over now.
			if (CTimer::getInstance().getTime() - getAnimationTime() >= (float)pAnimMesh->getAnimationLength())
			{
				pAnimMesh->setAnimationSetByName("Walk");
				// set back the velocity what it was before.
				setVelocity(m_vTempVel);
			}
		}
	}
	// update the base
	CActor::update();

	// character is dead
	if (m_nHealth <= 0)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		CAIManager::getInstance().killedEnemy();
		CObjectManager::getInstance().removeObject(this);
		// Zombie death
		if ((getType() & OBJ_ENEMY_ZOMBIECITIZEN) == OBJ_ENEMY_ZOMBIECITIZEN)
		{
			int nRand = rand() % 6;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEDEATH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEDEATH1);
				break;
			case 2:
				CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEDEATH2);
				break;
			case 3:
				CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEDEATH3);
				break;
			case 4:
				CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEDEATH4);
				break;
			case 5:
				CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIEDEATH5);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CCharacter::init
* @date Modified May 11, 2006
*/
void CCharacter::init(void)
{
	CActor::init();
}