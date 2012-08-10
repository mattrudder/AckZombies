/**
* @file PlacementState.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
*
* the editor state
*/ 

#include "PlacementState.h"
#include "Acespaceengine.h"
#include "ObjectManager.h"
#include "TestState.h"
#include "GameKernel.h"
#include "AIManager.h"
#include "AIStatePathPlan.h"
#include <fstream>

#define NODE_PTR(x) ((CAINode*)(*x))
#define SPAWN_PTR(x) ((CActorSpawn*)(*x))
#define TRIGGER_PTR(x) ((CActorSpawnTrigger*)(*x))
#define OBJECTIVE_PTR(x) ((CObjectiveTrigger*)(*x))
#define ACTOR_PTR(x) ((CActor*)(*x))
#define LIGHT_PTR(x) ((CLight*)(*x))

/**
* CPlacementState::CPlacementState
* @date Modified April 27, 2006
*/
CPlacementState::CPlacementState(void)
{
	CGameKernel* poGame = CGameKernel::getInstancePtr();

	// get all the types supported by the objectmanager
	for (unsigned int i = 0; i < poGame->getSizeOfObjects(); ++i)
	{
		// Enemies in sub zero
		if ((poGame->m_pObjects[i].eObjectTypeID & OBJ_ENEMY) == OBJ_ENEMY)
		{
			m_mapOfListsOfTypes[0].push_back(poGame->m_pObjects[i].eObjectTypeID);
		}

		// AI Nodes in sub 1
		if ((poGame->m_pObjects[i].eObjectTypeID & OBJ_AI) == OBJ_AI)
		{
			m_mapOfListsOfTypes[1].push_back(poGame->m_pObjects[i].eObjectTypeID);
		}

		// Pickups in sub 2
		if ((poGame->m_pObjects[i].eObjectTypeID & OBJ_WORLD) == OBJ_WORLD)
		{
			m_mapOfListsOfTypes[2].push_back(poGame->m_pObjects[i].eObjectTypeID);
		}

		// lights in sub 3
		if ((poGame->m_pObjects[i].eObjectTypeID & OBJ_LIGHT) == OBJ_LIGHT)
		{
			m_mapOfListsOfTypes[3].push_back(poGame->m_pObjects[i].eObjectTypeID);
		}
	}
}

/**
* CPlacementState::CPlacementState
* @date Modified April 27, 2006
*/
CPlacementState::~CPlacementState(void)
{
	if (m_pSphere)
		m_pSphere->Release();

	if (m_pBox)
		m_pBox->Release();
}

/**
* CPlacementState::onActivate
* @date Modified April 29, 2006
*/
void CPlacementState::onActivate(void)
{
	Debug << "Activate Placement State" << endl;

	m_eCurrentType = OBJ_AI_NODE;
	m_unIndex = 1;
	m_bCollisionGeometry = false;
	m_bDrawNodesSpawnTriggers = true;
	m_bDrawObjectiveTriggersBarriers = true;
	m_bEditing = false;
	m_tUnbufferedIncrease.setInterval(0.05f);

	// make the new cursor
	m_poCursor = (CActor*)CObjectManager::getInstancePtr()->createObject(OBJ_ACTOR);
	m_poStoredCursor = m_poCursor;

	// place the cursor where player one am
	CObjectManager::ObjectList loPlayers;
	CObjectManager::getInstancePtr()->getObjects(OBJ_PLAYER, &loPlayers);
	m_poCursor->setPosition(((CPlayer*)(loPlayers.front()))->getBV().centerPt);

	m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

	initAttributes();

	static bool bFirstTime = true;
	if (bFirstTime)
	{
		bFirstTime = false;

		m_unCurrentObjectiveNumber = 0;

		m_fntDina = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "HandelGothic.ini");
		m_fntDinaFPS = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "Dina.ini");

		// used to draw Nodes and triggers
		D3DXCreateSphere(CRenderSystem::getInstancePtr()->getRenderDevice().getD3DDevice(), 1.0f, 10, 10, &m_pSphere, NULL);
		D3DXCreateBox(CRenderSystem::getInstancePtr()->getRenderDevice().getD3DDevice(), 1.0f, 1.0f, 1.0f, &m_pBox, NULL);
	}
}

/**
* CPlacementState::onDeactivate
* @date Modified April 27, 2006
*/
void CPlacementState::onDeactivate(void)
{
	if (m_bEditing)
	{
		m_poCursor->setMesh(NULL);
		m_poCursor = m_poStoredCursor;
	}

	CObjectManager::getInstancePtr()->removeObjectImmediate(m_poCursor);

	CAIManager::getInstancePtr()->initSpawnTriggers();

	Debug << "Deactivate Placement State" << endl;
}

