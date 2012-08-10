/**
* @file AINode.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created March 28, 2006
*
* This file contains the implementation of the CAINode class.
*/

// Local Includes
#include "AINode.h"

/**
* CAINode::CAINode
* @date Modified April 2, 2006
*/
CAINode::CAINode(EObjectType eType) : CActor(eType), m_poParent(0), m_fRadius(0.0f), m_fDistanceToGoal(0.0f)
{
}

CAINode::~CAINode(void) {}