/**
* @file AIStateGaseousFollow.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CAIStateGaseousFollow class
*/

#ifndef _AISTATEGASEOUSFOLLOW_H_
#define _AISTATEGASEOUSFOLLOW_H_

// Local Includes
#include "AIState.h"
#include "AINode.h"
#include "Singleton.h"
#include "Math3D.h"

/**
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified May 4, 2006
*/
class CAIStateGaseousFollow : public CAIState, public CSingleton<CAIStateGaseousFollow>
{
	friend class CSingleton<CAIStateGaseousFollow>;

	//! a zero vector for comparison purposes
	D3DXVECTOR3 m_vVelocity;

	//! Singleton stuff.
	//!@{
	CAIStateGaseousFollow(void);
	CAIStateGaseousFollow(const CAIStateGaseousFollow&) {}
	operator=(const CAIStateGaseousFollow&){}
	virtual ~CAIStateGaseousFollow(void);
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

#endif /*_AISTATEGASEOUSFOLLOW_H_*/