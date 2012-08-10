/**
* @file AIEntity.h
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the definition of the CAIEntity class.
*/

#ifndef _AIENTITY_H_
#define _AIENTITY_H_

// Local Includes
#include "AIState.h"
#include "AINode.h"

// System Includes
#include <list>

class CAIGroup;

/**
* The main capsule of an actor’s AI behavior.
*
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
* @date Modified May 27, 2006
*/
class CAIEntity
{
	//! The collection of nodes the AI entity has in the path to the goal node
	std::list<CAINode*> m_loPath;

	//! The AI entity’s current state that determines behavior
	CAIState* m_poState;

	//! The Group that this AI entity is in - null if not in a group
	CAIGroup* m_poGroup;

	//! The amount of time this AI entity has been it it's current state
	double m_dCurrentStateTime;

	//! Cannot use default constructor
	CAIEntity(void);

	// Pathing Friends
	friend class CAIStatePathPlan;
	friend class CAIStatePathFollow;
	friend class CAIStateFollowLeader;
	friend class CAIStateGaseousFollow;
	friend class CAIStateAcidicFollow;
	friend class CAIStateQBFollow;
	friend class CAIStateFollowLeader;

public:

	/**
	* Constructor sets current state based on passed in type
	* 
	* @date Created March 27, 2006
	* @param[in]	eType	The type of the AI
	*
	*/
	CAIEntity(EObjectType eType);

	~CAIEntity(void);

	/**
	* Calls the current state's update
	* 
	* @date Created March 27, 2006
	* @param[in]	poCharacter		pointer to the character this AI is contained
	*								(this pointer)	
	*/
	void update(CCharacter* poCharacter);

	/**
	* Get the type of the current state
	* 
	* @date Created March 27, 2006
	* @return The type of the current state
	*/
	inline CAIState::EAIStateTypes getStateType(void) { return m_poState ? m_poState->getState() : CAIState::AIS_MAX; }

	/**
	* Get the time spent in the current state
	* 
	* @date Created March 30, 2006
	* @return The time spent in the current state
	*/
	inline double getCurrentStateTime(void) { return m_dCurrentStateTime; }

	/**
	* set the time spent in the current state
	* 
	* @date Created May 27, 2006
	* @param[in]	dTime	the time to set it to
	*/
	inline void setCurrentStateTime(double dTime) { m_dCurrentStateTime = dTime; }

	/**
	* set the type of the current state and zero the time
	* 
	* @date Created March 28, 2006
	* @param[in]	poState	the state to put this entity in
	*/
	inline void setState(CAIState* poState) { m_poState = poState; m_dCurrentStateTime = (double)((rand()%10)/10.0f); }

	/**
	* get the type of the current state
	* 
	* @date Created May 4, 2006
	* @return	the state this entity is in
	*/
	inline CAIState* getState(void) { return m_poState; }

	/**
	* set the group of this Entity
	* 
	* @date Created April 27, 2006
	* @param[in]	poGroup		the group for this entity
	*/
	inline void setGroup(CAIGroup* poGroup) { m_poGroup = poGroup; }

	/**
	* return the group of this Entity
	* 
	* @date Created April 27, 2006
	* @return		the group for this entity
	*/
	inline CAIGroup* getGroup(void) { return m_poGroup; }
};

#endif /*_AIENTITY_H_*/