/**
* @file LoadState.h
* @author Jonathan "Awesome" Zimmer
* @date Created May 8, 2006
*
* the loading state
*/ 

#ifndef _LOADSTATE_H_
#define _LOADSTATE_H_

// Local Includes
#include "GameState.h"
#include "BaseResource.h"
#include "Menu.h"

// System Includes
#include <fstream>
#include <list>
using std::ifstream;

/**
* Loading
*
* @author Jonathan "Awesome" Zimmer
* @date Created May 8, 2006
* @date Modified May 8, 2006
*/
class CLoadState : public CGameState<CLoadState>
{
	//! The resources that have been loaded by the load state
	std::list<CBaseResource*> m_loResources;

	//! list of load screens
	CString m_strLoadScreens[6];

public:

	//! Whether or not we have rendered yet
	bool m_bRendered;
	//! Menu to display loading progress
	CMenu *m_poLoadingScreen;
	//! whether or not we've loaded already
	bool m_bLoaded;
	//! whehter or not to continue with the game
	bool m_bProceed;
	//! Whether or not to load a second player
	bool m_bTwoPlayers;
	//! Display Font
	CBitmapFont* m_fntDina;

	CLoadState(void) : m_bRendered(false), m_poLoadingScreen(0), m_bLoaded(false), m_fntDina(0)
	{
		m_strLoadScreens[0] = "DeagleLoad.tga";
		m_strLoadScreens[1] = "TripleShotgunLoad.tga";
		m_strLoadScreens[2] = "PistolLoad.tga";
		m_strLoadScreens[3] = "RocketLauncherLoad.tga";
		m_strLoadScreens[4] = "ShotgunLoad.tga";
		m_strLoadScreens[5] = "HolyWaterCanonLoad.tga";
	} 

	~CLoadState(void) {}

	// Mutator
	void setTwoPlayers(bool bTwoPlayers) { m_bTwoPlayers = bTwoPlayers; }

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created May 8, 2006
	*/
	virtual void onActivate(void);

	/**
	* Called by the kernel when the active state is switched away from this 
	* state. This allows the state to perform any cleanup needed from its 
	* onActivate call.
	*
	* @date Created May 8, 2006
	*/
	virtual void onDeactivate(void);

	/**
	* Called each frame within the kernel's tick function, while this is the 
	* active state.
	*
	* @date Created May 8, 2006
	*/
	virtual void tick(void);

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created May 8, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice);

	/**
	* Called each frame within the render system, just after world/actor 
	* geometry is rendered but before the scene is presented while this 
	* is the active state.
	*
	* @date Created May 8, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void postRender(CRenderDevice& oRenderDevice);

	/**
	* Adds a resource only if it isn't in the collection already
	*
	* @param[in]	poResource	pointer to try and put in
	* @date Created May 8, 2006
	*/
	void addResource(CBaseResource* poResource);

	/**
	* Releases all of its CBaseResource Pointers
	*
	* @date Created May 8, 2006
	*/
	void releaseResources(void);
};

#endif /*_LOADSTATE_H_*/