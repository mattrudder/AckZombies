/**
* @file BigCluckingBomb.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the BigCluckingBomb Weapon class
*/
#include "BigCluckingBomb.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ParticleEmitter.h"
#include "SoundManager.h"

/**
* CBigCluckingBombWeapon::CBigCluckingBombWeapon
* @date Modified May 28, 2006
*/
CBigCluckingBombWeapon::CBigCluckingBombWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CGrenadeWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter), m_fMoveTimer(0)
{
	m_eType = OBJ_WEAPON_BIGCLUCKINGBOMB;

	m_poGrenadeMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "BigCluckingBomb.x");
	// Create the explosion particle effect
	m_pExplosion = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pExplosion->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "ChickenBomb.xml"));
}

/**
* CBigCluckingBombWeapon::~CBigCluckingBombWeapon
* @date Modified May 8, 2006
*/
CBigCluckingBombWeapon::~CBigCluckingBombWeapon()
{
}

/**
* CBigCluckingBombWeapon::fireWeapon
* @date Modified May 28, 2006
*/
bool CBigCluckingBombWeapon::fireWeapon()
{
	if (!m_nAmmoLeft)
		return false;
	// limit the amount of shooting per second
	if (CTimer::getInstance().getTime() - m_fFireTimer >= 1.0f)
	{
		// Initial time
		m_fTimer = CTimer::getInstance().getTime();

		CBigCluckingBombWeapon *poBCB = (CBigCluckingBombWeapon*)CObjectManager::getInstance().createObject(OBJ_WEAPON_BIGCLUCKINGBOMB);
		poBCB->m_pMesh = m_poGrenadeMesh;
		// increase reference count
		m_poGrenadeMesh->addRef();
		poBCB->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
		D3DXVECTOR3 vPos = m_poCharacter->getBV().centerPt + m_poCharacter->getOrientation() * 5;
		poBCB->setPosition(vPos);
		poBCB->m_oBV.centerPt = vPos;
		poBCB->m_oBV.fRadius = 2.5f;
		poBCB->setOrientation(m_poCharacter->getOrientation());
		poBCB->m_vVelocity = poBCB->getOrientation() * 25.0f;
		poBCB->m_vVelocity.y += 25.0f;
		poBCB->m_fTimer = m_fTimer;
		poBCB->activate();
		poBCB->m_nDamage = m_nDamage;
		poBCB->m_nAreaOfEffect = m_nAreaOfEffect;
		poBCB->setPlayer((CPlayer*)m_poCharacter);
		m_fFireTimer = CTimer::getInstance().getTime();
		m_nAmmoLeft--;
		return true;
	}
	return false;
}

/**
* CBigCluckingBombWeapon::collided
* @date Modified May 28, 2006
*/
void CBigCluckingBombWeapon::collided(CActor *poActor)
{
	CGrenadeWeapon::collided(poActor);
}

/**
* CBigCluckingBombWeapon::update
* @date Modified May 28, 2006
*/
void CBigCluckingBombWeapon::update()
{
	CSoundManager::getInstance().playSound(SND_EFFECT_BIGCLUCKINGBOMB_MOVE, 0, false, false);
	CGrenadeWeapon::update();
}
