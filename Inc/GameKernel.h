/**
* @file GameKernel.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Core of the game engine. Updates base engine systems and the game as a whole.
*/ 

#ifndef _GAMEKERNEL_H_
#define _GAMEKERNEL_H_

// Engine includes
#include "AcespaceEngine.h"
#include "BaseGameState.h"
#include "Singleton.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "ObjectTypes.h"
#include "ConfigFile.h"
#include "TickTimer.h"

// System includes
#include <windows.h>

/**
* Manages the main game loop, game states, and updates to various game systems.
*
* @author Matt Rudder
* @date Created Mar 06, 2006
* @date Modified Mar 06, 2006
*/
class CGameKernel : public CSingleton<CGameKernel>
{
	friend class CBaseGameState;
	friend class CRenderSystem;
	friend class CPlacementState;
protected:
	//! The active state within the game.
	CBaseGameState* m_poActiveState;
	//! The main game window
	HWND m_hWnd;
	//! Object creation structures
	static CObjectManager::SObjectCreation m_pObjects[];
	//! The size of the m_pObjects array
	unsigned int m_unSizeOfObjects;
	//! Resource creation structres
	static CResourceManager::SResourceCreation m_pResources[];
	//! Global configuration
	CConfigFile m_oConfig;
	
	/**
	* Tick timer's for individual subsystems.
	* @{
	*/
	CTickTimer	m_tRenderSys;
	CTickTimer	m_tInputSys;
	CTickTimer	m_tObjectSys;
	CTickTimer	m_tAISys;
	//!@}

public:

	/**
	* Accessors/Modifiers
	* @{
	*/
	CBaseGameState* getCurrentState(void)  { return m_poActiveState; }
	//!@}

	/**
	* Constructor
	*
	* @date Created Mar 08, 2006
	*/
	CGameKernel(void);

	/**
	* Destructor
	*
	* @date Created Mar 08, 2006
	*/
	virtual ~CGameKernel(void);

	/**
	* Accessors/Modifiers
	* @{
	*/
	CTickTimer& getRenderTimer(void) { return m_tRenderSys; }
	unsigned int getSizeOfObjects(void) { return m_unSizeOfObjects; }
	//!@}

	/**
	* Changes the game's current state, and therefore its behavior.
	*
	* @date Created Mar 08, 2006
	* @param[in]	pState		Pointer to the state to switch to.
	*/
	void changeState(CBaseGameState* pState);

	/**
	* Updates the game subsystems, and indirectly the game.
	*
	* @date Created Mar 08, 2006
	* @return true if the game should continue to tick, false if the game
	*		  should exit.
	*/
	bool tick(void);

	/**
	* Initialize the game engine and all subsystem.
	*
	* @date Created Mar 13, 2006
	* @return true if the game was initialized successfully, false if an error
	*		  occured.
	*/
	bool initialize(HINSTANCE hInst);

	/**
	* Returns the global instance of the configuration file.
	*
	* @date Created Mar 13, 2006
	* @return Instance to the config file
	*/
	CConfigFile& getConfig(void);
};

#endif //_GAMEKERNEL_H_