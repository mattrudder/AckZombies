/**
* @file SpeedPack.cpp
* @author Tom Williams
* @date Modified May 4, 2006
*
* Speed powerups within the world
*/
#include "speedpack.h"
#include "objectmanager.h"

/**
* constructor
*
* @date Created May 4, 2006
*/
CSpeedPack::CSpeedPack(EObjectType eType) : CActor(eType)
{
	m_fBoostAmount = BOOST_AMOUNT;
}

/**
* CSpeedPack::collided()
* @date Modified May 4, 2006
*/
void CSpeedPack::collided(CActor *poActor)
{
	if(!poActor)
		return;
		
	if(!isDeleted() && (poActor->getType() & OBJ_PLAYER) == OBJ_PLAYER)
	{
		// apply effect to the player
		((CPlayer*)poActor)->addToMaxSpeed(m_fBoostAmount);
		// remove from the object manager
		CObjectManager::getInstance().removeObject((CObject*)this);
	}		
}