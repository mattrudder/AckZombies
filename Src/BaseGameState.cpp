/**
* @file BaseGameState.cpp
* @author Matt Rudder
* @date Modified Mar 08, 2006
*
* Contains the base functionality of a single state within the game.
*/

// Local includes
#include "BaseGameState.h"
#include "GameKernel.h"

void CBaseGameState::changeState(CBaseGameState* pState)
{
	CGameKernel::getInstance().changeState(pState);
}