/**
* @file Object.cpp
* @author Matt Rudder
* @date Modified Apr 26, 2006
*
* Base object within the game engine. All game objects should derive from this
* type.
*/

// Engine includes
#include "AcespaceEngine.h"

// Local includes
#include "Object.h"

/**
* CObject::CObject
* @date Modified Mar 17, 2006
*/
CObject::CObject(EObjectType eType) : m_bDelete(false), m_bActive(true), m_eType(eType) { }

/**
* CObject::~CObject
* @date Modified Mar 17, 2006
*/
CObject::~CObject(void) { }

/**
* CObject::getType
* @date Modified Mar 17, 2006
*/
EObjectType CObject::getType(void)
{
	return m_eType;
}

/**
* CObject::update
* @date Modified Mar 17, 2006
*/
void CObject::update(void)
{
}

/**
* CObject::activate
* @date Modified Mar 07, 2006
*/
void CObject::activate(void)
{
	m_bActive = true;
}

/**
* CObject::deactivate
* @date Modified Mar 07, 2006
*/
void CObject::deactivate(void)
{
	m_bActive = false;
}

/**
* CObject::isActive
* @date Modified Mar 07, 2006
*/
bool CObject::isActive(void)
{
	return m_bActive;
}

/**
* CObject::init
* @date Modified May 11, 2006
*/
void CObject::init(void)
{
	m_bDelete = false;
}

/**
* CObject::destroy
* @date Modified May 26, 2006
*/
void CObject::destroy(void) { }