/**
* @file GameState.h
* @author Matt Rudder
* @date Created Mar 08, 2006
*
* Contains the base functionality of a single state within the game, templated
* to provide singleton access.
*/
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "BaseGameState.h"
#include "Singleton.h"

/**
* Defines the interface for all game states within the game.
*
* @author Matt Rudder
* @date Created Mar 08, 2006
* @date Modified Mar 08, 2006
*/
template<typename state_t>
class CGameState : public CBaseGameState, public CSingleton<state_t> {};

#endif