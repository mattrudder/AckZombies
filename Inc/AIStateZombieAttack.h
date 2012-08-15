/**
* @file AIStateZombieAttack.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CAIStateZombieAttack class
*/

#ifndef _AISTATEZOMBIEATTACK_H_
#define _AISTATEZOMBIEATTACK_H_

// Local Includes
#include "AIState.h"
#include "Singleton.h"

/**
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified April 13, 2006
*/
class CAIStateZombieAttack : public CAIState, public CSingleton<CAIStateZombieAttack>
{
	friend class CSingleton<CAIStateZombieAttack>;

	//! Singleton stuff.
	//!@{
	CAIStateZombieAttack(void);
	CAIStateZombieAttack(const CAIStateZombieAttack&) {}
	CAIStateZombieAttack& operator=(const CAIStateZombieAttack&) { return *this; }
	virtual ~CAIStateZombieAttack(void);
	//!@}

public:

	/**
	* Overload to implement the desired behavior of the state
	*
	* @date Created April 7, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)
	*/
	void update(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is entered
	* 
	* @date Created April 13, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void enter(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is exited
	* 
	* @date Created April 13, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void exit(CAIEntity* poAIEntity, CCharacter* poCharacter);
};

#endif /*_AISTATEZOMBIEATTACK_H_*/