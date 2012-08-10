/**
* @file Pistol.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Pistol class
* used for defining a Pistol type weapon
*/

#ifndef _PISTOL_H_
#define _PISTOL_H_

#include "RayCastWeapon.h"

/**
* Declares the Pistol Weapon class
* used for defining a Pistol weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CPistolWeapon : public CRayCastWeapon
{
private:

	//! Muzzle Flash
	CParticleEmitter *m_pRay;
	//! Muzzle Flash
	CParticleEmitter *m_pMuzzleFlash;
	//! timer to shoot
	float m_fTimer;
	//! Zombie Shot
	CParticleEmitter *m_pZombieShot;

public:

	/**
	* Constructs a Pistol Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CPistolWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 50;
		m_nCost = 0;
		m_nAmmoLeft = -1;
		m_nAreaOfEffect = 0;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a Pistol Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CPistolWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CPistolWeapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CPistolWeapon *pWeapon = new CPistolWeapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "pistola.x"));
		return pWeapon;
	}
};

#endif