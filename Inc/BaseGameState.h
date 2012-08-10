/**
* @file BaseGameState.h
* @author Matt Rudder
* @date Created Mar 08, 2006
*
* Contains the base functionality of a single state within the game.
*/

#ifndef _BASEGAMESTATE_H_
#define _BASEGAMESTATE_H_

// Engine includes
#include "RenderDevice.h"

/**
* Defines the interface for all game states within the game.
*
* @author Matt Rudder
* @date Created Mar 08, 2006
* @date Modified Mar 08, 2006
*/
class CBaseGameState
{
protected:
	void changeState(CBaseGameState* pState);
public:

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created Mar 08, 2006
	*/
	virtual void onActivate(void) = 0;

	/**
	* Called by the kernel when the active state is switched away from this 
	* state. This allows the state to perform any cleanup needed from its 
	* onActivate call.
	*
	* @date Created Mar 08, 2006
	*/
	virtual void onDeactivate(void) = 0;

	/**
	* Called each frame within the kernel's tick function, while this is the 
	* active state.
	*
	* @date Created Mar 08, 2006
	*/
	virtual void tick(void) = 0;

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created Apr 24, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice) = 0;

	/**
	* Called each frame within the render system, just after world/actor 
	* geometry is rendered but before the scene is presented while this 
	* is the active state.
	*
	* @date Created Apr 24, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void postRender(CRenderDevice& oRenderDevice) = 0;
};

#endif //_BASEGAMESTATE_H_