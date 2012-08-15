/**
* @file AIStatePathFollow.h
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the definition of the CAIStatePathFollow class
*/

#ifndef _AISTATEPATHFOLLOW_H_
#define _AISTATEPATHFOLLOW_H_

// Local Includes
#include "AIState.h"
#include "Singleton.h"
#include "Math3D.h"

/**
* Path Following implemented with a range of freedom
*
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
* @date Modified April 18, 2006
*/
class CAIStatePathFollow : public CAIState, public CSingleton<CAIStatePathFollow>
{
	friend class CSingleton<CAIStatePathFollow>;

	//! a zero vector for comparison purposes
	D3DXVECTOR3 m_vVelocity;

	//! Singleton stuff.
	//!@{
	CAIStatePathFollow(void);
	CAIStatePathFollow(const CAIStatePathFollow&) {}
	CAIStatePathFollow& operator=(const CAIStatePathFollow&){ return *this; }
	virtual ~CAIStatePathFollow(void);
	//!@}

public:

	/**
	* Overloaded to implement the Path Follow behavior
	* 
	* @date Created March 28, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void update(CAIEntity* poAIEntity, CCharacter* poCharacter);

	/**
	* Basic Path following behavior
	* 
	* @date Created April 18, 2006
	* @param[in]	poAIEntity		pointer to the AI the state is working on
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)
	* @param[in]	fSpeed			how fast to move the Character
	*/
	void followPath(CAIEntity* poAIEntity, CCharacter* poCharacter, float fSpeed);

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

#endif