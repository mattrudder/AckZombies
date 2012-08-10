/**
* @file RocketLauncher.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the RocketLauncher Weapon class
*/
#include "RocketLauncher.h"
#include "ObjectManager.h"
#include "Rocket.h"
#include "Player.h"
#include "Timer.h"
#include "SoundManager.h"

/**
* CRocketLauncherWeapon::CRocketLauncherWeapon
* @date Modified April 7, 2006
*/
CRocketLauncherWeapon::CRocketLauncherWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_ROCKETLAUNCHER;
	
	m_poRocketMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "PlainRocket.x");

	m_fTimer = CTimer::getInstance().getTime();
}

/**
* CRocketLauncherWeapon::~CRocketLauncherWeapon
* @date Modified April 7, 2006
*/
CRocketLauncherWeapon::~CRocketLauncherWeapon()
{
}

/**
* CRocketLauncherWeapon::fireWeapon
* @date Modified April 24, 2006
*/
bool CRocketLauncherWeapon::fireWeapon()
{
	if (!m_nAmmoLeft || !m_poCharacter)
		return false;

	if (CTimer::getInstance().getTime() - m_fTimer >= 0.65f)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsFired();

		CRocket* poRocket = (CRocket*)CObjectManager::getInstance().createObject(OBJ_WEAPONPROJ_ROCKET);
		poRocket->m_pMesh = m_poRocketMesh;
		// increase reference count
		m_poRocketMesh->addRef();

		D3DXVECTOR3 vPos;
		if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;

		vPos += m_poCharacter->getOrientation() * 5;
		vPos.y += 3.0;

		poRocket->setPosition(vPos);
		poRocket->m_oBV.centerPt = poRocket->getPosition();
		poRocket->m_oBV.fRadius = 3.0f;
		poRocket->setOrientation(m_poCharacter->getOrientation());
		poRocket->m_vVelocity = poRocket->getOrientation() * 100.0f;
		poRocket->m_nDamage = m_nDamage;
		poRocket->m_nAreaOfEffect = m_nAreaOfEffect;
		poRocket->setPlayer((CPlayer*)m_poCharacter);
		poRocket->activate();
		m_nAmmoLeft--;
		m_fTimer = CTimer::getInstance().getTime();
		// Play sound
		CSoundManager::getInstance().playSound(SND_EFFECT_ROCKETLAUNCH);
		return true;
	}
	return false;
}