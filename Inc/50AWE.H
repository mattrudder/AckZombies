/**
* @file 50AWE.h
* @author Jordan Leckner
* @date Created May 4, 2006
*
* This file includes the 50AWE class
* used for defining a 50AWE
*/

#ifndef _50AWE_H_
#define _50AWE_H_

#include "ProjectileWeapon.h"

/**
* Declares the 50AWE class
* used for defining a 50AWE weapon
* 
* @author Jordan Leckner
* @date Created May 4, 2006
* @date Modified May 4, 2006
*/
class C50AWE : public CProjectileWeapon
{
	friend class CDesertEagle50Weapon;
protected:

	/**
	* Constructs a Rocket.
	* 
	* @date Created May 4, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	C50AWE(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL) :
		 CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
	{
		m_eType = OBJ_WEAPONPROJ_50AWE;
	}

public:

	/**
	* de-constructs a 50AWE.
	* 
	* @date Created May 4, 2006	
	*/
	virtual ~C50AWE();

	/**
	* Creates the 50AWE.
	* 
	* @date Created May 4, 2006	
	*/
	static CObject* create(void)
	{
		return new C50AWE;
	}
};

#endif