/**
* @file AIManager.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the implementation of the CAIManager class.
*/

// Local Includes
#include "AIManager.h"
#include "AIStatePathPlan.h"
#include "AIStatePathFollow.h"
#include "AIStateFollowLeader.h"
#include "AIStateAcidicFollow.h"
#include "AIStateAcidicAttack.h"
#include "AIStateGaseousFollow.h"
#include "AIStateGaseousAttack.h"
#include "AIStateZombieAttack.h"
#include "AIStateSpawn.h"
#include "AIStateQBFollow.h"
#include "AIStateQBMeleeAttack.h"
#include "AIStateQBRangeAttack.h"
#include "AIStateMove.h"
#include "AIStateStraightToGoal.h"
#include "Math3D.h"
#include "CollisionGeometry.h"
#include "Player.h"
#include "GameKernel.h"
#include "WinState.h"
#include "SoundManager.h"

// System Includes
#include "Limits.h"

// cast an iterator to a Node*
#define NODE_PTR(x) ((CAINode*)(*x))
#define ACTOR_PTR(x) ((CActor*)(*x))
#define ENEMY_PTR(x) ((CEnemy*)(*x))
#define SPAWN_PTR(x) ((CActorSpawn*)(*x))
#define TRIGGER_PTR(x) ((CActorSpawnTrigger*)(*x))
#define OBJECTIVE_PTR(x) ((CObjectiveTrigger*)(*x))
#define BARRIER_PTR(x) ((CActor*)(*x))

#define SPAWN_SPACEING 5.0f

#define MAX_LINK_DISTANCE 150.0f

#ifndef NULL
#define NULL 0
#endif

/**
* CAIManager::CAIManager
* @date Modified March 30, 2006
*/
CAIManager::CAIManager(void) : m_bUpdateGoalLinks(true), m_bSkipCurrentObjective(false)
{
	// go to the object manager and get the players and create nodes for them
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CObjectManager::ObjectList loPlayers;
	poObjectManager->getObjects(OBJ_PLAYER, &loPlayers);

	CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();

	while (oPlayerIter != loPlayers.end())
	{
		CAINode* newNode = new CAINode;
		newNode->setPosition(ACTOR_PTR(oPlayerIter)->getBV().centerPt);
		newNode->setRadius(1.0f);
		m_loGoals.push_back(newNode);
		oPlayerIter++;
	}

	m_oLeaderTimer.setInterval(0.1f);
	m_oObjectiveTimer.setInterval(0.15f);
	m_poCurrentObjective = NULL;

	m_poObjectiveEmitter = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_poObjectiveEmitter->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "objective.xml"));
}

/**
* CAIManager::~CAIManager
* @date Modified March 30, 2006
*/
CAIManager::~CAIManager(void)
{
	CAIStateAcidicAttack::deleteInstance();
	CAIStateAcidicFollow::deleteInstance();
	CAIStateFollowLeader::deleteInstance();
	CAIStateGaseousAttack::deleteInstance();
	CAIStateGaseousFollow::deleteInstance();
	CAIStateMove::deleteInstance();
	CAIStatePathFollow::deleteInstance();
	CAIStatePathPlan::deleteInstance();
	CAIStateQBFollow::deleteInstance();
	CAIStateQBRangeAttack::deleteInstance();
	CAIStateQBMeleeAttack::deleteInstance();
	CAIStateStraightToGoal::deleteInstance();
	CAIStateZombieAttack::deleteInstance();
	CAIStateSpawn::deleteInstance();

	while (!m_loGoals.empty())
	{
		delete m_loGoals.back();
		m_loGoals.pop_back();
	}

	m_poCurrentObjective = NULL;
}

