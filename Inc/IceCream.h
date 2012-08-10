/**
* @file IceCream.h
* @author Jordan Leckner
* @date Created May 4, 2006
*
* This file includes the IceCream ball class
* used for defining a icecreamball
*/

#ifndef _ICECREAM_H_
#define _ICECREAM_H_

#include "ProjectileWeapon.h"
#include "ResourceManager.h"
#include "ObjectManager.h"

class CParticleEmitter;

/**
* Declares the IceCream class
* used for defining a Rocket weapon
* 
* @author Jordan Leckner
* @date Created May 4, 2006
* @date Modified May 28, 2006
*/
class CIceCream : public CProjectileWeapon
{
	friend class CAIStateAcidicAttack;
protected:

	//! Smoke particle effect
	CParticleEmitter* m_pMist;

	/**
	* Constructs a Ice Cream Ball.
	* 
	* @date Created May 4, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CIceCream(int nDamage = 15, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

public:

	/**
	* de-constructs a Ice Cream Ball.
	* 
	* @date Created May 4, 2006	
	*/
	virtual ~CIceCream();

	/**
	* Creates the Ice Cream Ball.
	* 
	* @date Created May 4, 2006	
	*/
	static CObject* create(void)
	{
		return new CIceCream;
	}

	/**
	* update the actor each frame
	*
	* @date Created May 28, 2006
	*/
	virtual void update();

	/**
	* checks a collision with a rocket
	* 
	* @date Created May 28, 2006	
	*/
	virtual void collided(CActor *poActor);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 28, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* Used to notify objects of removal from the object manager's active list.
	*
	* @date Created May 28, 2006
	*/
	virtual void destroy(void);
};

#endif