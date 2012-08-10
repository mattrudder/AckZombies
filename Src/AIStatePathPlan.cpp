/**
* @file AIStatePathPlan.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
*
* This file contains the implementation of the CAIStatePathPlan class.
*/

// Local Includes
#include "AIStatePathPlan.h"
#include "AIStatePathFollow.h"
#include "AIStateGaseousFollow.h"
#include "AIStateAcidicFollow.h"
#include "AIStateQBFollow.h"
#include "AIStateStraightToGoal.h"
#include "AIManager.h"
#include "ActorIncludes.h"

#ifndef NULL
#define NULL 0L
#endif

/**
* CAIStatePathPlan::CAIStatePathPlan
* @date Modified March 28, 2006
*/
CAIStatePathPlan::CAIStatePathPlan(void)
{
	setStateType(CAIState::AIS_PATHPLAN);

	m_poStartNode = new CAINode;
}

/**
* CAIStatePathPlan::~CAIStatePathPlan
* @date Modified March 28, 2006
*/
CAIStatePathPlan::~CAIStatePathPlan(void)
{
	if (m_poStartNode)
		delete m_poStartNode;

	m_poStartNode = NULL;
}

/**
* CAIStatePathPlan::update
* @date Modified May 4, 2006
*/
void CAIStatePathPlan::update(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
	CAIManager* pAIManager = CAIManager::getInstancePtr();

	// set up the goal
	m_poGoalNode = pAIManager->findBestGoal(poCharacter);

	// try to update the goals if they are dirty
	if (pAIManager->getUpdateGoalLinks())
		pAIManager->update();

	// clear out all the old info
	/////////////////////////////

	poAIEntity->m_loPath.clear();
	m_loOpenNodes.clear();
	m_loClosedNodes.clear();

	// create a node at the actor's position and set up the links
	m_poStartNode->setPosition(poCharacter->getBV().centerPt);
	pAIManager->unlinkNode(m_poStartNode);

	// test a direct link to the goal
	if (pAIManager->testDirectLink(m_poStartNode, m_poGoalNode))
	{
		poAIEntity->m_loPath.push_back(m_poGoalNode);

		// put the entity into the follow state
		EObjectType eType = poCharacter->getType();
		switch (eType)
		{
		case OBJ_ENEMY_ZOMBIECITIZEN:
			((CEnemy*)(poCharacter))->setAIState(CAIStatePathFollow::getInstancePtr());
			break;
		case OBJ_ENEMY_GASEOUSCLAY:
			((CEnemy*)(poCharacter))->setAIState(CAIStateGaseousFollow::getInstancePtr());
			break;
		case OBJ_ENEMY_ICECREAMMAN:
			{
				((CEnemy*)(poCharacter))->setAIState(CAIStateAcidicFollow::getInstancePtr());
				break;
			}
		case OBJ_ENEMY_QUARTERBACK:
			((CEnemy*)(poCharacter))->setAIState(CAIStateQBFollow::getInstancePtr());
			break;
		}
		return;
	}

	pAIManager->linkNode(m_poStartNode);

	// if we don't have any links we have gone through geometry and we are screwed
	// if the goal has no links he has gone through geometry and the entire game is screwed
	if (m_poStartNode->m_loLinks.empty() || m_poGoalNode->m_loLinks.empty())
	{
		// set ourselves for deletion and get out of here
		CObjectManager::getInstance().removeObject(poCharacter);
		return;
	}

	// pre-plan initializations
	CAINode* poCurrentNode = m_poStartNode;
	float fDistanceToGoal = 0.0f;
	// Links Iterator
	NODE_LIST_ITER oNodeIter;
	// Open and Closed list iterator
	NODE_LIST_ITER oListIter;

	// start the path planning
	//////////////////////////

	m_loOpenNodes.push_back(poCurrentNode);

	// continue as long as there are paths to explore
	while (!m_loOpenNodes.empty())
	{
		poCurrentNode = m_loOpenNodes.front();
		m_loOpenNodes.pop_front();
		m_loClosedNodes.push_back(poCurrentNode);

		// is this current node the goal
		if (poCurrentNode == m_poGoalNode)
		{
			// it is so fill up the solution path and get out
			/////////////////////////////////////////////////
			
			// don't add the start node for the same reason as the goal node
			while (poCurrentNode != m_poStartNode)
			{
				poAIEntity->m_loPath.push_back(poCurrentNode);
				poCurrentNode = poCurrentNode->m_poParent;
			}

			// get while the gettin' 's good
			break;
		}

		// add the children to the current list
		///////////////////////////////////////

		// Links Iterator
		oNodeIter = poCurrentNode->m_loLinks.begin();

		// go through every link this node has
		while (oNodeIter != poCurrentNode->m_loLinks.end())
		{
			// check the open list first because it is smaller
			oListIter = checkList(&m_loOpenNodes, (*oNodeIter));

			// is it not in the open list
			if (oListIter == m_loOpenNodes.end())
			{
				// check the closed list not
				oListIter = checkList(&m_loClosedNodes, (*oNodeIter));

				// is it not in the closed list
				if (oListIter == m_loClosedNodes.end())
				{
					// insert node into the open list
					/////////////////////////////////

					// set parent
					(*oNodeIter)->m_poParent = poCurrentNode;
					// set the distance
					(*oNodeIter)->m_fDistanceToGoal = computeDistanceSquared((*oNodeIter)->getPosition(), m_poGoalNode->getPosition());
					// prioritized insert
					insertOpenList(*oNodeIter);					
				}
			}

			oNodeIter++;
		}
	}

	// put the entity into the follow state
	EObjectType eType = poCharacter->getType();
	switch (eType)
	{
	case OBJ_ENEMY_ZOMBIECITIZEN:
		((CEnemy*)(poCharacter))->setAIState(CAIStatePathFollow::getInstancePtr());
		break;
	case OBJ_ENEMY_GASEOUSCLAY:
		((CEnemy*)(poCharacter))->setAIState(CAIStateGaseousFollow::getInstancePtr());
		break;
	case OBJ_ENEMY_ICECREAMMAN:
		((CEnemy*)(poCharacter))->setAIState(CAIStateAcidicFollow::getInstancePtr());
		break;
	case OBJ_ENEMY_QUARTERBACK:
		((CEnemy*)(poCharacter))->setAIState(CAIStateQBFollow::getInstancePtr());
		break;
	}
}

