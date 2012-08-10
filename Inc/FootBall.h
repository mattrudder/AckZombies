/**
* @file FootBall.h
* @author Jordan Leckner
* @date Created May 9, 2006
*
* This file includes the FootBall ball class
* used for defining a FootBall
*/

#ifndef _FOOTBALL_H_
#define _FOOTBALL_H_

#include "ProjectileWeapon.h"
#include "ResourceManager.h"

/**
* Declares the FootBall class
* used for defining a FootBall weapon
* 
* @author Jordan Leckner
* @date Created May 9, 2006
* @date Modified May 9, 2006
*/
class CFootBall : public CProjectileWeapon
{
	friend class CAIStateQBRangeAttack;
protected:

	/**
	* Constructs a FootBall.
	* 
	* @date Created May 4, 2006	
	* @param[in]	nDamage		Amount of damage this weapon does
	* @param[in]	nCost		Amount of money this weapon costs
	* @param[in]    nAmmo		Amount of ammo this weapon has
	* @param[in]	nAOE		Radius of area of effect this weapon has
	* @param[in]	poPlayer	Pointer to the player that has this weapon
	*/
	CFootBall(int nDamage = 15, int nCost = 0, int nAmmo = 0, int nAOE = 0, CCharacter *poCharacter = NULL) :
		 CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
	{
		m_eType = OBJ_WEAPONPROJ_FOOTBALL;
		m_pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "football.x");
	}

public:

	/**
	* de-constructs a Ice Cream Ball.
	* 
	* @date Created May 4, 2006	
	*/
	virtual ~CFootBall();

	/**
	* Creates the Ice Cream Ball.
	* 
	* @date Created May 4, 2006	
	*/
	static CObject* create(void)
	{
		return new CFootBall;
	}
};

#endif