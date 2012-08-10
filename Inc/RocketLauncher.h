/**
* @file RocketLauncher.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the RocketLauncher class
* used for defining a RocketLauncher type weapon
*/

#ifndef _ROCKETLAUNCHER_H_
#define _ROCKETLAUNCHER_H_

#include "ProjectileWeapon.h"

/**
* Declares the RocketLauncher Weapon class
* used for defining a RocketLauncher weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CRocketLauncherWeapon : public CProjectileWeapon
{
private:

	//! Mesh for the rocket
	CMesh *m_poRocketMesh;
	//! timer to shoot
	float m_fTimer;

public:

	/**
	* Constructs a RocketLauncher Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CRocketLauncherWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 900;
		m_nCost = 6500;
		m_nAmmoLeft = 5;
		m_nAreaOfEffect = 30;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a RocketLauncher Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CRocketLauncherWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CRocketLauncherWeapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CRocketLauncherWeapon *pWeapon = new CRocketLauncherWeapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "PlainRocket.x"));
		return pWeapon;
	}
};

#endif