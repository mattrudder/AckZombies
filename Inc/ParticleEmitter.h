/**
* @file ParticleEmitter.h
* @author Matt Rudder
* @date Created May 21, 2006
*
* Defines a movable object in world space that emits particles based on a 
* particle system.
*/

#ifndef _PARTICLEEMITTER_H_
#define _PARTICLEEMITTER_H_

// Local includes
#include "Actor.h"

class CParticleSystem;

/**
* Defines a movable object in world space that emits particles based on a 
* particle system.
*
* @author Matt Rudder
* @date Created May 21, 2006
* @date Modified May 21, 2006
*/
class CParticleEmitter : public CActor
{
protected:
	CParticleSystem* m_pSystem;
public:
	CParticleEmitter(EObjectType eType = OBJ_PARTICLE_EMITTER);
	virtual ~CParticleEmitter(void);

	void setSystem(CParticleSystem* pSys) { m_pSystem = pSys; }
	CParticleSystem* getSystem(void) { return m_pSystem; }
	static CObject* create(void);
	void pause(void) { if(m_pSystem) m_pSystem->pause(); }
	void play(bool bReset = true);
	void update(void) { if(m_pSystem) { m_pSystem->update(&m_ActorMatrix); CActor::update(); } }
};

#endif // _PARTICLEEMITTER_H_