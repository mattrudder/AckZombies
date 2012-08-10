#ifndef _TESTSTATE_H_
#define _TESTSTATE_H_

#include "Camera.h"
#include "AIManager.h"
#include "PhysicsManager.h"
#include "FreeCamera.h"
#include "GameKernel.h"
#include "CommandConsole.h"

#include "Pistol.h"
#include "RocketLauncher.h"
#include "Shotgun.h"
#include "Grenade.h"
#include "DesertEagle50.h"
#include "PotatoChainGun.h"
#include "HolyWaterBalloonCanon.h"
#include "Player.h"

#include "MenuManager.h"
#include "GameState.h"
#include "ActorSpawn.h"
#include "SoundManager.h"

#include "Material.h"
#include "Octree.h"
#include "Frustum.h"
#include "RenderSystem.h"
#include "ParticleManager.h"

#include <fstream>
using std::ifstream;
#define NODE_PTR(x) ((CAINode*)(*x))
#include "CollisionGeometry.h"

#include "AIStatePathPlan.h"
#include "MenuState.h"
#include "Options.h"

class CTestState : public CGameState<CTestState>
{
	friend class CPlacementState;
	friend class CLoadState;
protected:
	//! Display Font
	CBitmapFont* m_fntDina;

	//! Test objects
	// oops public now
public:
	CMesh* m_pMesh;
	CMesh* m_pMesh2;
	CMesh* m_pMesh3;
	CObject* m_pTestActor;


	COctree* m_pWorld;
	CFrustum m_oViewFrustum;

	CTickTimer	m_tParticles;
	CTickTimer	m_tAISys;
	CTickTimer m_tGameOver;
	bool m_bFreeRoam;

	bool m_bDisplayFPS;
public:
	~CTestState(void)
	{
		COM_RELEASE(m_fntDina);
		COM_RELEASE(m_pMesh2);
	}
	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created Mar 08, 2006
	*/
	virtual void onActivate(void)
	{
		static bool bFirstTime = true;
		if (bFirstTime)
		{
			m_bDisplayFPS = false;
			bFirstTime = false;
			m_bFreeRoam = false;
			// Create Font
			m_fntDina = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "Dina.ini");

			// set it to our player
			CObjectManager::ObjectList loPlayer;
			CObjectManager::getInstancePtr()->getObjects(OBJ_PLAYER, &loPlayer);
			m_pTestActor = loPlayer.front();

			m_tParticles.setInterval(0.016f);
			m_tAISys.setInterval(0.016f);

			// initially start playing the sound and pause it
			CSoundManager::getInstance().playSound(SND_MUSIC_MAINMENU, -1);
			CSoundManager::getInstance().pauseSound(SND_MUSIC_MAINMENU, true);
		}

		CSoundManager::getInstance().pauseSound(SND_MUSIC_MAINMENU, false);

