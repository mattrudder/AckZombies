/**
* @file RenderDevice.h
* @author Matt Rudder
* @date Created Mar 28, 2006
*
* Encapsulation of the Direct3D device, and all actions that can be taken on it
*/

#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_

// System includes
#include <d3d9.h>
#include <d3dx9.h>

/**
* Encapsulation of the Direct3D device, and all actions that can be taken on it
*
* @author Matt Rudder
* @date Created Mar 28, 2006
*/
class CRenderDevice
{
protected:
	//! Direct3D
	static LPDIRECT3D9 m_poD3D;
	//! The device
	LPDIRECT3DDEVICE9 m_poDevice;
	//! The sprite interface
	LPD3DXSPRITE m_poSprite;
	//! The line interface
	LPD3DXLINE m_poLine;
	//! The device capabilities
	D3DCAPS9 m_oCapabilities;
	//! The device's present parameters
	D3DPRESENT_PARAMETERS m_oPresentParams;
	//! The current display mode.
	D3DDISPLAYMODE m_displayMode;
	//! The current projection matrix.
	D3DMATRIX m_matProjection;
	//! Effect pool; used to share data between shaders.
	LPD3DXEFFECTPOOL m_poEffectPool;
	//! Device type
	D3DDEVTYPE m_eDevType;
	//! Default Material
	D3DMATERIAL9 m_matDefault;
	//! Default Light
	D3DLIGHT9 m_lightDefault;

	/**
	* Determines if a display mode is supported by the graphics card.
	*
	* @date Created Mar 28, 2006
	* @param[in]	nWidth		Horizontal Resolution
	* @param[in]	nHeight		Vertical Resolution
	* @param[in]	nRefresh	Refresh Rate
	* @return true if the display mode is supported, false otherwise.
	*/
	bool validateDisplayMode(int nWidth, int nHeight, int nRefresh);

	/**
	* Determines if a stencil format is supported by the graphics card.
	*
	* @date Created Mar 28, 2006
	* @param[in]	fmtStencil	stencil format
	* @return true if the format is supported, false otherwise.
	*/
    bool validateStencilFormat(D3DFORMAT fmtStencil);

	/**
	* Determines if a multisample type is supported by the graphics card.
	*
	* @date Created Mar 28, 2006
	* @param[in]	msType	Multisample type
	* @return true if the type is supported, false otherwise.
	*/
	bool validateMultisampleType(D3DMULTISAMPLE_TYPE msType);

	/**
	* Checks the device for possible loss, and resets it if needed.
	*
	* @date Created Mar 28, 2006
	* @return true if the device is up and running, false otherwise
	*/
	bool checkDeviceState(void);

	/**
	* Helper method to handle device loss
	*
	* @date Created Mar 28, 2006
	*/
	void onDeviceLoss(void);

	/**
	* Helper method to handle device restoration
	*
	* @date Created Mar 28, 2006
	*/
	void onDeviceRestore(void);

	/**
	* Helper method to handle device release
	*
	* @date Created Mar 28, 2006
	*/
	void onDeviceRelease(void);

public:
	CRenderDevice(void);
	virtual ~CRenderDevice(void);

	/**
	* Accessors/Modifiers
	* @date Created Mar 28, 2006
	* @{
	*/
	LPD3DXEFFECTPOOL getEffectPool(void) { return m_poEffectPool; }
	LPDIRECT3DDEVICE9 getD3DDevice(void) { return m_poDevice; }
	LPD3DXSPRITE getD3DXSprite(void) { return m_poSprite; }
	LPD3DXLINE getD3DXLine(void) { return m_poLine; }
	LPDIRECT3D9 getD3D(void) { return m_poD3D; }
	unsigned int getRefreshRate(void) {	return m_displayMode.RefreshRate; }
	unsigned int getDisplayWidth(void) { return m_displayMode.Width; }
	unsigned int getDisplayHeight(void) { return m_displayMode.Height; }
	bool isWindowed(void) { return m_oPresentParams.Windowed == TRUE; }
	//!@}

	/**
	* First time device initialization
	*
	* @date Created Mar 28, 2006
	* @param[in]	hWnd		Handle to the render window.
	* @param[in]	nWidth		Width of the back buffer.
	* @param[in]	nHeight		Height of the back buffer.
	* @param[in]	nSamples	Multisampling settings.
	* @param[in]	bVSync		Enable vertical sync.
	* @param[in]	bWindowed	Enable windowed mode.
	* @return Result of the operation.
	*/
	HRESULT initalizeDevice(HWND hWnd, int nWidth, int nHeight, int nSamples, bool bVSync, bool bWindowed);

	/**
	* Tests the device's cooperative level, and notifies the Render System if
	* an event has occurred.
	*
	* @date Created Mar 28, 2006
	*/
	void checkDevice(void);

	/**
	* Sets the default state of the device.
	*
	* @date Created Apr 05, 2006
	*/
	void resetDeviceState(void);
};

#endif //_RENDERDEVICE_H_