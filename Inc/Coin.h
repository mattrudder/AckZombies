/**
* @file Coin.h
* @author Tom Williams
* @date Created April 27, 2006
*
* Money objects within the world
*/
#ifndef _COIN_H_
#define _COIN_H_

// local includes/libraries
#include "actor.h"
#include "ticktimer.h"

// define the value of the coins within the game
enum ECoinTypes
{
	GOLD_VALUE	=	150L,
	SILVER_VALUE=	100L,
	COPPER_VALUE=	50L,
};

/**
* the class which describes money within the game
*
* @author Tom Williams
* @date Created April 6, 2006
* @date Modified May 15, 2006
*/
class CCoin : public CActor
{
	//! the value of the coin
	unsigned int m_unValue;
	//! the life span of the coin
	CTickTimer m_oTimeSpan;
	//! the distance for gravity to player
	float m_fGravDist;
	//! the angle for rotation
	float m_fAngle;
	//! the actor this coin is gravitating to
	CActor* m_poPlayer;

public:
	/**
	* constructor for the coin class
	* **The type of coin MUST be specified when created**
	*
	* @date Created April 6, 2006
	* @param[in]	The type of the object
	* @{
	*/
	CCoin(EObjectType eType = OBJ_ITEM_MONEY);

	/**
	* Modifiers for the character members
	*
	* @date Created March 27, 2006
	* @param[in]	The variable to set the member to.
	* @{
	*/
	virtual ~CCoin(void) {}

	/**
	* Modifiers for the coin members
	*
	* @date Created April 6, 2006
	* @param[in]	The variable to set the member to.
	* @{
	*/
	void setValue(ECoinTypes eValue);
	//!@}

	/**
	* Accessors for the character members
	*
	* @date Created March 27, 2006
	* @return	The value of the member.
	* @{
	*/
	unsigned int getValue() {return m_unValue;}
	//!@}

	/**
	* update the coin based on the object it collided with
	*
	* @date Created April 23, 2006
	* @param[in]	poActor		the object it collided with
	*/
	virtual void collided(CActor *pActor);

	/**
	* update the coin
	*
	* @date Created May 3, 2006
	* @param[in]	poActor		the object it collided with
	*/
	virtual void update();

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 15, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* move the coin to the player
	*
	* @date Created May 15, 2006
	* @return whether or not to continue the update function
	*/
	bool gravitate(void);

	/**
	* create a coin
	*
	* @date Created April 23, 2006
	*/
	static CObject* create()
	{
		return new CCoin;
	}
};

#endif