/**
* @file DesertEagle50.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the Desert Eagle 50 Weapon class
*/
#include "DesertEagle50.h"
#include "50AWE.h"
#include "ObjectManager.h"
#include "Player.h"
#include "SoundManager.h"

/**
* CDesertEagle50Weapon::CDesertEagle50Weapon
* @date Modified April 7, 2006
*/
CDesertEagle50Weapon::CDesertEagle50Weapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_DEAGLE;

	m_poBulletMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "50awe.x");

	m_fTimer = CTimer::getInstance().getTime();
}

/**
* CDesertEagle50Weapon::~CDesertEagle50Weapon
* @date Modified April 7, 2006
*/
CDesertEagle50Weapon::~CDesertEagle50Weapon()
{
}

/**
* CDesertEagle50Weapon::fireWeapon
* @date Modified April 24, 2006
*/
bool CDesertEagle50Weapon::fireWeapon()
{
	if (!m_nAmmoLeft || !m_poCharacter)
		return false;

	if (CTimer::getInstance().getTime() - m_fTimer >= 1.00f)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsFired();

		C50AWE* po50AWE = (C50AWE*)CObjectManager::getInstance().createObject(OBJ_WEAPONPROJ_50AWE);
		po50AWE->m_pMesh = m_poBulletMesh;
		po50AWE->setScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
		// increase reference count
		m_poBulletMesh->addRef();

		D3DXVECTOR3 vPos;

		if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;

		vPos += m_poCharacter->getOrientation() * 18.0f;
		vPos.y += 3.0;

		po50AWE->setPosition(vPos);
		po50AWE->m_oBV.centerPt = po50AWE->getPosition();
		po50AWE->m_oBV.fRadius = 4.0f;
		po50AWE->setOrientation(m_poCharacter->getOrientation());
		po50AWE->m_vVelocity = po50AWE->getOrientation() * 100.0f;
		po50AWE->m_nDamage = m_nDamage;
		po50AWE->m_nAreaOfEffect = m_nAreaOfEffect;
		po50AWE->activate();
		po50AWE->setPlayer((CPlayer*)m_poCharacter);
		m_nAmmoLeft--;
		m_fTimer = CTimer::getInstance().getTime();
		// Play sound
		CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION);
		return true;
	}
	return false;
}