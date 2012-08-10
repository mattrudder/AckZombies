/**
* @file DesertEagle50.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the DesertEagle50 class
* used for defining a DesertEagle50 type weapon
*/

#ifndef _DESERTEAGLE50_H_
#define _DESERTEAGLE50_H_

#include "ProjectileWeapon.h"

/**
* Declares the DesertEagle50 Weapon class
* used for defining a DesertEagle50 weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CDesertEagle50Weapon : public CProjectileWeapon
{
private:

	//! Mesh for the bullet
	CMesh *m_poBulletMesh;
	//! timer to shoot
	float m_fTimer;

public:

	/**
	* Constructs a DesertEagle50 Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CDesertEagle50Weapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 10000;
		m_nCost = 8250;
		m_nAmmoLeft = 15;
		m_nAreaOfEffect = 0;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a DesertEagle50 Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CDesertEagle50Weapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CDesertEagle50Weapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CDesertEagle50Weapon *pWeapon = new CDesertEagle50Weapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "desertEagle.x"));
		return pWeapon;
	}
};

#endif