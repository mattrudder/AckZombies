/**
* @file ProjectileWeapon.cpp
* @author Jordan Leckner
* @date Created May 4, 2006
*
* This file includes the definition of the Projectile Weapon class
*/
#include "ProjectileWeapon.h"
#include "ObjectManager.h"
#include "MenuManager.h"
#include "HUD.h"
#include "Enemy.h"
#include "Player.h"

/**
* CProjectileWeapon::collided
* @date Modified May 4, 2006
*/
void CProjectileWeapon::doSplashDamage(CObject *poObject)
{
	// Splash Damage ; check all enemies
	SSphere SplashArea(getBV().centerPt, (float)m_nAreaOfEffect);
	CObjectManager::ObjectList lEnemyList;
	D3DXVECTOR3 ptOfCol;
	float fDistance;
	if (checkCol_SphereSphere(&ptOfCol, ((CEnemy*)poObject)->getBV(), SplashArea))
	{
		// Distance from player to the center point of the AOE
		fDistance = computeDistance(((CEnemy*)poObject)->getBV().centerPt, SplashArea.centerPt);
		float fSplashDamage = (float)(m_nDamage << 2) / fDistance;
		((CEnemy*)poObject)->setHealth(((CEnemy*)poObject)->getHealth() - (int)fSplashDamage);
		// Add to the awesome meter if the enemy is dead
		if (((CEnemy*)poObject)->getHealth() <= 0)
		{
			CMenuManager::getInstance().getHUD()->addToAwesomeMeter(1);
			if(m_poCharacter->getType() == OBJ_PLAYER)
				((CPlayer*)m_poCharacter)->m_oStats.addEnemiesKilled();
		}
		else
		{
			// play shot animation on the enemy
			((CAnimatedMesh*)((CEnemy*)poObject)->getMesh())->setAnimationSetByName("Shot");
			((CEnemy*)poObject)->setAnimationTime(CTimer::getInstance().getTime());
			((CEnemy*)poObject)->setTempVel(((CEnemy*)poObject)->getVelocity());
		}
	}
}

/**
* CProjectileWeapon::collided
* @date Modified May 5, 2006
*/
void CProjectileWeapon::collided(CActor *poActor)
{
	if (!poActor)
	{
		if (!((m_eType & OBJ_WEAPON_GRENADE) == OBJ_WEAPON_GRENADE))
		{
			CObjectManager::getInstance().removeObject((CObject*)this);
		}
		return;
	}

	// Make sure projectiles don't hit the one who threw it.
	if ((poActor->getType() & OBJ_WEAPONPROJ) == OBJ_WEAPONPROJ)
	{
		// don't collide with ourselves.
		if (getCharacter() == (CCharacter*)poActor)
			return;
	}

	// Enemy collision
	if (poActor && (poActor->getType() & OBJ_ENEMY) == OBJ_ENEMY)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		((CPlayer*)(loPlayers.front()))->m_oStats.addShotsHit();

		if ((m_eType & OBJ_WEAPONPROJ_ACIDICE) != OBJ_WEAPONPROJ_ACIDICE &&
			(m_eType & OBJ_WEAPONPROJ_FOOTBALL) != OBJ_WEAPONPROJ_FOOTBALL)
		{
			// Damage the enemy
			((CEnemy*)poActor)->setHealth(((CEnemy*)poActor)->getHealth() - m_nDamage);

			// Add to the awesome meter if the enemy is dead
			if (((CEnemy*)poActor)->getHealth() <= 0)
			{
				CMenuManager::getInstance().getHUD()->addToAwesomeMeter(1);
				((CPlayer*)m_poCharacter)->m_oStats.addEnemiesKilled();
			}
		}

		if ((m_eType & OBJ_WEAPONPROJ_50AWE) != OBJ_WEAPONPROJ_50AWE &&
			(m_eType & OBJ_WEAPONPROJ_ACIDICE) != OBJ_WEAPONPROJ_ACIDICE)
		{
			// remove the projectile from the object manager
			CObjectManager::getInstance().removeObject((CObject*)this);
		}
	}
	// Player collision
	if (poActor && ((poActor->getType() & OBJ_PLAYER) == OBJ_PLAYER))
	{
		// Damage the player
		if ((m_eType & OBJ_WEAPONPROJ_ACIDICE) == OBJ_WEAPONPROJ_ACIDICE)
		{
			((CPlayer*)poActor)->setHealth(((CPlayer*)poActor)->getHealth() - m_nDamage);
			CObjectManager::getInstance().removeObject((CObject*)this);
		}
	}
	// Projectile collision
	if (poActor && (poActor->getType() & OBJ_WEAPONPROJ) == OBJ_WEAPONPROJ)
	{
		CObjectManager::getInstance().removeObject((CObject*)this);
	}
	if (poActor && (poActor->getType() & OBJ_OBJECTIVE_BARRIER) == OBJ_OBJECTIVE_BARRIER)
	{
		CObjectManager::getInstance().removeObject((CObject*)this);
	}

	// do Splash Damage
	CObjectManager::getInstance().iterateObjects(OBJ_ENEMY, &m_oSplashFunctor);
}