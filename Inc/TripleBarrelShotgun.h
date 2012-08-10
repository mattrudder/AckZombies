	/**
* @file TripleBarrelShotgun.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Triple Barrel Shotgun class
* used for defining a Triple Barrel Shotgun type weapon
*/

#ifndef _TRIPLEBARRELSHOTGUN_H_
#define _TRIPLEBARRELSHOTGUN_H_

#include "RayCastWeapon.h"

/**
* Declares the Triple Barrel Shotgun Weapon class
* used for defining a Triple Barrel Shotgun weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CTripleBarrelShotgunWeapon : public CRayCastWeapon
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
	* Constructs a Triple Barrel Shotgun Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CTripleBarrelShotgunWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 4000;
		m_nCost = 2150;
		m_nAmmoLeft = 30;
		m_nAreaOfEffect = 0;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a Triple Barrel Shotgun Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CTripleBarrelShotgunWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CTripleBarrelShotgunWeapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CTripleBarrelShotgunWeapon *pWeapon = new CTripleBarrelShotgunWeapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "tripleshotgun.x"));
		return pWeapon;
	}
};

#endif