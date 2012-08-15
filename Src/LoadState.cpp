/**
* @file LoadState.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created May 8, 2006
*
* the loading state
*/

#include "LoadState.h"

#include "DirectInputWrapper.h"
#include "CommandConsole.h"
#include "ResourceManager.h"
#include "ObjectTypes.h"
#include "ActorIncludes.h"
#include "ActorSpawn.h"
#include "ObjectiveTrigger.h"
#include <fstream>

#include "TestState.h"
#include "Options.h"
#include "VendingMachine.h"
#include "HUD.h"

#define CHECK_READ_FAILED if ((iflBin.rdstate() & std::ios::failbit) == std::ios::failbit) break
#define NUM_EMITTERS_TO_LOAD 100
#define NUM_ZOMBIECITIZENS_TO_LOAD NUM_EMITTERS_TO_LOAD

/**
* CLoadState::onActivate
* @date Modified May 8, 2006
*/
void CLoadState::onActivate(void)
{
	CConfigFile oConfig;
	m_bProceed = false;
	oConfig.LoadConfig("AckZombies.ini");
	CString str = oConfig.GetValue("LoadScreens", "LoadIndex");
	int nLoadIndex = str.ToInt();
	nLoadIndex %= 6;
	int nRandomLoad = rand() % 5;
	while (nRandomLoad == nLoadIndex)
	{
		nRandomLoad = rand() % 5;
	}
	CString oPath = "../textures/";
	oPath += m_strLoadScreens[nRandomLoad];

	// load the loading screen menu
	m_poLoadingScreen = new CMenu(0, 0, CMenuManager::getInstancePtr()->getWindowWidth(), 
		CMenuManager::getInstancePtr()->getWindowHeight(), "../Textures/Menu/LoadScreen.tga", 1009, 733);

	str.Format("%d", nRandomLoad);
	oConfig.SetValue("LoadScreens", "LoadIndex", str);
	oConfig.SaveConfig("AckZombies.ini");

	if (!m_fntDina)
		m_fntDina = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "HandelGothic.ini");

	CMenuManager::getInstance().getHUD()->addToAwesomeMeter(0);
}

/**
* CLoadState::onDeactivate
* @date Modified May 8, 2006
*/
void CLoadState::onDeactivate(void)
{
	// delete the loading screen
	if (m_poLoadingScreen)
	{
		delete m_poLoadingScreen;
		m_poLoadingScreen = NULL;
	}
}

