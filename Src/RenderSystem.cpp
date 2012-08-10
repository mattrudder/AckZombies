/**
* @file RenderSystem.cpp
* @author Matt Rudder
* @date Modified Mar 13, 2006
*
* Encapsulates the Direct3D API, more specifically covering the device.
*/

// Local includes
#include "RenderSystem.h"

// Engine includes
#include "AcespaceEngine.h"
#include "GameKernel.h"
#include "RenderResource.h"
#include "BitmapFont.h"
#include "FreeCamera.h"
#include "Camera.h"
#include "Material.h"
#include "DirectInputWrapper.h"
#include "Actor.h"
#include "CommandConsole.h"
#include "ObjectManager.h"
#include "Octree.h"
#include "Player.h"
#include "ParticleManager.h"

/**
* Creates a render window
*
* @date Created Mar 28, 2006
* @return The handle to the newly created window
*/
HWND CRenderSystem::createWindow(int nWidth, int nHeight)
{
	// Initialize render window.
	static TCHAR szAppName[] = TEXT("AckZombies");
	HWND hWnd;
	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = CRenderSystem::processWindowMsg;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = GetModuleHandle(NULL);
	wndclassex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = szAppName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if (!RegisterClassEx(&wndclassex))
	{
		MessageBox(NULL, TEXT("RegisterClassEx failed!"), szAppName, MB_ICONERROR);
		return 0;
	}

	CString sTitle;
	sTitle.Format("Ack! Zombies v%s", ACESPACE_VERSION);
	hWnd = CreateWindowEx(WS_EX_CONTROLPARENT, 
		szAppName, 
		sTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
		50, 
		50, 
		nWidth, 
		nHeight, 
		NULL, 
		NULL, 
		GetModuleHandle(NULL),
		NULL);

	return hWnd;
}

/**
* CRenderSystem::calcGoalColor
* @date Modified Mar 13, 2006
*/
D3DCOLOR CRenderSystem::calcGoalColor(float fVal, float fGoal)
{
	// Calculate Goal color based on optimal value
	float fGoalScale = (fVal / fGoal);
	D3DXVECTOR3 vRed(0.85f, 0.0f, 0.0f), vYellow(0.85f, 0.85f, 0.0f),
		vGreen(0.0f, 0.85f, 0.0f), vFinal(0.0f, 0.0f, 0.0f);

	// Interpolate colors
	if(fGoalScale > 1.0f) vFinal.y = 0.85f;
	else
	{
		if(fGoalScale >= 0.5f)
			D3DXVec3Lerp(&vFinal, &vYellow, &vGreen, (fGoalScale - 0.5f) * 2.0f);
		else
			D3DXVec3Lerp(&vFinal, &vRed, &vYellow, fGoalScale / 0.5f);
	}

	// Calculate Final Goal color
	return D3DCOLOR_XRGB((short)(vFinal.x * 255), (short)(vFinal.y * 255), (short)(vFinal.z * 255));
}

