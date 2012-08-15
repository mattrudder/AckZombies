/**
* @file AIStateFollowLeader.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 3, 2006
*
* This file contains the definition of the CAIStateFollowLeader class
*/

#ifndef _AISTATEFOLLOWLEADER_H_
#define _AISTATEFOLLOWLEADER_H_

// Local Includes
#include "AIState.h"
#include "AINode.h"
#include "Singleton.h"
#include "Math3D.h"

/**
* Leader Following Behavior
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 3, 2006
* @date Modified April 21, 2006
*/
class CAIStateFollowLeader : public CAIState, public CSingleton<CAIStateFollowLeader>
{
	friend class CSingleton<CAIStateFollowLeader>;

	//! Singleton stuff.
	//!@{
	CAIStateFollowLeader(void);
	CAIStateFollowLeader(const CAIStateFollowLeader&) {}
	CAIStateFollowLeader& operator=(const CAIStateFollowLeader&){ return *this; }
	virtual ~CAIStateFollowLeader(void);
	//!@}

	//! a zero vector for comparison purposes
	D3DXVECTOR3 m_vVelocity;

	//! the node to test direct connection with
	CAINode* m_poNode;

public:

	/**
	* Overload to implement the desired behavior of the state
	* 
	* @date Created March 28, 2006
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

#endif /*_AISTATEFOLLOWLEADER_H_*/