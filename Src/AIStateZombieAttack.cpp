/**
* @file AIStateZombieAttack.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateZombieAttack class
*/

#include "AIStateZombieAttack.h"
#include "AIStatePathPlan.h"
#include "Enemy.h"

/**
* CAIStateZombieAttack::CAIStateZombieAttack
* @date Modified April 7, 2006
*/
CAIStateZombieAttack::CAIStateZombieAttack(void)
{
	setStateType(CAIState::AIS_ZOMBIEATTACK);
}

/**
* CAIStateZombieAttack::~CAIStateZombieAttack
* @date Modified April 7, 2006
*/
CAIStateZombieAttack::~CAIStateZombieAttack(void) {}

/**
* CAIStateZombieAttack::update
* @date Modified April 7, 2006
*/
void CAIStateZombieAttack::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	poCharacter->setVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));

	if (poAIEntity->getCurrentStateTime() > ((CAnimatedMesh*)poCharacter->getMesh())->getAnimationLength())
		((CEnemy*)poCharacter)->setAIState(CAIStatePathPlan::getInstancePtr());
}

/**
* CAIStateZombieAttack::enter
* @date Modified April 13, 2006
*/
void CAIStateZombieAttack::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	poAIEntity->setCurrentStateTime(0.0);
	((CAnimatedMesh*)poCharacter->getMesh())->setAnimationSetByName("Attack");
}

/**
* CAIStateZombieAttack::exit
* @date Modified April 13, 2006
*/
void CAIStateZombieAttack::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	((CAnimatedMesh*)poCharacter->getMesh())->setAnimationSetByName("Walk");
}