/**
* @file RenderSystem.h
* @author Matt Rudder
* @date Created Mar 13, 2006
*
* Encapsulates the Direct3D API, more specifically covering the device.
*/

#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

// Engine includes
#include "Singleton.h"
#include "RenderDevice.h"
#include "ResourceManager.h"
#include "Frustum.h"
#include "RenderQueue.h"
#include "MethodSingleFunctor.h"

// System includes
#include <d3d9.h>
#include <d3dx9.h>
#include <list>

// DirectX Libraries
#pragma comment(lib, "d3d9.lib")
#ifdef _DEBUG
#	pragma comment(lib, "d3dx9d.lib")
#else
#	pragma comment(lib, "d3dx9.lib")
#endif

#define DEBUG_SHADERS 0
// Forward Declarations
class CRenderResource;
class CBitmapFont;
class CObject;

/**
* Encapsulates the Direct3D API, more specifically covering the device.
*
* @author Matt Rudder
* @date Created Mar 13, 2006
* @date Modified Mar 13, 2006
*/
class CRenderSystem : public CSingleton<CRenderSystem>
{
	friend class CSingleton;
protected:
	//! Window handle
	HWND m_hWnd;
	//! Direct3D Device
	CRenderDevice m_oDevice;
	//! Client Rectangle
	RECT m_rClient;
	//! List of current resources
	std::list<CRenderResource*> m_lResources;
	//! Accessibility Key structures
	STICKYKEYS m_oStickyKeys;
	TOGGLEKEYS m_oToggleKeys;
	FILTERKEYS m_oFilterKeys;
	//! Keyboard hook for disabling keys
	HHOOK m_hKeyboardHook;
	//! Windowed or not
	bool m_bWindowed;
	//! Render state blocks
	LPDIRECT3DSTATEBLOCK9 m_poDefaultState;
	//! View frustum
	CFrustum m_oViewFrustum;
	//! Function object for rendering.
	CMethodSingleFunctor<CRenderSystem, CObject*> m_oRenderFunc;

	//! Singleton stuff.
	//!@{
	CRenderSystem(void) : m_oRenderFunc(renderActor, this)
	{
		memset(&m_oStickyKeys, 0, sizeof(STICKYKEYS));
		memset(&m_oToggleKeys, 0, sizeof(TOGGLEKEYS));
		memset(&m_oFilterKeys, 0, sizeof(FILTERKEYS));
		m_oStickyKeys.cbSize = sizeof(STICKYKEYS);
		m_oToggleKeys.cbSize = sizeof(TOGGLEKEYS);
		m_oFilterKeys.cbSize = sizeof(FILTERKEYS);
	}
	CRenderSystem(const CRenderSystem&)  : m_oRenderFunc(renderActor, this) {}
	operator=(const CRenderSystem&) {}
	virtual ~CRenderSystem(void) { shutdown(); }
	//!@}

	/**
	* Creates a render window
	*
	* @date Created Mar 28, 2006
	* @param[in]	nWidth	Width of the new window.
	* @param[in]	nHeight	Height of the new window.
	* @return The handle to the newly created window
	*/
	HWND createWindow(int nWidth, int nHeight);

	/**
	* Enables/Disables special windows keys.
	*
	* @date Created Apr 18, 2006
	* @param[in]	bAllowKeys		Determines if special keys should be enabled
	*/
	void allowAccessibilityShortcutKeys(bool bAllowKeys);
public:

	/**
	* Accessors/Modifiers
	* @{
	*/
	CRenderDevice& getRenderDevice(void) { return m_oDevice; }
	HWND getRenderWindow(void) { return m_hWnd; }
	unsigned int getDisplayWidth(void) { return m_oDevice.getDisplayWidth(); }
	unsigned int getDisplayHeight(void) { return m_oDevice.getDisplayHeight(); }
	bool isWindowed() { return m_bWindowed; }
	CFrustum getViewFrustum() {return m_oViewFrustum;}
	//!@}

	/**
	* Calculates a color based on proximity to the goal value
	*
	* @date Created Mar 28, 2006
	* @param[in]	fVal	Current value
	* @param[in]	fGoal	Goal value.
	*/
	D3DCOLOR calcGoalColor(float fVal, float fGoal);

	/**
	* Initializes Direct3D and creates a render window.
	*
	* @date Created Mar 13, 2006
	* @remark Render System parameters are read from config file
	*/
	HWND initialize();

	/**
	* Cleans up everything to do with Direct3D
	*
	* @date Created Mar 13, 2006
	*/
	void shutdown(void);

	/**
	* Update the render system and draw objects queued for rendering.
	*
	* @date Created Mar 13, 2006
	*/
	void tick(void);

	/**
	* Adds a resource to a notification list, for device events.
	*
	* @date Created Mar 28, 2006
	*/
	void addResource(CRenderResource* pRes);

	/**
	* Removes a resource from the device event notification list.
	*
	* @date Created Mar 28, 2006
	*/
	void removeResource(CRenderResource* pRes);

	/**
	* Device Event Broadcasts. Called by the render device upon any change in 
	* state.
	*
	* @date Created Mar 28, 2006
	* @{
	*/
	void notifyDeviceLoss(void);
	void notifyDeviceRestore(void);
	void notifyDeviceRelease(void);
	//!@}

	/**
	* Window Procedure for the render window.
	*
	* @date Created Mar 13, 2006
	* @param[in]	hWnd	Handle to the window
	* @param[in]	unMsg	Message ID
	* @param[in]	wParam	Message Data
	* @param[in]	lParam	Message Data
	* @return		LRESULT	Response to the message
	*/
	static LRESULT CALLBACK processWindowMsg(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam);

	/**
	* Hook Procedure for disabling special keys (windows key)
	*
	* @date Created Mar 13, 2006
	* @param[in]	nCode	Message ID
	* @param[in]	wParam	Message Data
	* @param[in]	lParam	Message Data
	* @return		LRESULT	Response to the message
	*/
	static LRESULT CALLBACK disableKeysProc(int nCode, WPARAM wParam, LPARAM lParam);

	/**
	* Function used to render all actors. Called by the object manager's iterateObjects.
	*
	* @date Created May 26, 2006
	* @param[in]	pActor		The object to render.
	*/
	void renderActor(CObject* pActor);
};

#endif //_RENDERSYSTEM_H_