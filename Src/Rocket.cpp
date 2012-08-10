/**
* @file Rocket.cpp
* @author Jordan Leckner
* @date Created April 24, 2006
*
* This file includes the definition of the Rocket Weapon class
*/
#include "Rocket.h"
#include "CollisionGeometry.h"
#include "ObjectManager.h"
#include "Enemy.h"
#include "ResourceManager.h"
#include "ParticleEmitter.h"

/**
* CRocket::CRocket
* @date Modified May 30, 2006
*/
CRocket::CRocket(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPONPROJ_ROCKET;

	m_pSmoke = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pSmoke->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "SmokeStack.xml"));

	m_pExplosion = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pExplosion->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "Explosion.xml"));
}

/**
* CRocket::~CRocket
* @date Modified May 30, 2006
*/
CRocket::~CRocket()
{
	CObjectManager::getInstance().removeObject(m_pSmoke);
	CObjectManager::getInstance().removeObject(m_pExplosion);
}

/**
* CRocket::update
* @date Modified May 26, 2006
*/
void CRocket::update()
{
	CProjectileWeapon::update();

	m_pSmoke->setActorMatrix(getOrientation(), getPosition(), D3DXVECTOR3(1,1,1));
}

/**
* CRocket::collided
* @date Modified May 26, 2006
*/
void CRocket::collided(CActor *poActor)
{
	CProjectileWeapon::collided(poActor);
}

/**
* CRocket::init
* @date Modified May 26, 2006
*/
void CRocket::init(void)
{
	m_pSmoke->play();
	CProjectileWeapon::init();
}

/**
* CRocket::destroy
* @date Modified May 30, 2006
*/
void CRocket::destroy(void)
{
	m_pExplosion->setActorMatrix(getOrientation(), getPosition(), D3DXVECTOR3(1,1,1));
	m_pExplosion->play();
	m_pSmoke->pause();
}