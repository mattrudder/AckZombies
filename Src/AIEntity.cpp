/**
* @file AIEntity.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created March 27, 2006
*
* This file contains the implementation of the CAI class.
*/

// Local Includes
#include "AIEntity.h"
#include "Timer.h"

/**
* CAIEntity::CAIEntity
* @date Modified March 29, 2006
*/
CAIEntity::CAIEntity(EObjectType eType) : m_poState(0), m_dCurrentStateTime(0.0), m_poGroup(0)
{
}

/**
* CAIEntity::~CAIEntity
* @date Modified March 29, 2006
*/
CAIEntity::~CAIEntity(void)
{
}

/**
* CAIEntity::update
* @date Modified March 30, 2006
*/
void CAIEntity::update(CCharacter* poCharactor)
{
	m_dCurrentStateTime += CTimer::getInstancePtr()->getFrameTime();

	if (m_poState)
		m_poState->update(this, poCharactor);
}