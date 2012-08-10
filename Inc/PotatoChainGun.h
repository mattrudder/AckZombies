/**
* @file PotatoChainGun.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the PotatoChainGun class
* used for defining a PotatoChainGun type weapon
*/

#ifndef _POTATOCHAINGUN_H_
#define _POTATOCHAINGUN_H_

#include "ProjectileWeapon.h"

#define BARREL_RADIUS 1.0f

/**
* Declares the PotatoChainGun Weapon class
* used for defining a PotatoChainGun weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CPotatoChainGunWeapon : public CProjectileWeapon
{
private:

	//! Index of the barrel to shoot out of
	int m_nBarrelIndex;
	//! All of the angles of each barrel
	float m_nAngles[6];
	//! Potato Mehs
	CMesh* m_nPotatoMesh;
	//! timer to shoot
	float m_fTimer;

public:

	/**
	* Constructs a PotatoChainGun Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CPotatoChainGunWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 200;
		m_nCost = 4550;
		m_nAmmoLeft = 50;
		m_nAreaOfEffect = 0;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a PotatoChainGun Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CPotatoChainGunWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CPotatoChainGunWeapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CPotatoChainGunWeapon *pWeapon = new CPotatoChainGunWeapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "potato.x"));
		return pWeapon;
	}
};

#endif