/**
* CLoadState::tick
* @date Modified May 8, 2006
*/
void CLoadState::tick(void)
{
	// make sure we've rendered first before trying to load anything.
	if (!m_bRendered)
		return;

	// make sure we've notloaded first
	if (m_bProceed)
		return;

	CTestState* poGameState = CTestState::getInstancePtr();

	// load the file and parse it
	ifstream ifl;
	int nSize = 0;
	char buffer[128] = { 0 };

	CString string;
	std::vector<CString> delimited;

	ifl.open("LevelData.bin", std::ios_base::in | std::ios_base::binary);

	if (ifl.is_open())
	{
		while (!ifl.eof())
		{
			memset(buffer, 0, 128);
			nSize = 0;

			ifl.read(((char*)(&nSize)), sizeof(int));
			if (nSize == 0)
				break;
			ifl.read(buffer, nSize);

			delimited.clear();
			string.Format(buffer);
			string.ToList(delimited, ".");

			// load the resources specified by the file
			if (!m_bLoaded && delimited.back() == CString("x"))
			{	
				CBaseResource* poResource = (CBaseResource*)CResourceManager::getInstance().loadResource(RES_MESH, string);
				addResource(poResource);
			}
			else if (!m_bLoaded && delimited.back() == CString("anim"))
			{
				string.Trim(0, string.GetLength() - (delimited.back().GetLength() + 1));

				CBaseResource* poResource = (CBaseResource*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, string);
				addResource(poResource);
			}
			else if (!m_bLoaded && delimited.back() == CString("wld"))
			{
				string.Trim(0, string.GetLength() - (delimited.back().GetLength() + 1));

				// Level Mesh
				/////////////////////////////////////
				poGameState->m_pMesh2 = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, string.GetBuffer());
				//poGameState->m_pMesh2 = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "levelmap.x");
				addResource(poGameState->m_pMesh2);
			}
			else if (!m_bLoaded && delimited.back() == CString("col"))
			{
				string.Trim(0, string.GetLength() - (delimited.back().GetLength() + 1));

				// Collision Geometry get initialized here
				////////////////////////////////////////////
				CPhysicsManager::getInstance().initCollisionGeometry(string.GetBuffer());

				// Render the collision geometry
				/////////////////////////////////////
				poGameState->m_pMesh3 = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, string.GetBuffer());
				addResource(poGameState->m_pMesh3);
			}
			else if (!m_bLoaded && delimited.back() == CString("xml"))
			{
				CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
				// these are particle emitters and several of them should be available at run time
				unsigned int unCount = 0;
				while (unCount++ < NUM_EMITTERS_TO_LOAD)
				{
					addResource((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, string.GetBuffer()));
				}
			}
			else if (delimited.back() == CString("bin"))
			{
				ifstream iflBin;
				iflBin.open(string.GetBuffer(), ios_base::in | std::ios_base::binary);

				if (iflBin.is_open())
				{
					// common initialization variables
					D3DXVECTOR3 vRotation(0.0f,0.0f,1.0f);
					D3DXVECTOR3 vScale(1.0f,1.0f,1.0f);
					// the type of actors in this file
					EObjectType eType;

					// start new because we haven't failed yet
					iflBin.clear();
					// try and read
					iflBin.read((char*)&eType, sizeof(EObjectType));
					// did we fail to read
					CHECK_READ_FAILED;

					// now that we know the type, we know what to do
					switch (eType)
					{
					case OBJ_AI_NODE:
						{
							// set up variables that are needed
							D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
							float fRadius = 0.0f;

							while (!iflBin.eof())
							{
								// start new because we haven't failed yet
								iflBin.clear();

								// try and read
								iflBin.read((char*)&vPos, sizeof(D3DXVECTOR3));	CHECK_READ_FAILED;
								iflBin.read((char*)&fRadius, sizeof(float));	CHECK_READ_FAILED;

								// we have all the data we need to create an AI Node
								CAINode* poNode = (CAINode*)CObjectManager::getInstance().createObject(OBJ_AI_NODE);
								// set the new node's data
								poNode->setActorMatrix(vRotation, vPos, vScale);
								poNode->setRadius(fRadius);
							}
							break;
						}
					case OBJ_SPAWN:
						{
							// set up variables that are needed
							D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
							unsigned int unNum = 0;
							EObjectType eSpawnType = OBJ_ENEMY_ZOMBIECITIZEN;

							while (!iflBin.eof())
							{
								// start new because we haven't failed yet
								iflBin.clear();

								// try and read
								iflBin.read((char*)&vPos, sizeof(D3DXVECTOR3));			CHECK_READ_FAILED;
								iflBin.read((char*)&unNum, sizeof(unsigned int));		CHECK_READ_FAILED;
								iflBin.read((char*)&eSpawnType, sizeof(EObjectType));	CHECK_READ_FAILED;

								// we have all the data we need to create a Spawn
								CActorSpawn* poSpawn = (CActorSpawn*)CObjectManager::getInstance().createObject(OBJ_SPAWN);
								// set the new spawn's data
								poSpawn->setActorMatrix(vRotation, vPos, vScale);
								poSpawn->setNumActors(unNum);
								poSpawn->setSpawnType(eSpawnType);
							}
							break;
						}
					case OBJ_AI_SPAWNTRIGGER:
						{
							// set up variables that are needed
							D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
							float fRadius = 0.0f;
							float fDepth = 0.0f;
							float fWaitTime = 0.0f;
							unsigned int unNum = 0;
							int nMaxTimes = 0;

							while (!iflBin.eof())
							{
								// start new because we haven't failed yet
								iflBin.clear();

								// try and read
								iflBin.read((char*)&vPos, sizeof(D3DXVECTOR3));		CHECK_READ_FAILED;
								iflBin.read((char*)&fRadius, sizeof(float));		CHECK_READ_FAILED;
								iflBin.read((char*)&fDepth, sizeof(float));			CHECK_READ_FAILED;
								iflBin.read((char*)&fWaitTime, sizeof(float));		CHECK_READ_FAILED;
								iflBin.read((char*)&unNum, sizeof(unsigned int));	CHECK_READ_FAILED;
								iflBin.read((char*)&nMaxTimes, sizeof(int));		CHECK_READ_FAILED;

								// we have all the data we need to create an ActorSpawnTrigger
								CActorSpawnTrigger* poTrigger =
									(CActorSpawnTrigger*)CObjectManager::getInstance().createObject(OBJ_AI_SPAWNTRIGGER);
								// set the new trigger's data
								poTrigger->m_fRadius = fRadius;
								poTrigger->m_fDepth = fDepth;

								if (poTrigger->m_fDepth > 0.0f)
									poTrigger->setActorMatrix(vRotation, vPos, D3DXVECTOR3(fRadius, 1.0f, fDepth));
								else
									poTrigger->setActorMatrix(vRotation, vPos, D3DXVECTOR3(fRadius, 1.0f, fRadius));

								poTrigger->m_fWaitTime = fWaitTime;
								poTrigger->m_unNumSpawns = unNum;
								poTrigger->m_nMaxTimes = nMaxTimes;
								poTrigger->m_fCurrentTime = fWaitTime;
							}
							break;
						}
					case OBJ_OBJECTIVE_TRIGGER:
						{
							// set up the variables that are needed
							D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
							CObjectiveTrigger::EObjectiveTypes eObjectiveType;
							float fCountdownTime = 0.0f;
							unsigned int unNumDudes = 0;
							float fRadius = 0.0f;
							float fDepth = 0.0f;
							bool bGoal = false;
							unsigned int unOrderNumber = 0;
							unsigned int unNumBarriers = 0;
							unsigned int unStringSize = 0;
							char buffer[256] = { 0 };

							while (!iflBin.eof())
							{
								// start new because we haven't failed yet
								iflBin.clear();
								memset(buffer, 0, 256);

								// try and read
								iflBin.read((char*)&vPos, sizeof(D3DXVECTOR3));										CHECK_READ_FAILED;
								iflBin.read((char*)&eObjectiveType, sizeof(CObjectiveTrigger::EObjectiveTypes));	CHECK_READ_FAILED;
								iflBin.read((char*)&fCountdownTime, sizeof(float));									CHECK_READ_FAILED;
								iflBin.read((char*)&unNumDudes, sizeof(unsigned int));
								iflBin.read((char*)&fRadius, sizeof(float));										CHECK_READ_FAILED;
								iflBin.read((char*)&fDepth, sizeof(float));											CHECK_READ_FAILED;
								iflBin.read((char*)&bGoal, sizeof(bool));											CHECK_READ_FAILED;
								iflBin.read((char*)&unOrderNumber, sizeof(unsigned int));							CHECK_READ_FAILED;
								iflBin.read((char*)&unNumBarriers, sizeof(unsigned int));							CHECK_READ_FAILED;
								iflBin.read((char*)&unStringSize, sizeof(unsigned int));							CHECK_READ_FAILED;
								iflBin.read(buffer, unStringSize);

								// we have all the data we need to make an objective trigger
								CObjectiveTrigger* poTrigger = (CObjectiveTrigger*)CObjectManager::getInstance().createObject(OBJ_OBJECTIVE_TRIGGER);

								// set the new trigger's values
								poTrigger->m_fRadius = fRadius;
								poTrigger->m_fDepth = fDepth;

								if (poTrigger->m_fDepth > 0.0f)
									poTrigger->setActorMatrix(vRotation, vPos, D3DXVECTOR3(fRadius, 1.0f, fDepth));
								else
									poTrigger->setActorMatrix(vRotation, vPos, D3DXVECTOR3(fRadius, 1.0f, fRadius));

								poTrigger->m_eObjectiveType = eObjectiveType;
								poTrigger->m_fCountdownTime = fCountdownTime;
								poTrigger->m_unNumDudes = unNumDudes;
								poTrigger->m_bGoal = bGoal;
								poTrigger->m_unOrderNumber = unOrderNumber;
								poTrigger->m_unNumBarriers = unNumBarriers;
								poTrigger->m_strToolTip.Format("%s", buffer);

								poTrigger->m_bTriggered = false;
								poTrigger->m_unCurrentDudes = 0;
								poTrigger->m_fCurrentTime = 0;
							}
							break;
						}
					case OBJ_OBJECTIVE_BARRIER:
						{
							D3DXVECTOR3 vScale(0.5f, 0.5f, 0.5f);
							// set up the variables that are needed
							D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
							D3DXVECTOR3 vOrientation(0.0f, 0.0f, 1.0f);
							float fRadius = 0.0f;

							while (!iflBin.eof())
							{
								// start new because we haven't failed yet
								iflBin.clear();

								// try and read
								iflBin.read((char*)&vPos, sizeof(D3DXVECTOR3));				CHECK_READ_FAILED;
								iflBin.read((char*)&vOrientation, sizeof(D3DXVECTOR3));		CHECK_READ_FAILED;
								iflBin.read((char*)&fRadius, sizeof(float));				CHECK_READ_FAILED;

								// we have all the data we need to make an objective barrier
								CObjectiveBarrier* poBarrier = (CObjectiveBarrier*)CObjectManager::getInstance().createObject(OBJ_OBJECTIVE_BARRIER);

								// set the new Barrier's values
								poBarrier->setBV(vPos, fRadius);
								vPos.y = 0.0f;
								poBarrier->setPosition(vPos);
								poBarrier->setScale(vScale);
								poBarrier->setOrientation(vOrientation);
								poBarrier->deactivate();
								poBarrier->setMesh(NULL);
							}

							break;
						}
					case OBJ_LIGHT:
						{
							// set up the variables that are needed
							EObjectType eSubType = OBJ_LIGHT_SPOT;
							D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
							float fRadius = 0.0f;
							D3DCOLOR color = 0;

							while (!iflBin.eof())
							{
								// start new because we haven't failed yet
								iflBin.clear();

								// try and read
								iflBin.read((char*)&vPos, sizeof(D3DXVECTOR3));				CHECK_READ_FAILED;
								iflBin.read((char*)&eSubType, sizeof(EObjectType));			CHECK_READ_FAILED;

								if (eSubType == OBJ_LIGHT_POINT)
								{
									iflBin.read((char*)&fRadius, sizeof(float));			CHECK_READ_FAILED;

									CPointLight* poLight = (CPointLight*)CObjectManager::getInstance().createObject(OBJ_LIGHT_POINT);
									poLight->setPosition(vPos);
									poLight->setRadius(fRadius);
								}
								else /*eSubType == OBJ_LIGHT_SPOT*/
								{
									iflBin.read((char*)&color, sizeof(D3DCOLOR));			CHECK_READ_FAILED;

									CSpotLight* poLight = (CSpotLight*)CObjectManager::getInstance().createObject(OBJ_LIGHT_SPOT);
									poLight->setPosition(vPos);
									poLight->setColor(color);
								}
							}
							break;
						}
					}

					iflBin.close();
				}
			}
		}

		ifl.close();
	}
	else
	{
		MessageBox(CRenderSystem::getInstance().getRenderWindow(), "Failed to Load LevelData.bin", "Error", MB_OK);
		// we are boned
		CGameKernel::getInstancePtr()->changeState(NULL);
		return;
	}

	// create some enemies so they are ready when we need them
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();
	unsigned int unNumEnemies = 0;
	while (unNumEnemies++ < NUM_ZOMBIECITIZENS_TO_LOAD)
	{
		CEnemy* poEnemy = (CEnemy*)poObjectManager->createObject(OBJ_ENEMY_ZOMBIECITIZEN);
		poObjectManager->removeObjectImmediate(poEnemy);
	}

	// create the player and weapons
	CActor* pActor = (CActor*)poObjectManager->createObject(OBJ_PLAYER);
	pActor->setPosition(D3DXVECTOR3(-448.0f, 0.0f, -414.0f));
	pActor->setOrientation(D3DXVECTOR3(0, 0, 1));
	pActor->activate();
	pActor->setBV(D3DXVECTOR3(pActor->getPosition().x, 8.5f, pActor->getPosition().z), 3.0f);
	CAnimatedMesh* pAnim = (CAnimatedMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Johnny.x");
	pAnim->setAnimationSetByName("Idle");
	pActor->setMesh((CMesh*)pAnim);
	((CPlayer*)pActor)->setDefaultScale(D3DXVECTOR3(0.65f, 0.65f, 0.65f));

	CPistolWeapon *pPistol = new CPistolWeapon;
	pPistol->init();
	pPistol->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "pistola.x"));
	pPistol->getMesh()->addRef();
	pPistol->setPlayer((CPlayer*)pActor);
	CGrenadeWeapon *pGrenade = new CGrenadeWeapon;
	pGrenade->init();


	((CPlayer*)pActor)->setIcon("icon1.x");
	((CPlayer*)pActor)->addWeapon(pPistol);
	((CPlayer*)pActor)->addGrenade(pGrenade);
	pGrenade->setPlayer((CPlayer*)pActor);
	((CPlayer*)pActor)->setOrientation(D3DXVECTOR3(0, 0, 1));
	COptions::getInstance().setPlayerControls(((CPlayer*)pActor), true);

	CActor* pActor2;
	// Create the 2nd player if necessary
	if (m_bTwoPlayers)
	{
		// create the player and weapons
		pActor2 = (CActor*)poObjectManager->createObject(OBJ_PLAYER);
		pActor2->setPosition(D3DXVECTOR3(-445.0f, 0.0f, -435.0f));
		pActor2->setOrientation(D3DXVECTOR3(0, 0, 1));
		pActor2->activate();
		pActor2->setBV(D3DXVECTOR3(pActor2->getPosition().x, 8.5f, pActor2->getPosition().z), 3.0f);
		pAnim = (CAnimatedMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Johnny.x");
		pAnim->setAnimationSetByName("Idle");
		pActor2->setMesh((CMesh*)pAnim);
		((CPlayer*)pActor2)->setDefaultScale(D3DXVECTOR3(0.65f, 0.65f, 0.65f));

		pPistol = new CPistolWeapon;
		pPistol->init();
		pPistol->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "pistola.x"));
		pPistol->getMesh()->addRef();
		pPistol->setPlayer((CPlayer*)pActor2);
		pGrenade = new CGrenadeWeapon;
		pGrenade->init();


		((CPlayer*)pActor2)->setIcon("icon2.x");
		((CPlayer*)pActor2)->addWeapon(pPistol);
		((CPlayer*)pActor2)->addGrenade(pGrenade);
		pGrenade->setPlayer((CPlayer*)pActor2);
		((CPlayer*)pActor2)->setOrientation(D3DXVECTOR3(0, 0, 1));
		COptions::getInstance().setPlayerControls(((CPlayer*)pActor2), false);
	}

	// Hard Coded Vending machine placement is BAD
	// Create the vending machine
	CVendingMachine *pVending;
	pVending = (CVendingMachine*)poObjectManager->createObject(OBJ_VENDINGMACHINE);
	CMesh *pMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "vending.x");
	pVending->setMesh(pMesh);
	pVending->setPosition(D3DXVECTOR3(-390.0f, 0.0f, -285.0f));
	pVending->setOrientation(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	pVending->setScale(D3DXVECTOR3(0.15f, 0.1f, 0.15f));
	D3DXVECTOR3 VendPos = pVending->getPosition();
	VendPos.y += 7.0f;
	pVending->setBV(VendPos, 8.0f);
	pVending->setActionBounds(SSphere(VendPos, 20.0f));
	pVending->addWeapon(VMB_SHOTGUN);
	pVending->addWeapon(VMB_DESERTEAGLE);
	pVending->addWeapon(VMB_POTATOCHAINGUN);

	// Create the vending machine
	pVending = (CVendingMachine*)poObjectManager->createObject(OBJ_VENDINGMACHINE);
	pVending->setMesh(pMesh);
	pVending->setPosition(D3DXVECTOR3(22.0f, 0.0f, -50.0f));
	pVending->setOrientation(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	pVending->setScale(D3DXVECTOR3(0.15f, 0.1f, 0.15f));
	pVending->setBV(pVending->getPosition(), 10.0f);
	pVending->setActionBounds(SSphere(pVending->getPosition(), 20.0f));
	pVending->addWeapon(VMB_TRIPLEBARRELSHOTGUN);
	pVending->addWeapon(VMB_HOLYWATERBALLOONCANON);
	pVending->addWeapon(VMB_BIGCLUCKINGBOMB);

	// Create the vending machine
	pVending = (CVendingMachine*)poObjectManager->createObject(OBJ_VENDINGMACHINE);
	pVending->setMesh(pMesh);
	pVending->setPosition(D3DXVECTOR3(257.0f, 0.0f, 253.0f));
	pVending->setOrientation(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	pVending->setScale(D3DXVECTOR3(0.15f, 0.1f, 0.15f));
	pVending->setBV(pVending->getPosition(), 10.0f);
	pVending->setActionBounds(SSphere(pVending->getPosition(), 20.0f));
	pVending->addWeapon(VMB_ROCKETLAUNCHER);
	pVending->addWeapon(VMB_DESERTEAGLE);
	pVending->addWeapon(VMB_POTATOCHAINGUN);

	// set up the camera
	if (m_bTwoPlayers)
		CCamera::getInstance().setCamera(D3DXVECTOR3(-448.0f, 75.0f, -500.0f), pActor->getPosition(), pActor2->getPosition());
	else
		CCamera::getInstance().setCamera(D3DXVECTOR3(-448.0f, 75.0f, -500.0f), pActor->getPosition());

	CAIManager* poAIManager = CAIManager::getInstancePtr();
	poAIManager->createNodeLinks();
	poAIManager->updateGoalLinks();
	poAIManager->initSpawnTriggers();
	poAIManager->initObjectiveTriggers();

	// we're done rendering now
	m_bRendered = false;
	// we've loaded now
	m_bLoaded = m_bProceed = true;
}

/**
* CLoadState::preRender
* @date Modified May 8, 2006
*/
void CLoadState::preRender(CRenderDevice& oRenderDevice)
{
	m_bRendered = true;
}

/**
* CLoadState::postRender
* @date Modified June 5, 2006
*/
void CLoadState::postRender(CRenderDevice& oRenderDevice)
{
	// get the current cull mode and save it to restore later
	DWORD dwCullMode;
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CMenuManager::getInstance().getReadyToRender();
	m_poLoadingScreen->drawMenu(NULL);
	CMenuManager::getInstance().endRender();
	// restore cull mode
	CRenderSystem::getInstance().getRenderDevice().getD3DDevice()->SetRenderState(D3DRS_CULLMODE, dwCullMode);

	CString str;

	const CString red = "{0xffff0000}";
	const CString white = "{0xffffffff}";
	static CString stringColor = white;
	static float fFlashTimer = CTimer::getInstance().getTime();

	str = stringColor;

	if (!m_bLoaded)
	{
		str += "Loading..";
	}
	else
	{
		if (CTimer::getInstance().getTime() - fFlashTimer >= 0.5f)
		{
			if (stringColor == white)
				stringColor = red; 
			else
				stringColor = white;
			fFlashTimer = CTimer::getInstance().getTime();
		}

		str += "Press any key..";
	}

	m_fntDina->drawStringMarkup(str, (float)(CMenuManager::getInstance().getWindowWidth() >> 1)
		- (str.GetLength() - 12) * 4.0f, (float)(CMenuManager::getInstance().getWindowHeight() - 52));

	CDirectInputWrapper::getInstance().getXInputDevice().updateControllers();
	
	// display to the user to press a key to continue
	if (m_bProceed)
	{
		// check for any key
		DWORD dwKey;
		if (CDirectInputWrapper::getInstance().getKey(dwKey) || 
			CDirectInputWrapper::getInstance().getXInputDevice().getKey(0, dwKey) ||
			CDirectInputWrapper::getInstance().getXInputDevice().getKey(1, dwKey))
		{
			m_bProceed = false;
			CGameKernel::getInstance().changeState(CTestState::getInstancePtr());
			// delete the loading screen
			CMenuManager::getInstance().deleteMenu(m_poLoadingScreen);
		}
	}
}

/**
* CLoadState::addResource
* @date Modified May 8, 2006
*/
void CLoadState::addResource(CBaseResource* poResource)
{
	if (poResource == NULL) return;

	std::list<CBaseResource*>::iterator oResourceIter = m_loResources.begin();

	bool bDuplicate = false;

	while (oResourceIter != m_loResources.end())
	{
		if (((CBaseResource*)(*oResourceIter)) == poResource)
		{
			bDuplicate = true;
			break;
		}
		oResourceIter++;
	}

	if (!bDuplicate)
	{
		m_loResources.push_back(poResource);
	}
}

/**
* CLoadState::releaseResources
* @date Modified May 8, 2006
*/
void CLoadState::releaseResources(void)
{
	std::list<CBaseResource*>::iterator oResourceIter = m_loResources.begin();

	while (oResourceIter != m_loResources.end())
	{
		// decrements reference count
		// deletes if no more references
		((CBaseResource*)(*oResourceIter))->Release();
		oResourceIter++;
	}

	m_loResources.clear();
}