/**
* CPlacementState::tick
* @date Modified April 27, 2006
*/
void CPlacementState::tick(void)
{
	CDirectInputWrapper* pInput = CDirectInputWrapper::getInstancePtr();
	CObjectManager* poObjectManager = CObjectManager::getInstancePtr();

	if(CCommandConsole::getInstance().isVisible())
	{
		CCommandConsole::getInstance().update();
		return;
	}

	// move the light up or down
	if (pInput->isKeyDown(DIK_U))
	{
		if (m_tUnbufferedIncrease.tick())
		{
			D3DXVECTOR3 vPos = m_poCursor->getPosition();
			vPos.y += 1.0f;
			m_poCursor->setPosition(vPos);
		}
	}
	else if (pInput->isKeyDown(DIK_J))
	{
		if (m_tUnbufferedIncrease.tick())
		{
			D3DXVECTOR3 vPos = m_poCursor->getPosition();
			vPos.y -= 1.0f;
			m_poCursor->setPosition(vPos);
		}
	}

	// update current attribute
	if (pInput->isKeyDownBuffered(DIK_UP, PLACEMENT_SYSTEM))
	{
		m_unIndex--;

		if (m_unIndex < 1)
			m_unIndex = (unsigned int)m_vunAttributes.size() - 1;
	}

	if (pInput->isKeyDownBuffered(DIK_DOWN, PLACEMENT_SYSTEM))
	{
		m_unIndex++;

		if (m_unIndex > (unsigned int)(m_vunAttributes.size() - 1))
			m_unIndex = 1;
	}

	// update current attribute value
	if (pInput->isKeyDown(DIK_LSHIFT))
	{
		if (pInput->isKeyDownBuffered(DIK_RIGHT, PLACEMENT_SYSTEM))
		{
			m_vunAttributes[m_unIndex]++;
		}

		if (pInput->isKeyDownBuffered(DIK_LEFT, PLACEMENT_SYSTEM))
		{
			m_vunAttributes[m_unIndex]--;
			if (m_vunAttributes[m_unIndex] < 0)
				m_vunAttributes[m_unIndex] = 0;
		}
	}
	else
	{
		if (m_tUnbufferedIncrease.tick())
		{
			if (pInput->isKeyDown(DIK_RIGHT))
			{
				if (m_eCurrentType == OBJ_ITEM_MONEY)
				{
					m_vunAttributes[m_unIndex] += 100;
				}
				else
					m_vunAttributes[m_unIndex]++;
			}

			if (pInput->isKeyDown(DIK_LEFT))
			{
				if (m_eCurrentType == OBJ_ITEM_MONEY)
				{
					m_vunAttributes[m_unIndex] -= 100;
				}
				else
					m_vunAttributes[m_unIndex]--;

				if (m_vunAttributes[m_unIndex] < 0)
					m_vunAttributes[m_unIndex] = 0;
			}
		}
	}

	checkChangeInType();

	// placement
	if (pInput->isKeyDownBuffered(DIK_SPACE, PLACEMENT_SYSTEM))
	{
		switch (m_eCurrentType)
		{
		case OBJ_LIGHT_POINT:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					((CPointLight*)m_poCursor)->setRadius((float)m_vunAttributes[1]);
					break;
				}
				// create the new point light
				CPointLight* poLight = (CPointLight*)poObjectManager->createObject(m_eCurrentType);
				poLight->setPosition(m_poCursor->getPosition());
				poLight->setRadius((float)m_vunAttributes[1]);
				break;
			}
		case OBJ_LIGHT_SPOT:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					((CSpotLight*)m_poCursor)->setColor(D3DCOLOR_XRGB(m_vunAttributes[1], m_vunAttributes[2], m_vunAttributes[3]));
					break;
				}
				// create the new spot light
				CSpotLight* poLight = (CSpotLight*)poObjectManager->createObject(m_eCurrentType);
				poLight->setPosition(m_poCursor->getPosition());
				poLight->setColor(D3DCOLOR_XRGB(m_vunAttributes[1], m_vunAttributes[2], m_vunAttributes[3]));
				break;
			}
		case OBJ_OBJECTIVE_BARRIER:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					CObjectManager::ObjectList loPlayers;
					CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
					D3DXVECTOR3 vPlayerPos = ((CPlayer*)loPlayers.front())->getBV().centerPt;
					((CObjectiveBarrier*)(m_poCursor))->setBV(D3DXVECTOR3(m_poCursor->getPosition().x, vPlayerPos.y, m_poCursor->getPosition().z), (float)m_vunAttributes[1]);
					break;
				}
				// create the new barrier
				CObjectiveBarrier* poBarrier = (CObjectiveBarrier*)poObjectManager->createObject(m_eCurrentType);
				CObjectManager::ObjectList loPlayers;
				CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
				D3DXVECTOR3 vPlayerPos = ((CPlayer*)loPlayers.front())->getBV().centerPt;
				poBarrier->setBV(D3DXVECTOR3(m_poCursor->getPosition().x, vPlayerPos.y, m_poCursor->getPosition().z), (float)m_vunAttributes[1]);
				vPlayerPos.y = 0.0f;
				poBarrier->setPosition(D3DXVECTOR3(m_poCursor->getPosition().x, vPlayerPos.y, m_poCursor->getPosition().z));
				poBarrier->setOrientation(m_poCursor->getOrientation());
				poBarrier->setScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
				poBarrier->deactivate();
				break;
			}
		case OBJ_OBJECTIVE_TRIGGER:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					((CObjectiveTrigger*)(m_poCursor))->m_fRadius = (float)m_vunAttributes[1];
					((CObjectiveTrigger*)(m_poCursor))->m_fDepth = (float)m_vunAttributes[2];

					// default the type to Goto objective
					((CObjectiveTrigger*)(m_poCursor))->m_eObjectiveType = CObjectiveTrigger::OJT_GOTO;

					// if there is a time for this objective
					if (m_vunAttributes[4] > 0)
					{
						// reset the type and set the countdown time
						((CObjectiveTrigger*)(m_poCursor))->m_eObjectiveType = CObjectiveTrigger::OJT_TIMED;
						((CObjectiveTrigger*)(m_poCursor))->m_fCountdownTime = (float)m_vunAttributes[4];
					}
					// if there is a number of dudes that need to be killed
					if (m_vunAttributes[3] > 0)
					{
						// reset the type and countdown time and then set the number of dudes
						((CObjectiveTrigger*)(m_poCursor))->m_eObjectiveType = CObjectiveTrigger::OJT_DUDES;
						((CObjectiveTrigger*)(m_poCursor))->m_fCountdownTime = 0.0f;
						((CObjectiveTrigger*)(m_poCursor))->m_unNumDudes = m_vunAttributes[3];
					}

					// set which number objective this is for the level
					((CObjectiveTrigger*)(m_poCursor))->m_unOrderNumber = (int)m_vunAttributes[5];

					((CObjectiveTrigger*)(m_poCursor))->m_unNumBarriers = m_vunAttributes[6];

					// decide if this is the final objective
					if (m_vunAttributes[7] > 0)
						((CObjectiveTrigger*)(m_poCursor))->m_bGoal = true;
					else
						((CObjectiveTrigger*)(m_poCursor))->m_bGoal = false;

					// set the tooltip string
					((CObjectiveTrigger*)(m_poCursor))->m_strToolTip = m_strToolTip;
					break;
				}

				// create the new trigger in the objectmanager
				CObjectiveTrigger* poObjective = (CObjectiveTrigger*)poObjectManager->createObject(m_eCurrentType);
				poObjective->setPosition(m_poCursor->getPosition());
				
				// if the trigger has any value for depth make it a box
				if (m_vunAttributes[2] > 0)
					poObjective->setActorMatrix(D3DXVECTOR3(0.0f, 0.0f, 1.0f), poObjective->getPosition(), D3DXVECTOR3((float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[2]));
				// else leave it as a circle/sphere
				else
					poObjective->setActorMatrix(D3DXVECTOR3(0.0f, 0.0f, 1.0f), poObjective->getPosition(), D3DXVECTOR3((float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[1]));

				// make sure physics doesn't affect this
				poObjective->deactivate();
				// set size variables
				poObjective->m_fRadius = (float)m_vunAttributes[1];
				poObjective->m_fDepth = (float)m_vunAttributes[2];

				// default the type to Goto objective
				poObjective->m_eObjectiveType = CObjectiveTrigger::OJT_GOTO;

				// if there is a time for this objective
				if (m_vunAttributes[4] > 0)
				{
					// reset the type and set the countdown time
					poObjective->m_eObjectiveType = CObjectiveTrigger::OJT_TIMED;
					poObjective->m_fCountdownTime = (float)m_vunAttributes[4];
				}
				// if there is a number of dudes that need to be killed
				if (m_vunAttributes[3] > 0)
				{
					// reset the type and countdown time and then set the number of dudes
					poObjective->m_eObjectiveType = CObjectiveTrigger::OJT_DUDES;
					poObjective->m_fCountdownTime = 0.0f;
					poObjective->m_unNumDudes = m_vunAttributes[3];
				}

				// set which number objective this is for the level
				poObjective->m_unOrderNumber = (int)m_vunAttributes[5];

				poObjective->m_unNumBarriers = m_vunAttributes[6];

				// decide if this is the final objective
				if (m_vunAttributes[7] > 0)
					poObjective->m_bGoal = true;
				else
					poObjective->m_bGoal = false;

				// set the tooltip string
				poObjective->m_strToolTip = m_strToolTip;

				m_unCurrentObjectiveNumber++;
				break;
			}
		case OBJ_AI_NODE:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					((CAINode*)(m_poCursor))->setRadius((float)m_vunAttributes[1]);
					break;
				}

				// create the new node the objectmanager
				CActor* poObject = (CActor*)poObjectManager->createObject(m_eCurrentType);
				poObject->setPosition(m_poCursor->getPosition());
				((CAINode*)poObject)->setRadius((float)m_vunAttributes[1]);
				// link it up with the rest of the mesh
				CAIManager::getInstance().linkNode((CAINode*)poObject);
				break;
			}
		case OBJ_AI_SPAWNTRIGGER:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					((CActorSpawnTrigger*)m_poCursor)->m_fRadius = (float)m_vunAttributes[1];
					((CActorSpawnTrigger*)m_poCursor)->m_fDepth = (float)m_vunAttributes[2];
					((CActorSpawnTrigger*)m_poCursor)->m_fWaitTime = (float)m_vunAttributes[4];
					((CActorSpawnTrigger*)m_poCursor)->m_unNumSpawns = m_vunAttributes[3];
					((CActorSpawnTrigger*)m_poCursor)->m_nMaxTimes = (int)m_vunAttributes[5];
					CAIManager::getInstance().initSpawnTriggers();
					break;
				}

				CActor* poObject = (CActor*)poObjectManager->createObject(m_eCurrentType);
				poObject->setPosition(m_poCursor->getPosition());

				// if the trigger has any value for depth make it a box
				if (m_vunAttributes[2] > 0)
					poObject->setActorMatrix(D3DXVECTOR3(0.0f, 0.0f, 1.0f), poObject->getPosition(), D3DXVECTOR3((float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[2]));
				else
					poObject->setActorMatrix(D3DXVECTOR3(0.0f, 0.0f, 1.0f), poObject->getPosition(), D3DXVECTOR3((float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[1]));

				poObject->deactivate();
				// set all the corresponding values
				((CActorSpawnTrigger*)poObject)->m_fRadius = (float)m_vunAttributes[1];
				((CActorSpawnTrigger*)poObject)->m_fDepth = (float)m_vunAttributes[2];
				((CActorSpawnTrigger*)poObject)->m_fWaitTime = (float)m_vunAttributes[4];
				((CActorSpawnTrigger*)poObject)->m_unNumSpawns = m_vunAttributes[3];
				((CActorSpawnTrigger*)poObject)->m_nMaxTimes = (int)m_vunAttributes[5];
				CAIManager::getInstance().initSpawnTriggers();
				break;
			}
		case OBJ_ENEMY_ZOMBIECITIZEN:
		case OBJ_ENEMY_GASEOUSCLAY:
		case OBJ_ENEMY_ICECREAMMAN:
		case OBJ_ENEMY_QUARTERBACK:
			{
				// just update the variables, don't make a new one
				if (m_bEditing)
				{
					((CActorSpawn*)(m_poCursor))->setNumActors(m_vunAttributes[1]);
					((CActorSpawn*)(m_poCursor))->setSpawnType(m_eCurrentType);
					break;
				}

				// create the spawn in the object manager
				CActorSpawn* poSpawn = (CActorSpawn*)poObjectManager->createObject(OBJ_SPAWN);
				// set all the coresponding values
				poSpawn->setPosition(m_poCursor->getPosition());
				poSpawn->setNumActors(m_vunAttributes[1]);
				poSpawn->setSpawnType(m_eCurrentType);
				break;
			}
		case OBJ_ITEM_HEALTH:
			break;
		case OBJ_ITEM_SLICKSHOES:
			break;
		case OBJ_ITEM_MONEY:
			{
				CZombie oPawn;
				oPawn.setPosition(m_poCursor->getPosition());
				oPawn.spawnCoins(m_vunAttributes[1], 0.0f, 0.0f);
				break;
			}
		}

		if (m_bEditing)
		{
			m_bEditing = false;
			m_poCursor->setMesh(NULL);
			m_poCursor = m_poStoredCursor;
			initAttributes();
		}
	}

	// remove
	if (pInput->isKeyDownBuffered(DIK_BACKSPACE, PLACEMENT_SYSTEM))
	{
		CObjectManager::ObjectList loDelete;
		poObjectManager->getObjects(m_eCurrentType, &loDelete);
		CObjectManager::ObjectList::iterator oDeleteIter = loDelete.end();
		oDeleteIter--;

		if (!loDelete.empty())
		{
			switch (m_eCurrentType)
			{
			case OBJ_OBJECTIVE_BARRIER:
				{
					poObjectManager->removeObjectImmediate(!m_bEditing ? *oDeleteIter : m_poCursor);
					break;
				}
			case OBJ_OBJECTIVE_TRIGGER:
				m_unCurrentObjectiveNumber--;
				poObjectManager->removeObjectImmediate(!m_bEditing ? *oDeleteIter : m_poCursor);
				break;
			case OBJ_AI_NODE:
				CAIManager::getInstancePtr()->unlinkNode(((CAINode*)(*oDeleteIter)));
				poObjectManager->removeObjectImmediate(!m_bEditing ? *oDeleteIter : m_poCursor);
				break;
			case OBJ_AI_SPAWNTRIGGER:
				{
					poObjectManager->removeObjectImmediate(!m_bEditing ? *oDeleteIter : m_poCursor);
					break;
				}
			case OBJ_ENEMY_ZOMBIECITIZEN:
			case OBJ_ENEMY_GASEOUSCLAY:
			case OBJ_ENEMY_ICECREAMMAN:
			case OBJ_ENEMY_QUARTERBACK:
			case OBJ_ENEMY_CROSSINGGUARD:
			case OBJ_ENEMY_GRAVEKEEPER:
				{
					CObjectManager::ObjectList loDeleteSpawn;
					poObjectManager->getObjects(OBJ_SPAWN, &loDeleteSpawn);
					CObjectManager::ObjectList::iterator oDeleteSpawnIter = loDeleteSpawn.end();
					oDeleteSpawnIter--;

					if (!loDeleteSpawn.empty())
					{
						// remove the spawn
						poObjectManager->removeObjectImmediate(!m_bEditing ? *oDeleteSpawnIter : m_poCursor);
					}
					break;
				}
			}
		}

		if (m_bEditing)
		{
			m_bEditing = false;
			m_poCursor->setMesh(NULL);
			m_poCursor = m_poStoredCursor;
			initAttributes();
		}
	}

}

