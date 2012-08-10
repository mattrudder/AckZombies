/**
* @file RenderDevice.cpp
* @author Matt Rudder
* @date Modified Mar 28, 2006
*
* Encapsulation of the Direct3D device, and all actions that can be taken on it
*/

// Local includes
#include "AcespaceEngine.h"
#include "RenderDevice.h"
#include "RenderSystem.h"


// Statics
LPDIRECT3D9 CRenderDevice::m_poD3D = NULL;

/**
* CRenderDevice::validateDisplayMode
* @date Modified Mar 28, 2006
*/
bool CRenderDevice::validateDisplayMode(int nWidth, int nHeight, int nRefresh)
{
	if(m_poD3D)
	{
		D3DDISPLAYMODE oDispMode;
		UINT unModeCount = m_poD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
		for(UINT i = 0; i < unModeCount; ++i)
		{
			if(D3D_OK == m_poD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &oDispMode))
			{
				if(oDispMode.Height == nHeight && oDispMode.Width == nWidth &&
					oDispMode.RefreshRate == nRefresh)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

/**
* CRenderDevice::validateStencilFormat
* @date Modified Mar 28, 2006
*/
bool CRenderDevice::validateStencilFormat(D3DFORMAT fmtStencil)
{
	if(m_poD3D)
	{
		return SUCCEEDED(m_poD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, 
			m_eDevType, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, 
			D3DRTYPE_SURFACE, fmtStencil));
	}
	return false;
}

/**
* CRenderDevice::validateMultisampleType
* @date Modified Mar 28, 2006
*/
bool CRenderDevice::validateMultisampleType(D3DMULTISAMPLE_TYPE msType)
{
	if(m_poD3D)
	{
		return SUCCEEDED(m_poD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, m_eDevType, 
			D3DFMT_X8R8G8B8, m_oPresentParams.Windowed, msType, NULL));
	}
	return false;
}

/**
* CRenderDevice::checkDeviceState
* @date Modified Mar 28, 2006
*/
bool CRenderDevice::checkDeviceState(void)
{
	if(m_poDevice)
	{
		switch(m_poDevice->TestCooperativeLevel())
		{
		case D3DERR_DEVICELOST: return false;
		case D3DERR_DEVICENOTRESET:
			{
				onDeviceLoss();
				if(FAILED(m_poDevice->Reset(&m_oPresentParams)))
					return false;
				onDeviceRestore();
				return true;
			}
		default: return true;
		}
	}
	return false;
}

/**
* CRenderDevice::CRenderDevice
* @date Modified Mar 28, 2006
*/
void CRenderDevice::onDeviceLoss(void)
{
	// PIX Stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderDevice::onDeviceLoss");

	CRenderSystem::getInstance().notifyDeviceLoss();
	m_poSprite->OnLostDevice();
	m_poLine->OnLostDevice();

	D3DPERF_EndEvent();
}

/**
* CRenderDevice::CRenderDevice
* @date Modified Mar 28, 2006
*/
void CRenderDevice::onDeviceRestore(void)
{
	// PIX Stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderDevice::onDeviceRestore");

	m_poSprite->OnResetDevice();
	m_poLine->OnResetDevice();
	CRenderSystem::getInstance().notifyDeviceRestore();

	D3DPERF_EndEvent();
}

/**
* CRenderDevice::CRenderDevice
* @date Modified Mar 28, 2006
*/
void CRenderDevice::onDeviceRelease(void)
{
	// PIX Stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderDevice::onDeviceRestore");

	CRenderSystem::getInstance().notifyDeviceRelease();
	COM_RELEASE(m_poEffectPool);
	COM_RELEASE(m_poSprite);
	COM_RELEASE(m_poLine);
	COM_RELEASE(m_poDevice);
	COM_RELEASE(m_poD3D);

	D3DPERF_EndEvent();
}

/**
* CRenderDevice::CRenderDevice
* @date Modified Apr 30, 2006
*/
CRenderDevice::CRenderDevice(void) : m_poDevice(NULL), m_poEffectPool(NULL)
{
	// PIX Stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderDevice::CRenderDevice");

	// Save device type
#if DEBUG_SHADERS == 1
	m_eDevType = D3DDEVTYPE_REF;
#else
	m_eDevType = D3DDEVTYPE_HAL;
#endif

	// Initialize Direct3D
	m_poD3D = Direct3DCreate9(D3D_SDK_VERSION);
	m_poD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, m_eDevType, &m_oCapabilities);

	// Init minimum device params
	ZeroMemory(&m_oPresentParams, sizeof(D3DPRESENT_PARAMETERS));
	m_oPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_oPresentParams.BackBufferCount = 1;
	m_oPresentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_oPresentParams.EnableAutoDepthStencil = true;
	
	D3DPERF_EndEvent();
}

/**
* CRenderDevice::~CRenderDevice
* @date Modified Mar 28, 2006
*/
CRenderDevice::~CRenderDevice(void)
{
	onDeviceRelease();
}

