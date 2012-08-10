/**
* @file BigCluckingBomb.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the BigCluckingBomb class
* used for defining a BigCluckingBomb type weapon
*/

#ifndef _BIGCLUCKINGBOMB_H_
#define _BIGCLUCKINGBOMB_H_

#include "Grenade.h"

/**
* Declares the BigCluckingBomb Weapon class
* used for defining a BigCluckingBomb weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CBigCluckingBombWeapon : public CGrenadeWeapon
{
private:

	//! Timer to play the move sound
	float m_fMoveTimer;

public:

	/**
	* Constructs a BigCluckingBomb Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CBigCluckingBombWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 1800;
		m_nCost = 3250;
		m_nAmmoLeft = 5;
		m_nAreaOfEffect = 30;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a BigCluckingBomb Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CBigCluckingBombWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* checks a collision with a big clucking bomb
	* 
	* @date Created May 8, 2006
	*/
	virtual void collided(CActor *poActor);

	/**
	* Creates the Big Clucking Bomb.
	* 
	* @date Created May 8, 2006	
	*/
	static CObject* create(void)
	{
		return new CBigCluckingBombWeapon;
	}

	/**
	* update the big clucking bomb each frame
	*
	* @date Created May 8, 2006
	*/
	virtual void update();
};

#endif