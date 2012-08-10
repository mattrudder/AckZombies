/**
* @file AIStateFollowLeader.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 3, 2006
*
* This file contains the implementation of the CAIStateFollowLeader class.
*/

// Local Includes
#include "AIStateFollowLeader.h"
#include "AIStatePathPlan.h"
#include "AIManager.h"
#include "Enemy.h"

/**
* CAIStateFollowLeader::CAIStateFollowLeader
* @date Modified April 5, 2006
*/
CAIStateFollowLeader::CAIStateFollowLeader(void)
{
	setStateType(CAIState::AIS_FOLLOWLEADER);
	memset(&m_vVelocity, 0, sizeof(D3DXVECTOR3));

	m_poNode = new CAINode;
}

/**
* CAIStateFollowLeader::~CAIStateFollowLeader
* @date Modified April 5, 2006
*/
CAIStateFollowLeader::~CAIStateFollowLeader(void)
{
	if (m_poNode)
		delete m_poNode;

	m_poNode;
}

/**
* CAIStateFollowLeader::update
* @date Modified May 4, 2006
*/
void CAIStateFollowLeader::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	CAIManager* poAIManager = CAIManager::getInstancePtr();

	// make sure the Group is active
	if (poAIEntity->getGroup())
	{
		// just take the leader's velocity
		poCharacter->setVelocity(poAIEntity->getGroup()->getLeader()->getVelocity());
		poCharacter->setOrientation(poAIEntity->getGroup()->getLeader()->getOrientation());
	}
	// if we are in the leader follow state and we have no leader, we have a problem
	else
	{
		// start looking for a path
		((CEnemy*)(poCharacter))->setAIState(CAIStatePathPlan::getInstancePtr());
	}
}

/**
* CAIStateFollowLeader::enter
* @date Modified April 13, 2006
*/
void CAIStateFollowLeader::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStateFollowLeader::exit
* @date Modified April 13, 2006
*/
void CAIStateFollowLeader::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}