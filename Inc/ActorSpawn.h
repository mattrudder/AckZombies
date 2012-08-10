/**
* @file ActorSpawn.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 29, 2006
*
* Contains the definition of the CActorSpawn class
*/

#ifndef _ACTORSPAWN_H_
#define _ACTORSPAWN_H_

#include "Actor.h"

/**
* Defines an actor spawn location
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 29, 2006
* @date Modified May 9, 2006
* @remark actor type must be set based on what you want to spawn
*/
class CActorSpawn : public CActor
{
	//! the number of actors to spawn
	unsigned int m_unNumActors;

	//! the type of actors to spawn
	EObjectType m_eType;

	//! whether guys have been spawned here already during the update
	bool m_bUsed;

public:

	CActorSpawn(void) : CActor(OBJ_SPAWN), m_bUsed(false) {}
	~CActorSpawn(void) {}

	/**
	* accessors for the member variable of the actor spawn
	* 
	* @date Created April 30, 2006
	* @return The value of the actor member
	* @{
	*/
	EObjectType getSpawnType(void) { return m_eType; }
	void setSpawnType(EObjectType eType) { m_eType = eType; }
	unsigned int getNumActors(void) { return m_unNumActors; }
	void setNumActors(unsigned int unNumActors) { m_unNumActors = unNumActors; }
	bool getUsed(void) { return m_bUsed; }
	void setUsed(bool bUsed) { m_bUsed = bUsed; }
	//!@}

	/**
	* update
	*
	* @date Created April 30, 2006
	*/
	virtual void update(void)
	{
	}

	/**
	* Creates a new spawn location
	*
	* @date Created April 29, 2006
	*/
	static CObject* create(void)
	{
		CActorSpawn* poSpawn = new CActorSpawn;
		poSpawn->setMesh(NULL/*(CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "starZombie.x")*/);
		poSpawn->deactivate();
		return poSpawn;
	}
};
#endif /*_ACTORSPAWN_H_*/