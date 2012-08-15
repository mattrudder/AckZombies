/**
* @file AIStateGaseousAttack.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CAIStateGaseousAttack class
*/

#ifndef _AISTATEGASEOUSATTACK_H_
#define _AISTATEGASEOUSATTACK_H_

// Local Includes
#include "AIState.h"
#include "Singleton.h"

/**
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified April 13, 2006
*/
class CAIStateGaseousAttack : public CAIState, public CSingleton<CAIStateGaseousAttack>
{
	friend class CSingleton<CAIStateGaseousAttack>;

	//! Singleton stuff.
	//!@{
	CAIStateGaseousAttack(void);
	CAIStateGaseousAttack(const CAIStateGaseousAttack&) {}
	CAIStateGaseousAttack& operator=(const CAIStateGaseousAttack&){ return *this; }
	virtual ~CAIStateGaseousAttack(void);
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

#endif /*_AISTATEGASEOUSATTACK_H_*/