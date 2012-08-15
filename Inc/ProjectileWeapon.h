/**
* @file ProjectileWeapon.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Projectile Weapon class
* used for defining a Projectile weapon
*/

#ifndef _PROJECTILEWEAPON_H_
#define _PROJECTILEWEAPON_H_

#include "BaseWeapon.h"
#include "methodsinglefunctor.h"

/**
* Declares the Projectile Weapon class
* used for defining a Projectile weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CProjectileWeapon : public CBaseWeapon
{
protected:

	//! functor to use for the splash damage
	CMethodSingleFunctor<CProjectileWeapon, CObject*> m_oSplashFunctor;

public:

	/**
	* Constructs a Projectile Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CProjectileWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL) : 
	  CBaseWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter), m_oSplashFunctor(&CProjectileWeapon::doSplashDamage, this)
	{
	}

	/**
	* de-constructs a Projectile Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CProjectileWeapon() = 0
	{
	}

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon()
	{
		return false;
	}

	/**
	* checks a collision with a rocket
	* 
	* @date Created April 24, 2006	
	*/
	virtual void collided(CActor *poActor);

	/**
	* Computes Splash damage for a projectile
	* 
	* @date Created May 4, 2006	
	* @param[in]	vPosition		Position of explosion
	* @param[in]	nAreaOfEffect	Area of effect
	*/
	void doSplashDamage(CObject *poObject);
};

#endif