		CSoundManager::getInstance().playSound(SND_EFFECT_IHATEZOMBIES);
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
	virtual void tick(void)
	{
		// Update physics manager (internally ticks self)
		if(!m_bFreeRoam)
			CObjectManager::getInstance().updateAllObjects();

		if(m_tAISys.tick())
			CAIManager::getInstancePtr()->update();

		float fTime;
		if(m_tParticles.tick(&fTime))
			CParticleManager::getInstance().update(fTime);

		CSoundManager::getInstance().playSound(SND_MUSIC_MAINMENU, 0, false, false);

		// check all non-player inputs for activity
		EPlayerInputType eInputType= checkNewPlayer();
		if (eInputType != PIT_NUM_OBJECTS)
		{
			// get the player
			CObjectManager::ObjectList loPlayers;
			CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
			CPlayer* poNewPlayer = (CPlayer*)CObjectManager::getInstance().createObject(OBJ_PLAYER);
			D3DXVECTOR3 vPlayerPos = ((CPlayer*)(loPlayers.front()))->getPosition();
			vPlayerPos.x += 0.1f;
			poNewPlayer->setPosition(vPlayerPos);
			poNewPlayer->activate();
			poNewPlayer->setBV(D3DXVECTOR3(poNewPlayer->getPosition().x, 8.5f, poNewPlayer->getPosition().z), 3.0f);
			CAnimatedMesh* pAnim = (CAnimatedMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Johnny.x");
			pAnim->setAnimationSetByName("Idle");
			poNewPlayer->setMesh((CMesh*)pAnim);
			poNewPlayer->setDefaultScale(D3DXVECTOR3(0.65f, 0.65f, 0.65f));
			poNewPlayer->setAmtMoney(0);

			CPistolWeapon *pPistol = new CPistolWeapon;
			pPistol->init();
			pPistol->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "pistola.x"));
			pPistol->setPlayer(poNewPlayer);
			CGrenadeWeapon *pGrenade = new CGrenadeWeapon;
			pGrenade->init();

			poNewPlayer->setIcon("icon2.x");
			poNewPlayer->addWeapon(pPistol);
			poNewPlayer->addGrenade(pGrenade);
			pGrenade->setPlayer(poNewPlayer);
			poNewPlayer->setOrientation(D3DXVECTOR3(0, 0, 1));
			COptions::getInstance().setPlayerControls(poNewPlayer, false);

			CCamera::getInstance().setCamera(CCamera::getInstance().getCameraPos(), ((CPlayer*)(loPlayers.front()))->getPosition(), poNewPlayer->getPosition());
		}
	}

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created Apr 24, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice)
	{
		LPDIRECT3DDEVICE9 pDev = oRenderDevice.getD3DDevice();
		CDirectInputWrapper* pInput = CDirectInputWrapper::getInstancePtr();

		// Sync view matrix with camera class
		D3DXMATRIX matWorld, matView, matProj, matWVP;
		static D3DXVECTOR2 vMousePos = pInput->getMousePosition();
		D3DXVECTOR2 vMouseMove = pInput->getMousePosition(),
			vMouseDelta;
		CActor* pActor = (CActor*)m_pTestActor;
		D3DXVECTOR3 vPos = pActor->getPosition();
		CFreeCamera* pFreeCam = CFreeCamera::getInstancePtr();
		CCamera* pCam = CCamera::getInstancePtr();
		if(!CCommandConsole::getInstance().isVisible())
		{
			if(m_bFreeRoam)
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
				CObjectManager::ObjectList lPlayers;
				CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
				CObjectManager::ObjectList::iterator playerIter = lPlayers.begin();
				if (lPlayers.size() == 1)
					pCam->updateCameraSP(((CPlayer*)(lPlayers.front()))->getPosition());
				else if (lPlayers.size() == 2)
				{
					playerIter++;
					CPlayer* pPlayer2 = (CPlayer*)*playerIter;
					pCam->updateCameraMP(((CPlayer*)(lPlayers.front()))->getPosition(), pPlayer2->getPosition());
				}

				pCam->getViewMatrix(&matView);
			}

			pDev->SetTransform(D3DTS_VIEW, &matView);
#if _DEBUG
			if(pInput->isKeyDownBuffered(DIK_F, PLACEMENT_SYSTEM))
				m_bFreeRoam = !m_bFreeRoam;
#endif
		}

		if(pInput->isKeyDownBuffered(DIK_GRAVE))
			CCommandConsole::getInstance().toggleConsole();

		pCam->getViewMatrix(&matView);

		// TODO: Base position off object
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranslation(&matWorld, vPos.x, vPos.y, vPos.z);
		pDev->SetTransform(D3DTS_WORLD, &matWorld);
		pDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(100), (float)CRenderSystem::getInstance().getRenderDevice().getDisplayWidth() / (float)CRenderSystem::getInstance().getRenderDevice().getDisplayHeight(), 1.0f, 2000.0f);
		m_oViewFrustum.setMatrix(matProj, matView, 1000.0f);

		// Get WorldViewProjection Matrix
		D3DXMatrixMultiply(&matWVP, D3DXMatrixMultiply(&matWVP, &matWorld, &matView), &matProj);

		// Temp Light
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		D3DXCOLOR white = D3DCOLOR_XRGB(255, 255, 255);	
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse = white;
		light.Specular = white * 0.6f;
		light.Ambient = white * 0.4f;
		light.Direction = D3DXVECTOR3(0, -1, 0);
		pDev->SetLight(0, &light);
		pDev->LightEnable(0, true);

		D3DMATERIAL9 mtrl;
		mtrl.Ambient = white;
		mtrl.Diffuse = white;
		mtrl.Specular = white;
		mtrl.Emissive = white;
		mtrl.Power = 5.0f;
		pDev->SetMaterial(&mtrl);

		D3DXMatrixIdentity(&matWorld);
		pDev->SetTransform(D3DTS_WORLD, &matWorld);


		m_pMesh2->draw();
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_oViewFrustum.renderFrustum(D3DCOLOR_XRGB(0, 215, 0));
		pDev->SetMaterial(&mtrl);
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
		CMenuManager::getInstance().updateHUD();

		// draw spheres on player
		static ID3DXMesh* pSphere = NULL;
		if (pSphere == NULL)
		{
			D3DXCreateSphere(oRenderDevice.getD3DDevice(), 1.0f, 10, 10, &pSphere, NULL);
		}

		// drawing of the bounding spheres
