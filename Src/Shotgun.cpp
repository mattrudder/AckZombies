/**
* @file Shotgun.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the Shotgun Weapon class
*/
#include "Shotgun.h"
#include "ObjectManager.h"
#include "Enemy.h"
#include "Player.h"
#include "SoundManager.h"
#include "MenuManager.h"
#include "HUD.h"

/**
* CShotgunWeapon::CShotgunWeapon
* @date Modified April 7, 2006
*/
CShotgunWeapon::CShotgunWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CRayCastWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_SHOTGUN;

	m_fTimer = CTimer::getInstance().getTime();

	m_pMuzzleFlash = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pMuzzleFlash->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "MuzzleFlash.xml"));

	m_pShotgunSpray = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pShotgunSpray->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "ShotgunSpray.xml"));

	m_pZombieShot = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pZombieShot->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "ZombieShot.xml"));
}

/**
* CShotgunWeapon::~CShotgunWeapon
* @date Modified April 7, 2006
*/
CShotgunWeapon::~CShotgunWeapon()
{
	CObjectManager::getInstance().removeObject(m_pMuzzleFlash);
	CObjectManager::getInstance().removeObject(m_pZombieShot);
	CObjectManager::getInstance().removeObject(m_pShotgunSpray);
}

/**
* CShotgunWeapon::fireWeapon
* @date Modified April 29, 2006
*/
bool CShotgunWeapon::fireWeapon()
{
	if (!m_nAmmoLeft) // make sure we've got enough ammo
		return false;

	// limit the amount of shooting per second
	if (CTimer::getInstance().getTime() - m_fTimer >= 0.45f)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsFired();

		// list of enemies
		CObjectManager::ObjectList lEnemyList;
		CObjectManager::getInstance().getObjects(OBJ_ENEMY, &lEnemyList);
		bool bHit = false;
		CEnemy *poEnemy;
		float fEnemyDist;
		D3DXVECTOR3 Spread[3]; // the spread of the shotgun made into a triangle
		float fLength = 20.0f; // Max length of distance to shoot
		float fAngle = acosf(D3DXVec3Dot(&m_poCharacter->getOrientation(), &CCamera::getInstance().getCameraRight()));
		D3DXVECTOR3 pos;
		if (!getPlayer()->getWeaponActorPos(&pos))
			pos = m_poCharacter->getBV().centerPt;

		Spread[0].x = pos.x + cosf(fAngle - M_PI/12.0f) * fLength;
		Spread[0].y = pos.y;
		Spread[0].z = pos.z + sinf(fAngle - M_PI/12.0f) * fLength;
		Spread[1].x = pos.x + cosf(fAngle + M_PI/12.0f) * fLength;
		Spread[1].y = pos.y;
		Spread[1].z = pos.z + sinf(fAngle + M_PI/12.0f) * fLength;
		Spread[2] = m_poCharacter->getOrientation();

		Spread[0] = Spread[0] - pos;
		Spread[1] = Spread[1] - pos;

		D3DXVec3Normalize(NULL, &Spread[0], &Spread[0]);
		D3DXVec3Normalize(NULL, &Spread[1], &Spread[1]);

		for (CObjectManager::ObjectList::iterator iter = lEnemyList.begin(); iter != lEnemyList.end(); ++iter)
		{
			poEnemy = (CEnemy*)*iter;

			// can't shoot spawning enemies
			if (!poEnemy->isActive())
				continue;

			// loop through the ray casts created
			for (int i = 0; i < 3; ++i)
				// if the spread is hitting the enemy
				if (CRayCastWeapon::fireWeapon(poEnemy, Spread[i]))
				{
					fEnemyDist = computeDistance(pos, poEnemy->getPosition());
					poEnemy->setHealth(poEnemy->getHealth() - m_nDamage / (int)fEnemyDist);
					// Add to the awesome meter if the enemy is dead
					if (poEnemy->getHealth() <= 0)
					{
						CMenuManager::getInstance().getHUD()->addToAwesomeMeter(1);
						((CPlayer*)m_poCharacter)->m_oStats.addEnemiesKilled();
					}
					bHit = true;
					// play shot animation on the enemy
					((CAnimatedMesh*)poEnemy->getMesh())->setAnimationSetByName("Shot");
					poEnemy->setAnimationTime(CTimer::getInstance().getTime());
					poEnemy->setTempVel(poEnemy->getVelocity());
					// Zombie Shot Particle
					m_pZombieShot->setActorMatrix(poEnemy->getOrientation(), poEnemy->getBV().centerPt, 
						D3DXVECTOR3(1,1,1));
					m_pZombieShot->play();
				}
		}
		m_nAmmoLeft--;
		m_fTimer = CTimer::getInstance().getTime();
		// Play sound
		CSoundManager::getInstance().playSound(SND_EFFECT_SHOTGUNFIRE);

		// Muzzle Flash
		D3DXVECTOR3 vPos;
		if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;
		vPos += getPlayer()->getOrientation() * 7.5f;
		m_pMuzzleFlash->setActorMatrix(getPlayer()->getOrientation(), vPos, D3DXVECTOR3(1,1,1));
		m_pMuzzleFlash->play();
		// Shotgun Spray
		if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;
		m_pShotgunSpray->setActorMatrix(getPlayer()->getOrientation(), vPos, D3DXVECTOR3(1,1,1));
		m_pShotgunSpray->play();

		// only one hit if we've hit anything.
		if (bHit)
			((CPlayer*)(loPlayers.front()))->m_oStats.addShotsHit();
		return bHit;
	}
	return false;
}