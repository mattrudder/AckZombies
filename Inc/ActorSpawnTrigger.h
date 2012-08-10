/**
* @file ActorSpawnTrigger.h
* @author Jonathan "Awesome" Zimmer
* @date Created May 9, 2006
*
* Contains the definition of the CActorSpawnTrigger class
*/

#ifndef _ACTORSPAWNTRIGGER_H_
#define _ACTORSPAWNTRIGGER_H_

#include "Actor.h"
#include "ActorSpawn.h"
#include <list>
#include "limits.h"
#include "TickTimer.h"

/**
* Defines an actor spawn trigger location
*
* @author Jonathan "Awesome" Zimmer
* @date Created May 9, 2006
* @date Modified May 27, 2006
*/
class CActorSpawnTrigger : public CActor
{
public:
	//! the spawns to control
	std::list<CActorSpawn*> m_loSpawns;

	//! the time to wait before triggering
	float m_fWaitTime, m_fCurrentTime;

	//! the number of spawns this trigger controls
	unsigned int m_unNumSpawns;

	//! Used to determine if this trigger is a box or sphere, 0.0f is this is a sphere
	float m_fDepth;

	//! the radius of this trigger
	float m_fRadius;

	//! the max number of times this trigger can be triggered, -1 for infinite
	int m_nMaxTimes, m_nNumTimes;

	CActorSpawnTrigger(void) : CActor(OBJ_AI_SPAWNTRIGGER), m_fWaitTime(FLT_MAX), m_fCurrentTime(0.0f), m_unNumSpawns(0), m_fDepth(0.0f), m_nMaxTimes(0), m_nNumTimes(0) { this->deactivate(); }
	~CActorSpawnTrigger(void) {}

	/**
	* update
	*
	* @date Created May 9, 2006
	*/
	virtual void update(void) { m_fCurrentTime += CTimer::getInstance().getFrameTime(); }

	/**
	* Try to spawn dudes
	*
	* @date Created May 9, 2006
	*/
	void activateSpawns(void);

	/**
	* Creates a new spawn location
	*
	* @date Created May 9, 2006
	*/
	static CObject* create(void)
	{
		CActorSpawnTrigger* poSpawn = new CActorSpawnTrigger;
		poSpawn->setMesh(NULL/*(CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "ring.x")*/);
		poSpawn->deactivate();
		return poSpawn;
	}
};
#endif /*_ACTORSPAWNTRIGGER_H_*/