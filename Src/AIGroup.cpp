/**
* @file AIGroup.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 19, 2006
*
* This file contains the implementation of the CAIGroup class.
*/

// Local Includes
#include "AIGroup.h"
#include "AIManager.h"
#include "AIStatePathPlan.h"
#include "AIStateMove.h"
#include "AIStateFollowLeader.h"
#include "AIStateSpawn.h"

// System Includes
#include "Limits.h"

#define ENEMY_PTR(x) ((CEnemy*)(*x))

/**
* CAIGroup::CAIGroup
* @date Modified May 30, 2006
*/
CAIGroup::CAIGroup(void) : m_poLeader(NULL) {}

/**
* CAIGroup::~CAIGroup
* @date Modified April 19, 2006
*/
CAIGroup::~CAIGroup(void){}

/**
* CAIGroup::calculateAvgPos
* @date Modified May 11, 2006
*/
void CAIGroup::calculateAvgPos(void)
{
	std::list<CEnemy*>::iterator oEnemyIter = m_loEnemies.begin();
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	while (oEnemyIter != m_loEnemies.end())
	{
		vPos += ((CEnemy*)(*oEnemyIter))->getBV().centerPt;
		oEnemyIter++;
	}

	D3DXVec3Scale(&m_vAvgPos, &vPos, (1.0f / (float)m_loEnemies.size()));
}

/**
* CAIGroup::updateLeader
* @date Modified May 30, 2006
*/
void CAIGroup::updateLeader(bool bSpawn)
{
	// if this group has just been created, they need to spawn
	if (bSpawn)
	{
		std::list<CEnemy*>::iterator oEnemyIter = m_loEnemies.begin();
		// set all of there states to spawning
		while (oEnemyIter != m_loEnemies.end())
		{
			// tell them what group they are in
			((CEnemy*)(*oEnemyIter))->getAI()->setGroup(this);
			((CEnemy*)(*oEnemyIter))->setAIState(CAIStateSpawn::getInstancePtr());
			oEnemyIter++;
		}
		return;
	}

	// as long as there are members of the group still spawning don't update
	std::list<CEnemy*>::iterator oEnemyIter = m_loEnemies.begin();
	while (oEnemyIter != m_loEnemies.end())
	{
		if (((CEnemy*)(*oEnemyIter))->getStateType() == CAIState::AIS_SPAWN)
			return;

		oEnemyIter++;
	}

	CAIManager* poAIManager = CAIManager::getInstancePtr();
	CEnemy* poBestLeader;
	double dDist = FLT_MAX, dTempDist = FLT_MAX;

	// make sure the list is not empty
	if (m_loEnemies.empty())
	{
		m_poLeader = NULL;
		return;
	}
	oEnemyIter = m_loEnemies.begin();
	poAIManager->findBestGoal(*oEnemyIter, &dDist);
	poBestLeader = *oEnemyIter;
	oEnemyIter++;

	// find the member of the group closest to a goal
	while (oEnemyIter != m_loEnemies.end())
	{
		poAIManager->findBestGoal(*oEnemyIter, &dTempDist);

		if (dTempDist < dDist)
		{
			poBestLeader = *oEnemyIter;
			dDist = dTempDist;
		}
		oEnemyIter++;
	}

	// if the leader hasn't changed, just leave it and get out of this function
	if (poBestLeader == m_poLeader)
		return;

	setLeader(poBestLeader);

	oEnemyIter = m_loEnemies.begin();

	while (oEnemyIter != m_loEnemies.end())
	{
		if ((*oEnemyIter) != poBestLeader)
			(*oEnemyIter)->setAIState(CAIStateFollowLeader::getInstancePtr());

		(*oEnemyIter)->getAI()->setGroup(this);

		oEnemyIter++;
	}

	// now make that member a Leader
	poBestLeader->setAIState(CAIStatePathPlan::getInstancePtr());
}

/**
* CAIGroup::disband
* @date Modified April 26, 2006
*/
void CAIGroup::disband(bool bDisperse)
{
	std::list<CEnemy*>::iterator oEnemyIter = m_loEnemies.begin();
	CAIStatePathPlan* poPlan = CAIStatePathPlan::getInstancePtr();

	// for dispersal
	CAIStateMove* poMove = NULL;
	D3DXVECTOR3 vFrontBack(0.0f, 0.0f, 0.0f), vLeftRight(0.0f, 0.0f, 0.0f), vTemp(0.0f, 0.0f, 0.0f);
	CAINode* poGoalNode = NULL;
	float fDot = 0.0f;

	// if the enemies are to disperse, this information is needed
	if (bDisperse)
	{
		poMove = CAIStateMove::getInstancePtr();

		calculateAvgPos();
		poGoalNode = CAIManager::getInstancePtr()->findBestGoal(*oEnemyIter);
		// calculate vectors to perform a halfspace tests to determine members' position in the group
		D3DXVec3Subtract(&vFrontBack, &poGoalNode->getPosition(), &m_vAvgPos);
		D3DXVec3Normalize(NULL, &vFrontBack, &vFrontBack);
		D3DXVec3Cross(&vLeftRight, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &vLeftRight);
		D3DXVec3Normalize(NULL, &vLeftRight, &vLeftRight);
	}

	while (oEnemyIter != m_loEnemies.end())
	{
		(*oEnemyIter)->getAI()->setGroup(NULL);

		if (bDisperse)
		{
			if ((*oEnemyIter)->getType() == OBJ_ENEMY_ZOMBIECITIZEN)
			{
				D3DXVec3Subtract(&vTemp, &ENEMY_PTR(oEnemyIter)->getBV().centerPt, &m_vAvgPos);
				D3DXVec3Normalize(NULL, &vTemp, &vTemp);
				fDot = D3DXVec3Dot(&vTemp, &vFrontBack);

				// if this enemy is in back send him straight to the goal
				if (fDot <= 0.0f)
				{
					(*oEnemyIter)->setAIState(poPlan);
				}
				// if this enemy is in front, make him move forward
				else
				{
					ENEMY_PTR(oEnemyIter)->setOrientation(vTemp);
					D3DXVec3Scale(&vTemp, &vFrontBack, 10.0f);
					ENEMY_PTR(oEnemyIter)->setVelocity(vTemp);
					(*oEnemyIter)->setAIState(poMove);
				}
			}
			else
			{
				(*oEnemyIter)->setAIState(poPlan);
			}

		}
		else
		{
			(*oEnemyIter)->setAIState(poPlan);
		}
		
		oEnemyIter = m_loEnemies.erase(oEnemyIter);
	}
}

/**
* CAIGroup::remove
* @date Modified April 30, 2006
*/
void CAIGroup::remove(CEnemy* poEnemy)
{
	std::list<CEnemy*>::iterator oEnemyIter = m_loEnemies.begin();
	while (oEnemyIter != m_loEnemies.end())
	{
		if ((*oEnemyIter) == poEnemy)
		{
			oEnemyIter = m_loEnemies.erase(oEnemyIter);
			break;
		}
		oEnemyIter++;
	}

	// check for removing the leader
	if (m_poLeader == poEnemy)
	{
		updateLeader();
	}
}