/**
* CAIManager::update
* @date Modified April 26, 2006
*/
void CAIManager::update(void)
{
	// go to the object manager and get the players and update the nodes corresponding to them
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CObjectManager::ObjectList loPlayers;
	poObjectManager->getObjects(OBJ_PLAYER, &loPlayers);
	CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();

	// now that we have the players, check to see it there are any cluckin bombs to add as goals
	CObjectManager::ObjectList loBombs;
	poObjectManager->getObjects(OBJ_WEAPON_BIGCLUCKINGBOMB, &loBombs);
	CObjectManager::ObjectList::iterator oBombIter = loBombs.begin();
	while (oBombIter != loBombs.end())
	{
		loPlayers.push_back((*oBombIter));
		oBombIter++;
	}

	std::list<CAINode*>::iterator oNodeIter = m_loGoals.begin();

	bool bUpdateGoalLinks = false;
	if (m_bUpdateGoalLinks)
	{
		bUpdateGoalLinks = true;
		m_bUpdateGoalLinks = false;
	}

	while (oNodeIter != m_loGoals.end() && oPlayerIter != loPlayers.end())
	{
		// to cover the case of a big cluckin bomb, keep the goal node off the ground
		(*oNodeIter)->setPosition(D3DXVECTOR3(ACTOR_PTR(oPlayerIter)->getBV().centerPt.x, ((CPlayer*)loPlayers.front())->getBV().centerPt.y, ACTOR_PTR(oPlayerIter)->getBV().centerPt.z));

		if (bUpdateGoalLinks)
		{
			unlinkNode(*oNodeIter);
			linkNode(*oNodeIter);
		}			

		oNodeIter++;
		oPlayerIter++;
	}

	// cases where a player quits
	if (oNodeIter != m_loGoals.end())
	{
		while (oNodeIter != m_loGoals.end())
		{
			unlinkNode(*oNodeIter);
			delete *oNodeIter;
			oNodeIter = m_loGoals.erase(oNodeIter);
		}
	}
	
	// case where a player joins
	if (oPlayerIter != loPlayers.end())
	{
		while (oPlayerIter != loPlayers.end())
		{
			CAINode* newNode = new CAINode;
			// to cover the case of a big cluckin bomb, keep the goal node off the ground
			newNode->setPosition(D3DXVECTOR3(ACTOR_PTR(oPlayerIter)->getBV().centerPt.x, ((CPlayer*)loPlayers.front())->getBV().centerPt.y, ACTOR_PTR(oPlayerIter)->getBV().centerPt.z));
			newNode->setRadius(1.0f);
			m_loGoals.push_back(newNode);
			linkNode(newNode);
			oPlayerIter++;
		}
	}

	updateGroupLeaders();
	updateSpawnTriggers();
	updateCurrentObjective();

	// make sure this gets reset every time or the objectives will skip to gameover
	m_bSkipCurrentObjective = false;
}

/**
* CAIManager::createNodeLinks
* @date Modified April 26, 2006
*/
void CAIManager::createNodeLinks(void)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CCollisionGeometry* poGeo = CCollisionGeometry::getInstancePtr();

	CObjectManager::ObjectList oNodeList;
	poObjectManager->getObjects(OBJ_AI_NODE, &oNodeList);
	CObjectManager::ObjectList::iterator PrimaryIter, SecondIter;

	float fLength = 0.0f;
	D3DXVECTOR3 vDirection;
	std::vector<D3DXVECTOR3> vPoints;
	std::vector<float> vLengths;
	bool bLink = true;

	for (PrimaryIter = oNodeList.begin(); PrimaryIter != oNodeList.end(); ++PrimaryIter)
	{
		for (SecondIter = oNodeList.begin(); SecondIter != oNodeList.end(); ++SecondIter)
		{
			bLink = true;

			// don't check against same node
			if (NODE_PTR(PrimaryIter) == NODE_PTR(SecondIter))
				continue;

			D3DXVec3Subtract(&vDirection, &NODE_PTR(SecondIter)->getPosition(), &NODE_PTR(PrimaryIter)->getPosition());
			D3DXVec3Normalize(&fLength, &vDirection, &vDirection);

			// if the node is too far away, don't even try to link to it
			if (fLength > (MAX_LINK_DISTANCE + NODE_PTR(PrimaryIter)->m_fRadius + NODE_PTR(SecondIter)->m_fRadius))
				continue;

			// collision check
			if (poGeo->checkRayCollision(vPoints, vLengths, poGeo->getRoot(), NODE_PTR(PrimaryIter)->getPosition(), vDirection))
			{
				// have to check to see if the collision happened after the node we want to link to
				for (unsigned int i = 0; i < vLengths.size(); ++i)
				{
					if (vLengths[i] < fLength)
					{
						// no link
						bLink = false;
						break;
					}
				}

				if (bLink)
				{
					NODE_PTR(PrimaryIter)->m_loLinks.push_back(NODE_PTR(SecondIter));
				}
			}
			// not a single collision so they can link
			else
			{
				NODE_PTR(PrimaryIter)->m_loLinks.push_back(NODE_PTR(SecondIter));
			}

			vPoints.clear();
			vLengths.clear();			
		}
	}
}

