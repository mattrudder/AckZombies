/**
* @file AIStateQBFollow.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CAIStateQBFollow class
*/

#ifndef _AISTATEQBFOLLOW_H_
#define _AISTATEQBFOLLOW_H_

// Local Includes
#include "AIState.h"
#include "AINode.h"
#include "Singleton.h"
#include "Math3D.h"

/**
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified April 13, 2006
*/
class CAIStateQBFollow : public CAIState, public CSingleton<CAIStateQBFollow>
{
	friend class CSingleton<CAIStateQBFollow>;

	//! a zero vector for comparison purposes
	D3DXVECTOR3 m_vVelocity;

	//! node at current position for direct link test
	CAINode* m_poCurrentPos;

	//! Singleton stuff.
	//!@{
	CAIStateQBFollow(void);
	CAIStateQBFollow(const CAIStateQBFollow&) {}
	operator=(const CAIStateQBFollow&){}
	virtual ~CAIStateQBFollow(void);
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

#endif /*_AISTATEQBFOLLOW_H_*/