/**
* @file MenuState.h
* @author Jordan Leckner
* @date Created April 24, 2006
*
* This file includes the Menu game state
*/
#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "Gamestate.h"
#include "MenuManager.h"
#include "SoundManager.h"
#include "LoadState.h"
#include "ObjectManager.h"
#include "octree.h"
#include "aimanager.h"

class CMenuState : public CGameState<CMenuState>
{
protected:

	//! Whether or not to change the state to the loading state
	bool m_bLoadingState;
	//! Whether or not to change the state to the game state
	bool m_bGameState;

public:

	CMenuState() : m_bLoadingState(false), m_bGameState(false)
	{
	}

	void setLoadingState(bool bLoad) { m_bLoadingState = bLoad; }

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created April 24, 2006
	*/
	virtual void onActivate(void)
	{
		static bool bFirstTime = true;
		if(bFirstTime)
		{
			bFirstTime = false;
			CSoundManager::getInstance().playSound(SND_MUSIC_MAINMENU, -1);
			CSoundManager::getInstance().pauseSound(SND_MUSIC_MAINMENU, true);
		}
		else
		{
			// Load up a fresh level
			// Delete all objects from the object manager
			CObjectManager::getInstance().deallocAllObjectsPlusDeadList();
			//COctree::getInstance().deallocateTree();
			CAIManager::getInstance().deleteInstance();
			//CResourceManager::getInstance().deAllocAllResources();
		}

		CSoundManager::getInstance().pauseSound(SND_MUSIC_MAINMENU, false);
	}

	/**
	* Called by the kernel when the active state is switched away from this 
	* state. This allows the state to perform any cleanup needed from its 
	* onActivate call.
	*
	* @date Created Mar 08, 2006
	*/
	virtual void onDeactivate(void)
	{
		CSoundManager::getInstance().pauseSound(SND_MUSIC_MAINMENU, true);
	}

	/**
	* Called each frame within the kernel's tick function, while this is the 
	* active state.
	*
	* @date Created Mar 08, 2006
	*/
	virtual void tick(void);

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created Apr 24, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice)
	{
	}

	/**
	* Called each frame within the render system, just after world/actor 
	* geometry is rendered but before the scene is presented while this 
	* is the active state.
	*
	* @date Created Apr 24, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void postRender(CRenderDevice& oRenderDevice)
	{
		CMenuManager::getInstance().updateCurrentMenu();
	}
};

#endif