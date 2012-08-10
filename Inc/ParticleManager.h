/**
* @file ParticleManager.h
* @author Matt Rudder
* @date Created Jun 01, 2006
*
* Handles the creation and update of particles, regardless of their emitter.
*/

#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

// Engine includes
#include "Singleton.h"
#include "VertexCache.h"

// System includes
#include <vector>
#include <map>
using std::vector;
using std::map;
#include <d3dx9.h>
#include <d3d9.h>

class CParticleEffect;
class CParticleSystem;

/**
* Handles the creation and update of particles, regardless of their emitter.
*
* @author Matt Rudder
* @date Created Jun 01, 2006
* @date Modified Jun 01, 2006
*/
class CParticleManager : public CSingleton<CParticleManager>
{
	friend class CSingleton<CParticleManager>;
public:
	//! Vertex format used for a particle.
	struct SParticleVert
	{
		D3DXVECTOR3 Position;
		DWORD Color;
		float Tu, Tv;
	};
	static D3DVERTEXELEMENT9 m_pVertElem[];

	//! Layout of a single particle.
	struct SParticle
	{
		D3DXVECTOR3 Position;
		DWORD Color;
		float Size;
		float LivingTime;
		float Rotation;
		D3DXVECTOR3 Velocity;
		D3DXVECTOR3 Acceleration;
		CParticleSystem* System;
	};


protected:
	static const DWORD FVF;
	CVertexCache m_oVertexCache;

	//! List of currently active particles.
	typedef vector<SParticle*> ParticleList;
	typedef map<CParticleEffect*, ParticleList> ParticleMap;
	ParticleMap m_mActiveParticles;

	//! List of recently deceased particles, for reuse.
	ParticleList m_lInactiveParticles;
	vector<CParticleEffect*> m_lLoadedEffects;

	//! Singleton stuff.
	//!@{
	CParticleManager(void);
	CParticleManager(const CParticleManager&);
	operator=(const CParticleManager&){}
	virtual ~CParticleManager(void);
	//!@}

	/**
	* Helper function to draw a single particle.
	*
	* @date Created Jun 01, 2006
	* @param[in]	pParticle	The particle to draw.
	* @param[in]	vRight		The current camera right vector.
	* @param[in]	vUp			The current camera up vector.
	* @param[in]	vAt			The current camera at vector.
	*/
	__forceinline void drawParticle(SParticle* pParticle, D3DXVECTOR3& vRight, D3DXVECTOR3& vUp, D3DXVECTOR3& vAt);
public:
	/**
	* Creates a new particle based on a given effect and adds it to the manager.
	*
	* @date Created Jun 01, 2006
	* @param[in]	mOffset		Offset matrix.
	* @param[in]	pEffect		The effect the particle belongs to.
	* @param[in]	pEffect		The system (effect instance) the particle belongs to.
	*/
	void addParticle(D3DXMATRIX* mOffset, CParticleEffect* pEffect, CParticleSystem* pSys);

	/**
	* Updates all the particles currently managed
	*
	* @date Created Jun 01, 2006
	* @param[in]	fTime	Time passed since the last update.
	*/
	void update(float fTime);

	/**
	* Renders all currently active particles.
	*
	* @date Created Jun 01, 2006
	*/
	void draw(void);

	/**
	* Tells the manager about a loaded effect so it can be deallocated later.
	*
	* @date Created Jun 01, 2006
	* @param[in]	pEffect		The effect to add.
	*/
	void addEffect(CParticleEffect* pEffect);
};

#endif //_PARTICLEMANAGER_H_