/**
* CAIStatePathPlan::enter
* @date Modified April 13, 2006
*/
void CAIStatePathPlan::enter(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStatePathPlan::exit
* @date Modified April 13, 2006
*/
void CAIStatePathPlan::exit(CAIEntity* poAIEntity, CCharacter* poCharacter)
{
}

/**
* CAIStatePathPlan::checkList
* @date Modified March 28, 2006
*/
NODE_LIST_ITER CAIStatePathPlan::checkList(std::list<CAINode*>* poList, CAINode* poNode)
{
	NODE_LIST_ITER NodeIter = poList->begin();

	while (NodeIter != poList->end())
	{
		if ((*NodeIter) == poNode)
			return NodeIter;

		NodeIter++;
	}

	return NodeIter;
}

/**
* CAIStatePathPlan::insertOpenList
* @date Modified March 28, 2006
*/
void CAIStatePathPlan::insertOpenList(CAINode* poNode)
{
	NODE_LIST_ITER oNodeIter = m_loOpenNodes.begin();

	while (oNodeIter != m_loOpenNodes.end())
	{
		if (poNode->m_fDistanceToGoal < (*oNodeIter)->m_fDistanceToGoal)
		{
			m_loOpenNodes.insert(oNodeIter, poNode);
			return;
		}

		oNodeIter++;
	}

	if (m_loOpenNodes.empty())
	{
		m_loOpenNodes.insert(oNodeIter, poNode);
	}
}