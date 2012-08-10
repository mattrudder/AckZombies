/**
* @file HolyWaterBalloonCanon.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the HolyWaterBalloonCanon class
* used for defining a HolyWaterBalloonCanon type weapon
*/

#ifndef _HOLYWATERBALLOONCANON_H_
#define _HOLYWATERBALLOONCANON_H_

#include "ProjectileWeapon.h"

/**
* Declares the HolyWaterBalloonCanon Weapon class
* used for defining a HolyWaterBalloonCanon weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CHolyWaterBalloonCanonWeapon : public CProjectileWeapon
{
private:

	//! Mesh for the grenade
	CMesh *m_pGrenadeMesh;
	//! timer to shoot
	float m_fTimer;

public:

	/**
	* Constructs a HolyWaterBalloonCanon Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CHolyWaterBalloonCanonWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 350;
		m_nCost = 3750;
		m_nAmmoLeft = 15;
		m_nAreaOfEffect = 10;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a HolyWaterBalloonCanon Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CHolyWaterBalloonCanonWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* Creates the CHolyWaterBalloonCanonWeapon.
	* 
	* @date Created May 21, 2006	
	*/
	static CObject* create(void)
	{
		CHolyWaterBalloonCanonWeapon *pWeapon = new CHolyWaterBalloonCanonWeapon;
		pWeapon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "balonzooka.x"));
		return pWeapon;
	}
};

#endif