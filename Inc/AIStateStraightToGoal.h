/**
* @file AIStateStraightToGoal.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
*
* This file contains the definition of the CAIStateStraightToGoal class.
*/

#ifndef _AISTATESTRAIGHTTOGOAL_H_
#define _AISTATESTRAIGHTTOGOAL_H_

// Local Includes
#include "AIState.h"
#include "Singleton.h"

/**
* Path Planning implemented with the Best First Algorithm
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
* @date Modified April 27, 2006
*/
class CAIStateStraightToGoal : public CAIState, public CSingleton<CAIStateStraightToGoal>
{
	friend class CSingleton<CAIStateStraightToGoal>;

	//! Singleton stuff.
	//!@{
	CAIStateStraightToGoal(void);
	CAIStateStraightToGoal(const CAIStateStraightToGoal&) {}
	operator=(const CAIStateStraightToGoal&){}
	virtual ~CAIStateStraightToGoal(void);
	//!@}

public:

	/**
	* Overloaded to implement the Path Plan behavior
	* 
	* @date Created April 27, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void update(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is entered
	* 
	* @date Created April 27, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void enter(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is exited
	* 
	* @date Created April 27, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void exit(CAIEntity* poAIEntity, CCharacter* poCharacter);
};

#endif /*_AISTATESTRAIGHTTOGOAL_H_*/