/**
* CPlacementState::preRender
* @date Modified April 29, 2006
*/
void CPlacementState::preRender(CRenderDevice& oRenderDevice)
{
	LPDIRECT3DDEVICE9 pDev = oRenderDevice.getD3DDevice();
	CDirectInputWrapper* pInput = CDirectInputWrapper::getInstancePtr();

	// Sync view matrix with camera class
	D3DXMATRIX matWorld, matView, matProj, matWVP;
	static D3DXVECTOR2 vMousePos = pInput->getMousePosition();
	D3DXVECTOR2 vMouseMove = pInput->getMousePosition(),
		vMouseDelta;
	CActor* pActor = (CActor*)m_poCursor;
	D3DXVECTOR3 vPos = pActor->getPosition();
	CFreeCamera* pFreeCam = CFreeCamera::getInstancePtr();
	CCamera* pCam = CCamera::getInstancePtr();
	if(!CCommandConsole::getInstance().isVisible())
	{
		static bool bFreeRoam = false;
		if(bFreeRoam)
		{
			D3DXVec2Subtract(&vMouseDelta, &vMouseMove, &vMousePos);
			vMousePos = vMouseMove;
			if(pInput->isKeyDown(DIK_W))
				pFreeCam->walk(1.0f);
			if(pInput->isKeyDown(DIK_S))
				pFreeCam->walk(-1.0f);
			if(pInput->isKeyDown(DIK_D))
				pFreeCam->strafe(1.0f);
			if(pInput->isKeyDown(DIK_A))
				pFreeCam->strafe(-1.0f);
			pFreeCam->yaw(vMouseDelta.x*0.01f);
			pFreeCam->pitch(vMouseDelta.y*0.01f);
			pFreeCam->getViewMatrix(matView);
		}
		else
		{
			D3DXVECTOR3 vAt, vUp(0, 1, 0), vRight;
			D3DXVec3Subtract(&vAt, &vPos, &pCam->getCameraPos());
			vAt.y = 0;
			D3DXVec3Normalize(NULL, &vAt, &vAt);
			D3DXVec3Cross(&vRight, &vAt, &vUp);
			D3DXVec3Normalize(NULL, &vRight, &vRight);
			if(pInput->isKeyDown(DIK_W))
				D3DXVec3Add(&vPos, &vPos, &vAt);
			if(pInput->isKeyDown(DIK_S))
				D3DXVec3Subtract(&vPos, &vPos, &vAt);
			if(pInput->isKeyDown(DIK_A))
				D3DXVec3Add(&vPos, &vPos, &vRight);
			if(pInput->isKeyDown(DIK_D))
				D3DXVec3Subtract(&vPos, &vPos, &vRight);

			pCam->updateCameraSP(vPos);

			if(pInput->isKeyDown(DIK_Q))
				pCam->rotateCamera(0.05f);
			if(pInput->isKeyDown(DIK_E))
				pCam->rotateCamera(-0.05f);

			if (pInput->isKeyDownBuffered(DIK_EQUALS))
			{
				CCamera* pCam = CCamera::getInstancePtr();
				pCam->moveCamera(-10.0f);
			}

			if (pInput->isKeyDownBuffered(DIK_MINUS))
			{
				CCamera* pCam = CCamera::getInstancePtr();
				pCam->moveCamera(10.0f);
			}

			pActor->setPosition(vPos);
			pActor->setBV(vPos, pActor->getBV().fRadius);
			pCam->getViewMatrix(&matView);
		}

		if(pInput->isKeyDownBuffered(DIK_F, PLACEMENT_SYSTEM))
			bFreeRoam = !bFreeRoam;
	}

	if(pInput->isKeyDownBuffered(DIK_GRAVE, PLACEMENT_SYSTEM))
		CCommandConsole::getInstance().toggleConsole();


	pCam->getViewMatrix(&matView);
	pDev->SetTransform(D3DTS_VIEW, &matView);

	// TODO: Base position off object
	D3DXMatrixIdentity(&matWorld);
	pDev->SetTransform(D3DTS_WORLD, &matWorld);
	pDev->GetTransform(D3DTS_PROJECTION, &matProj);

	// Get WorldViewProjection Matrix
	D3DXMatrixMultiply(&matWVP, D3DXMatrixMultiply(&matWVP, &matWorld, &matView), &matProj);

	// Temp Light
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	D3DXCOLOR white = D3DCOLOR_XRGB(255, 255, 255);	
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = white;
	light.Specular = white * 0.3f;
	light.Ambient = white * 0.6f;
	light.Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	//pDev->SetLight(0, &light);
	//pDev->LightEnable(0, true);
	//pDev->SetRenderState(D3DRS_LIGHTING, true);

	D3DMATERIAL9 mtrl;
	mtrl.Ambient = white;
	mtrl.Diffuse = white;
	mtrl.Specular = white;
	mtrl.Emissive = white;
	mtrl.Power = 5.0f;

	D3DXMatrixIdentity(&matWorld);
	pDev->SetTransform(D3DTS_WORLD, &matWorld);
	pDev->SetMaterial(&mtrl);

	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	CTestState::getInstance().m_pMesh2->draw();

	if (m_bCollisionGeometry)
		if(CTestState::getInstance().m_pMesh3) CTestState::getInstance().m_pMesh3->draw();

	// resize cusor if it is
	if (m_eCurrentType == OBJ_AI_SPAWNTRIGGER || m_eCurrentType == OBJ_OBJECTIVE_TRIGGER)
	{
		if (m_vunAttributes[2] > 0)
		{
			m_poCursor->setMesh(NULL);
		}
		else
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "ring.x"));
			m_poCursor->setActorMatrix(D3DXVECTOR3(0.0f, 0.0f, 1.0f), m_poCursor->getPosition(), D3DXVECTOR3((float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[1]));
		}

        if (m_eCurrentType == OBJ_OBJECTIVE_TRIGGER)
		{
			m_vunAttributeNames.back() = m_strToolTip.GetBuffer();
		}
	}

	// resize cusor if it is
	if (m_eCurrentType == OBJ_OBJECTIVE_BARRIER)
	{
		m_poCursor->setScale(D3DXVECTOR3(0.25f, 0.25f, 0.25f));
		m_poCursor->setOrientation(D3DXVECTOR3(0.0f, 0.0f, 1.0f));

		m_poCursor->rotateActorY(m_vunAttributes[2] * (D3DX_PI / 180.0f));
	}
}

