/**
* @file ParticleEffect.cpp
* @author Matt Rudder
* @date Modified May 20, 2006
*
* Defines a collection of attributes that dictate the appearance of particles.
*/

// Local includes
#include "ParticleEffect.h"
#include "ParticleAttribute.h"
#include "ParticleManager.h"

// Color read macros
#define GET_BYTE(dw, byte) ((BYTE)((dw >> 8*byte) & 0xff))
#define GET_ALPHA(dw) GET_BYTE(dw, 3)
#define GET_RED(dw) GET_BYTE(dw, 2)
#define GET_GREEN(dw) GET_BYTE(dw, 1)
#define GET_BLUE(dw) GET_BYTE(dw, 0)

/**
* CParticleEffect::CParticleEffect
* @date Modified May 20, 2006
*/
CParticleEffect::CParticleEffect(void) : m_eSpawnMode(SM_LOOPING),
	m_bBounce(false), m_fInvRestitution(FLT_MAX)
{
	D3DXMatrixIdentity(&m_mOffset);
}

/**
* CParticleEffect::CParticleEffect
* @date Modified May 26, 2006
*/
CParticleEffect::CParticleEffect(const CParticleEffect& effect)
{
	m_eBlendMode = effect.m_eBlendMode;
	m_eSpawnShape = effect.m_eSpawnShape;
	m_eSpawnMode = effect.m_eSpawnMode;

	m_vSpawnRadius = effect.m_vSpawnRadius;
	m_vMaxVelocity = effect.m_vMaxVelocity;
	m_vMinVelocity = effect.m_vMinVelocity;

	m_nMaxParticles = effect.m_nMaxParticles;
	m_fEffectLength = effect.m_fEffectLength;

	m_bBounce = effect.m_bBounce;
	m_fInvRestitution = effect.m_fInvRestitution;

	m_pTexture = (CTexture2D*)effect.m_pTexture->cloneResource();

	for(size_t i = 0; i < effect.m_vAttributes.size(); ++i)
		m_vAttributes.push_back(new CParticleAttribute(*effect.m_vAttributes[i]));

	D3DXMatrixIdentity(&m_mOffset);
}

/**
* CParticleEffect::~CParticleEffect
* @date Modified May 20, 2006
*/
CParticleEffect::~CParticleEffect(void)
{
	for(size_t i = 0; i < m_vAttributes.size(); ++i)
		delete m_vAttributes[i];

	m_pTexture->Release();
}

/**
* CParticleEffect::addAttribute
* @date Modified May 20, 2006
*/
bool CParticleEffect::addAttribute(CParticleAttribute* pAttribute)
{
	for(size_t i = 0; i < m_vAttributes.size(); ++i)
		if(m_vAttributes[i]->getType() == pAttribute->getType())
			return false;

	m_vAttributes.push_back(pAttribute);
	return true;
}