/**
* CRenderDevice::initalizeDevice
* @date Modified Mar 28, 2006
*/
HRESULT CRenderDevice::initalizeDevice(HWND hWnd, int nWidth, int nHeight, int nSamples, bool bVSync, bool bWindowed)
{
	// PIX Stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderDevice::initalizeDevice");
	// Make sure the display mode is supported
	CString sPrompt;
	if(!validateDisplayMode(nWidth, nHeight, 60))
	{
		sPrompt.Format("The selected display mode (%ix%i) is not supported by your graphics card.", nWidth, nHeight);
		MessageBox(hWnd, sPrompt, APP_NAME, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// Fill out device params
	m_oPresentParams.Windowed = bWindowed;
	m_oPresentParams.hDeviceWindow = hWnd;
	m_oPresentParams.BackBufferFormat = bWindowed ? D3DFMT_UNKNOWN : D3DFMT_A8R8G8B8;
	m_oPresentParams.BackBufferWidth = bWindowed ? 0 : nWidth;
	m_oPresentParams.BackBufferHeight = bWindowed ? 0 : nHeight;
	m_oPresentParams.FullScreen_RefreshRateInHz = bWindowed ? 0 : 60;
	m_oPresentParams.PresentationInterval = bVSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Check Stencil Format
	if(validateStencilFormat(D3DFMT_D24S8))
		m_oPresentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	else if(validateStencilFormat(D3DFMT_D16))
		m_oPresentParams.AutoDepthStencilFormat = D3DFMT_D16;

	// Check Multisample settings
	if(validateMultisampleType((D3DMULTISAMPLE_TYPE)nSamples))
		m_oPresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)nSamples;
	else
		m_oPresentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)0;

	HRESULT hr;
	if(FAILED(hr = m_poD3D->CreateDevice(D3DADAPTER_DEFAULT, m_eDevType, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_oPresentParams, &m_poDevice)))
	{
		return E_FAIL;
	}

	// Create effect pool
	D3DXCreateEffectPool(&m_poEffectPool);

	// Set default flags and states
	resetDeviceState();

	// Create Helper Devices
	D3DXCreateSprite(m_poDevice, &m_poSprite);
	D3DXCreateLine(m_poDevice, &m_poLine);

	// Return success
	D3DPERF_EndEvent();
	return S_OK;
}

/**
* CRenderDevice::resetDeviceState
* @date Modified Apr 05, 2006
*/
void CRenderDevice::resetDeviceState(void)
{
	// PIX Stuff.
	D3DPERF_BeginEvent(D3DCOLOR_XRGB(0, 215, 0), L"CRenderDevice::resetDeviceState");

	// Get display mode and setup defaults
	m_poDevice->GetDisplayMode(0, &m_displayMode);
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.25f, (float)m_displayMode.Width/(float)m_displayMode.Height, 1.0f, 2000.0f);
	m_poDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	m_poDevice->SetRenderState(D3DRS_ZENABLE, true);
	m_poDevice->SetRenderState(D3DRS_LIGHTING, true);

	m_poDevice->SetRenderState(D3DRS_AMBIENT, 0);
	m_poDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_poDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_poDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Blending modes
	m_poDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_poDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_poDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_poDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	ZeroMemory(&m_matDefault, sizeof(D3DMATERIAL9));
	m_matDefault.Ambient.r  = 1.0f;
	m_matDefault.Ambient.g  = 1.0f;
	m_matDefault.Ambient.b  = 1.0f;
	m_matDefault.Ambient.a  = 1.0f;
	m_matDefault.Diffuse.r  = 1.0f;
	m_matDefault.Diffuse.g  = 1.0f;
	m_matDefault.Diffuse.b  = 1.0f;
	m_matDefault.Diffuse.a  = 1.0f;
	m_matDefault.Specular.r = 1.0f;
	m_matDefault.Specular.g = 1.0f;
	m_matDefault.Specular.b = 1.0f;
	m_matDefault.Specular.a = 1.0f;
	m_matDefault.Power      = 37.777777777777777777777772f;
	m_poDevice->SetMaterial(&m_matDefault);

	// Create Light 
	ZeroMemory(&m_lightDefault, sizeof(D3DLIGHT9));
	m_lightDefault.Type          = D3DLIGHT_POINT;
	m_lightDefault.Diffuse.r     = 1.0f;
	m_lightDefault.Diffuse.g     = 1.0f;
	m_lightDefault.Diffuse.b     = 1.0f;
	m_lightDefault.Diffuse.a     = 1.0f;
	m_lightDefault.Specular.r    = 3.0f;
	m_lightDefault.Specular.g    = 3.0f;
	m_lightDefault.Specular.b    = 3.0f;
	m_lightDefault.Specular.a    = 1.0f;
	m_lightDefault.Attenuation0  = 1.1f;
	m_lightDefault.Range         = 50.0f;
	m_lightDefault.Position      = D3DXVECTOR3( 0.0f, 10.0f, 10.0f );

	m_poDevice->SetLight(0, &m_lightDefault);
	m_poDevice->LightEnable(0, true);


	D3DPERF_EndEvent();
}