/**
* @file AIStateStraightToGoal.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
*
* This file contains the implementation of the CAIStateStraightToGoal class.
*/

// Local Includes
#include "AIStateStraightToGoal.h"
#include "AIStatePathPlan.h"
#include "AIManager.h"

/**
* CAIStateStraightToGoal::CAIStateStraightToGoal
* @date Modified April 27, 2006
*/
CAIStateStraightToGoal::CAIStateStraightToGoal(void)
{
	setStateType(CAIState::AIS_STRAIGHTTOGOAL);
}

/**
* CAIStateStraightToGoal::~CAIStateStraightToGoal
* @date Modified April 27, 2006
*/
CAIStateStraightToGoal::~CAIStateStraightToGoal(void)
{
}

/**
* CAIStateStraightToGoal::update
* @date Modified April 27, 2006
*/
void CAIStateStraightToGoal::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	CAIManager* poAIManager = CAIManager::getInstancePtr();

	D3DXVECTOR3 vVelocity;
	D3DXVec3Subtract(&vVelocity, &poAIManager->findBestGoal(poCharacter)->getPosition(), &poCharacter->getPosition());
	D3DXVec3Normalize(NULL, &vVelocity, &vVelocity);
	
	switch (poCharacter->getType())
	{
		// fall through on purpose
	case OBJ_ENEMY_ZOMBIECITIZEN:
	case OBJ_ENEMY_GASEOUSCLAY:
		D3DXVec3Scale(&vVelocity, &vVelocity, 10.0f);
		break;
		// fall through on purpose
	case OBJ_ENEMY_ICECREAMMAN:
	case OBJ_ENEMY_QUARTERBACK:
		D3DXVec3Scale(&vVelocity, &vVelocity, 25.0f);
		break;
	}

	poCharacter->setVelocity(vVelocity);
}

/**
* CAIStateStraightToGoal::enter
* @date Modified April 27, 2006
*/
void CAIStateStraightToGoal::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStateStraightToGoal::exit
* @date Modified April 27, 2006
*/
void CAIStateStraightToGoal::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}