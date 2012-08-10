/**
* @file BaseWeapon.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Base Weapon class
* used for defining a weapon from which all others will derive from
*/

#ifndef _BASEWEAPON_H_
#define _BASEWEAPON_H_

#include "Actor.h"
class CPlayer;
class CCharacter;

#ifndef NULL
#define NULL 0L
#endif

/**
* Declares the Base Weapon class
* used for defining a base weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CBaseWeapon : public CActor
{
protected:

	//! Amount of damage this weapon does
	int			m_nDamage;
	//! Amount of money this weapon costs to buy
	int			m_nCost;
	//! Amount of ammo left for this weapon
	int			m_nAmmoLeft;
	//! Radius of effect, if any
	int			m_nAreaOfEffect;
	//! Pointer to the player that has this weapon
	CCharacter*	m_poCharacter;

	/**
	* Constructs a Base Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CBaseWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL) :
		m_nDamage(nDamage), m_nCost(nCost), m_nAmmoLeft(nAmmo), m_nAreaOfEffect(nAOE), m_poCharacter(poCharacter),
		CActor(OBJ_WEAPON)
	{
	}

public:

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		CActor::init();
	}

	/**
	* de-constructs a Base Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CBaseWeapon()
	{
	}

	//! Accessors
	inline int getAmmo() { return m_nAmmoLeft; }
	inline int getDamage() { return m_nDamage; }
	inline int getCost() { return m_nCost; }
	inline int getAmmoLeft() { return m_nAmmoLeft; }
	inline int getAOE() { return m_nAreaOfEffect; }
	inline CPlayer* getPlayer() { return (CPlayer*)m_poCharacter; }
	inline CCharacter* getCharacter() { return m_poCharacter; }

	//! Modifiers
	inline void setPlayer(CPlayer *poPlayer) { m_poCharacter = (CCharacter*)poPlayer; }
	inline void setAmmo(int nAmmo) { m_nAmmoLeft = nAmmo; }
	inline void setDamage(int nDamage) { m_nDamage = nDamage; }
	inline void setCost(int nCost) { m_nCost = nCost; }
	inline void setAOE(int nAOE) { m_nAreaOfEffect = nAOE; }

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
	* Creates the CDesertEagle50Weapon.
	* 
	* @date Created June 2, 2006	
	*/
	static CObject* create(void)
	{
		CBaseWeapon *pWeapon = new CBaseWeapon;
		return pWeapon;
	}
};

#endif