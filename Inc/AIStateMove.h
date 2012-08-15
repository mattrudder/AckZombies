/**
* @file AIStateMove.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 3, 2006
*
* This file contains the definition of the CAIStateMove class
*/

#ifndef _AISTATEMOVE_H_
#define _AISTATEMOVE_H_

// Local Includes
#include "AIState.h"
#include "Singleton.h"
#include "Math3D.h"

/**
* Move based on velocity for two seconds or until it gets close to a collision
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 21, 2006
* @date Modified April 21, 2006
*/
class CAIStateMove : public CAIState, public CSingleton<CAIStateMove>
{
	friend class CSingleton<CAIStateMove>;

	//! Singleton stuff.
	//!@{
	CAIStateMove(void);
	CAIStateMove(const CAIStateMove&) {}
	CAIStateMove& operator=(const CAIStateMove&){ return *this; }
	virtual ~CAIStateMove(void);
	//!@}

	//! a zero vector for comparison purposes
	D3DXVECTOR3 m_vVelocity;

public:

	/**
	* Overload to implement the desired behavior of the state
	* 
	* @date Created April 21, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void update(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is entered
	* 
	* @date Created April 21, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void enter(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* must be called when the state is exited
	* 
	* @date Created April 21, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void exit(CAIEntity* poAIEntity, CCharacter* poCharacter);
};

#endif /*_AISTATEMOVE_H_*/