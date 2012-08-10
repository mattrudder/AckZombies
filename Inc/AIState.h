/**
* @file AIState.h
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
*
* This file contains the definition of the CAIState class.
*/

#ifndef _AISTATE_H_
#define _AISTATE_H_

// Local Includes
#include "Timer.h"

// Forward Declarations
class CAIEntity;
class CCharacter;

/**
* Abstract base class for interface to define behaviors of AI entities
*
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
* @date Modified May 30, 2006
*/
class CAIState
{
public:
	//! Defines the individual AI state types
	enum EAIStateTypes
	{
		AIS_PATHPLAN,		//! Path Planning using best first
		AIS_PATHFOLLOW,		//! Path Following using stored nodes
		AIS_FOLLOWLEADER,	//! Following a leader
		AIS_ZOMBIEATTACK,	//! Zombie Citizen attack state
		AIS_GASEOUSFOLLOW,	//! Gaseous Clay path following
		AIS_GASEOUSATTACK,	//! Gaseous Clay attack state
		AIS_ACIDICFOLLOW,	//! Acidic Ice Cream Man path following
		AIS_ACIDICATTACK,	//! Acidic Ice Cream Man attack state
		AIS_QBFOLLOW,		//! Quarterback Zombie path following
		AIS_QBRANGEATTACK,	//! Quarterback Zombie throws a flamming football
		AIS_QBMELEEATTACK,	//! Quarterback Zombie charges
		AIS_STRAIGHTTOGOAL,	//! Sends the enemy straight to the closest goal
		AIS_SPAWN,			//! Moves the enemy for below ground to above ground
		AIS_MAX,
	};

private:

	//! The type of state this state is
	EAIStateTypes m_eStateType;

protected:
	/**
	* Set the state type
	* 
	* @date Created March 28, 2006
	* @param[in]	eType	the type to set this state to
	*/
	inline void setStateType(EAIStateTypes eType) { m_eStateType = eType; }

public:

	virtual ~CAIState() {}

	/**
	* Overload to implement the desired behavior of the state
	* 
	* @date Created March 28, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	virtual void update(CAIEntity* poAIEntity, CCharacter* poCharacter) = 0;

	/**
	* must be called when the state is entered
	* 
	* @date Created April 13, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	virtual void enter(CAIEntity* poAIEntity, CCharacter* poCharacter) = 0;

	/**
	* must be called when the state is exited
	* 
	* @date Created April 13, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	virtual void exit(CAIEntity* poAIEntity, CCharacter* poCharacter) = 0;

	/**
	* Get the state type
	* 
	* @date Created March 28, 2006
	* @return The type of this state
	*/
	inline EAIStateTypes getState(void) { return m_eStateType; }
};

#endif /*_AISTATE_H_*/