/**
* CPlacementState::postRender
* @date Modified April 27, 2006
*/
void CPlacementState::postRender(CRenderDevice& oRenderDevice)
{
	LPDIRECT3DDEVICE9 pDev = oRenderDevice.getD3DDevice();

	D3DXMATRIX matWorld, scale;
	if (m_eCurrentType == OBJ_AI_NODE)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranslation(&matWorld, ((CActor*)m_poCursor)->getPosition().x, ((CActor*)m_poCursor)->getPosition().y, ((CActor*)m_poCursor)->getPosition().z);
		D3DXMatrixScaling(&scale, (float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[1]);
		matWorld = scale * matWorld;
		pDev->SetTransform(D3DTS_WORLD, &matWorld);
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pSphere->DrawSubset(0);
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	// resize cusor if it is
	if (m_eCurrentType == OBJ_AI_SPAWNTRIGGER)
	{
		if (m_vunAttributes[2] > 0)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, ((CActor*)m_poCursor)->getPosition().x, ((CActor*)m_poCursor)->getPosition().y, ((CActor*)m_poCursor)->getPosition().z);
			D3DXMatrixScaling(&scale, (float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[2]);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pBox->DrawSubset(0);
			pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, ((CActor*)m_poCursor)->getPosition().x, ((CActor*)m_poCursor)->getPosition().y, ((CActor*)m_poCursor)->getPosition().z);
			D3DXMatrixScaling(&scale, (float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[1]);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pSphere->DrawSubset(0);
			pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

	// resize cusor if it is
	if (m_eCurrentType == OBJ_OBJECTIVE_TRIGGER)
	{
		if (m_vunAttributes[2] > 0)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, ((CActor*)m_poCursor)->getPosition().x, ((CActor*)m_poCursor)->getPosition().y, ((CActor*)m_poCursor)->getPosition().z);
			D3DXMatrixScaling(&scale, (float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[2]);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pBox->DrawSubset(0);
		}
		else
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, ((CActor*)m_poCursor)->getPosition().x, ((CActor*)m_poCursor)->getPosition().y, ((CActor*)m_poCursor)->getPosition().z);
			D3DXMatrixScaling(&scale, (float)m_vunAttributes[1], 1.0f, (float)m_vunAttributes[1]);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pSphere->DrawSubset(0);
		}
	}

	// resize cusor if it is
	if (m_eCurrentType == OBJ_OBJECTIVE_BARRIER)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranslation(&matWorld, ((CActor*)m_poCursor)->getPosition().x, ((CActor*)m_poCursor)->getPosition().y, ((CActor*)m_poCursor)->getPosition().z);
		D3DXMatrixScaling(&scale, (float)m_vunAttributes[1], (float)m_vunAttributes[1], (float)m_vunAttributes[1]);
		matWorld = scale * matWorld;
		pDev->SetTransform(D3DTS_WORLD, &matWorld);
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pSphere->DrawSubset(0);
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	if (m_bDrawNodesSpawnTriggers)
	{
		// NODES Radiuseses
		D3DXCOLOR blue = D3DCOLOR_XRGB(0, 0, 255);
		CObjectManager::ObjectList loAINodes;
		CObjectManager::getInstancePtr()->getObjects(OBJ_AI_NODE, &loAINodes);
		CObjectManager::ObjectList::iterator oNodeIter = loAINodes.begin();
		D3DXVECTOR3 pos;
#if 1
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		while (oNodeIter != loAINodes.end())
		{
			pos = ((CAINode*)(*oNodeIter))->getPosition();
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
			D3DXMatrixScaling(&scale, ((CAINode*)(*oNodeIter))->m_fRadius, 1.0f, ((CAINode*)(*oNodeIter))->m_fRadius);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			m_pSphere->DrawSubset(0);

			oNodeIter++;
		}

#endif

		// DRAW SPAWNTRIGGERS
		CObjectManager::ObjectList loSpawnTriggers;
		CObjectManager::getInstancePtr()->getObjects(OBJ_AI_SPAWNTRIGGER, &loSpawnTriggers);
		CObjectManager::ObjectList::iterator oTriggerIter = loSpawnTriggers.begin();
#if 1
		D3DMATERIAL9 blueMaterial;
		memset(&blueMaterial, 0, sizeof(D3DMATERIAL9));
		pDev->SetMaterial(&blueMaterial);
		while (oTriggerIter != loSpawnTriggers.end())
		{
			pos = ((CActorSpawnTrigger*)(*oTriggerIter))->getPosition();
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
			D3DXMatrixScaling(&scale, ((CActorSpawnTrigger*)(*oTriggerIter))->getScale().x, 1.0f, ((CActorSpawnTrigger*)(*oTriggerIter))->getScale().z);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			if (((CActorSpawnTrigger*)(*oTriggerIter))->m_fDepth > 0.0f)
				m_pBox->DrawSubset(0);
			else
				m_pSphere->DrawSubset(0);

			oTriggerIter++;
		}
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

		CObjectManager::ObjectList loSpawns;
		CObjectManager::getInstancePtr()->getObjects(OBJ_SPAWN, &loSpawns);
		CObjectManager::ObjectList::iterator oSpawnIter = loSpawns.begin();
#if 1
		while (oSpawnIter != loSpawns.end())
		{
			pos = ((CActorSpawn*)(*oSpawnIter))->getPosition();
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
			D3DXMatrixScaling(&scale, 2.0f, 2.0f, 2.0f);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			m_pSphere->DrawSubset(0);

			oSpawnIter++;
		}
#endif

#if 1
		// NODES LINKS
		oNodeIter = loAINodes.begin();
		std::list<CAINode*>::iterator oSecondNodeIter;
		D3DXVECTOR3 vPoints[2];

		D3DXMatrixIdentity(&matWorld);
		oRenderDevice.getD3DDevice()->SetTransform(D3DTS_WORLD, &matWorld);

		while (oNodeIter != loAINodes.end())
		{
			vPoints[0] = ((CAINode*)(*oNodeIter))->getPosition();

			oSecondNodeIter = ((CAINode*)(*oNodeIter))->m_loLinks.begin();
			while (oSecondNodeIter != ((CAINode*)(*oNodeIter))->m_loLinks.end())
			{
				vPoints[1] = ((CAINode*)(*oSecondNodeIter))->getPosition();

				pDev->SetFVF(D3DFVF_XYZ);
				pDev->DrawPrimitiveUP(D3DPT_LINELIST, 1, vPoints, sizeof(D3DXVECTOR3));
				oSecondNodeIter++;
			}
			oNodeIter++;
		}
#endif
	}

	if (m_bDrawObjectiveTriggersBarriers)
	{
		// DRAW OBJECTIVETRIGGERS
		CObjectManager::ObjectList loObjectiveTriggers;
		CObjectManager::getInstancePtr()->getObjects(OBJ_OBJECTIVE_TRIGGER, &loObjectiveTriggers);
		CObjectManager::ObjectList::iterator oTriggerIter = loObjectiveTriggers.begin();
#if 1
		D3DXVECTOR3 pos;
		while (oTriggerIter != loObjectiveTriggers.end())
		{
			pos = ((CObjectiveTrigger*)(*oTriggerIter))->getPosition();
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
			D3DXMatrixScaling(&scale, ((CObjectiveTrigger*)(*oTriggerIter))->getScale().x, 1.0f, ((CActorSpawnTrigger*)(*oTriggerIter))->getScale().z);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			if (((CObjectiveTrigger*)(*oTriggerIter))->m_fDepth > 0.0f)
				m_pBox->DrawSubset(0);
			else
				m_pSphere->DrawSubset(0);

			oTriggerIter++;
		}
#endif

		// draw the barriers
		CObjectManager::ObjectList loBarriers;
		CObjectManager::getInstancePtr()->getObjects(OBJ_OBJECTIVE_BARRIER, &loBarriers);
		CObjectManager::ObjectList::iterator oBarrierIter = loBarriers.begin();
#if 1
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		while (oBarrierIter != loBarriers.end())
		{
			pos = ((CActor*)(*oBarrierIter))->getBV().centerPt;
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
			float fScale = ((CActor*)(*oBarrierIter))->getBV().fRadius;
			D3DXMatrixScaling(&scale, fScale, fScale, fScale);
			matWorld = scale * matWorld;
			pDev->SetTransform(D3DTS_WORLD, &matWorld);
			m_pSphere->DrawSubset(0);

			oBarrierIter++;
		}
		pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
	}

	CString sOutput;

	sOutput.Format("%s", m_vunAttributeNames[0].GetBuffer());
	m_fntDina->drawString(sOutput, 3.0f, 300.0f);

	for (unsigned int i = 1; i < m_vunAttributes.size(); ++i)
	{
		if (i == m_unIndex)
		{
			sOutput.Format("{0xFF111111}%s: %i", m_vunAttributeNames[i].GetBuffer(), m_vunAttributes[i]);
			m_fntDina->drawStringMarkup(sOutput, 3.0f, 300.0f + (i * m_fntDina->getLineHeight()));
		}
		else
		{
			sOutput.Format("%s: %i", m_vunAttributeNames[i].GetBuffer(), m_vunAttributes[i]);
			m_fntDina->drawString(sOutput, 3.0f, 300.0f + (i * m_fntDina->getLineHeight()));
		}
	}

	// Display Debug Information
	CString sFPS;
	sFPS.Format("%s\nKernel ticks per second: %.2f\nFrames rendered per second: %.2f", APP_VERSION,
		CTimer::getInstance().getFPS(), CGameKernel::getInstance().getRenderTimer().getTicksPerSecond());
	m_fntDinaFPS->drawString(sFPS, 3.0f, 3.0f, 0x80000000);

	sFPS.Format("%s\nKernel ticks per second: {%#x}%.2f\n{%#x}Frames rendered per second: {%#x}%.2f", APP_VERSION,
		CRenderSystem::getInstance().calcGoalColor(CTimer::getInstance().getFPS(), 60.0f), CTimer::getInstance().getFPS(),
		D3DCOLOR_XRGB(255, 255, 255), CRenderSystem::getInstance().calcGoalColor(CGameKernel::getInstance().getRenderTimer().getTicksPerSecond(), (float)oRenderDevice.getRefreshRate()), CGameKernel::getInstance().getRenderTimer().getTicksPerSecond());
	m_fntDinaFPS->drawStringMarkup(sFPS.GetBuffer(), 2.0f, 2.0f);
}

