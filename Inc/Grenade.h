/**
* @file Grenade.h
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the Grenade class
* used for defining a Grenade type weapon
*/

#ifndef _GRENADE_H_
#define _GRENADE_H_

#include "ProjectileWeapon.h"

class CParticleEmitter;

/**
* Declares the Grenade Weapon class
* used for defining a Grenade weapon
* 
* @author Jordan Leckner
* @date Created April 7, 2006
* @date Modified April 7, 2006
*/
class CGrenadeWeapon : public CProjectileWeapon
{
	friend class CHolyWaterBalloonCanonWeapon;
	friend class CBigCluckingBombWeapon;
private:

	//! Timer to explode the grenade
	float		m_fTimer;
	float		m_fFireTimer;
	//! counter to limit fire rate
	UINT m_unGrenadeCounter;
	//! current number of kills
	UINT m_unKillCounter;
	//! kills last frame
	UINT m_unKillsLastFrame;
	//! Mesh for the grenade
	CMesh *m_poGrenadeMesh;
	//! Explosion particle effect
	CParticleEmitter* m_pExplosion;

public:

	/**
	* Constructs a Grenade Weapon.
	* 
	* @date Created April 7, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CGrenadeWeapon(int nDamage = 0, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void)
	{
		m_nDamage = 900;
		m_nCost = 0;
		m_nAmmoLeft = 500;
		m_nAreaOfEffect = 20;
		m_poCharacter = NULL;
		CBaseWeapon::init();
	}

	/**
	* de-constructs a Grenade Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual ~CGrenadeWeapon();

	/**
	* Fires a Weapon.
	* 
	* @date Created April 7, 2006	
	*/
	virtual bool fireWeapon();

	/**
	* checks a collision with a grenade
	* 
	* @date Created April 27, 2006
	*/
	virtual void collided(CActor *poActor);

	/**
	* Creates the Grenade.
	* 
	* @date Created April 27, 2006	
	*/
	static CObject* create(void)
	{
		return new CGrenadeWeapon;
	}

	/**
	* update the grenade each frame
	*
	* @date Created May 8, 2006
	*/
	virtual void update();

	/**
	* Used to notify objects of removal from the object manager's active list.
	*
	* @date Created May 26, 2006
	*/
	virtual void destroy(void);

	/**
	* returns the amount of available grenades
	*
	* @date Created June 10, 2006
	*/
	UINT getNumGrenades() {return m_unGrenadeCounter;}

	/**
	* Creates a random color
	*
	* @date Created May 30, 2006
	*/
	static DWORD getRandomColor(void);

	/**
	* updates with spectaculariousnessity
	*
	* @date Created June 10, 2006
	*/
	void spectaculupdate();
};

#endif