/**
* @file GameKernel.cpp
* @author Matt Rudder
* @date Modified Mar 08, 2006
*
* Core of the game engine. Updates base engine systems and the game as a whole.
*/

// Local includes
#include "GameKernel.h"
#include "TestState.h"
#include "ActorIncludes.h"
// NOTE: if you want to include an actor derived class' header file go to "ActorIncludes.h" and do it

// Engine includes
#include "AcespaceEngine.h"
#include "StaticFunctor.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "DirectInputWrapper.h"
#include "Commands.h"		
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "MenuState.h"
#include "VendingMachine.h"
#include "Options.h"
#include "GameoverState.h"
#include "pausestate.h"
#include "winstate.h"
#include "VendingState.h"
#include <ctime>

// Define object and resource creation.
CObjectManager::SObjectCreation CGameKernel::m_pObjects[] = 
{
	{	OBJ_ACTOR,					new CStaticFunctor<CObject*>(CActor::create)					},
	{	OBJ_ENEMY_ZOMBIECITIZEN,	new CStaticFunctor<CObject*>(CZombie::create)					},
	{	OBJ_ENEMY_GASEOUSCLAY,		new CStaticFunctor<CObject*>(CGaseous::create)					},
	{	OBJ_ENEMY_ICECREAMMAN,		new CStaticFunctor<CObject*>(CAcidic::create)					},
	{	OBJ_ENEMY_QUARTERBACK,		new CStaticFunctor<CObject*>(CQBZombie::create)					},
	{	OBJ_AI_NODE,				new CStaticFunctor<CObject*>(CAINode::create)					},
	{	OBJ_PLAYER,					new CStaticFunctor<CObject*>(CPlayer::create)					},
	{	OBJ_WEAPON,					new CStaticFunctor<CObject*>(CBaseWeapon::create)				},
	{	OBJ_WEAPONPROJ_ROCKET,		new CStaticFunctor<CObject*>(CRocket::create)					},
	{	OBJ_WEAPONPROJ_50AWE,		new CStaticFunctor<CObject*>(C50AWE::create)					},
	{	OBJ_WEAPON_GRENADE,			new CStaticFunctor<CObject*>(CGrenadeWeapon::create)			},
	{	OBJ_SPAWN,					new CStaticFunctor<CObject*>(CActorSpawn::create)				},
	{	OBJ_ITEM_MONEY,				new CStaticFunctor<CObject*>(CCoin::create)						},
	{	OBJ_WEAPONPROJ_ACIDICE,		new CStaticFunctor<CObject*>(CIceCream::create)					},
	{	OBJ_WEAPONPROJ_POTATO,		new CStaticFunctor<CObject*>(CPotato::create)					},
	{	OBJ_ITEM_HEALTH,			new CStaticFunctor<CObject*>(CHealthPack::create)				},
	{	OBJ_ITEM_SLICKSHOES,		new CStaticFunctor<CObject*>(CSpeedPack::create)				},
	{	OBJ_WEAPON_BIGCLUCKINGBOMB,	new CStaticFunctor<CObject*>(CBigCluckingBombWeapon::create)	},
	{	OBJ_WEAPONPROJ_FOOTBALL,	new CStaticFunctor<CObject*>(CFootBall::create)					},
	{	OBJ_AI_SPAWNTRIGGER,		new CStaticFunctor<CObject*>(CActorSpawnTrigger::create)		},
	{	OBJ_VENDINGMACHINE,			new CStaticFunctor<CObject*>(CVendingMachine::create)			},
	{	OBJ_PARTICLE_EMITTER,		new CStaticFunctor<CObject*>(CParticleEmitter::create)			},
	{	OBJ_OBJECTIVE_TRIGGER,		new CStaticFunctor<CObject*>(CObjectiveTrigger::create)			},
	{	OBJ_OBJECTIVE_BARRIER,		new CStaticFunctor<CObject*>(CObjectiveBarrier::create)			},
	{	OBJ_WEAPON,					new CStaticFunctor<CObject*>(CBaseWeapon::create)				},
	{	OBJ_LIGHT_POINT,			new CStaticFunctor<CObject*>(CPointLight::create)				},
	{	OBJ_LIGHT_SPOT,				new CStaticFunctor<CObject*>(CSpotLight::create)				},
};

CResourceManager::SResourceCreation CGameKernel::m_pResources[] = 
{
	// Textures
	{	RES_TEXTURE2D,			CTexture2D::createTextureFromFile,			"Textures"		},
	// Mesh
	{	RES_MESH,				CMesh::createMeshFromFile,					"Meshes"		},
	{	RES_ANIMATEDMESH,		CAnimatedMesh::createMeshFromFile,			"Animations"	},
	// Miscellaneous
	{	RES_FONT,				CBitmapFont::createFontFromFile,			"Fonts"			},
	{	RES_PARTICLEEMITTER,	CParticleSystem::createEmitterFromFile,		"Particles"		},
	// Sounds
	{	RES_SOUND,				CSound::createSoundFromFile,				"Sounds"		},
};

/**
* CGameKernel::CGameKernel
* @date Modified Mar 08, 2006
*/
CGameKernel::CGameKernel(void) : m_poActiveState(NULL)
{
	CObjectManager::getInstance().registerObjectTypes(m_pObjects, sizeof(m_pObjects) / sizeof(m_pObjects[0]));
	CResourceManager::getInstance().registerResourceTypes(m_pResources, sizeof(m_pResources) / sizeof(m_pResources[0]));
	m_unSizeOfObjects = sizeof(m_pObjects) / sizeof(m_pObjects[0]);
	srand((unsigned int)time(0));
}

