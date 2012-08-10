/**
* @file HealthPack.cpp
* @author Tom Williams
* @date Modified May 4, 2006
*
* Health powerups within the world
*/
#include "healthpack.h"
#include "objectmanager.h"

/**
* constructor
*
* @date Created May 4, 2006
*/
CHealthPack::CHealthPack(EObjectType eType) : CActor(eType)
{
	m_unHealthBoost = EHT_SMALL_PACK;
}

/**
* CSpeedPack::collided()
* @date Modified May 4, 2006
*/
void CHealthPack::collided(CActor *poActor)
{
	if(!poActor)
		return;

	if(!isDeleted() && (poActor->getType() & OBJ_PLAYER) == OBJ_PLAYER)
	{
		// apply effect to the player
		((CPlayer*)poActor)->addToHealth(m_unHealthBoost);
		// remove from the object manager
		CObjectManager::getInstance().removeObject((CObject*)this);
	}		
}