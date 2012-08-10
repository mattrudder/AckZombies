/**
* @file ParticleEmitter.h
* @author Matt Rudder
* @date Created May 20, 2006
*
* Defines a collection of particle effects and their location in the game world.
*/

#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

// Engine include
#include "BaseResource.h"
#include "VertexCache.h"
#include "RenderSystem.h"

// System includes
#include <vector>
using std::vector;

class CParticleEffect;

/**
* Defines a collection of particle effects and their location in the game world.
*
* @author Matt Rudder
* @date Created May 20, 2006
* @date Modified May 20, 2006
*/
class CParticleSystem : public CBaseResource
{
	friend class CParticleEffectContentHandler;
protected:

	struct SEffectInstance
	{
		unsigned int unNumSpawned;
		unsigned int unNumActive;
		CParticleEffect* pEffect;
	};

	vector<SEffectInstance> m_vEffects;
	bool m_bPlaying;
	float m_fCurrentTime;

public:
	CParticleSystem(void);
	CParticleSystem(const CParticleSystem& rSys);

	virtual ~CParticleSystem(void);

	/**
	* Called by the Resource Manager when a previously loaded resource is 
	* requested again. Derived classes may override the default behavior for
	* any special needs cases, where the same object should not be given out
	* multiple times.
	*
	* @date Created Mar 31, 2006
	* @return The original object.
	*/
	virtual CBaseResource* cloneResource(void);

	/**
	* Allows a resource to output information about itself for debugging purposes
	*
	* @date Created Apr 01, 2006
	* @return Description string
	*/
	virtual CString toString(void) const;

	/**
	* Loads a particle emitter from XML definition file.
	*
	* @date Created May 20, 2006
	* @param[in]	sFilename	The file to load from.
	* @return The newly created emitter.
	*/
	static CBaseResource* createEmitterFromFile(const CString sFilename);

	/**
	* update the actor each frame
	*
	* @date Created April 23, 2006
	*/
	virtual void update(D3DXMATRIX* mOffset = NULL);

	void pause(void) { m_bPlaying = false; }
	void play(void) { m_bPlaying = true; }
	bool isPlaying(void) { return m_bPlaying; }
	void removeParticle(CParticleEffect* pEffect);
	void reset(bool bPlay = false);
};

#endif //_PARTICLESYSTEM_H_