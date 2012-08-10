/**
* @file AIStateAcidicAttack.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateAcidicAttack class
*/

#include "AIStateAcidicAttack.h"
#include "AIStateAcidicFollow.h"
#include "IceCream.h"
#include "AIManager.h"
#include "Acidic.h"

/**
* CAIStateAcidicAttack::CAIStateAcidicAttack
* @date Modified April 7, 2006
*/
CAIStateAcidicAttack::CAIStateAcidicAttack(void)
{
	setStateType(CAIState::AIS_ACIDICATTACK);
}

/**
* CAIStateAcidicAttack::~CAIStateAcidicAttack
* @date Modified April 7, 2006
*/
CAIStateAcidicAttack::~CAIStateAcidicAttack(void) {}

/**
* CAIStateAcidicAttack::update
* @date Modified May 4, 2006
*/
void CAIStateAcidicAttack::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	D3DCOLOR color = poCharacter->getColor();
	if (color == 0xff000000)
		poCharacter->setColor(0xff005555);
	else
		poCharacter->setColor(0xff000000);

	// look at the player
	D3DXVECTOR3 vAtGoal;
	CAINode* poGoalNode = CAIManager::getInstancePtr()->findBestGoal(poCharacter);
	D3DXVec3Subtract(&vAtGoal, &poGoalNode->getPosition(), &poCharacter->getPosition());
	D3DXVec3Normalize(&vAtGoal, &vAtGoal);
	poCharacter->setOrientation(vAtGoal);

	// wait so we can flash a bit and then throw
	if (poAIEntity->getCurrentStateTime() < 0.5)
		return;

	((CAnimatedMesh*)poCharacter->getMesh())->setAnimationSetByName("Attack");

	// wait for the animation to play
	if (poAIEntity->getCurrentStateTime() < (((CAnimatedMesh*)poCharacter->getMesh())->getAnimationLength() + 0.5))
		return;

	// spawn projectile
	CIceCream* poBall = (CIceCream*)CObjectManager::getInstancePtr()->createObject(OBJ_WEAPONPROJ_ACIDICE);
	vAtGoal = poCharacter->getBV().centerPt;
	vAtGoal.y += 10.0f;
	poBall->setPosition(vAtGoal);
	D3DXVec3Subtract(&vAtGoal, &poGoalNode->getPosition(), &vAtGoal);
	D3DXVec3Normalize(NULL, &vAtGoal, &vAtGoal);
	poBall->setOrientation(vAtGoal);
	poBall->setVelocity(*D3DXVec3Scale(&vAtGoal, &vAtGoal, 50.0f));
	poBall->setBV(poBall->getPosition(), 2.0f);
	poBall->setPlayer((CPlayer*)poCharacter);
	
	((CEnemy*)(poCharacter))->setAIState(CAIStateAcidicFollow::getInstancePtr());
}

/**
* CAIStateAcidicAttack::enter
* @date Modified April 13, 2006
*/
void CAIStateAcidicAttack::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	poAIEntity->setCurrentStateTime(0.0);
}

/**
* CAIStateAcidicAttack::exit
* @date Modified April 13, 2006
*/
void CAIStateAcidicAttack::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	poCharacter->setColor(0xff000000);
	((CAnimatedMesh*)poCharacter->getMesh())->setAnimationSetByName("Walk");
}