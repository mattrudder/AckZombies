/**
* @file RayCastWeapon.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Ray Cast Weapon class
* used for defining a ray cast weapon
*/

#ifndef _RAYCASTWEAPON_H_
#define _RAYCASTWEAPON_H_

#include "BaseWeapon.h"
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "ObjectManager.h"

/**
* Declares the Ray Cast Weapon class
* used for defining a Ray Cast weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CRayCastWeapon : public CBaseWeapon
{
protected:

public:

	/**
	* Constructs a Ray Cast Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CRayCastWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* de-constructs a Ray Cast Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CRayCastWeapon() = 0
	{
	}

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	bool fireWeapon(CEnemy *poEnemy);

	/**
	* Fires a Weapon.
	* 
	* @date Created April 29, 2006	
	*/
	bool fireWeapon(CEnemy *poEnemy, D3DXVECTOR3 &Orientation);
};

#endif