/**
* CAIManager::linkNode
* @date Modified April 26, 2006
*/
void CAIManager::linkNode(CAINode* poNode)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CCollisionGeometry* poGeo = CCollisionGeometry::getInstancePtr();

	CObjectManager::ObjectList oNodeList;
	poObjectManager->getObjects(OBJ_AI_NODE, &oNodeList);
	CObjectManager::ObjectList::iterator oNodeIter;

	float fLength = 0.0f;
	D3DXVECTOR3 vDirection;
	std::vector<D3DXVECTOR3> vPoints;
	std::vector<float> vLengths;
	bool bLink = true;

	for (oNodeIter = oNodeList.begin(); oNodeIter != oNodeList.end(); ++oNodeIter)
	{
		bLink = true;

		// don't check against same node
		if (poNode == NODE_PTR(oNodeIter))
			continue;

		D3DXVec3Subtract(&vDirection, &NODE_PTR(oNodeIter)->getPosition(), &poNode->getPosition());
		D3DXVec3Normalize(&fLength, &vDirection, &vDirection);

		// if the node is too far away, don't even try to link to it
		// make it relative to the radius es
		if (fLength > (MAX_LINK_DISTANCE + NODE_PTR(oNodeIter)->m_fRadius + poNode->m_fRadius))
			continue;

		// collision check
		if (poGeo->checkRayCollision(vPoints, vLengths, poGeo->getRoot(), poNode->getPosition(), vDirection))
		{
			// have to check to see if the collision happened after the node we want to link to
			for (unsigned int i = 0; i < vLengths.size(); ++i)
			{
				if (vLengths[i] < fLength)
				{
					// no link
					bLink = false;
					break;
				}
			}

			if (bLink)
			{
				// will most likely to be a goal node connection
				// and for the sake of iteration we want it first
				poNode->m_loLinks.push_front(NODE_PTR(oNodeIter));

				// link it both ways
				NODE_PTR(oNodeIter)->m_loLinks.push_front(poNode);
			}
		}
		// not a single collision so they can link
		else
		{
			// will most likely to be a goal node connection
			// and for the sake of iteration we want it first
			poNode->m_loLinks.push_front(NODE_PTR(oNodeIter));

			// link it both ways
			NODE_PTR(oNodeIter)->m_loLinks.push_front(poNode);
		}

		vPoints.clear();
		vLengths.clear();
	}
}

/**
* CAIManager::unlinkNode
* @date Modified March 29, 2006
*/
void CAIManager::unlinkNode(CAINode* poNode)
{
	// iterate all of the links to tell each node to forget
	// about the passed in node
	std::list<CAINode*>::iterator oLinkIter, oSubLinkIter;
	oLinkIter = poNode->m_loLinks.begin();

	for (oLinkIter; oLinkIter != poNode->m_loLinks.end(); oLinkIter++)
	{
		// loop through the current link's links
		for (oSubLinkIter = (*oLinkIter)->m_loLinks.begin(); oSubLinkIter != (*oLinkIter)->m_loLinks.end(); oSubLinkIter++)
		{
			// did we find it
			if ((*oSubLinkIter) == poNode)
			{
				// yes, erase it
				(*oLinkIter)->m_loLinks.erase(oSubLinkIter);
				break;
			}
		}
	}

    // clear out the passed in node's list
	poNode->m_loLinks.clear();
}

