/**
* @file AIStateAcidicFollow.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CAIStateAcidicFollow class
*/

#ifndef _AISTATEACIDICFOLLOW_H_
#define _AISTATEACIDICFOLLOW_H_

// Local Includes
#include "AIState.h"
#include "AINode.h"
#include "Singleton.h"

/**
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified April 13, 2006
*/
class CAIStateAcidicFollow : public CAIState, public CSingleton<CAIStateAcidicFollow>
{
	friend class CSingleton<CAIStateAcidicFollow>;

	//! a zero vector for comparison purposes
	D3DXVECTOR3 m_vVelocity;

	//! node at current position for direct link test
	CAINode* m_poCurrentPos;

	//! Singleton stuff.
	//!@{
	CAIStateAcidicFollow(void);
	CAIStateAcidicFollow(const CAIStateAcidicFollow&) {}
	CAIStateAcidicFollow& operator=(const CAIStateAcidicFollow&){ return *this; }
	virtual ~CAIStateAcidicFollow(void);
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

#endif /*_AISTATEACIDICFOLLOW_H_*/