/**
* @file PotatoChainGun.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the PotatoChainGun Weapon class
*/
#include "PotatoChainGun.h"
#include "ObjectManager.h"
#include "Potato.h"
#include "Player.h"
#include "SoundManager.h"

/**
* CPotatoChainGunWeapon::CPotatoChainGunWeapon
* @date Modified May 5, 2006
*/
CPotatoChainGunWeapon::CPotatoChainGunWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_POTATOGUN;
	m_nBarrelIndex = 0;

	// Angles of the barrels
	m_nAngles[0] = 30.0f;
	m_nAngles[1] = 90.0f;
	m_nAngles[2] = 150.0f;
	m_nAngles[3] = 210.0f;
	m_nAngles[4] = 270.0f;
	m_nAngles[5] = 330.0f;

	m_nPotatoMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "potato.x");

	m_fTimer = CTimer::getInstance().getTime();
}

/**
* CPotatoChainGunWeapon::~CPotatoChainGunWeapon
* @date Modified April 7, 2006
*/
CPotatoChainGunWeapon::~CPotatoChainGunWeapon()
{
}

/**
* CPotatoChainGunWeapon::fireWeapon
* @date Modified May 5, 2006
*/
bool CPotatoChainGunWeapon::fireWeapon()
{
	if (!m_nAmmoLeft)
		return false;

	if (CTimer::getInstance().getTime() - m_fTimer >= 0.15f)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsFired();

		// use the player's position as the initial position of the potato
		D3DXVECTOR3 vPotatoPos;
		if (!getPlayer()->getWeaponActorPos(&vPotatoPos))
			vPotatoPos = m_poCharacter->getBV().centerPt;

		vPotatoPos += m_poCharacter->getOrientation() * 5.0f;
		
		vPotatoPos.x += cosf(degreesToRadians(m_nAngles[m_nBarrelIndex]));
		vPotatoPos.z += sinf(degreesToRadians(m_nAngles[m_nBarrelIndex]));
		// loop back to zero if we go over
		++m_nBarrelIndex %= 5;

		vPotatoPos.x *= BARREL_RADIUS;
		vPotatoPos.z *= BARREL_RADIUS;

		CPotato *pPotato = (CPotato*)CObjectManager::getInstance().createObject(OBJ_WEAPONPROJ_POTATO);

		pPotato->m_pMesh = m_nPotatoMesh;
		pPotato->m_pMesh->addRef();
		pPotato->setPosition(vPotatoPos + m_poCharacter->getOrientation() * 5);
		pPotato->m_oBV.centerPt = pPotato->getPosition();
		pPotato->m_oBV.fRadius = 1.0f;
		pPotato->setOrientation(m_poCharacter->getOrientation());
		pPotato->m_vVelocity = pPotato->getOrientation()* 100.0f;
		pPotato->m_nDamage = m_nDamage;
		pPotato->m_nAreaOfEffect = m_nAreaOfEffect;
		pPotato->setPlayer((CPlayer*)m_poCharacter);
		pPotato->activate();

		m_nAmmoLeft--;
		m_fTimer = CTimer::getInstance().getTime();
		// Play sound
		CSoundManager::getInstance().playSound(SND_EFFECT_POTATOFIRE);
		return true;
	}
	return false;
}



