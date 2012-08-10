/**
* @file RayCastWeapon.cpp
* @author Jordan Leckner
* @date Created April 13, 2006
*
* This file includes the definition of the RayCastWeapon class
*/
#include "RayCastWeapon.h"
#include "Math3d.h"
#include "Enemy.h"
#include "Player.h"
#include "CollisionGeometry.h"
#include "SoundManager.h"

/**
* CRayCastWeapon::CRayCastWeapon
* @date Modified June 2, 2006
*/
CRayCastWeapon::CRayCastWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) : 
	CBaseWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
}

/**
* CRayCastWeapon::fireWeapon
* @date Modified April 18, 2006
*/
bool CRayCastWeapon::fireWeapon(CEnemy *poEnemy)
{
	if (!poEnemy)
		return false;

	D3DXVECTOR3 point(0,0,0);
	vector<D3DXVECTOR3> ptsOfCol;
	vector<float> dirLengths;
	CCollisionGeometry *poCollision = CCollisionGeometry::getInstancePtr();
	D3DXVECTOR3 vPos;
	if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;

	// get the distance from the player to the enemy to test against geometry
	float fEnemyDist = computeDistance(poEnemy->getBV().centerPt, vPos);
	if (checkCol_RaySphere(&point, vPos, m_poCharacter->getOrientation(), poEnemy->getBV()))
	{
		if (poCollision->checkRayCollision(ptsOfCol, dirLengths, poCollision->getRoot(), vPos,
			m_poCharacter->getOrientation()))
		{
			// if the player is farther than any geometry, then they cannot be hit
			// because they are obstructed by geometry.
			for (unsigned int i = 0; i < dirLengths.size(); ++i)
				if (fEnemyDist > dirLengths[i])
					return false;
		}
		// Zombie sounds
		if ((poEnemy->getType() & OBJ_ENEMY_ZOMBIECITIZEN) == OBJ_ENEMY_ZOMBIECITIZEN)
		{
			CSoundManager::getInstance().playSound(SND_EFFECT_ZOMBIETAKEDAMAGE);
		}
		return true;
	}
	return false;
}

/**
* CRayCastWeapon::fireWeapon
* @date Modified April 18, 2006
*/
bool CRayCastWeapon::fireWeapon(CEnemy *poEnemy, D3DXVECTOR3 &Orientation)
{
	if (!poEnemy)
		return false;

	D3DXVECTOR3 point(0,0,0);
	vector<D3DXVECTOR3> ptsOfCol;
	vector<float> dirLengths;
	CCollisionGeometry *poCollision = CCollisionGeometry::getInstancePtr();

	D3DXVECTOR3 vPos;
	if (!getPlayer()->getWeaponActorPos(&vPos))
			vPos = m_poCharacter->getBV().centerPt;

	// get the distance from the player to the enemy to test against geometry
	float fEnemyDist = computeDistance(poEnemy->getBV().centerPt, vPos);

	if (checkCol_RaySphere(&point, vPos, Orientation, poEnemy->getBV()))
	{
		if (poCollision->checkRayCollision(ptsOfCol, dirLengths, poCollision->getRoot(), vPos,
			Orientation))
		{
			// if the player is farther than any geometry, then they cannot be hit
			// because they are obstructed by geometry.
			for (unsigned int i = 0; i < dirLengths.size(); ++i)
				if (fEnemyDist > dirLengths[i])
					return false;
		}
		return true;
	}
	return false;
}