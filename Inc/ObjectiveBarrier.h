/**
* @file ObjectiveBarrier.h
* @author Jonathan "Awesome" Zimmer
* @date Created June 3, 2006
*
* Contains the definition of the CObjectiveBarrier class
*/

#ifndef _OBJECTIVEBARRIER_H_
#define _OBJECTIVEBARRIER_H_

#include "Actor.h"
#include "ProjectileWeapon.h"
#include "ParticleEmitter.h"
#include "ObjectManager.h"

/**
* Defines an objective barrier location
*
* @author Jonathan "Awesome" Zimmer
* @date Created June 3, 2006
* @date Modified June 3, 2006
*/
class CObjectiveBarrier : public CActor
{
public:

	//! Explosion particle effect
	CParticleEmitter* m_pExplosion;

	CObjectiveBarrier(void) : CActor(OBJ_OBJECTIVE_BARRIER)
	{
		// Create the explosion particle effect
		m_pExplosion = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
		m_pExplosion->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "Explosion.xml"));
	}
	~CObjectiveBarrier(void) {}

	/**
	* updates the barrier
	*
	* @date Created June 3, 2006
	*/



	/**
	* Creates a new objective barrier
	*
	* @date Created June 3, 2006
	*/
	static CObject* create(void)
	{
		CObjectiveBarrier* poBarrier = new CObjectiveBarrier;
		poBarrier->setMesh(NULL);
		poBarrier->deactivate();
		return poBarrier;
	}
};
#endif /*_OBJECTIVEBARRIER_H_*/