/**
* @file AIStateAcidicFollow.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the implementation of the CAIStateAcidicFollow class
*/

#include "AIStateAcidicFollow.h"
#include "AIStateAcidicAttack.h"
#include "AIStatePathFollow.h"
#include "AIManager.h"
#include "Acidic.h"

#define PATH_BACK poAIEntity->m_loPath.back()
#define PATH_FRONT poAIEntity->m_loPath.front()

#define ACIDIC_BUFFER_ZONE 4.9f
#define ACIDIC_SPEED 25.0f
#define ACIDIC_CIRCLE_TIME 1.0f

/**
* CAIStateAcidicFollow::CAIStateAcidicFollow
* @date Modified April 12, 2006
*/
CAIStateAcidicFollow::CAIStateAcidicFollow(void)
{
	setStateType(CAIState::AIS_ACIDICFOLLOW);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));
	m_poCurrentPos = new CAINode;
}

/**
* CAIStateAcidicFollow::~CAIStateAcidicFollow
* @date Modified April 12, 2006
*/
CAIStateAcidicFollow::~CAIStateAcidicFollow(void)
{
	if (m_poCurrentPos)
		delete m_poCurrentPos;

	m_poCurrentPos = NULL;
}

/**
* CAIStateAcidicFollow::update
* @date Modified April 18, 2006
*/
void CAIStateAcidicFollow::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
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

	m_poCurrentPos->setPosition(poCharacter->getPosition());

	// ACIDIC SPECIFIC
	//////////////////

	// see if we are close enough
	float fDist = computeDistance(PATH_FRONT->getPosition(), poCharacter->getBV().centerPt);
	if (fDist < ((CAcidic*)(poCharacter))->getAttackDist() + ACIDIC_BUFFER_ZONE)
	{
		// needs to get to the center of the range before starting to circle
		if (fDist < ((CAcidic*)(poCharacter))->getAttackDist())
		{
			((CAcidic*)(poCharacter))->addCircleTime(CTimer::getInstance().getFrameTime());
		}

		// continue to go to the player
		D3DXVECTOR3 vNewVelocity;
		D3DXVec3Subtract(&vNewVelocity, &PATH_BACK->getPosition(), &poCharacter->getBV().centerPt);
		D3DXVec3Normalize(NULL, &vNewVelocity, &vNewVelocity);

		// only start to circle if we have time
		if (((CAcidic*)(poCharacter))->getCircleTime() > 0.0)
		{
			// add time for as long as we have gotten close enough to start timing
			((CAcidic*)(poCharacter))->addCircleTime(CTimer::getInstance().getFrameTime());

			// if we are far enough away, continue to circle
			if (fDist > (((CAcidic*)(poCharacter))->getAttackDist()))
			{
				// circle the player
				if (((CAcidic*)poCharacter)->getWay())
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

		// not fall through plane HACK
		vNewVelocity.y = 0.0f;
		poCharacter->setOrientation(vNewVelocity);
		D3DXVec3Scale(&vNewVelocity, &vNewVelocity, ACIDIC_SPEED);
		// set velocity
		poCharacter->setVelocity(vNewVelocity);

		// circled long enough to attack
		if (((CAcidic*)(poCharacter))->getCircleTime() > ACIDIC_CIRCLE_TIME)
		{
			// change to attack state
		
			// remove influences
			poCharacter->setVelocity(m_vVelocity);
			((CEnemy*)(poCharacter))->setAIState(CAIStateAcidicAttack::getInstancePtr());
			((CAcidic*)(poCharacter))->resetCircleTime();
		}

		return;
	}

	//////////////////

	CAIStatePathFollow::getInstancePtr()->followPath(poAIEntity, poCharacter, ACIDIC_SPEED);
}

/**
* CAIStateAcidicFollow::enter
* @date Modified April 13, 2006
*/
void CAIStateAcidicFollow::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	float fRandFloat = ((float)(rand()%200) + 300.0f) / 10.0f;
	((CAcidic*)(poCharacter))->setAttackDist(fRandFloat);
	((CAcidic*)(poCharacter))->setWay((rand()%2 == 1));
	((CAcidic*)(poCharacter))->resetCircleTime();
}

/**
* CAIStateAcidicFollow::exit
* @date Modified April 13, 2006
*/
void CAIStateAcidicFollow::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}