#if 0
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();
		D3DXMATRIX matWorld, scale;
		while (oPlayerIter != loPlayers.end())
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, ((CPlayer*)(*oPlayerIter))->getBV().centerPt.x, ((CPlayer*)(*oPlayerIter))->getBV().centerPt.y, ((CPlayer*)(*oPlayerIter))->getBV().centerPt.z);
			D3DXMatrixScaling(&scale, ((CPlayer*)(*oPlayerIter))->getBV().fRadius, ((CPlayer*)(*oPlayerIter))->getBV().fRadius, ((CPlayer*)(*oPlayerIter))->getBV().fRadius);
			matWorld = scale * matWorld;
			oRenderDevice.getD3DDevice()->SetTransform(D3DTS_WORLD, &matWorld);
			pSphere->DrawSubset(0);
			oPlayerIter++;
		}

		CObjectManager::ObjectList loEnemies;
		CObjectManager::getInstance().getObjects(OBJ_ENEMY, &loEnemies);
		CObjectManager::ObjectList::iterator oEnemyIter = loEnemies.begin();
		while (oEnemyIter != loEnemies.end())
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matWorld, ((CEnemy*)(*oEnemyIter))->getBV().centerPt.x, ((CEnemy*)(*oEnemyIter))->getBV().centerPt.y, ((CEnemy*)(*oEnemyIter))->getBV().centerPt.z);
			D3DXMatrixScaling(&scale, ((CEnemy*)(*oEnemyIter))->getBV().fRadius, ((CEnemy*)(*oEnemyIter))->getBV().fRadius, ((CEnemy*)(*oEnemyIter))->getBV().fRadius);
			matWorld = scale * matWorld;
			oRenderDevice.getD3DDevice()->SetTransform(D3DTS_WORLD, &matWorld);
			pSphere->DrawSubset(0);
			oEnemyIter++;
		}
