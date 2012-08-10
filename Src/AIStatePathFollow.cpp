/**
* @file AIStatePathFollow.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the implementation of the CAIStatePathFollow class.
*/

// Local Includes
#include "AIStatePathFollow.h"
#include "AIStatePathPlan.h"
#include "AIManager.h"
#include "Character.h"

#define PATH_BACK poAIEntity->m_loPath.back()
#define PATH_FRONT poAIEntity->m_loPath.front()

/**
* CAIStatePathFollow::CAIStatePathFollow
* @date Modified March 27, 2006
*/
CAIStatePathFollow::CAIStatePathFollow(void)
{
	setStateType(CAIState::AIS_PATHFOLLOW);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));
}

/**
* CAIStatePathFollow::~CAIStatePathFollow
* @date Modified March 27, 2006
*/
CAIStatePathFollow::~CAIStatePathFollow(void) {}

/**
* CAIStatePathFollow::update
* @date Modified April 18, 2006
*/
void CAIStatePathFollow::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	followPath(poAIEntity, poCharacter, 10.0f);
}

/**
* CAIStatePathFollow::followPath
* @dateModified May 4, 2006
*/
void CAIStatePathFollow::followPath(CAIEntity* poAIEntity, CCharacter* poCharacter, float fSpeed)
{
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

	// if it isn't moving to a node yet, start the movement
	if (memcmp(&m_vVelocity, &poCharacter->getVelocity(), sizeof(D3DXVECTOR3)) == 0)
	{
		// get vector from current position to next node
		D3DXVECTOR3 vNextNode;
		D3DXVec3Subtract(&vNextNode, &PATH_BACK->getPosition(), &poCharacter->getBV().centerPt);
		D3DXVec3Normalize(NULL, &vNextNode, &vNextNode);

		// scale by speed
		poCharacter->setOrientation(vNextNode);
		D3DXVec3Scale(&vNextNode, &vNextNode, fSpeed);

		// set velocity
		poCharacter->setVelocity(vNextNode);
	}
	// check to see if we should target to the next node
	else if (computeDistanceSquared(poCharacter->getBV().centerPt, PATH_BACK->getPosition()) < (PATH_BACK->m_fRadius * PATH_BACK->m_fRadius))
	{
		// we need to go to the next node
		poAIEntity->m_loPath.pop_back();

		// are we now at the goal
		if (poAIEntity->m_loPath.empty())
		{
			// we are at the goal
			// remove influence
			poCharacter->setVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			// but not for now
			((CEnemy*)(poCharacter))->setAIState(NULL);
			return;
		}

		// loose movement around the nodes
		//////////////////////////////////

		D3DXVECTOR3 vNextNode, vGoalNode;
		// get vector from entity to next node
		D3DXVec3Subtract(&vNextNode, &PATH_BACK->getPosition(), &poCharacter->getBV().centerPt);
		// get vector from entity to goal node
		D3DXVec3Subtract(&vGoalNode, &PATH_FRONT->getPosition(), &poCharacter->getBV().centerPt);

		D3DXVec3Normalize(NULL, &vGoalNode, &vGoalNode);

		// project vector from entity to next node onto vector from entity to goal node
		float fProjection = D3DXVec3Dot(&vNextNode, &vGoalNode);
		// store the position there now
		vNextNode = PATH_BACK->getPosition();
		// scale vector from entity to goal node by the projection value
		D3DXVec3Scale(&vGoalNode, &vGoalNode, fProjection);
		// add vector from entity to goal node to get point closest to next node
		D3DXVec3Add(&vGoalNode, &poCharacter->getBV().centerPt, &vGoalNode);
		// get the vector from the next node to the closest point
		D3DXVec3Subtract(&vGoalNode, &vGoalNode, &vNextNode);

		// if the magnitude of the vector between the next node and the closest point
		// is greater than the radius of the node, do some randomization
		if (D3DXVec3Length(&vGoalNode) > PATH_BACK->m_fRadius)
		{
			D3DXVec3Normalize(NULL, &vGoalNode, &vGoalNode);
			
			int nRandInt = (int)(PATH_BACK->m_fRadius * 10);
			float fRandFloat = (float)(rand()%nRandInt) / 10.0f;
			D3DXVec3Scale(&vGoalNode, &vGoalNode, fRandFloat);
		}

		// add vector from next node to point within radius
		D3DXVec3Add(&vNextNode, &vNextNode, &vGoalNode);
		// get vector from entity to point within radius to be new velocity
		D3DXVec3Subtract(&vGoalNode, &vNextNode, &poCharacter->getBV().centerPt);

		// normalize and scale by speed
		poCharacter->setOrientation(vGoalNode);
		D3DXVec3Normalize(NULL, &vGoalNode, &vGoalNode);
		D3DXVec3Scale(&vGoalNode, &vGoalNode, fSpeed);

		vGoalNode.y = 0.0f;

		// set velocity
		poCharacter->setVelocity(vGoalNode);
		return;
	}

	// have we been following for too long that the goal may have moved
	if (poAIEntity->getCurrentStateTime() > 0.25f)
	{
		// remove our influence
		poCharacter->setVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		// should probably path again
		((CEnemy*)(poCharacter))->setAIState(CAIStatePathPlan::getInstancePtr());
	}
}

/**
* CAIStatePathFollow::enter
* @date Modified April 13, 2006
*/
void CAIStatePathFollow::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStatePathFollow::exit
* @date Modified April 13, 2006
*/
void CAIStatePathFollow::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}
