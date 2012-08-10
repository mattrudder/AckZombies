/**
* @file Rocket.h
* @author Jordan Leckner
* @date Created April 24, 2006
*
* This file includes the Rocket class
* used for defining a rocket
*/

#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "ProjectileWeapon.h"

class CParticleEmitter;

/**
* Declares the Rocket class
* used for defining a Rocket weapon
* 
* @author Jordan Leckner
* @date Created April 24, 2006
* @date Modified April 24, 2006
*/
class CRocket : public CProjectileWeapon
{
	friend class CRocketLauncherWeapon;
protected:

	//! Smoke particle effect
	CParticleEmitter* m_pSmoke;
	//! Explosion particle effect
	CParticleEmitter* m_pExplosion;

	/**
	* Constructs a Rocket.
	* 
	* @date Created April 24, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CRocket(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

public:

	/**
	* de-constructs a Rocket.
	* 
	* @date Created April 24, 2006	
	*/
	virtual ~CRocket();

	/**
	* Creates the Rocket.
	* 
	* @date Created April 24, 2006	
	*/
	static CObject* create(void)
	{
		return new CRocket;
	}

	/**
	* update the actor each frame
	*
	* @date Created April 23, 2006
	*/
	virtual void update();

	/**
	* checks a collision with a rocket
	* 
	* @date Created April 24, 2006	
	*/
	virtual void collided(CActor *poActor);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* Used to notify objects of removal from the object manager's active list.
	*
	* @date Created May 26, 2006
	*/
	virtual void destroy(void);
};

#endif