/**
* CGameKernel::~CGameKernel
* @date Modified Mar 08, 2006
*/
CGameKernel::~CGameKernel(void)
{
	// Cleanup object creation pointers
	for(unsigned int i = 0; i < sizeof(m_pObjects) / sizeof(m_pObjects[0]); ++i)
		delete m_pObjects[i].poCreationFunction;

	// Shutdown subsystems.
	//CObjectManager::getInstance().deallocAllObjects(true);
	CMenuManager::deleteInstance();
	CTimer::deleteInstance();
	CCamera::deleteInstance();
	CFreeCamera::deleteInstance();
	CCommandConsole::deleteInstance();
	COctree::deleteInstance();
	
	COptions::deleteInstance();
	// DOING THIS CRASHES FOR SOME REASON :(
	//CTestState::deleteInstance();
	CTestState::deleteInstance();
	CGameoverState::deleteInstance();
	CLoadState::deleteInstance();
	CMenuState::deleteInstance();
	CVendingState::deleteInstance();
	COptions::deleteInstance();
	CPauseState::deleteInstance();
	CWinState::deleteInstance();
	CHighScoreTable::deleteInstance();

	// Resource manager should be cleaned up before render system.
	// States and other non-manager singletons should be cleaned up before the manager singletons.
	CObjectManager::deleteInstance();
	CDirectInputWrapper::deleteInstance();
	CSoundManager::getInstance().deinitialize();
	CSoundManager::deleteInstance();
	CAIManager::deleteInstance();
	CParticleManager::deleteInstance();
	CResourceManager::deleteInstance();
	CPhysicsManager::deleteInstance();
	// states get deleted in CAIManager::~CAIManager
	CRenderSystem::deleteInstance();
}

/**
* CGameKernel::tick
* @date Created Mar 08, 2006
*/
bool CGameKernel::tick(void)
{
	// Tick required systems
	CTimer::getInstance().tick();

	if(m_tRenderSys.tick())
		CRenderSystem::getInstance().tick();

	if(m_tInputSys.tick())
		CDirectInputWrapper::getInstance().updateInput();

	CSoundManager::getInstance().update();

	// Update game state specifics
	if(m_poActiveState)
		m_poActiveState->tick();
	else
		return false;

	return true;
}

/**
* CGameKernel::changeState
* @date Created Mar 08, 2006
*/
void CGameKernel::changeState(CBaseGameState* pState)
{
	// If there was a previous state, and it is being changed
	// trigger the previous state's deactivate.
	if(m_poActiveState && m_poActiveState != pState)
		m_poActiveState->onDeactivate();

	m_poActiveState = pState;
	if(pState)
		pState->onActivate();
}

/**
* CGameKernel::initialize
* @date Created Mar 08, 2006
*/
bool CGameKernel::initialize(HINSTANCE hInst)
{
	// Init global logs
	CString sTitle;
	
	Log.openLog("AckZombies.log");
	Debug.openLog("debug.log");
	Log << "Logging System Initialized!" << endl;
	Debug << "Debug Log Initialized!" << endl;

	// Open Configuration
	if(!m_oConfig.LoadConfig("AckZombies.ini"))
		return false;

	// TODO : Initialize Subsystems.
	m_hWnd = CRenderSystem::getInstance().initialize();
	if(!m_hWnd)
	{
		Debug << "Render System Initialization Failed!" << endl;
		return false;
	}

	Log << "Render System Initialized!" << endl;
	if(!CDirectInputWrapper::getInstance().initializeDirectInput(hInst, m_hWnd))
	{
		Debug << "Input System Initialization Failed!" << endl;
		return false;
	}
	Log << "Input System Initialized!" << endl;

	// Initialize timers
	m_tRenderSys.setInterval(1.0f/CRenderSystem::getInstance().getRenderDevice().getRefreshRate());
	m_tInputSys.setInterval(0.016f);
	m_tObjectSys.setInterval(0.016f);
	m_tAISys.setInterval(0.016f);

	// intialize the sound manager
	CSoundManager::getInstance().initialize();

	// Init the options for the sound volumes
	COptions::getInstance();

	// Change to first game state (currently test state)
	changeState(CMenuState::getInstancePtr());

	// Initialize the menu manager
	CMenuManager::getInstance().initializeManager("../Textures/Menu/mouseCursor.tga", 10, 18, 10, 18);

	// Initialize console commands
	CCommandConsole* pCon = CCommandConsole::getInstancePtr();
	pCon->registerCommand(new CQuitCommand);
	pCon->registerCommand(new CQuitPlacementCommand);
	pCon->registerCommand(new CStartPlacementCommand);
	pCon->registerCommand(new CDrawPlacementCommand);
	pCon->registerCommand(new CExportPlacementCommand);
	pCon->registerCommand(new CCheatCommand);
	pCon->registerCommand(new CShowFPSCommand);
	pCon->registerCommand(new CStringPlacementCommand);

	return true;
}

/**
* CGameKernel::getConfig
* @date Created Mar 28, 2006
*/
CConfigFile& CGameKernel::getConfig(void)
{
	return m_oConfig;
}