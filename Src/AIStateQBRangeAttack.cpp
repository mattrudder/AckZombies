/**
* @file AIStateQBRangeAttack.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateQBRangeAttack class
*/

#include "AIStateQBRangeAttack.h"
#include "AIStateQBMeleeAttack.h"
#include "QBZombie.h"
#include "FootBall.h"
#include "AIManager.h"
#include "AIEntity.h"

/**
* CAIStateQBRangeAttack::CAIStateQBRangeAttack
* @date Modified April 7, 2006
*/
CAIStateQBRangeAttack::CAIStateQBRangeAttack(void)
{
	setStateType(CAIState::AIS_QBRANGEATTACK);
}

/**
* CAIStateQBRangeAttack::~CAIStateQBRangeAttack
* @date Modified April 7, 2006
*/
CAIStateQBRangeAttack::~CAIStateQBRangeAttack(void) {}

/**
* CAIStateQBRangeAttack::update
* @date Modified May 9, 2006
*/
void CAIStateQBRangeAttack::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	((CAnimatedMesh*)poCharacter->getMesh())->setAnimationSetByName("Attack");
	// look at the player
	D3DXVECTOR3 vAtGoal;
	CAINode* poGoalNode = CAIManager::getInstancePtr()->findBestGoal(poCharacter);
	D3DXVec3Subtract(&vAtGoal, &poGoalNode->getPosition(), &poCharacter->getPosition());
	D3DXVec3Normalize(&vAtGoal, &vAtGoal);
	poCharacter->setOrientation(vAtGoal);

	// wait for the animation to play
	if (poAIEntity->getCurrentStateTime() < ((CAnimatedMesh*)poCharacter->getMesh())->getAnimationLength())
		return;

	// spawn projectile
	CFootBall* poBall = (CFootBall*)CObjectManager::getInstancePtr()->createObject(OBJ_WEAPONPROJ_FOOTBALL);
	vAtGoal = poCharacter->getBV().centerPt;
	vAtGoal.y += 10.0f;
	poBall->setPosition(vAtGoal);
	D3DXVec3Subtract(&vAtGoal, &poGoalNode->getPosition(), &vAtGoal);
	D3DXVec3Normalize(NULL, &vAtGoal, &vAtGoal);
	poBall->setOrientation(vAtGoal);
	poBall->setVelocity(*D3DXVec3Scale(&vAtGoal, &vAtGoal, 50.0f));
	poBall->setBV(poBall->getPosition(), 2.0f);
	poBall->setPlayer((CPlayer*)poCharacter);
	
	((CEnemy*)(poCharacter))->setAIState(CAIStateQBMeleeAttack::getInstancePtr());
}

/**
* CAIStateQBRangeAttack::enter
* @date Modified April 13, 2006
*/
void CAIStateQBRangeAttack::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	poAIEntity->setCurrentStateTime(0.0);
}

/**
* CAIStateQBRangeAttack::exit
* @date Modified April 13, 2006
*/
void CAIStateQBRangeAttack::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}