/**
* CAIManager::testDirectLink
* @date Modified April 26, 2006
*/
bool CAIManager::testDirectLink(CAINode* poNode0, CAINode* poNode1)
{
	CCollisionGeometry* poGeo = CCollisionGeometry::getInstancePtr();

	float fLength = 0.0f;
	D3DXVECTOR3 vDirection;
	std::vector<D3DXVECTOR3> vPoints;
	std::vector<float> vLengths;
	bool bLink = true;

	//D3DXVec3Subtract(&vDirection, &poNode0->getPosition(), &poNode1->getPosition());
	//D3DXVec3Normalize(&fLength, &vDirection, &vDirection);
	fLength = computeDistanceSquared(poNode0->getPosition(), poNode1->getPosition());

	// if the node is too far away, don't even try to link to it
	if (fLength > (MAX_LINK_DISTANCE * MAX_LINK_DISTANCE))
		return false;

	// For now all we care about is distance for enemy direct connections
	return true;

	// collision check
	if (poGeo->checkRayCollision(vPoints, vLengths, poGeo->getRoot(), poNode1->getPosition(), vDirection))
	{
		// have to check to see if the collision happened after the node we want to link to
		for (unsigned int i = 0; i < vLengths.size(); ++i)
		{
			if (vLengths[i] < fLength)
			{
				// no link
				bLink = false;
				break;
			}
		}

		if (bLink)
		{
			return true;
		}
	}
	// not a single collision so they can link
	else
	{
		return true;
	}

	return false;
}

/**
* CAIManager::findBestGoal
* @date Modified May 4, 2006
*/
CAINode* CAIManager::findBestGoal(CActor* poActor, double* dDist)
{
	float fShortest = FLT_MAX, fTemp = FLT_MAX;
	CAINode* poClosestGoal = NULL;

	std::list<CAINode*>::iterator oNodeIter = m_loGoals.begin();

	// this function needs to stop returning NULL
	poClosestGoal = (*oNodeIter);

	while (oNodeIter != m_loGoals.end())
	{
		fTemp = computeDistanceSquared(poActor->getBV().centerPt, (*oNodeIter)->getPosition());
		if (fTemp < fShortest)
		{
			fShortest = fTemp;
			poClosestGoal = (*oNodeIter);
		}

		oNodeIter++;
	}

	if (dDist)
		*dDist = sqrt(fShortest);

	if (poClosestGoal == NULL)
	{
		// this cannot be NULL, do something
		poClosestGoal = ((CAINode*)m_loGoals.front());
	}

	return poClosestGoal;
}

/**
* CAIManager::updateGroupLeaders
* @date Modified April 20, 2006
*/
void CAIManager::updateGroupLeaders(void)
{
	std::list<CAIGroup>::iterator oGroupIter = m_loGroups.begin();

	bool bLeaders = m_oLeaderTimer.tick();

	while (oGroupIter != m_loGroups.end())
	{
		// deleted group
		if (oGroupIter->getLeader() == NULL)
		{
			//oGroupIter = m_loGroups.erase(oGroupIter);
			oGroupIter++;
			continue;
		}

		// empty group
		if (oGroupIter->getEnemies()->empty())
		{
			oGroupIter = m_loGroups.erase(oGroupIter);
			continue;
		}

		// close enough to attack
		if (computeDistanceSquared(oGroupIter->getLeader()->getBV().centerPt, findBestGoal(oGroupIter->getLeader())->getBV().centerPt) < 225.0f)
		{
			oGroupIter->disband(true);
			oGroupIter = m_loGroups.erase(oGroupIter);
			continue;
		}

		// normal
		if (bLeaders)
			oGroupIter->updateLeader();

		oGroupIter++;
	}
}

/**
* CAIManager::updateSpawnTriggers
* @date Modified May 9, 2006
*/
void CAIManager::updateSpawnTriggers(void)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CObjectManager::ObjectList loTriggers;
	poObjectManager->getObjects(OBJ_AI_SPAWNTRIGGER, &loTriggers);
	CObjectManager::ObjectList::iterator oTriggerIter = loTriggers.begin();

	while (oTriggerIter != loTriggers.end())
	{
		((CActorSpawnTrigger*)(*oTriggerIter))->activateSpawns();
		oTriggerIter++;
	}

	CObjectManager::ObjectList loSpawns;
	poObjectManager->getObjects(OBJ_SPAWN, &loSpawns);
	CObjectManager::ObjectList::iterator oSpawnIter = loSpawns.begin();

	while (oSpawnIter != loSpawns.end())
	{
		((CActorSpawn*)(*oSpawnIter))->setUsed(false);
		oSpawnIter++;
	}
}

