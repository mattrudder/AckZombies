/**
* @file Zombie.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CZombie class
*/

#ifndef _ZOMBIE_H_
#define _ZOMBIE_H_

#include "Enemy.h"
#include "TickTimer.h"

/**
* The Zombie enemy in the game
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified June 1, 2006
*/
class CZombie : public CEnemy
{
	//! The timer to check for self deletion if the player is too far away
	CTickTimer m_tSelfDelete;

	//! How long before we play a sound
	int m_nPlaySoundTime;
	//! Our sound timer
	float m_fSoundTimer;

public:

	CZombie(void);
	~CZombie(void);

	/**
	* Update the enemy and its AI
	*
	* @date Created April 12, 2006
	*/
	void update(void);

	/**
	* create a new Zombie enemy
	*
	* @date Created April 23, 2006
	*/
	static CObject* create();

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* calculate how long until we play a sound
	*
	* @date Created June 8, 2006
	*/
	void calcPlaySoundTime(void);
};

#endif /*_ZOMBIE_H_*/