#endif

		if (m_bDisplayFPS)
		{
			// Display Debug Information
			CString sFPS;
			RECT rText = { 4, 0, oRenderDevice.getDisplayWidth(), oRenderDevice.getDisplayHeight() };
			sFPS.Format("%s\nKernel ticks per second: %.2f\nFrames rendered per second: %.2f", APP_VERSION,
				CTimer::getInstance().getFPS(), CGameKernel::getInstance().getRenderTimer().getTicksPerSecond());
			m_fntDina->drawString(sFPS, 3.0f, 3.0f, 0x80000000);

			sFPS.Format("%s\nKernel ticks per second: {%#x}%.2f\n{%#x}Frames rendered per second: {%#x}%.2f", APP_VERSION,
				CRenderSystem::getInstance().calcGoalColor(CTimer::getInstance().getFPS(), 60.0f), CTimer::getInstance().getFPS(),
				D3DCOLOR_XRGB(255, 255, 255), CRenderSystem::getInstance().calcGoalColor(CGameKernel::getInstance().getRenderTimer().getTicksPerSecond(), (float)oRenderDevice.getRefreshRate()), CGameKernel::getInstance().getRenderTimer().getTicksPerSecond());
			m_fntDina->drawStringMarkup(sFPS.GetBuffer(), 2.0f, 2.0f);
		}
	}

	/**
	* Check all input devices for activity
	*
	* @date Created May 17, 2006
	* @return	the type of controls PIT_NUM_OBJECTS if no activity
	*/
	EPlayerInputType checkNewPlayer(void)
	{
		CDirectInputWrapper* pInput = CDirectInputWrapper::getInstancePtr();

		// input types of currently connected players
		std::list<EPlayerInputType> loUsedTypes;
		std::list<EPlayerInputType>::iterator oUsedTypeIter;
		
		// get all the current players to fill out the list of input types
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();
		while (oPlayerIter != loPlayers.end())
		{
			loUsedTypes.push_back(((CPlayer*)(*oPlayerIter))->getInputType());
			oPlayerIter++;
		}


		bool bNotInUse = true;
		oUsedTypeIter = loUsedTypes.begin();
		while (oUsedTypeIter != loUsedTypes.end())
		{
			if (*oUsedTypeIter == PIT_2ND_XBOX360PAD)
			{
				bNotInUse = false;
				break;
			}
			oUsedTypeIter++;
		}

		if (bNotInUse)
		{
			// check PIT_2ND_XBOX360PAD for input
			if (pInput->m_oXInputWrapper.isConnected(1))
			{
				if (pInput->m_oXInputWrapper.isButtonDownBuffered(1, XINPUT_GAMEPAD_START))
				{
					return PIT_2ND_XBOX360PAD;
				}
			}
		}

		bNotInUse = true;
		oUsedTypeIter = loUsedTypes.begin();
		while (oUsedTypeIter != loUsedTypes.end())
		{
			if (*oUsedTypeIter == PIT_1ST_XBOX360PAD)
			{
				bNotInUse = false;
				break;
			}
			oUsedTypeIter++;
		}

		if (bNotInUse)
		{
			// check PIT_1ST_XBOX360PAD for input
			if (pInput->m_oXInputWrapper.isConnected(0))
			{
				if (pInput->m_oXInputWrapper.isButtonDownBuffered(0, XINPUT_GAMEPAD_START))
				{
					return PIT_1ST_XBOX360PAD;
				}
			}
		}

		bNotInUse = true;
		oUsedTypeIter = loUsedTypes.begin();
		while (oUsedTypeIter != loUsedTypes.end())
		{
			if (*oUsedTypeIter == PIT_KEYBOARDMOUSE)
			{
				bNotInUse = false;
				break;
			}
			oUsedTypeIter++;
		}

		if (bNotInUse)
		{
			// check PIT_KEYBOARDMOUSE for input
			if (pInput->isKeyDownBuffered(DIK_RETURN))
			{
				return PIT_KEYBOARDMOUSE;
			}

		}

		bNotInUse = true;
		oUsedTypeIter = loUsedTypes.begin();
		while (oUsedTypeIter != loUsedTypes.end())
		{
			if (*oUsedTypeIter == PIT_2ND_GAMEPAD)
			{
				bNotInUse = false;
				break;
			}
			oUsedTypeIter++;
		}

		if (bNotInUse)
		{
			// check PIT_2ND_GAMEPAD for input
			if (pInput->getNumLegacyJoypadDevicesConnected() > 1)
			{
				if (pInput->isJoypadButtonDownBuffered(1, 0))
				{
					return PIT_2ND_GAMEPAD;
				}
			}
		}

        return PIT_NUM_OBJECTS;
	}
};

#endif