/**
* CPlacementState::exportNodes
* @date Modified May 26, 2006
*/
void CPlacementState::exportNodes(void)
{
	ofstream ofl;
	ofl.open("Nodes.creation.bin", std::ios_base::out | std::ios_base::binary);

	if (!ofl.is_open())
		return;

	// get all the actor information
	CObjectManager::ObjectList loExport;
	CObjectManager::getInstancePtr()->getObjects(OBJ_AI_NODE, &loExport);
	CObjectManager::ObjectList::iterator oExportIter = loExport.begin();
	// set up variables that are needed
	EObjectType eType = OBJ_AI_NODE;
	CAINode* poNode = NULL;
	D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
	float fRadius = 0.0f;

	// write out the type of actor in this file
	ofl.write((const char *)&eType, sizeof(EObjectType));

	// now get all of the actors into the file
	while (oExportIter != loExport.end())
	{
		// get easy access to the node
		poNode = NODE_PTR(oExportIter);
		// set the variables with this node's data
		vPos = poNode->getPosition();
		fRadius = poNode->m_fRadius;

		// output the values in the correct order
		ofl.write((const char *)&vPos, sizeof(D3DXVECTOR3));
		ofl.write((const char *)&fRadius, sizeof(float));

		oExportIter++;
	}

	ofl.close();
}

/**
* CPlacementState::exportEnemies
* @date Modified May 26, 2006
*/
void CPlacementState::exportEnemies(void)
{
	// this function is depricated
	return;
}

