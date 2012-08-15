/**
* @file AIStateSpawn.h
* @author Jonathan "Awesome" Zimmer
* @date Created May 30, 2006
*
* This file contains the definition of the CAIStateSpawn class
*/

#ifndef _AISTATESPAWN_H_
#define _AISTATESPAWN_H_

// Local Includes
#include "AIState.h"
#include "Singleton.h"
#include "Math3D.h"

/**
* Set position below the world and move up until completely out of the ground
*
* @author Jonathan "Awesome" Zimmer
* @date Created May 30, 2006
* @date Modified May 30, 2006
*/
class CAIStateSpawn : public CAIState, public CSingleton<CAIStateSpawn>
{
	friend class CSingleton<CAIStateSpawn>;

	//! Singleton stuff.
	//!@{
	CAIStateSpawn(void);
	CAIStateSpawn(const CAIStateSpawn&) {}
	CAIStateSpawn& operator=(const CAIStateSpawn&){ return *this; }
	virtual ~CAIStateSpawn(void);
	//!@}

public:

	/**
	* Overload to implement the desired behavior of the state
	* 
	* @date Created May 30, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void update(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is entered
	* 
	* @date Created May 30, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void enter(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is exited
	* 
	* @date Created May 30, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void exit(CAIEntity* poAIEntity, CCharacter* poCharacter);
};

#endif /*_AISTATESPAWN_H_*/