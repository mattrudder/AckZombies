/**
* @file HealthPack.h
* @author Tom Williams
* @date Created May 4, 2006
*
* Health powerups within the world
*/
#ifndef _HEALTHPACK_H_
#define _HEALTHPACK_H_

#include "actor.h"
#include "player.h"

//! the types of health pack within the game
enum EHealthPackType
{
	EHT_SMALL_PACK = 25,
	EHT_LARGE_PACK = 75,
};

/**
* the class which describes health powerups
*
* @author Tom Williams
* @date Created May 4, 2006
* @date Modified May 4, 2006
*/
class CHealthPack : public CActor
{
	//! amount to increase player health by
	unsigned int m_unHealthBoost;

public:
	/**
	* constructor
	*
	* @date Created May 4, 2006
	*/
	CHealthPack(EObjectType eType = OBJ_ITEM_HEALTH);

	/**
	* destructor
	*
	* @date Created May 4, 2006
	*/
	~CHealthPack(){}

	/**
	* set the health increase amount
	*
	* @date Created May 4, 2006
	*/
	void setHealthPackType(EHealthPackType ePack) {m_unHealthBoost = ePack;}

	/**
	* called when collided with
	*
	* @date Created May 4, 2006
	*/
	void collided(CActor *poActor);

	/**
	* creates a new health pack
	*
	* @date Created May 4, 2006
	*/
	static CObject* create()
	{
		return new CHealthPack;
	}
};

#endif