/**
* CPlacementState::exportEnemySpawns
* @date Modified May 26, 2006
*/
void CPlacementState::exportEnemySpawns(void)
{
	ofstream ofl;
	ofl.open("Spawns.creation.bin", std::ios_base::out | std::ios_base::binary);

	if (!ofl.is_open())
		return;

	// get all the actor information
	CObjectManager::ObjectList loExport;
	CObjectManager::getInstancePtr()->getObjects(OBJ_SPAWN, &loExport);
	CObjectManager::ObjectList::iterator oExportIter = loExport.begin();
	// set up variables that are needed
	EObjectType eType = OBJ_SPAWN;
	CActorSpawn* poSpawn = NULL;
	D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
	unsigned int unNum = 0;
	EObjectType eSpawnType = OBJ_ENEMY_ZOMBIECITIZEN;


	// write out the type of actor in this file
	ofl.write((const char *)&eType, sizeof(EObjectType));

	// now get all of the actors into the file
	while (oExportIter != loExport.end())
	{
		// get easy access to the spawn
		poSpawn = SPAWN_PTR(oExportIter);
		// set the variables with the spawn's data
		vPos = poSpawn->getPosition();
		unNum = poSpawn->getNumActors();
		eSpawnType = poSpawn->getSpawnType();

		// output the values in the correct order
		ofl.write((const char*)&vPos, sizeof(D3DXVECTOR3));
		ofl.write((const char*)&unNum, sizeof(unsigned int));
		ofl.write((const char*)&eSpawnType, sizeof(EObjectType));

		oExportIter++;
	}

	ofl.close();
}

/**
* CPlacementState::exportSpawnTriggers
* @date Modified May 26, 2006
*/
void CPlacementState::exportSpawnTriggers(void)
{
	ofstream ofl;
	ofl.open("SpawnTriggers.creation.bin", std::ios_base::out | std::ios_base::binary);

	if (!ofl.is_open())
		return;

	// get all the actor information
	CObjectManager::ObjectList loExport;
	CObjectManager::getInstancePtr()->getObjects(OBJ_AI_SPAWNTRIGGER, &loExport);
	CObjectManager::ObjectList::iterator oExportIter = loExport.begin();
	// set up variables that are needed
	EObjectType eType = OBJ_AI_SPAWNTRIGGER;
	CActorSpawnTrigger* poTrigger = NULL;
	D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
	float fRadius = 0.0f;
	float fDepth = 0.0f;
	float fWaitTime = 0.0f;
	unsigned int unNum = 0;
	int nMaxTimes = 0;

	// write out the type of actor in this file
	ofl.write((const char *)&eType, sizeof(EObjectType));

	// now get all of the actors into the file
	while (oExportIter != loExport.end())
	{
		// get easy access to the trigger
		poTrigger = TRIGGER_PTR(oExportIter);
		// set the variables with the trigger's data
		vPos = poTrigger->getPosition();
		fRadius = poTrigger->m_fRadius;
		fDepth = poTrigger->m_fDepth;
		fWaitTime = poTrigger->m_fWaitTime;
		unNum = poTrigger->m_unNumSpawns;
		nMaxTimes = poTrigger->m_nMaxTimes;

		// output the values in the correct order
		ofl.write((const char*)&vPos, sizeof(D3DXVECTOR3));
		ofl.write((const char*)&fRadius, sizeof(float));
		ofl.write((const char*)&fDepth, sizeof(float));
		ofl.write((const char*)&fWaitTime, sizeof(float));
		ofl.write((const char*)&unNum, sizeof(unsigned int));
		ofl.write((const char*)&nMaxTimes, sizeof(int));

		oExportIter++;
	}

	ofl.close();
}

/**
* CPlacementState::exportObjectiveTriggers
* @date Modified June 2, 2006
*/
void CPlacementState::exportObjectiveTriggers(void)
{
	ofstream ofl;
	ofl.open("ObjectiveTriggers.creation.bin", std::ios_base::out | std::ios_base::binary);

	if (!ofl.is_open())
		return;

	// get all the actor information
	CObjectManager::ObjectList loExport;
	CObjectManager::getInstancePtr()->getObjects(OBJ_OBJECTIVE_TRIGGER, &loExport);
	CObjectManager::ObjectList::iterator oExportIter = loExport.begin();
	// set up the variables that are needed
	EObjectType eType = OBJ_OBJECTIVE_TRIGGER;
	CObjectiveTrigger* poObjectiveTrigger = NULL;
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
	CString strToolTip;

	// write of the type of actor in this file
	ofl.write((const char*)&eType, sizeof(EObjectType));

	// now get all the actors into the file
	while (oExportIter != loExport.end())
	{
		// get easy access to the objective trigger
		poObjectiveTrigger = OBJECTIVE_PTR(oExportIter);
		// set the variables with the objective's data
		vPos = poObjectiveTrigger->getPosition();
		eObjectiveType = poObjectiveTrigger->m_eObjectiveType;
		fCountdownTime = poObjectiveTrigger->m_fCountdownTime;
		unNumDudes = poObjectiveTrigger->m_unNumDudes;
		fRadius = poObjectiveTrigger->m_fRadius;
		fDepth = poObjectiveTrigger->m_fDepth;
		bGoal = poObjectiveTrigger->m_bGoal;
		unOrderNumber = poObjectiveTrigger->m_unOrderNumber;
		unNumBarriers = poObjectiveTrigger->m_unNumBarriers;
		unStringSize = poObjectiveTrigger->m_strToolTip.GetLength();
		strToolTip = poObjectiveTrigger->m_strToolTip;

		// output the values in the correct order
		ofl.write((const char*)&vPos, sizeof(D3DXVECTOR3));
		ofl.write((const char *)&eObjectiveType, sizeof(CObjectiveTrigger::EObjectiveTypes));
		ofl.write((const char *)&fCountdownTime, sizeof(float));
		ofl.write((const char *)&unNumDudes, sizeof(unsigned int));
		ofl.write((const char *)&fRadius, sizeof(float));
		ofl.write((const char *)&fDepth, sizeof(float));
		ofl.write((const char *)&bGoal, sizeof(bool));
		ofl.write((const char *)&unOrderNumber, sizeof(unsigned int));
		ofl.write((const char *)&unNumBarriers, sizeof(unsigned int));
		ofl.write((const char *)&unStringSize, sizeof(unsigned int));
		ofl.write((const char *)strToolTip.GetBuffer(), unStringSize);

		oExportIter++;
	}	
}

/**
* CPlacementState::exportObjectiveBarriers
* @date Modified June 3, 2006
*/
void CPlacementState::exportObjectiveBarriers(void)
{
	ofstream ofl;
	ofl.open("ObjectiveBarriers.creation.bin", std::ios_base::out | std::ios_base::binary);

	if (!ofl.is_open())
		return;

	// get all the actor information
	CObjectManager::ObjectList loExport;
	CObjectManager::getInstancePtr()->getObjects(OBJ_OBJECTIVE_BARRIER, &loExport);
	CObjectManager::ObjectList::iterator oExportIter = loExport.begin();
	// set up the variables that are needed
	EObjectType eType = OBJ_OBJECTIVE_BARRIER;
	CActor* poBarrier = NULL;
	D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
	D3DXVECTOR3 vOrientation(0.0f, 0.0f, 1.0f);
	float fRadius = 0.0f;

	// write of the type of actor in this file
	ofl.write((const char*)&eType, sizeof(EObjectType));

	// now get all the actors into the file
	while (oExportIter != loExport.end())
	{
		// get easy access to the objective barrier
		poBarrier = ACTOR_PTR(oExportIter);
		// set the variables with the objective's data
		vPos = poBarrier->getBV().centerPt;
		vOrientation = poBarrier->getOrientation();
		fRadius = poBarrier->getBV().fRadius;

		// output the values in the correct order
		ofl.write((const char*)&vPos, sizeof(D3DXVECTOR3));
		ofl.write((const char*)&vOrientation, sizeof(D3DXVECTOR3));
		ofl.write((const char*)&fRadius, sizeof(float));

		oExportIter++;
	}
}

