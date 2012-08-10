/**
* @file SpeedPack.h
* @author Tom Williams
* @date Created May 4, 2006
*
* Speed powerups within the world
*/
#ifndef _SPEEDPACK_H_
#define _SPEEDPACK_H_

#include "actor.h"
#include "player.h"

#define BOOST_AMOUNT 20.0f

/**
* the class which describes speed powerups
*
* @author Tom Williams
* @date Created May 4, 2006
* @date Modified May 4, 2006
*/
class CSpeedPack : public CActor
{
	//! amount to boost player speed by
	float m_fBoostAmount;

public:
	/**
	* constructor
	*
	* @date Created May 4, 2006
	*/
	CSpeedPack(EObjectType eType = OBJ_ITEM_SLICKSHOES);

	/**
	* destructor
	*
	* @date Created May 4, 2006
	*/
	~CSpeedPack(){}

	/**
	* set the speed boost amount
	*
	* @date Created May 4, 2006
	* @param[in]	unBoost	the amount to set to
	*/
	void setBoost(float fBoost) {m_fBoostAmount = fBoost;}

	/**
	* get the speed boost amount
	*
	* @date Created May 4, 2006
	* @return	the amount of boost
	*/
	float getBoost() {return m_fBoostAmount;}

	/**
	* function called on collision
	*
	* @date Created May 4, 2006
	*/
	virtual void collided(CActor *poActor);

	/**
	* creates a new speed pack for object manager
	*
	* @date Created May 4, 2006
	* @return	created pointer
	*/
	static CObject* create()
	{
		return new CSpeedPack;
	}
};




#endif