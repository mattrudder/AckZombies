/**
* @file Shotgun.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Shotgun class
* used for defining a Shotgun type weapon
*/

#ifndef _SHOTGUN_H_
#define _SHOTGUN_H_

#include "RayCastWeapon.h"

/**
* Declares the Shotgun Weapon class
* used for defining a Shotgun weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CShotgunWeapon : public CRayCastWeapon
{
private:

	//! Muzzle Flash
	CParticleEmitter *m_pMuzzleFlash;
	//! Shotgun Spray
	CParticleEmitter *m_pShotgunSpray;
	//! Zombie Shot
	CParticleEmitter *m_pZombieShot;
	//! timer to shoot
	float m_fTimer;

public:

	/**
	* Constructs a Shotgun Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CShotgunWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 2000;
		m_nCost = 1250;
		m_nAmmoLeft = 30;
		m_nAreaOfEffect = 0;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a Shotgun Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CShotgunWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CShotgunWeapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CShotgunWeapon *pWeapon = new CShotgunWeapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "shotgun.x"));
		return pWeapon;
	}
};

#endif