/**
* CPlacementState::exportLights
* @date Modified June 8, 2006
*/
void CPlacementState::exportLights(void)
{
	ofstream ofl;
	ofl.open("ObjectiveBarriers.creation.bin", std::ios_base::out | std::ios_base::binary);

	if (!ofl.is_open())
		return;

	// get all the actor information
	CObjectManager::ObjectList loExport;
	CObjectManager::getInstancePtr()->getObjects(OBJ_LIGHT, &loExport);
	CObjectManager::ObjectList::iterator oExportIter = loExport.begin();
	// set up the variables that are needed
	EObjectType eType = OBJ_LIGHT;
	EObjectType eSubType = OBJ_LIGHT_SPOT;
	CLight* poLight = NULL;
	D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
	float fRadius = 0.0f;
	D3DCOLOR color = 0;

	// write of the type of actor in this file
	ofl.write((const char*)&eType, sizeof(EObjectType));

	// now get all the actors into the file
	while (oExportIter != loExport.end())
	{
		// get easy access to the objective barrier
		poLight = LIGHT_PTR(oExportIter);
		// set the variables with the objective's data
		vPos = poLight->getPosition();
		eSubType = poLight->getType();

		if (eSubType == OBJ_LIGHT_POINT)
		{
			fRadius = ((CPointLight*)poLight)->getRadius();

			// output the values in the correct order
			ofl.write((const char*)&vPos, sizeof(D3DXVECTOR3));
			ofl.write((const char*)&eSubType, sizeof(EObjectType));
			ofl.write((const char*)&fRadius, sizeof(float));
		}
		else /*eSubType == OBJ_LIGHT_SPOT*/
		{
			color = ((CSpotLight*)poLight)->getColor();

			// output the values in the correct order
			ofl.write((const char*)&vPos, sizeof(D3DXVECTOR3));
			ofl.write((const char*)&eSubType, sizeof(EObjectType));
			ofl.write((const char*)&color, sizeof(D3DCOLOR));
		}

		oExportIter++;
	}
}

/**
* CPlacementState::updateAttributes
* @date Modified June 2, 2006
*/
void CPlacementState::initAttributes(void)
{
	m_bEditing = false;
	m_vunAttributes.clear();
	m_vunAttributeNames.clear();

	m_unIndex = 1;

	CObjectManager::ObjectList loPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
	D3DXVECTOR3 vPos = m_poCursor->getPosition();
	vPos.y = ((CPlayer*)loPlayers.front())->getBV().centerPt.y;
	m_poCursor->setPosition(vPos);
	m_poCursor->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_poCursor->setOrientation(D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	// always put an extra in to offset for the name of the actor
	m_vunAttributes.push_back(1);

	switch (m_eCurrentType)
	{
	case OBJ_LIGHT_POINT:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("Point Light"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Radius"));
			break;
		}
	case OBJ_LIGHT_SPOT:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("Spot Light"));
			m_vunAttributes.push_back(255);
			m_vunAttributeNames.push_back(CString("Red"));
			m_vunAttributes.push_back(255);
			m_vunAttributeNames.push_back(CString("Blue"));
			m_vunAttributes.push_back(255);
			m_vunAttributeNames.push_back(CString("Green"));
			break;
		}
	case OBJ_OBJECTIVE_BARRIER:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "roadBlockSign.x"));

			m_vunAttributeNames.push_back(CString("Barrier"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Radius"));
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back(CString("Rotation"));
			break;
		}
	case OBJ_OBJECTIVE_TRIGGER:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "ring.x"));

			m_vunAttributeNames.push_back(CString("Objective"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Radius"));
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back(CString("Depth"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Num Dudes"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back("CountDown Time");
			m_vunAttributes.push_back(m_unCurrentObjectiveNumber);
			m_vunAttributeNames.push_back("Order Num");
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back("Num Barriers");
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back("Goal (0 or 1)");
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back(m_strToolTip.GetBuffer());
			break;
		}
	case OBJ_AI_NODE:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("AI Node"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Radius"));
			break;
		}
	case OBJ_AI_SPAWNTRIGGER:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "ring.x"));

			m_vunAttributeNames.push_back(CString("Spawn Trigger"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Radius"));
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back(CString("Depth"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Num Spawns"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back("Wait Time");
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back("Max Times");
			break;
		}
	case OBJ_ENEMY_ZOMBIECITIZEN:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("Zombie Citizen"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Group Size"));

			break;
		}
	case OBJ_ENEMY_GASEOUSCLAY:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));
			m_vunAttributeNames.push_back(CString("Gaseous Clay"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Group Size"));
			break;
		}
	case OBJ_ENEMY_ICECREAMMAN:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));
			m_vunAttributeNames.push_back(CString("Acidic IceCream"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Group Size"));
			break;
		}
	case OBJ_ENEMY_QUARTERBACK:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("QB Zombie"));
			m_vunAttributes.push_back(1);
			m_vunAttributeNames.push_back(CString("Group Size"));
			break;
		}
	case OBJ_ITEM_HEALTH:
		{
			m_vunAttributeNames.push_back(CString("Health"));
			break;
		}
	case OBJ_ITEM_SLICKSHOES:
		{
			m_vunAttributeNames.push_back(CString("Slick Shoes"));
			break;
		}
	case OBJ_ITEM_MONEY:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));
			m_vunAttributeNames.push_back(CString("Money"));
			m_vunAttributes.push_back(100);
			m_vunAttributeNames.push_back(CString("Amount"));
			break;
		}
	}
}

/**
* CPlacementState::checkChangeInType
* @date Modified May 5, 2006
*/
void CPlacementState::checkChangeInType(void)
{
	CDirectInputWrapper* pInput = CDirectInputWrapper::getInstancePtr();

	// actor spawns
	if (pInput->isKeyDownBuffered(DIK_1, PLACEMENT_SYSTEM))
	{
		std::list<EObjectType>::iterator oTypeIter = m_mapOfListsOfTypes[0].begin();

		while (oTypeIter != m_mapOfListsOfTypes[0].end())
		{
			if ((*oTypeIter) == m_eCurrentType)
			{
				oTypeIter++;
				m_eCurrentType = (*oTypeIter);
				break;
			}
			oTypeIter++;
		}

		// went past the end
		if (oTypeIter == m_mapOfListsOfTypes[0].end())
			m_eCurrentType = m_mapOfListsOfTypes[0].front();

		initAttributes();
	}

	// AI stuff
	if (pInput->isKeyDownBuffered(DIK_2, PLACEMENT_SYSTEM))
	{
		std::list<EObjectType>::iterator oTypeIter = m_mapOfListsOfTypes[1].begin();

		while (oTypeIter != m_mapOfListsOfTypes[1].end())
		{
			if ((*oTypeIter) == m_eCurrentType)
			{
				oTypeIter++;
				m_eCurrentType = (*oTypeIter);
				break;
			}
			oTypeIter++;
		}

		// went past the end
		if (oTypeIter == m_mapOfListsOfTypes[1].end())
			m_eCurrentType = m_mapOfListsOfTypes[1].front();

		initAttributes();
	}

	// pickups, money/heath/speed
	if (pInput->isKeyDownBuffered(DIK_3, PLACEMENT_SYSTEM))
	{
		std::list<EObjectType>::iterator oTypeIter = m_mapOfListsOfTypes[2].begin();

		while (oTypeIter != m_mapOfListsOfTypes[2].end())
		{
			if ((*oTypeIter) == m_eCurrentType)
			{
				oTypeIter++;
				m_eCurrentType = (*oTypeIter);
				break;
			}
			oTypeIter++;
		}

		// went past the end
		if (oTypeIter == m_mapOfListsOfTypes[2].end())
			m_eCurrentType = m_mapOfListsOfTypes[2].front();

		initAttributes();
	}

	// lights
	if (pInput->isKeyDownBuffered(DIK_4, PLACEMENT_SYSTEM))
	{
		std::list<EObjectType>::iterator oTypeIter = m_mapOfListsOfTypes[3].begin();

		while (oTypeIter != m_mapOfListsOfTypes[3].end())
		{
			if ((*oTypeIter) == m_eCurrentType)
			{
				oTypeIter++;
				m_eCurrentType = (*oTypeIter);
				break;
			}
			oTypeIter++;
		}

		// went past the end
		if (oTypeIter == m_mapOfListsOfTypes[3].end())
			m_eCurrentType = m_mapOfListsOfTypes[3].front();

		initAttributes();
	}

	// edit mode
	if (pInput->isKeyDownBuffered(DIK_RETURN, CONSOLE_SYSTEM))
	{
		m_bEditing = !m_bEditing;

		if (m_bEditing)
			editClosestCurrentType();
		else
			initAttributes();
	}
}