/**
* CAIManager::updateCurrentObjective
* @date Modified June 2, 2006
*/
void CAIManager::updateCurrentObjective(void)
{
	// if we don't have an objective we need to find the first one
	if (m_poCurrentObjective == NULL)
	{
		CObjectManager::ObjectList loObjectives;
		CObjectManager::getInstance().getObjects(OBJ_OBJECTIVE_TRIGGER, &loObjectives);
		CObjectManager::ObjectList::iterator oObjectiveIter = loObjectives.begin();
		while (oObjectiveIter != loObjectives.end())
		{
			if (((CObjectiveTrigger*)(*oObjectiveIter))->m_unOrderNumber == 0)
			{
				m_poCurrentObjective = ((CObjectiveTrigger*)(*oObjectiveIter));
				D3DXVECTOR3 pos = m_poCurrentObjective->getPosition();
				pos.y += 5.0f;
				m_poObjectiveEmitter->setActorMatrix(m_poCurrentObjective->getOrientation(), pos, D3DXVECTOR3(1,1,1));
				m_poObjectiveEmitter->play();

				// if this is a OJT_GOTO activate the barriers
				if (m_poCurrentObjective->m_eObjectiveType == CObjectiveTrigger::OJT_GOTO)
				{
					if (!m_poCurrentObjective->m_loBarriers.empty())
					{
						// activate the barriers
						std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_poCurrentObjective->m_loBarriers.begin();
						while (oBarrierIter != m_poCurrentObjective->m_loBarriers.end())
						{
							((CObjectiveBarrier*)(*oBarrierIter))->activate();
							((CObjectiveBarrier*)(*oBarrierIter))->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "roadBlockSign.x"));
							((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
							((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
							oBarrierIter++;
						}

						int nRand = rand() % 3;
						switch (nRand)
						{
						case 0:
							CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
							break;
						case 1:
							CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
							break;
						case 2:
							CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
							break;
						default:
							break;
						}
					}
				}
				break;
			}
			oObjectiveIter++;
		}

		// if we couldn't find it just get out of here
		if (m_poCurrentObjective == NULL)
			return;
	}

	// check for people trying to cheat
	if (m_poCurrentObjective->m_eObjectiveType == CObjectiveTrigger::OJT_GOTO && m_bSkipCurrentObjective)
	{
		m_poCurrentObjective->m_bTriggered = true;
	}

	// if it isn't triggered check to see if it can be
	if (!m_poCurrentObjective->m_bTriggered)
	{
		m_poCurrentObjective->checkTriggerActivate();
	}

	// check to see if it has been triggered because it could have been updated by the function call above
	if (m_poCurrentObjective->m_bTriggered)
	{
		m_poObjectiveEmitter->pause();
		// if it is complete we can move to the next trigger
		if (m_poCurrentObjective->checkTriggerComplete(m_bSkipCurrentObjective))
		{
			// if this was the goal objective we are done
			if (m_poCurrentObjective->m_bGoal)
			{
				CGameKernel::getInstance().changeState(CWinState::getInstancePtr());
				return;
			}
			else
			{
				// find the next goal
				CObjectManager::ObjectList loObjectives;
				CObjectManager::getInstance().getObjects(OBJ_OBJECTIVE_TRIGGER, &loObjectives);
				CObjectManager::ObjectList::iterator oObjectiveIter = loObjectives.begin();
				unsigned int unNextObjective = m_poCurrentObjective->m_unOrderNumber + 1;
				m_poCurrentObjective = NULL;
				while (oObjectiveIter != loObjectives.end())
				{
					if (((CObjectiveTrigger*)(*oObjectiveIter))->m_unOrderNumber == unNextObjective)
					{
						m_poCurrentObjective = ((CObjectiveTrigger*)(*oObjectiveIter));
						D3DXVECTOR3 pos = m_poCurrentObjective->getPosition();
						pos.y += 5.0f;
						m_poObjectiveEmitter->setActorMatrix(m_poCurrentObjective->getOrientation(),pos, D3DXVECTOR3(1,1,1));
						m_poObjectiveEmitter->play();

						// if this is a OJT_GOTO activate the barriers
						if (m_poCurrentObjective->m_eObjectiveType == CObjectiveTrigger::OJT_GOTO)
						{
							if (!m_poCurrentObjective->m_loBarriers.empty())
							{
								// activate the barriers
								std::list<CObjectiveBarrier*>::iterator oBarrierIter = m_poCurrentObjective->m_loBarriers.begin();
								while (oBarrierIter != m_poCurrentObjective->m_loBarriers.end())
								{
									((CObjectiveBarrier*)(*oBarrierIter))->activate();
									((CObjectiveBarrier*)(*oBarrierIter))->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "roadBlockSign.x"));
									((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->setActorMatrix(((CObjectiveBarrier*)(*oBarrierIter))->getOrientation(), ((CObjectiveBarrier*)(*oBarrierIter))->getPosition(), ((CObjectiveBarrier*)(*oBarrierIter))->getScale());
									((CObjectiveBarrier*)(*oBarrierIter))->m_pExplosion->play();
									oBarrierIter++;
								}

								int nRand = rand() % 3;
								switch (nRand)
								{
								case 0:
									CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
									break;
								case 1:
									CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
									break;
								case 2:
									CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
									break;
								default:
									break;
								}
							}
						}

						break;
					}
					oObjectiveIter++;
				}

				// if we couldn't find it just get out of here
				if (m_poCurrentObjective == NULL)
					return;
			}
		}
	}

}

/**
* CAIManager::initSpawnTriggers
* @date Modified June 3, 2006
*/
void CAIManager::initSpawnTriggers(void)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();

	CObjectManager::ObjectList loTriggers, loSpawns;
	poObjectManager->getObjects(OBJ_AI_SPAWNTRIGGER, &loTriggers);
	poObjectManager->getObjects(OBJ_SPAWN, &loSpawns);

	CObjectManager::ObjectList::iterator oTriggerIter = loTriggers.begin();
	CObjectManager::ObjectList::iterator oSpawnIter = loSpawns.begin();

	// can't make triggers for spawns that don't exist
	if (loSpawns.empty())
		return;

	std::list<SSpawnAndDist> loTemp;
	std::list<SSpawnAndDist>::iterator oTempIter;

	SSpawnAndDist oCurrent;

	while (oTriggerIter != loTriggers.end())
	{
		loTemp.clear();
		oSpawnIter = loSpawns.begin();

		oCurrent.fDist = computeDistanceSquared(TRIGGER_PTR(oTriggerIter)->getPosition(), SPAWN_PTR(oSpawnIter)->getPosition());
		oCurrent.poSpawn = SPAWN_PTR(oSpawnIter);

		// get the first on in there
		loTemp.push_back(oCurrent);
		oSpawnIter++;

		// do a priority insert on the rest of the spawns
		while (oSpawnIter != loSpawns.end())
		{
			oTempIter = loTemp.begin();

			oCurrent.fDist = computeDistanceSquared(TRIGGER_PTR(oTriggerIter)->getPosition(), SPAWN_PTR(oSpawnIter)->getPosition());
			oCurrent.poSpawn = SPAWN_PTR(oSpawnIter);

			bool bInserted = false;
			while (oTempIter != loTemp.end())
			{
				if (oCurrent.fDist < oTempIter->fDist)
				{
					loTemp.insert(oTempIter, oCurrent);
					bInserted = true;
					break;
				}
				oTempIter++;
			}

			// if this dist is greater than all the others it won't be inserted
			if (!bInserted)
				loTemp.push_back(oCurrent);

			oSpawnIter++;
		}

		oTempIter = loTemp.begin();

		// clear out the old so this can be called more than once
		TRIGGER_PTR(oTriggerIter)->m_loSpawns.clear();

		// now that the spawns are in the loTemp in order of closest to farthest
		// get the number we need that is stored in the current time on load
		for (unsigned int i = 0; i < TRIGGER_PTR(oTriggerIter)->m_unNumSpawns; ++i)
		{
			// not enough spawns in the world
			if (oTempIter == loTemp.end())
				break;

			TRIGGER_PTR(oTriggerIter)->m_loSpawns.push_back((CActorSpawn*)oTempIter->poSpawn);
			oTempIter++;
		}

		// set it to go off as soon as it's hit
		TRIGGER_PTR(oTriggerIter)->m_fCurrentTime = TRIGGER_PTR(oTriggerIter)->m_fWaitTime;

		oTriggerIter++;
	}
}

/**
* CAIManager::initObjectiveTriggers
* @date Modified June 3, 2006
*/
void CAIManager::initObjectiveTriggers(void)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();

	CObjectManager::ObjectList loObjectives, loBarriers;
	poObjectManager->getObjects(OBJ_OBJECTIVE_TRIGGER, &loObjectives);
	poObjectManager->getObjects(OBJ_OBJECTIVE_BARRIER, &loBarriers);

	CObjectManager::ObjectList::iterator oObjectiveIter = loObjectives.begin();
	CObjectManager::ObjectList::iterator oBarrierIter = loBarriers.begin();

	// can't make triggers for spawns that don't exist
	if (loBarriers.empty())
		return;

	std::list<SSpawnAndDist> loTemp;
	std::list<SSpawnAndDist>::iterator oTempIter;

	SSpawnAndDist oCurrent;

	while (oObjectiveIter != loObjectives.end())
	{
		loTemp.clear();
		oBarrierIter = loBarriers.begin();

		oCurrent.fDist = computeDistanceSquared(OBJECTIVE_PTR(oObjectiveIter)->getPosition(), BARRIER_PTR(oBarrierIter)->getPosition());
		oCurrent.poSpawn = BARRIER_PTR(oBarrierIter);

		// get the first on in there
		loTemp.push_back(oCurrent);
		oBarrierIter++;

		// do a priority insert on the rest of the spawns
		while (oBarrierIter != loBarriers.end())
		{
			oTempIter = loTemp.begin();

			oCurrent.fDist = computeDistanceSquared(OBJECTIVE_PTR(oObjectiveIter)->getPosition(), BARRIER_PTR(oBarrierIter)->getPosition());
			oCurrent.poSpawn = BARRIER_PTR(oBarrierIter);

			bool bInserted = false;
			while (oTempIter != loTemp.end())
			{
				if (oCurrent.fDist < oTempIter->fDist)
				{
					loTemp.insert(oTempIter, oCurrent);
					bInserted = true;
					break;
				}
				oTempIter++;
			}

			// if this dist is greater than all the others it won't be inserted
			if (!bInserted)
				loTemp.push_back(oCurrent);

			oBarrierIter++;
		}

		oTempIter = loTemp.begin();

		// clear out the old so this can be called more than once
		OBJECTIVE_PTR(oObjectiveIter)->m_loBarriers.clear();

		// now that the spawns are in the loTemp in order of closest to farthest
		// get the number we need that is stored in the current time on load
		for (unsigned int i = 0; i < OBJECTIVE_PTR(oObjectiveIter)->m_unNumBarriers; ++i)
		{
			// not enough barriers in the world
			if (oTempIter == loTemp.end())
				break;

			OBJECTIVE_PTR(oObjectiveIter)->m_loBarriers.push_back((CObjectiveBarrier*)oTempIter->poSpawn);
			oTempIter++;
		}

		oObjectiveIter++;
	}
}

