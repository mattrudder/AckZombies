/**
* @file AIStateGaseousFollow.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateGaseousFollow class
*/

#include "AIStateGaseousFollow.h"
#include "AIStateGaseousAttack.h"
#include "AIStatePathFollow.h"
#include "AIManager.h"
#include "Gaseous.h"

#define PATH_BACK poAIEntity->m_loPath.back()
#define PATH_FRONT poAIEntity->m_loPath.front()

#define GASEOUS_ATTACK_RADIUS 100.0f // this is squared
#define SPEED 10.0f

#ifndef NULL
#define NULL 0
#endif

/**
* CAIStateGaseousFollow::CAIStateGaseousFollow
* @date Modified May 4, 2006
*/
CAIStateGaseousFollow::CAIStateGaseousFollow(void)
{
	setStateType(CAIState::AIS_GASEOUSFOLLOW);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));
}

/**
* CAIStateGaseousFollow::~CAIStateGaseousFollow
* @date Modified May 4, 2006
*/
CAIStateGaseousFollow::~CAIStateGaseousFollow(void)
{
}

/**
* CAIStateGaseousFollow::update
* @date Modified May 4, 2006
*/
void CAIStateGaseousFollow::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	// right now this class is mostly the same as the zombie path follow
	// but this would be the place to add something about trying to stay away
	// from other enemies to avoid friendly fire

	// check for a valid path, one with nodes in it
	if (poAIEntity->m_loPath.empty())
	{
		// this is bad that there are no nodes to go to
		// remove our influence
		poCharacter->setVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		// but not for now
		((CEnemy*)(poCharacter))->setAIState(NULL);
		return;
	}

	// GASEOUS SPECIFIC
	///////////////////

	// see if we are close enough
	if (computeDistanceSquared(PATH_FRONT->getPosition(), poCharacter->getBV().centerPt) < GASEOUS_ATTACK_RADIUS)
	{
		// change to attack state
		
		// remove influences
		poCharacter->setVelocity(m_vVelocity);
		((CEnemy*)(poCharacter))->setAIState(CAIStateGaseousAttack::getInstancePtr());
		return;
	}

	///////////////////

	CAIStatePathFollow::getInstancePtr()->followPath(poAIEntity, poCharacter, SPEED);
}

/**
* CAIStateGaseousFollow::enter
* @date Modified April 13, 2006
*/
void CAIStateGaseousFollow::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStateGaseousFollow::exit
* @date Modified April 13, 2006
*/
void CAIStateGaseousFollow::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}