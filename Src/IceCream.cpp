/**
* @file IceCream.cpp
* @author Jordan Leckner
* @date Created May 4, 2006
*
* This file includes the definition of the Ice Cream Ball Weapon class
*/
#include "IceCream.h"
#include "CollisionGeometry.h"
#include "ObjectManager.h"
#include "Enemy.h"
#include "ParticleEmitter.h"

/**
* CIceCream::CIceCream
* @date Modified May 28
*/
CIceCream::CIceCream(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
		 CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPONPROJ_ACIDICE;
	m_pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "icecreamball.x");

	m_pMist = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pMist->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "acidice.xml"));
}
/**
* CIceCream::~CIceCream
* @date Modified May 4, 2006
*/
CIceCream::~CIceCream()
{
}

/**
* CIceCream::update
* @date Modified May 28, 2006
*/
void CIceCream::update()
{
	CProjectileWeapon::update();

	m_pMist->setActorMatrix(getOrientation(), getPosition(), D3DXVECTOR3(1,1,1));
}

/**
* CIceCream::collided
* @date Modified May 28, 2006
*/
void CIceCream::collided(CActor *poActor)
{
	CProjectileWeapon::collided(poActor);
}

/**
* CIceCream::init
* @date Modified May 28, 2006
*/
void CIceCream::init(void)
{
	m_pMist->play();
	CProjectileWeapon::init();
}

/**
* CIceCream::destroy
* @date Modified May 28, 2006
*/
void CIceCream::destroy(void)
{
	m_pMist->pause();
}