/**
* CAIManager::killedEnemy
* @date Modified June 3, 2006
*/
void CAIManager::killedEnemy(void)
{
	if (!m_poCurrentObjective)
		return;

	if (m_poCurrentObjective->m_eObjectiveType == CObjectiveTrigger::OJT_DUDES)
	{
		if (m_poCurrentObjective->m_bTriggered)
		{
			if (m_poCurrentObjective->m_unCurrentDudes > 0)
				m_poCurrentObjective->m_unCurrentDudes--;
		}
	}
}

/**
* CAIManager::spawnGroup
* @date Modified May 30, 2006
*/
void CAIManager::spawnGroup(EObjectType eType, D3DXVECTOR3 vPos, int nNum)
{
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();

	// these values can be different for each enemy type that is being spawned
	// based on the specific models being used
	D3DXVECTOR3 vScale, vOrientation(((float)(rand()%100))+1.0f, 0.0f, ((float)(rand()%100))+1.0f);
	D3DXVec3Normalize(NULL, &vOrientation, &vOrientation);
	float fBVHeight = 0.0f, fBVRadius = 0.0f;
	switch (eType)
	{
	case OBJ_ENEMY_ZOMBIECITIZEN:
		vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		fBVHeight = 15.0f;
		fBVRadius = 3.0f;
		break;
	case OBJ_ENEMY_GASEOUSCLAY:
		vScale = D3DXVECTOR3(0.75f, 0.75f, 0.75f);
		fBVHeight = 15.0f;
		fBVRadius = 4.0f;
		break;
	case OBJ_ENEMY_ICECREAMMAN:
		vScale = D3DXVECTOR3(0.75f, 0.75f, 0.75f);
		fBVHeight = 15.0f;
		fBVRadius = 4.0f;
		break;
	case OBJ_ENEMY_QUARTERBACK:
		vScale = D3DXVECTOR3(0.75f, 0.75f, 0.75f);
		fBVHeight = 15.0f;
		fBVRadius = 4.0f;
		break;
	}

	// make the new group, local variable problem is not an issue because the newGroup gets pushed into the list
	CAIGroup newGroup;
	newGroup.setAvgPos(vPos);

	// set the center enemy attributes
	CEnemy* newEnemy = (CEnemy*)poObjectManager->createObject(eType);
	newEnemy->setPosition(vPos);
	newEnemy->setBV(D3DXVECTOR3(newEnemy->getPosition().x, fBVHeight, newEnemy->getPosition().z), fBVRadius);
	newEnemy->setActorMatrix(vOrientation, newEnemy->getPosition(), vScale);
	newGroup.getEnemies()->push_back(newEnemy);

	int nNumCreated = 1, nCurrentRing = 0;
	float fRadius = SPAWN_SPACEING, fAngle = 0.0f, fEnemyWidth = newEnemy->getBV().fRadius * 2.0f, fAngleChange = 0.0f;
	// for truncation
	int nNumInRing = (int)((fRadius*2.0f*3.14159f)/fEnemyWidth);
	++nNumInRing;
	fAngleChange = (2.0f*3.14159f)/(float)nNumInRing;

	while (nNumCreated < nNum)
	{
		// another ring
		if (nCurrentRing == nNumInRing)
		{
			fRadius += SPAWN_SPACEING;
			// update offset
			nNumInRing = (int)((fRadius*2.0f*3.14159f)/fEnemyWidth);
			++nNumInRing;
			fAngleChange = (2.0f*3.14159f)/(float)nNumInRing;
			nCurrentRing = 0;
		}

		CEnemy* newEnemy = (CEnemy*)poObjectManager->createObject(eType);
		newEnemy->setPosition(D3DXVECTOR3(vPos.x + (fRadius * cos(fAngle)), vPos.y, vPos.z + (fRadius * sin(fAngle))));
		newEnemy->setBV(D3DXVECTOR3(newEnemy->getPosition().x, fBVHeight, newEnemy->getPosition().z), fBVRadius);
		newEnemy->setActorMatrix(vOrientation, newEnemy->getPosition(), vScale);
		newGroup.getEnemies()->push_back(newEnemy);

		// keep going around the circle
		fAngle += fAngleChange;
		++nNumCreated;
		++nCurrentRing;
	}

	m_loGroups.push_back(newGroup);
	m_loGroups.back().updateLeader(true);

	// AwesomeTime code
	CObjectManager::ObjectList loPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
	((CPlayer*)(loPlayers.front()))->m_oStats.addEnemiesSpawned((unsigned int)nNum);
	if(((CPlayer*)(loPlayers.front()))->isAwesome()) ((CPlayer*)(loPlayers.front()))->m_oStats.addAwesomeEnemiesSpawned(nNum);

	// Play spawn sound
	CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIESPAWN);
}

/**
* CAIManager::getDistanceToObjective
* @date Modified June 2, 2006
*/
float CAIManager::getDistanceToObjective(D3DXVECTOR3* vDirection)
{
	if (m_poCurrentObjective == NULL)
	{
		memset(vDirection, 0, sizeof(D3DXVECTOR3));
		return 0.0f;
	}

	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	CObjectManager::ObjectList loPlayers;
	poObjectManager->getObjects(OBJ_PLAYER, &loPlayers);

	D3DXVec3Subtract(vDirection, &m_poCurrentObjective->getPosition(), &((CPlayer*)loPlayers.front())->getPosition());
	vDirection->y = 0.0f;

	float fLength = 0.0f;
	D3DXVec3Normalize(&fLength, vDirection, vDirection);
	return fLength;
}