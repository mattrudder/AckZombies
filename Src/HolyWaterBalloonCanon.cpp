/**
* @file HolyWaterBalloonCanon.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the HolyWaterBalloonCanon Weapon class
*/
#include "HolyWaterBalloonCanon.h"
#include "Grenade.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "Player.h"

/**
* CBigCluckingBombWeapon::CBigCluckingBombWeapon
* @date Modified May 6, 2006
*/
CHolyWaterBalloonCanonWeapon::CHolyWaterBalloonCanonWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_HOLYWATERCANNON;

	m_pGrenadeMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "waterballoon.x");

	m_fTimer = CTimer::getInstance().getTime();
}

/**
* CHolyWaterBalloonCanonWeapon::~CHolyWaterBalloonCanonWeapon
* @date Modified April 7, 2006
*/
CHolyWaterBalloonCanonWeapon::~CHolyWaterBalloonCanonWeapon()
{
}

/**
* CHolyWaterBalloonCanonWeapon::fireWeapon
* @date Modified June 10, 2006
*/
bool CHolyWaterBalloonCanonWeapon::fireWeapon()
{
	if (!m_nAmmoLeft || !m_poCharacter)
		return false;

	if (CTimer::getInstance().getTime() - m_fTimer >= 0.35f)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsFired();

		CGrenadeWeapon* poGrenade = (CGrenadeWeapon*)CObjectManager::getInstance().createObject(OBJ_WEAPON_GRENADE);
		poGrenade->m_pMesh = m_pGrenadeMesh;
		// increase reference count
		m_pGrenadeMesh->addRef();
		// set the color to the grenade to a random one
		poGrenade->setColor(CGrenadeWeapon::getRandomColor());

		D3DXVECTOR3 vPos;
		if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;

		vPos += m_poCharacter->getOrientation() * 5;
		vPos.y += 3.0;

		poGrenade->setPosition(vPos);
		poGrenade->m_oBV.centerPt = poGrenade->getPosition();
		poGrenade->m_oBV.fRadius = 1.0f;
		poGrenade->setOrientation(m_poCharacter->getOrientation());
		poGrenade->m_vVelocity = poGrenade->getOrientation() * 100.0f;
		poGrenade->m_vVelocity.y += 5.0f;
		poGrenade->m_nDamage = m_nDamage;
		poGrenade->m_nAreaOfEffect = m_nAreaOfEffect;
		poGrenade->setScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
		poGrenade->setPlayer((CPlayer*)m_poCharacter);
		// Initial time
		poGrenade->m_fTimer = CTimer::getInstance().getTime();
		poGrenade->activate();
		m_nAmmoLeft--;
		m_fTimer = CTimer::getInstance().getTime();
		// Play sound
		CSoundManager::getInstance().playSound(SND_EFFECT_WATERCANNONFIRE);
		return true;
	}
	return false;
}