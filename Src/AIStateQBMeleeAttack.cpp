/**
* @file AIStateQBMeleeAttack.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateQBMeleeAttack class
*/

#include "AIStateQBMeleeAttack.h"
#include "AIStateQBFollow.h"
#include "AIManager.h"
#include "QBZombie.h"

#define ATTACK_SPEED 40.0f

/**
* CAIStateQBMeleeAttack::CAIStateQBMeleeAttack
* @date Modified April 12, 2006
*/
CAIStateQBMeleeAttack::CAIStateQBMeleeAttack(void)
{
	setStateType(CAIState::AIS_QBMELEEATTACK);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));
}

/**
* CAIStateQBMeleeAttack::~CAIStateQBMeleeAttack
* @date Modified April 7, 2006
*/
CAIStateQBMeleeAttack::~CAIStateQBMeleeAttack(void) {}

/**
* CAIStateQBMeleeAttack::update
* @date Modified April 12, 2006
*/
void CAIStateQBMeleeAttack::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	// wait for the projectile to get there
	if (poAIEntity->getCurrentStateTime() < 2.0f)
		return;

	auto vel = poCharacter->getVelocity();
	if (memcmp(&m_vVelocity, &vel, sizeof(D3DXVECTOR3)) == 0)
	{
		D3DXVECTOR3 vGoalNode;
		auto goalPos = CAIManager::getInstancePtr()->findBestGoal(poCharacter)->getPosition();
		auto charPos = poCharacter->getBV().centerPt;
		D3DXVec3Subtract(&vGoalNode, &goalPos, &charPos);
		D3DXVec3Normalize(NULL, &vGoalNode, &vGoalNode);

		poCharacter->setOrientation(vGoalNode);
		D3DXVec3Scale(&vGoalNode, &vGoalNode, ATTACK_SPEED);
		poCharacter->setVelocity(vGoalNode);
	}

	if (poAIEntity->getCurrentStateTime() > 4.0f)
		((CEnemy*)(poCharacter))->setAIState(CAIStateQBFollow::getInstancePtr());

}

/**
* CAIStateQBMeleeAttack::enter
* @date Modified April 13, 2006
*/
void CAIStateQBMeleeAttack::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStateQBMeleeAttack::exit
* @date Modified April 13, 2006
*/
void CAIStateQBMeleeAttack::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}