/**
* CPlacementState::editClosestCurrentType
* @date Modified June 5, 2006
*/
void CPlacementState::editClosestCurrentType(void)
{
	EObjectType eType = m_eCurrentType;
	if ((eType & OBJ_ENEMY) == OBJ_ENEMY)
		eType = OBJ_SPAWN;

	// find the closest of the current type
	CObjectManager::ObjectList loCurrentType;
	CObjectManager::getInstance().getObjects(eType, &loCurrentType);
	CObjectManager::ObjectList::iterator oCurrentTypeIter = loCurrentType.begin();

	if (loCurrentType.empty())
		return;

	oCurrentTypeIter++;
	CActor* poClosest = (CActor*)loCurrentType.front();
	// don't try to edit something that doesn't exist
	if (poClosest == NULL)
		return;

	// find the distance to compare
	float fDist = computeDistanceSquared(poClosest->getPosition(), m_poCursor->getPosition());
	float fTempDist = 0.0f;
	while (oCurrentTypeIter != loCurrentType.end())
	{
		fTempDist = computeDistanceSquared(((CActor*)(*oCurrentTypeIter))->getPosition(), m_poCursor->getPosition());
		if (fTempDist < fDist)
		{
			poClosest = ((CActor*)(*oCurrentTypeIter));
			fDist = fTempDist;
		}
		oCurrentTypeIter++;
	}

	m_poStoredCursor = m_poCursor;
	m_poCursor = poClosest;

	// set up the UI variables
	m_vunAttributes.clear();
	m_vunAttributeNames.clear();

	m_unIndex = 1;

	// always put an extra in to offset for the name of the actor
	m_vunAttributes.push_back(1);

	switch (m_eCurrentType)
	{
	case OBJ_LIGHT_POINT:
		{
			m_vunAttributeNames.push_back(CString("Point Light"));
			m_vunAttributes.push_back((unsigned int)((CPointLight*)m_poCursor)->getRadius());
			m_vunAttributeNames.push_back(CString("Radius"));
			break;
		}
	case OBJ_LIGHT_SPOT:
		{
			D3DCOLOR color = ((CSpotLight*)m_poCursor)->getColor();
			unsigned int a, r, g, b;
			decomposeD3DCOLOR(color, a, r, g, b);
			m_vunAttributeNames.push_back(CString("Spot Light"));
			m_vunAttributes.push_back(r);
			m_vunAttributeNames.push_back(CString("Red"));
			m_vunAttributes.push_back(g);
			m_vunAttributeNames.push_back(CString("Blue"));
			m_vunAttributes.push_back(b);
			m_vunAttributeNames.push_back(CString("Green"));
			break;
		}
	case OBJ_OBJECTIVE_BARRIER:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "roadBlockSign.x"));

			m_vunAttributeNames.push_back(CString("Barrier"));
			m_vunAttributes.push_back((unsigned int)(m_poCursor->getBV().fRadius));
			m_vunAttributeNames.push_back(CString("Radius"));
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back(CString("Rotation"));
			break;
		}
	case OBJ_OBJECTIVE_TRIGGER:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "ring.x"));

			m_vunAttributeNames.push_back(CString("Objective"));
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_fRadius);
			m_vunAttributeNames.push_back(CString("Radius"));
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_fDepth);
			m_vunAttributeNames.push_back(CString("Depth"));
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_unNumDudes);
			m_vunAttributeNames.push_back(CString("Num Dudes"));
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_fCountdownTime);
			m_vunAttributeNames.push_back("CountDown Time");
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_unOrderNumber);
			m_vunAttributeNames.push_back("Order Num");
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_unNumBarriers);
			m_vunAttributeNames.push_back("Num Barriers");
			m_vunAttributes.push_back((unsigned int)((CObjectiveTrigger*)(m_poCursor))->m_bGoal);
			m_vunAttributeNames.push_back("Goal (0 or 1)");
			m_vunAttributes.push_back(0);
			m_vunAttributeNames.push_back(((CObjectiveTrigger*)(m_poCursor))->m_strToolTip);
			m_strToolTip = ((CObjectiveTrigger*)(m_poCursor))->m_strToolTip;
			break;
		}
	case OBJ_AI_NODE:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("AI Node"));
			m_vunAttributes.push_back((unsigned int)((CAINode*)(m_poCursor))->m_fRadius);
			m_vunAttributeNames.push_back(CString("Radius"));
			break;
		}
	case OBJ_AI_SPAWNTRIGGER:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "ring.x"));

			m_vunAttributeNames.push_back(CString("Spawn Trigger"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawnTrigger*)(m_poCursor))->m_fRadius);
			m_vunAttributeNames.push_back(CString("Radius"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawnTrigger*)(m_poCursor))->m_fDepth);
			m_vunAttributeNames.push_back(CString("Depth"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawnTrigger*)(m_poCursor))->m_unNumSpawns);;
			m_vunAttributeNames.push_back(CString("Num Spawns"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawnTrigger*)(m_poCursor))->m_fWaitTime);
			m_vunAttributeNames.push_back("Wait Time");
			m_vunAttributes.push_back((unsigned int)((CActorSpawnTrigger*)(m_poCursor))->m_nMaxTimes);
			m_vunAttributeNames.push_back("Max Times");
			break;
		}
	case OBJ_ENEMY_ZOMBIECITIZEN:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("Zombie Citizen"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawn*)(m_poCursor))->getNumActors());
			m_vunAttributeNames.push_back(CString("Group Size"));

			break;
		}
	case OBJ_ENEMY_GASEOUSCLAY:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));
			m_vunAttributeNames.push_back(CString("Gaseous Clay"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawn*)(m_poCursor))->getNumActors());
			m_vunAttributeNames.push_back(CString("Group Size"));
			break;
		}
	case OBJ_ENEMY_ICECREAMMAN:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));
			m_vunAttributeNames.push_back(CString("Acidic IceCream"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawn*)(m_poCursor))->getNumActors());
			m_vunAttributeNames.push_back(CString("Group Size"));
			break;
		}
	case OBJ_ENEMY_QUARTERBACK:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));

			m_vunAttributeNames.push_back(CString("QB Zombie"));
			m_vunAttributes.push_back((unsigned int)((CActorSpawn*)(m_poCursor))->getNumActors());
			m_vunAttributeNames.push_back(CString("Group Size"));
			break;
		}
	case OBJ_ITEM_HEALTH:
		{
			m_vunAttributeNames.push_back(CString("Health"));
			break;
		}
	case OBJ_ITEM_SLICKSHOES:
		{
			m_vunAttributeNames.push_back(CString("Slick Shoes"));
			break;
		}
	case OBJ_ITEM_MONEY:
		{
			m_poCursor->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "star.x"));
			m_vunAttributeNames.push_back(CString("Money"));
			m_vunAttributes.push_back(100);
			m_vunAttributeNames.push_back(CString("Amount"));
			break;
		}
	}
}