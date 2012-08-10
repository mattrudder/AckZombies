/**
* @file ParticleEffect.h
* @author Matt Rudder
* @date Created May 20, 2006
*
* Defines a collection of attributes that dictate the appearance of particles.
*/

#ifndef _PARTICLEEFFECT_H_
#define _PARTICLEEFFECT_H_

// System includes
#include <vector>
using std::vector;

// Engine includes
#include "ParticleSystem.h"
#include "ParticleManager.h"

class CParticleAttribute;
class CTexture2D;

/**
* Defines a collection of attributes that dictate the appearance of particles.
*
* @author Matt Rudder
* @date Created May 20, 2006
* @date Modified May 20, 2006
*/
class CParticleEffect
{
	friend class CParticleEffectContentHandler;
	friend class CParticleManager;
public:
	enum EBlendOp
	{
		OP_REPLACE,
		OP_ADDITIVE,
		OP_MULTIPLY,
	};

	enum EShape
	{
		SH_SQUARE,
		SH_CUBE,
		SH_SPHERE,
		SH_CIRCLE,
	};

	enum ESpawnMode
	{
		SM_INSTANT,
		SM_LOOPING,
	};

protected:
	EBlendOp m_eBlendMode;
	EShape m_eSpawnShape;
	ESpawnMode m_eSpawnMode;

	D3DXVECTOR3 m_vSpawnRadius;
	D3DXVECTOR3 m_vMaxVelocity;
	D3DXVECTOR3 m_vMinVelocity;

	D3DXVECTOR3 m_vSpawnRadiusTrans;
	D3DXVECTOR3 m_vMaxVelocityTrans;
	D3DXVECTOR3 m_vMinVelocityTrans;
	
	unsigned int m_nMaxParticles;
	float m_fEffectLength;

	bool m_bBounce;
	float m_fInvRestitution;

	CTexture2D* m_pTexture;

	vector<CParticleAttribute*> m_vAttributes;
	D3DXMATRIX m_mOffset;

	void initParticle(CParticleManager::SParticle* pParticle, D3DXMATRIX* mOffset);
	bool updateParticle(float fTime, CParticleManager::SParticle* pParticle);
public:
	CParticleEffect(void);
	CParticleEffect(const CParticleEffect& effect);
	virtual ~CParticleEffect(void);

	/**
	* Accessors/Modifiers
	* @date Created Jun 01, 2006
	* @{
	*/
	EBlendOp getBlendMode(void) { return m_eBlendMode; }
	EShape getSpawnShape(void) { return m_eSpawnShape; }
	ESpawnMode getSpawnMode(void) { return m_eSpawnMode; }
	unsigned int getMaxParticles(void) { return m_nMaxParticles; }
	float getEffectLength(void) { return m_fEffectLength; }
	CTexture2D* getTexture(void) { return m_pTexture; }
	//!@}

	bool addAttribute(CParticleAttribute* pAttribute);
};

#endif //_PARTICLEEFFECT_H_