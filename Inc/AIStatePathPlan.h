/**
* @file AIStatePathPlan.h
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
*
* This file contains the definition of the CAIStatePathPlan class.
*/

#ifndef _AISTATEPATHPLAN_H_
#define _AISTATEPATHPLAN_H_

// Local Includes
#include "AIState.h"
#include "AINode.h"
#include "Math3D.h"
#include "Singleton.h"

#define NODE_LIST_ITER std::list<CAINode*>::iterator

/**
* Path Planning implemented with the Best First Algorithm
*
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
* @date Modified April 21, 2006
*/
class CAIStatePathPlan : public CAIState, public CSingleton<CAIStatePathPlan>
{
	//! The list of open nodes
	std::list<CAINode*> m_loOpenNodes;

	//! The list of closed nodes
	std::list<CAINode*> m_loClosedNodes;		// OPTIMIZE hash table

	//! The start node set by each path planning entity
	CAINode* m_poStartNode;

	//! The goal node set by each path planning entity
	CAINode* m_poGoalNode;

	friend class CSingleton<CAIStatePathPlan>;

	//! Singleton stuff.
	//!@{
	CAIStatePathPlan(void);
	CAIStatePathPlan(const CAIStatePathPlan&) {}
	CAIStatePathPlan& operator=(const CAIStatePathPlan&){ return *this; }
	virtual ~CAIStatePathPlan(void);
	//!@}

public:

	/**
	* Overloaded to implement the Path Plan behavior
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

	/**
	* Gets access to the start node pointer
	*
	* @date Created March 30, 2006
	* @return		the start node pointer
	*/
	inline CAINode* getStartNode(void) { return m_poStartNode; }

	/**
	* Gets access to the goal node pointer
	*
	* @date Created March 30, 2006
	* @return		the goal node pointer
	*/
	inline CAINode* getGoalNode(void) { return m_poGoalNode; }

private:
	/**
	* Check to See if the value is represented in the list
	* 
	* @date Created March 28, 2006
	* @param[in]	poList		pointer to the list of nodes to look in
	* @param[in]	poNode		pointer to the node to look for
	* @return		iterator to node in the list, the list's end if the node is not in it
	*/
	NODE_LIST_ITER checkList(std::list<CAINode*>* poList, CAINode* poNode);

	/**
	* Insert a node into the open list with shortest distance at begining
	* 
	* @date Created March 28, 2006
	* @param[in]	poNode		pointer to the node to look for
	*/
	void insertOpenList(CAINode* poNode);
};

#endif /*_AISTATEPATHPLAN_H_*/