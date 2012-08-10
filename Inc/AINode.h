/**
* @file AINode.h
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the definition of the CAINode class.
*/

#ifndef _AINODE_H_
#define _AINODE_H_

// Local Includes
#include "Actor.h"

// System Includes
#include <list>

/**
* Defines a section of moveable space in the game world
*
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
* @date Modified April 2, 2006
*/
class CAINode : public CActor
{
public:
	//! The connects to other nodes
	std::list<CAINode*> m_loLinks;

	//! The parent of this node
	CAINode* m_poParent;

	//! The amount of passable space around the node
	float m_fRadius;

	//! The distance to the goal from this node, NOTE: not guaranteed to be accurate from one frame to the next
	float m_fDistanceToGoal;

	/**
	* Constructor
	* 
	* @date Created March 28, 2006
	*/
	CAINode(EObjectType eType = OBJ_AI_NODE);

	/**
	* Destructor
	* 
	* @date Created March 28, 2006
	*/
	~CAINode(void);

	/**
	* sets the radius of freedom for the node
	*
	* @date Created Mar 28, 2006
	*/
	void setRadius(float fRadius) { m_fRadius = fRadius; }

	/**
	* create a new AI node
	*
	* @date Created April 23, 2006
	*/
	static CObject* create()
	{
		CAINode* poNode = new CAINode;
		poNode->m_pMesh = NULL;//(CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x");
		poNode->deactivate();
		return poNode;
	}

	virtual void update(void){}
};

#endif /*_AINODE_H_*/