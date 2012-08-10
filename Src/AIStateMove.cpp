/**
* @file AIStateMove.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 3, 2006
*
* This file contains the implementation of the CAIStateMove class.
*/

// Local Includes
#include "AIStateMove.h"
#include "AIStatePathPlan.h"
#include "AIManager.h"
#include "Enemy.h"

#define ZOMBIE_SPEED 10.0f
#define GASEOUS_SPEED 10.0f
#define ACIDIC_SPEED 25.0f
#define QB_SPEED 25.0f

/**
* CAIStateMove::CAIStateMove
* @date Modified April 21, 2006
*/
CAIStateMove::CAIStateMove(void)
{
	setStateType(CAIState::AIS_FOLLOWLEADER);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));
}

/**
* CAIStateMove::~CAIStateMove
* @date Modified April 21, 2006
*/
CAIStateMove::~CAIStateMove(void)
{
}

/**
* CAIStateMove::update
* @date Modified April 26, 2006
*/
void CAIStateMove::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	if (poAIEntity->getCurrentStateTime() > 3.0f)
	{
		// just follow current velocity for 3 seconds, then try to go straight to the goal
		((CEnemy*)poCharacter)->setAIState(CAIStatePathPlan::getInstancePtr());
	}
}

/**
* CAIStateMove::enter
* @date Modified April 21, 2006
*/
void CAIStateMove::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	D3DXVECTOR3 vVelocity(0.0f, 0.0f, 0.0f);

	D3DXVec3Normalize(NULL, &vVelocity, &poCharacter->getVelocity());
	poCharacter->setOrientation(vVelocity);

	// scale the unit velocity based on the entity's type
	switch (poCharacter->getType())
	{
	case OBJ_ENEMY_ZOMBIECITIZEN:
		D3DXVec3Scale(&vVelocity, &vVelocity, ZOMBIE_SPEED);
		break;
	case OBJ_ENEMY_GASEOUSCLAY:
		D3DXVec3Scale(&vVelocity, &vVelocity, GASEOUS_SPEED);
		break;
	case OBJ_ENEMY_ICECREAMMAN:
		D3DXVec3Scale(&vVelocity, &vVelocity, ACIDIC_SPEED);
		break;
	case OBJ_ENEMY_QUARTERBACK:
		D3DXVec3Scale(&vVelocity, &vVelocity, QB_SPEED);
		break;
	}

	poCharacter->setVelocity(vVelocity);
}

/**
* CAIStateMove::exit
* @date Modified April 21, 2006
*/
void CAIStateMove::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}