/**
* CRenderSystem::allowAccessibilityShortcutKeys
* @date Modified Apr 18, 2006
*/
void CRenderSystem::allowAccessibilityShortcutKeys(bool bAllowKeys)
{
	if(bAllowKeys)
	{
		// Restore key settings to original state.
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &m_oStickyKeys, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &m_oToggleKeys, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &m_oFilterKeys, 0);
	}
	else
	{
		// Disable key settings if they are not active.
		STICKYKEYS skOff = m_oStickyKeys;
		if((skOff.dwFlags & SKF_STICKYKEYSON) == 0)
		{
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}

		TOGGLEKEYS tkOff = m_oToggleKeys;
		if((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0)
		{
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}

		FILTERKEYS fkOff = m_oFilterKeys;
		if((fkOff.dwFlags & FKF_FILTERKEYSON) == 0)
		{
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
			fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
	}
}

/**
* CRenderSystem::initialize
* @date Modified Mar 13, 2006
*/
HWND CRenderSystem::initialize()
{
	// Read info from config
	CConfigFile& refConfig = CGameKernel::getInstance().getConfig();
	CString sConfVal = refConfig.GetValue("RenderSystem", "Windowed");
	bool bWindowed = SET_BOOL(sConfVal, false);
	sConfVal = refConfig.GetValue("RenderSystem", "HorizontalRes");
	int nHorizRes = SET_INT(sConfVal, 800);
	sConfVal = refConfig.GetValue("RenderSystem", "VerticalRes");
	int nVertRes = SET_INT(sConfVal, 600);
	sConfVal = refConfig.GetValue("RenderSystem", "MultiSample");
	int nMultisample = SET_INT(sConfVal, 0);
	sConfVal = refConfig.GetValue("RenderSystem", "RefreshRate");
	int nRefreshRate = SET_INT(sConfVal, 60);
	sConfVal = refConfig.GetValue("RenderSystem", "VSync");
	bool bVSync = SET_BOOL(sConfVal, false);

	// Force Windowed mode in debug
#ifdef _DEBUG
	bWindowed = true;
#endif

	m_bWindowed = bWindowed;

	// Get keyboard hook
	m_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, CRenderSystem::disableKeysProc, GetModuleHandle(NULL), 0);

	// Create main render window
	m_hWnd = createWindow(nHorizRes, nVertRes);

	// Define client rect
	m_rClient.top = m_rClient.left = 0;
	m_rClient.bottom = nVertRes;
	m_rClient.right = nHorizRes;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	// Initialize Direct3D
	if(FAILED(m_oDevice.initalizeDevice(m_hWnd, nHorizRes, nVertRes, nMultisample, bVSync, bWindowed)))
	{
		CloseWindow(m_hWnd);
		DestroyWindow(m_hWnd);
		m_hWnd = 0;
		return 0;
	}

	// Disable accessibility features if in windowed mode
	if(m_oDevice.isWindowed())
	{
		// Save the current accessibility key settings
		SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &m_oStickyKeys, 0);
		SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &m_oToggleKeys, 0);
		SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &m_oFilterKeys, 0);

		allowAccessibilityShortcutKeys(false);
	}

	m_oDevice.getD3DDevice()->CreateStateBlock(D3DSBT_ALL, &m_poDefaultState);

	
	// Init subsystems
	CCommandConsole::getInstance().loadConsole();
	CParticleManager::getInstance();
	return m_hWnd;
}

/**
* CRenderSystem::shutdown
* @date Modified Mar 13, 2006
*/
void CRenderSystem::shutdown(void)
{
	// Deallocate state blocks
	COM_RELEASE(m_poDefaultState);

	// Enable shortcut keys
	allowAccessibilityShortcutKeys(true);

	// Release keyboard hook
	UnhookWindowsHookEx(m_hKeyboardHook);

	// Shutdown subsystems
	CCommandConsole::deleteInstance();
	CParticleManager::deleteInstance();
}

/**
* CRenderSystem::tick
* @date Modified Mar 13, 2006
*/
void CRenderSystem::tick(void)
{
	// PIX stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderSystem::tick");
	// Handle Windows Messages
	MSG msg;
	if(GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
	else
	{
		// Exit game.
		CGameKernel::getInstance().changeState(NULL);
		return;
	}

	LPDIRECT3DDEVICE9 pDev = m_oDevice.getD3DDevice();

	// Update console and check for quit.
	CCommandConsole::getInstance().update();
	if(!CGameKernel::getInstance().getCurrentState())
		return;

	pDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDev->BeginScene();

	CGameKernel::getInstance().getCurrentState()->preRender(m_oDevice);

	// Render World/Actor geometry.
	D3DXMATRIX mView, mProj;
	pDev->GetTransform(D3DTS_VIEW, &mView);
	pDev->GetTransform(D3DTS_PROJECTION, &mProj);
	m_oViewFrustum.setMatrix(mProj, mView);

	D3DMATERIAL9 white;
	white.Ambient.a = white.Ambient.r = white.Ambient.b = white.Ambient.g = 0.2f;
	white.Power = 5.0f;
	white.Emissive.a = white.Emissive.r = white.Emissive.b = white.Emissive.g = 1.0f;
	pDev->SetMaterial(&white);
	pDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mView));
	//COctree::getInstance().renderTree(m_oViewFrustum);
	

	// Iterate objects
	pDev->SetMaterial(&white);
	CObjectManager::getInstance().iterateObjects(OBJ_ACTOR, &m_oRenderFunc);

	// Draw particles
	D3DXMATRIX mWorld;
	pDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mWorld));
	CParticleManager::getInstance().draw();

	CGameKernel::getInstance().getCurrentState()->postRender(m_oDevice);
	CCommandConsole::getInstance().draw();

	pDev->EndScene();
	pDev->Present(NULL, NULL, NULL, NULL);
	
	D3DPERF_EndEvent();
}

