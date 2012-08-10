/**
* @file AIManager.h
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the definition of the CAIManager class.
*/

#ifndef _AIMANAGER_H_
#define _AIMANAGER_H_

// Local Includes
#include "AIEntity.h"
#include "Singleton.h"
#include "ObjectManager.h"
#include "AIGroup.h"
#include "TickTimer.h"
#include "ActorSpawnTrigger.h"
#include "ObjectiveTrigger.h"

// System Includes
#include <list>

/**
* Updates elements used by all AI entities: node links, leaders, and followers
*
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
* @date Modified June 10, 2006
*/
class CAIManager : public CSingleton<CAIManager>
{
	friend class CSingleton<CAIManager>;

	//! Singleton stuff.
	//!@{
	CAIManager(void);
	CAIManager(const CAIManager&) {}
	operator=(const CAIManager&){}
	virtual ~CAIManager(void);
	//!@}

	//! The buffer for the updated leaders
	std::list<CAIGroup> m_loGroups;

	//! The list of goals for the AI Entities to use
	std::list<CAINode*> m_loGoals;

	//! The timer to update the leaders
	CTickTimer m_oLeaderTimer;

	//! The timer to check to trigger spawns
	CTickTimer m_oTriggerTimer;

	//! The timer to check for objective completion
	CTickTimer m_oObjectiveTimer;

	//! The current Objective
	CObjectiveTrigger* m_poCurrentObjective;

	//! The particle effect highlighting the current objective
	CParticleEmitter* m_poObjectiveEmitter;

	//! Whether or not the goals need to be updated. set when a goal moves
	bool m_bUpdateGoalLinks;

	//! Whether or not to skip the current objective
	bool m_bSkipCurrentObjective;

	/**
	* Used to sort spawns for trigger inits
	*
	* @author Jonathan "Awesome" Zimmer
	* @date Created May 9, 2006
	* @date Modified June 3, 2006
	*/
	struct SSpawnAndDist
	{
		//! the distance to the trigger
		float fDist;
		//! pointer to the Spawn
		CActor* poSpawn;

		SSpawnAndDist(void) {}
		SSpawnAndDist(float fDistance, CActorSpawn* poSpawnPtr)
		{
			fDist = fDistance;
			poSpawn = poSpawnPtr;
		}
		~SSpawnAndDist(void) {}
	};

public:

	/**
	* Update AI entities
	*
	* @date Created March 27, 2006
	*/
	void update(void);

	/**
	* Create the links between static nodes and there radii of freedom
	*
	* @date Created March 28, 2006
	*/
	void createNodeLinks(void);

	/**
	* Create this dynamic node's links
	*
	* @date Created March 28, 2006
	* @param[in]	poNode	The node to link
	*/
	void linkNode(CAINode* poNode);

	/**
	* Sever this dynamic node's links
	*
	* @date Created March 28, 2006
	* @param[in]	poNode	The node to sever
	*/
	void unlinkNode(CAINode* poNode);

	/**
	* Checks for a direct connection between the nodes
	*
	* @date Created March 30, 2006
	* @param[in]	poNode0	The first node
	* @param[in]	poNode1	The other node
	* @return		true if there can be a connection
	*/
	bool testDirectLink(CAINode* poNode0, CAINode* poNode1);

	/**
	* Finds the closest goal to the actor and returns the node pointer
	*
	* @date Created March 30, 2006
	* @param[in]	poActor	The actor to find the goal for
	* @param[out]	pdDist	where to store the distance to the player
	* @return		the best goal node
	*/
	CAINode* findBestGoal(CActor* poActor, double* dDist = 0);

	/**
	* Calculate group leaders based on distance to player
	*
	* @date Created April 19, 2006
	*/
	void updateGroupLeaders(void);

	/**
	* Checks all spawn triggers and spawns are triggered if necessary
	*
	* @date Created May 19, 2006
	*/
	void updateSpawnTriggers(void);

	/**
	* Updates the current objective
	*
	* @date Created June 2, 2006
	*/
	void updateCurrentObjective(void);

	/**
	* sets the triggers spawns
	*
	* @date Created May 19, 2006
	*/
	void initSpawnTriggers(void);

	/**
	* sets the objectives barriers
	*
	* @date Created June 3, 2006
	*/
	void initObjectiveTriggers(void);

	/**
	* tells the current objective about a kill, if applicable
	*
	* @date Created June 3, 2006
	*/
	void killedEnemy(void);

	/**
	* Goes to the object manager and creates the enemies
	*
	* @date Created April 19, 2006
	* @param[in]	eType	The type of enemies in the group
	* @param[in]	vPos	The position to spawn it at
	* @param[in]	nNum	The number of enemies in the group
	*/
	void spawnGroup(EObjectType eType, D3DXVECTOR3 vPos, int nNum);

	/**
	* Remove the last group added to the AI manager
	*
	* @date Created April 30, 2006
	*/
	void removeLastGroup(void) { m_loGroups.pop_back(); }

	/**
	* tells the AI manager that a goal has moved and should be relinked
	*
	* @date Created April 28, 2006
	*/
	inline void updateGoalLinks(void) { m_bUpdateGoalLinks = true; }

	/**
	* see if we need to update the goals
	*
	* @date Created April 28, 2006
	*/
	inline bool getUpdateGoalLinks(void) { return m_bUpdateGoalLinks; }

	/**
	* get access to the current goal
	*
	* @date Created June 2, 2006
	* @return	pointer to the current objective
	*/
	inline CObjectiveTrigger* getCurrentObjective(void) { return m_poCurrentObjective; }

	/**
	* get the vector to the next objective
	*
	* @date Created June 2, 2006
	* @param[out]	normalized vector pointing at the next objective
	* @return		fDist	the distance to the objective
	*/
	float getDistanceToObjective(D3DXVECTOR3* vDirection);

	/**
	* sets the AIManager to skip the current objective
	*
	* @date Created June 10, 2006
	*/
	inline void skipCurrentObjective(void) { m_bSkipCurrentObjective = true; }
};

#endif /*_AIMANAGER_H_*/