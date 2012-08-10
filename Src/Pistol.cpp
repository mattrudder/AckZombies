/**
* @file Pistol.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the Pistol Weapon class
*/
#include "Pistol.h"
#include "ObjectManager.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "Player.h"
#include "MenuManager.h"
#include "HUD.h"

/**
* CPistolWeapon::CPistolWeapon
* @date Modified May 18, 2006
*/
CPistolWeapon::CPistolWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
CRayCastWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_PISTOL;

	m_fTimer = CTimer::getInstance().getTime();

	m_pMuzzleFlash = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pMuzzleFlash->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "MuzzleFlash.xml"));

	m_pRay = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pRay->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "Rays.xml"));

	m_pZombieShot = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pZombieShot->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "ZombieShot.xml"));
}

/**
* CPistolWeapon::~CPistolWeapon
* @date Modified April 7, 2006
*/
CPistolWeapon::~CPistolWeapon()
{
	CObjectManager::getInstance().removeObject(m_pMuzzleFlash);
	CObjectManager::getInstance().removeObject(m_pRay);
	CObjectManager::getInstance().removeObject(m_pZombieShot);
}

/**
* CPistolWeapon::fireWeapon
* @date Modified May 18, 2006
*/
bool CPistolWeapon::fireWeapon()
{
	// limit the amount of shooting per second
	if (CTimer::getInstance().getTime() - m_fTimer >= 0.25f)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsFired();

		// list of enemies
		CObjectManager::ObjectList lEnemyList;
		CObjectManager::getInstance().getObjects(OBJ_ENEMY, &lEnemyList);
		CEnemy *poEnemy;
		bool bHit = false;

		for (CObjectManager::ObjectList::iterator iter = lEnemyList.begin(); iter != lEnemyList.end(); ++iter)
		{
			poEnemy = (CEnemy*)*iter;
			// can't shoot spawning enemies
			if (!poEnemy->isActive())
				continue;

			if (CRayCastWeapon::fireWeapon(poEnemy))
			{
				poEnemy->setHealth(poEnemy->getHealth() - m_nDamage);
				// Add to the awesome meter if the enemy is dead
				if (poEnemy->getHealth() <= 0)
				{
					CMenuManager::getInstance().getHUD()->addToAwesomeMeter(1);
					((CPlayer*)m_poCharacter)->m_oStats.addEnemiesKilled();
				}
				bHit = true;
				((CPlayer*)(loPlayers.front()))->m_oStats.addShotsHit();
				m_pMuzzleFlash->pause();
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
		m_fTimer = CTimer::getInstance().getTime();
		// Play sound
		CSoundManager::getInstance().playSound(SND_EFFECT_PISTOLFIRE);
		// Muzzle Flash
		D3DXVECTOR3 vPos;
		if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;

		vPos += getPlayer()->getOrientation() * 3.5f;
		vPos.y += 0.5f;
		m_pMuzzleFlash->setActorMatrix(getPlayer()->getOrientation(), vPos, D3DXVECTOR3(1,1,1));
		m_pMuzzleFlash->play();
		// Ray
		m_pRay->setActorMatrix(getPlayer()->getOrientation(), vPos, D3DXVECTOR3(1,1,1));
		m_pRay->play();

		return bHit;
	}
	return false;
}

