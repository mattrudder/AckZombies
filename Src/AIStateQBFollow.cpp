/**
* @file AIStateQBFollow.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateQBFollow class
*/

#include "AIStateQBFollow.h"
#include "AIStateQBRangeAttack.h"
#include "AIStateQBMeleeAttack.h"
#include "AIStatePathFollow.h"
#include "AIManager.h"
#include "QBZombie.h"

#define PATH_BACK poAIEntity->m_loPath.back()
#define PATH_FRONT poAIEntity->m_loPath.front()

#define QB_BUFFER_ZONE 4.9f
#define QB_SPEED 25.0f
#define QB_CIRCLE_TIME 1.0f

/**
* CAIStateQBFollow::CAIStateQBFollow
* @date Modified April 12, 2006
*/
CAIStateQBFollow::CAIStateQBFollow(void)
{
	setStateType(CAIState::AIS_QBFOLLOW);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));
	m_poCurrentPos = new CAINode;
}

/**
* CAIStateQBFollow::~CAIStateQBFollow
* @date Modified April 12, 2006
*/
CAIStateQBFollow::~CAIStateQBFollow(void)
{
	if (m_poCurrentPos)
		delete m_poCurrentPos;

	m_poCurrentPos = NULL;
}

/**
* CAIStateQBFollow::update
* @date Modified April 18, 2006
*/
void CAIStateQBFollow::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	// check for a valid path, one with nodes in it
	if (poAIEntity->m_loPath.empty())
	{
		// this is bad that there are no nodes to go to
		// remove our influence
		poCharacter->setVelocity(m_vVelocity);
		// but not for now
		((CEnemy*)(poCharacter))->setAIState(NULL);
		return;
	}

	m_poCurrentPos->setPosition(poCharacter->getBV().centerPt);

	// QB SPECIFIC
	//////////////////

	// see if we are close enough
	float fDist = computeDistance(PATH_FRONT->getPosition(), poCharacter->getBV().centerPt);
	if (fDist < ((CQBZombie*)(poCharacter))->getAttackDist() + QB_BUFFER_ZONE)
	{
		// needs to get to the center of the range before starting to circle
		if (fDist < ((CQBZombie*)(poCharacter))->getAttackDist())
		{
			((CQBZombie*)(poCharacter))->addCircleTime(CTimer::getInstance().getFrameTime());
		}

		// continue to go to the player
		D3DXVECTOR3 vNewVelocity;
		D3DXVec3Subtract(&vNewVelocity, &PATH_BACK->getPosition(), &poCharacter->getBV().centerPt);
		D3DXVec3Normalize(NULL, &vNewVelocity, &vNewVelocity);

		// only start to circle if we have time
		if (((CQBZombie*)(poCharacter))->getCircleTime() > 0.0)
		{
			// add time for as long as we have gotten close enough to start timing
			((CQBZombie*)(poCharacter))->addCircleTime(CTimer::getInstance().getFrameTime());

			// if we are far enough away, continue to circle
			if (fDist > (((CQBZombie*)(poCharacter))->getAttackDist()))
			{
				// circle the player
				if (((CQBZombie*)poCharacter)->getWay())
					D3DXVec3Cross(&vNewVelocity, &vNewVelocity, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				else
					D3DXVec3Cross(&vNewVelocity, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &vNewVelocity);
			}
			else
			{
				// move away from the player
				D3DXVec3Subtract(&vNewVelocity, &poCharacter->getBV().centerPt, &PATH_BACK->getPosition());
			}

			D3DXVec3Normalize(NULL, &vNewVelocity, &vNewVelocity);
		}

		poCharacter->setOrientation(vNewVelocity);
		D3DXVec3Scale(&vNewVelocity, &vNewVelocity, QB_SPEED);
		// set velocity
		poCharacter->setVelocity(vNewVelocity);

		// circled long enough to attack
		if (((CQBZombie*)(poCharacter))->getCircleTime() > QB_CIRCLE_TIME)
		{
			// change to attack state
		
			// remove influences
			poCharacter->setVelocity(m_vVelocity);
			((CEnemy*)(poCharacter))->setAIState(CAIStateQBRangeAttack::getInstancePtr());
			((CQBZombie*)(poCharacter))->resetCircleTime();
		}

		return;
	}

	//////////////////

	CAIStatePathFollow::getInstancePtr()->followPath(poAIEntity, poCharacter, QB_SPEED);
}

/**
* CAIStateQBFollow::enter
* @date Modified April 13, 2006
*/
void CAIStateQBFollow::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	float fRandFloat = ((float)(rand()%150) + 100.0f) / 10.0f;
	((CQBZombie*)(poCharacter))->setAttackDist(fRandFloat);
	((CQBZombie*)(poCharacter))->setWay((rand()%2 == 1));
	((CQBZombie*)(poCharacter))->resetCircleTime();
}

/**
* CAIStateQBFollow::exit
* @date Modified April 13, 2006
*/
void CAIStateQBFollow::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}