/**
* CParticleEffect::initParticle
* @date Modified June 01, 2006
*/
void CParticleEffect::initParticle(CParticleManager::SParticle* pParticle, D3DXMATRIX* mOffset)
{
	D3DXMATRIX mPosOffset, mOrientation = *mOffset;
	D3DXMatrixIdentity(&mPosOffset);
	mPosOffset._41 = mOffset->_41;
	mPosOffset._42 = mOffset->_42;
	mPosOffset._43 = mOffset->_43;
	mOrientation._41 = mOrientation._42 = mOrientation._43 = 0.0f;
	mOrientation._44 = 1.0f;

	float fMinVel, fMaxVel;
	D3DXVec3Normalize(&fMinVel, &m_vMinVelocity, &m_vMinVelocity);
	D3DXVec3Normalize(&fMaxVel, &m_vMaxVelocity, &m_vMaxVelocity);

	D3DXVec3TransformCoord(&m_vMinVelocityTrans, &m_vMinVelocity, &mOrientation);
	D3DXVec3TransformCoord(&m_vMaxVelocityTrans, &m_vMaxVelocity, &mOrientation);

	m_vMinVelocity *= fMinVel;
	m_vMaxVelocity *= fMaxVel;

	// Update attributes
	BYTE cR = GET_RED(pParticle->Color), 
		 cG = GET_GREEN(pParticle->Color), 
		 cB = GET_BLUE(pParticle->Color), 
		 cA = GET_ALPHA(pParticle->Color);
	for(size_t j = 0; j < m_vAttributes.size(); ++j)
	{
		float fScale = m_vAttributes[j]->getValue(0.0f);
		switch(m_vAttributes[j]->getType())
		{
		case CParticleAttribute::ATR_COLORRED: cR = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_COLORGREEN: cG = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_COLORBLUE: cB = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_COLORALPHA: cA = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_SIZE: pParticle->Size = fScale; break;
		case CParticleAttribute::ATR_ROTATION: pParticle->Rotation = degreesToRadians(fScale); break;
		case CParticleAttribute::ATR_ACCELX: pParticle->Acceleration.x = fScale; break;
		case CParticleAttribute::ATR_ACCELY: pParticle->Acceleration.y = fScale; break;
		case CParticleAttribute::ATR_ACCELZ: pParticle->Acceleration.z = fScale; break;
		}
	}
	pParticle->Color = D3DCOLOR_ARGB(cA, cR, cG, cB);
	getRandomVector(&pParticle->Velocity, &m_vMinVelocityTrans, &m_vMaxVelocityTrans);
	pParticle->Velocity *= getRandomFloat(fMinVel, fMaxVel);
	m_vMinVelocityTrans *= fMinVel;
	m_vMaxVelocityTrans *= fMaxVel;
	
	D3DXVECTOR3 vMin(0.0f, 0.0f, 0.0f), vMax(0.0f, 0.0f, 0.0f);
	switch(m_eSpawnShape)
	{
	case SH_CUBE:
		vMin.x = -m_vSpawnRadius.x;
		vMin.y = -m_vSpawnRadius.y;
		vMin.z = -m_vSpawnRadius.z;
		getRandomVector(&pParticle->Position, &vMin, &m_vSpawnRadius);
		break;
	case SH_SQUARE:
		vMin.x = -m_vSpawnRadius.x;
		vMin.z = -m_vSpawnRadius.z;
		vMax.x = m_vSpawnRadius.x;
		vMax.z = m_vSpawnRadius.z;
		getRandomVector(&pParticle->Position, &vMin, &vMax);
		break;
	case SH_SPHERE:
		getRandomVector(&vMin, -1.0f, 1.0f);
		D3DXVec3Normalize(&vMin, &vMin);
		pParticle->Position.x = vMin.x * m_vSpawnRadius.x;
		pParticle->Position.y = vMin.y * m_vSpawnRadius.x;
		pParticle->Position.z = vMin.z * m_vSpawnRadius.x;
		break;
	case SH_CIRCLE:
		getRandomVector(&vMin, -1.0f, 1.0f);
		D3DXVec3Normalize(&vMin, &vMin);
		pParticle->Position.x = vMin.x * m_vSpawnRadius.x;
		pParticle->Position.y = 0.0f;
		pParticle->Position.z = vMin.z * m_vSpawnRadius.x;
		break;
	}
	
	D3DXVec3TransformCoord(&pParticle->Position, &pParticle->Position, &mPosOffset);
}

/**
* CParticleEffect::updateParticle
* @date Modified Jun 01, 2006
*/
bool CParticleEffect::updateParticle(float fTime, CParticleManager::SParticle* pParticle)
{
	CParticleManager::SParticle* p = pParticle;
	p->LivingTime += fTime;

	// Check for dead particles
	if(p->LivingTime >= m_fEffectLength)
	{
		return false;
	}
	else
	{
		D3DXVECTOR3 vVel = p->Velocity * fTime;
		D3DXVECTOR3 vAccel = p->Acceleration * fTime;
		D3DXVec3Add(&p->Position, &p->Position, &vVel);
		D3DXVec3Add(&p->Velocity, &p->Velocity, &vAccel);

		if(m_bBounce && p->Position.y < 0.0f)
		{
			p->Position.y = 0.0f;
			p->Velocity.x /= m_fInvRestitution;
			p->Velocity.y = (-p->Velocity.y) / m_fInvRestitution;
			p->Velocity.z /= m_fInvRestitution;
		}
	}

	// Update attributes
	BYTE cR = GET_RED(p->Color), cG = GET_GREEN(p->Color), cB = GET_BLUE(p->Color), cA = GET_ALPHA(p->Color);
	for(size_t j = 0; j < m_vAttributes.size(); ++j)
	{
		float fScale = m_vAttributes[j]->getValue(p->LivingTime / m_fEffectLength);
		switch(m_vAttributes[j]->getType())
		{
		case CParticleAttribute::ATR_COLORRED: cR = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_COLORGREEN: cG = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_COLORBLUE: cB = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_COLORALPHA: cA = (BYTE)(fScale * 255.0f); break;
		case CParticleAttribute::ATR_SIZE: p->Size = fScale; break;
		case CParticleAttribute::ATR_ROTATION: p->Rotation = degreesToRadians(fScale); break;
		case CParticleAttribute::ATR_ACCELX: p->Acceleration.x = fScale; break;
		case CParticleAttribute::ATR_ACCELY: p->Acceleration.y = fScale; break;
		case CParticleAttribute::ATR_ACCELZ: p->Acceleration.z = fScale; break;
		}
	}
	p->Color = D3DCOLOR_ARGB(cA, cR, cG, cB);
	return true;
}