/**
* CRenderSystem::addResource
* @date Modified Mar 28, 2006
*/
void CRenderSystem::addResource(CRenderResource* pRes)
{
	// Make sure the resource isn't already in the list.
	std::list<CRenderResource*>::iterator itResList = m_lResources.begin();
	while(itResList != m_lResources.end())
	{
		if((*itResList) == pRes)
			return;
		++itResList;
	}
	// If not, add it
	m_lResources.push_front(pRes);
}

/**
* CRenderSystem::removeResource
* @date Modified Mar 28, 2006
*/
void CRenderSystem::removeResource(CRenderResource* pRes)
{
	std::list<CRenderResource*>::iterator itResList = m_lResources.begin();
	while(itResList != m_lResources.end())
	{
		if((*itResList) == pRes)
		{
			itResList = m_lResources.erase(itResList);
			return;
		}
		else
			++itResList;
	}
}

/**
* CRenderSystem::removeResource
* @date Modified Mar 28, 2006
*/
void CRenderSystem::notifyDeviceLoss(void)
{
	if(m_lResources.size())
	{
		std::list<CRenderResource*>::iterator itResList = m_lResources.begin();
		while(itResList != m_lResources.end())
		{
			(*itResList)->onDeviceLoss();
			++itResList;
		}
	}
}

/**
* CRenderSystem::removeResource
* @date Modified Mar 28, 2006
*/
void CRenderSystem::notifyDeviceRestore(void)
{
	if(m_lResources.size())
	{
		std::list<CRenderResource*>::iterator itResList = m_lResources.begin();
		while(itResList != m_lResources.end())
		{
			(*itResList)->onDeviceRestore();
			++itResList;
		}
	}
}

/**
* CRenderSystem::removeResource
* @date Modified Mar 28, 2006
*/
void CRenderSystem::notifyDeviceRelease(void)
{
	if(m_lResources.size())
	{
		std::list<CRenderResource*>::iterator itResList = m_lResources.begin();
		while(itResList != m_lResources.end())
		{
			(*itResList)->onDeviceRelease();
			++itResList;
		}
	}
}

/**
* CRenderSystem::processWindowMsg
* @date Modified Mar 13, 2006
*/
LRESULT CALLBACK CRenderSystem::processWindowMsg(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam)
{
    switch (unMsg)
    {
    case WM_CREATE:
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
	case WM_PAINT:
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
    }
    return DefWindowProc(hWnd, unMsg, wParam, lParam);
}

/**
* CRenderSystem::disableKeysProc
* @date Modified Mar 13, 2006
*/
LRESULT CALLBACK CRenderSystem::disableKeysProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CRenderSystem* pRen = CRenderSystem::getInstancePtr();
	// Only process commands in our range.
	if(nCode < 0 || nCode != HC_ACTION)
		return CallNextHookEx(pRen->m_hKeyboardHook, nCode, wParam, lParam);

	bool bEatKey = false;
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
	switch(wParam)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			bEatKey = GetActiveWindow() == pRen->m_hWnd && (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN);
			break;
		}
	}

	if(bEatKey)
		return 1;
	else
		return CallNextHookEx(pRen->m_hKeyboardHook, nCode, wParam, lParam);
}

/**
* CRenderSystem::renderActor
* @date Modified May 26, 2006
*/
void CRenderSystem::renderActor(CObject* pActor)
{
	CActor* pRenderActor = (CActor*)(pActor);
	D3DXMATRIX mActorPos;
	if(!pRenderActor->m_pMesh)
		return;

	// Sphere frustum collision
	if(m_oViewFrustum.containsSphere(pRenderActor->getBV()) == CFrustum::TEST_OUT)
		return;

	pRenderActor->draw();
}