/**
* @file ParticleEmitter.cpp
* @author Matt Rudder
* @date Modified May 21, 2006
*
* Defines a movable object in world space that emits particles based on a 
* particle system.
*/

// Local includes
#include "ParticleEmitter.h"
#include "ParticleSystem.h"


/**
* CParticleEmitter::CParticleEmitter
* @date Modified May 21, 2006
*/
CParticleEmitter::CParticleEmitter(EObjectType eType) : CActor(eType), m_pSystem(NULL)
{
	m_bActive = false;
}

/**
* CParticleEmitter::~CParticleEmitter
* @date Modified May 21, 2006
*/
CParticleEmitter::~CParticleEmitter(void)
{
}

/**
* CParticleEmitter::create
* @date Modified May 21, 2006
*/
CObject* CParticleEmitter::create(void)
{
	return new CParticleEmitter;
}

void CParticleEmitter::play(bool bReset)
{
	 if(m_pSystem) 
	 { 
		 m_pSystem->play(); 
		 
		 if(bReset) 
			 m_pSystem->reset(true); 
	 }
}