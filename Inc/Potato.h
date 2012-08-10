/**
* @file Potato.h
* @author Jordan Leckner
* @date Created April 24, 2006
*
* This file includes the Potato class
* used for defining a Potato
*/

#ifndef _POTATO_H_
#define _POTATO_H_

#include "ProjectileWeapon.h"

/**
* Declares the Potato class
* used for defining a Potato weapon
* 
* @author Jordan Leckner
* @date Created Mary 5, 2006
* @date Modified May 5, 2006
*/
class CPotato : public CProjectileWeapon
{
	friend class CPotatoChainGunWeapon;
protected:

	/**
	* Constructs a Potato.
	* 
	* @date Created May 5, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CPotato(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL) :
		 CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
	{
		m_eType = OBJ_WEAPONPROJ_POTATO;
	}

public:

	/**
	* de-constructs a CPotato.
	* 
	* @date Created May 5, 2006	
	*/
	virtual ~CPotato();

	/**
	* Creates the CPotato.
	* 
	* @date Created May 5, 2006	
	*/
	static CObject* create(void)
	{
		return new CPotato;
	}
};

#endif