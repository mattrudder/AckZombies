/**
* @file DirectInputWrapper.h
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the Direct Input Wrapper class
* used for getting keyboard, mouse, and joystick (non XBOX360 controller) input
*/

#ifndef _DIRECTINPUTWRAPPER_H_
#define _DIRECTINPUTWRAPPER_H_

#include "Singleton.h"

// Used for XBOX360 controllers
#include "XInputWrapper.h"

// Define this so that the compiler doesn't complain :)
#define DIRECTINPUT_VERSION 0x0800

#include "d3dx9.h"
#include "dinput.h"
#pragma comment (lib, "dinput8.lib")

// Macro to check if a key is down
#define KEYDOWN(buffer, key)  (buffer[key] & 0x80)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

enum ESystemInput
{
	MAIN_SYSTEM,
	MENU_SYSTEM,
	INPUT_SYSTEM,
	CONSOLE_SYSTEM,
	PLACEMENT_SYSTEM,
	PLAYER_SYSTEM,
	OTHER_SYSTEM,
	MAX_SYSTEMS
};

#define LMOUSE 149457
#define RMOUSE 149458

/**
* Declares the Direct Input Wrapper class
* used for displaying a menu and its buttons
* 
* @author Jordan Leckner
* @date Created March 06, 2006
* @date Modified April 7, 2006
*/
class CDirectInputWrapper : public CSingleton<CDirectInputWrapper>
{
	friend class CSingleton<CDirectInputWrapper>;
public:

	//! Handle to the window used for setting cooperation levels
	HWND						m_hWnd;
	//! Main Direct Input Object
	IDirectInput8				*m_pDIObject;
	//! Keyboard device
	IDirectInputDevice8			*m_pDIDKeyboard;
	//! Mouse device
	IDirectInputDevice8			*m_pDIDMouse;
	//! array used for buffered input
	unsigned char				m_ucBufferedKeys[MAX_SYSTEMS][256];
	//! Mouse cursor position
	D3DXVECTOR2					m_CursorPos;
	//! Change in mouse cursor position
	DIMOUSESTATE				m_MouseChange;
	//! Whether or not the left mouse button is down
	bool						m_bLeftMouseButtonDown;
	//! Whether or not the right mouse button is down
	bool						m_bRightMouseButtonDown;
	//! Keybuffer used for getting the keyboard input
	unsigned char				m_ucKeyboardBuffer[256];
	//! Wrapper for XInput used with the XBOX360 controllers
	CXInputWrapper				m_oXInputWrapper;
	//! Legacy joysticks (non-XBOX360) 
	IDirectInputDevice8			*m_pDIDJoysticks[XI_MAX_CONTROLLERS];
	//! Joystick states
	DIJOYSTATE2					m_JoystickStates[XI_MAX_CONTROLLERS];

	/**
	* Sets up the Direct Input Wrapper
	* 
	* @date Created March 06, 2006
	*/
	CDirectInputWrapper();
	//! Singleton stuff.
	//!@{
	CDirectInputWrapper(const CDirectInputWrapper&) {}
	operator=(const CDirectInputWrapper&){}
	virtual ~CDirectInputWrapper(void);
	//!@}

public:

	//! Accessors
	CXInputWrapper& getXInputDevice() { return m_oXInputWrapper; }

	/**
	* Gets the how many XInput devices are plugged in
	* 
	* @date Created March 29, 2006
	*/
	inline int getNumXInputDevicesConnected(void)
	{
		int nNumDevices = 0;
		if (m_oXInputWrapper.isConnected(0))
			++nNumDevices;
		if (m_oXInputWrapper.isConnected(1))
			++nNumDevices;
		return nNumDevices;
	}

	/**
	* Gets the how many legacy Joypad devices are plugged in
	* 
	* @date Created March 29, 2006
	*/
	inline int getNumLegacyJoypadDevicesConnected(void)
	{
		int nNumDevices = 0;
		if (m_pDIDJoysticks[0])
			++nNumDevices;
		if (m_pDIDJoysticks[1])
			++nNumDevices;
		return nNumDevices;
	}

	/**
	* Gets the current mouse position
	* 
	* @date Created March 06, 2006
	*/
	inline D3DXVECTOR2 getMousePosition(void)
	{
		return m_CursorPos;
	}

	/**
	* Sets the current mouse position
	* 
	* @date Created June 17, 2006
	*/
	inline void setMousePosition(D3DXVECTOR2 mousePos)
	{
		m_CursorPos = mousePos;
	}

	/**
	* Gets the current mouse position
	* 
	* @date Created April 6, 2006
	*/
	inline D3DXVECTOR2* getMousePositionPtr(void)
	{
		return &m_CursorPos;
	}

	/**
	* Gets the change in mouse position
	* 
	* @date Created April 3, 2006
	*/
	inline D3DXVECTOR2 getMouseChange(void)
	{
		return D3DXVECTOR2((float)m_MouseChange.lX, (float)m_MouseChange.lY);
	}

	/**
	* Initializes Direct Input
	* 
	* @date Created March 06, 2006
	* @param[in]	hInstance		Instance of the window
	* @param[in]	hWnd			Handle to the window
	*/
	bool initializeDirectInput(HINSTANCE hInstance, HWND hWnd);

	/**
	* Polls the system for all input
	* 
	* @date Created March 06, 2006
	*/
	void updateInput(void);

	/**
	* Checks to see if a key is down
	* 
	* @date Created March 06, 2006
	* @param[in]	dwKey		The key to check for
	*/
	bool isKeyDown(DWORD dwKey);

	/**
	* Checks to see if a key is down (buffered)
	* 
	* @date Created March 27, 2006
	* @param[in]	dwKey		The key to check for
	*/
	bool isKeyDownBuffered(DWORD dwKey, ESystemInput eSystemInput = MAIN_SYSTEM);

	/**
	* Checks to see if the left mouse button is down
	* 
	* @date Created March 06, 2006
	* @return true if the left mouse button is down
	*/
	inline bool isLeftMouseButtonDown()
	{
		return m_bLeftMouseButtonDown;
	}

	/**
	* Checks to see if the right mouse button is down
	* 
	* @date Created March 06, 2006
	* @return true if the right mouse button is down
	*/
	inline bool isRightMouseButtonDown()
	{
		return m_bRightMouseButtonDown;
	}

	/**
	* Checks to see if a joystick is an XInput device
	* 
	* @date Created March 27, 2006
	* @param[in]	pGuidProductFromDirectInput		The GUID from Direct Input
	*/
	HRESULT IsXInputDevice(const GUID* pGuidProductFromDirectInput);

	/**
	* Enumerates through all of the connected joysticks and checks to see if they're XInput devices
	* 
	* @date Created March 27, 2006
	* @param[in]	pdidInstance	The instance of the device
	* @param[in]	pContext		not used, required for Direct Input CALLBACK
	* @return true to keep iterating or false to quit
	*/
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	/**
	* Gets the direction of a thumbstick on a legacy joypad
	* 
	* @date Created March 27, 2006
	* @param[in]	nIndex		The index to check for
	* @param[in]	eAxis		Which axis to check for
	* @return float of direction
	*/
	float getJoystickDirection(int nIndex, EXInputAxis eAxis);

	/**
	* Checks whether or not a button is down on a joypad
	* 
	* @date Created March 27, 2006
	* @param[in]	nIndex		The index to check for
	* @param[in]	Button		Button to look for
	* @return true if button is down
	*/
	bool isJoypadButtonDown(int nIndex, BYTE Button);

	/**
	* Checks whether or not a button is down on a joypad, buffered
	* 
	* @date Created March 27, 2006
	* @param[in]	nIndex		The index to check for
	* @param[in]	Button		Button to look for
	* @return true if button is down
	*/
	bool isJoypadButtonDownBuffered(int nIndex, BYTE Button);

	/**
	* Checks if any key is pressed
	* 
	* @date Created March 29, 2006
	* @param[in]	dwBind		the key to bind to whatever is pressed
	* @return true if any key is down
	*/
	bool getKey(DWORD &dwBind);

	/**
	* Checks if any joypad button is pressed
	* 
	* @date Created March 29, 2006
	* @param[in]	nIndex		The index to check for
	* @param[in]	dwBind		the key to bind to whatever is pressed
	* @return true if any joypad button is down
	*/
	bool getJoypadButton(int nIndex, DWORD &dwBind);

	/**
	* Checks if any XInput button is pressed
	* 
	* @date Created March 29, 2006
	* @param[in]	nIndex		The index to check for
	* @param[in]	dwBind		the key to bind to whatever is pressed
	* @return true if any XInput button is down
	*/
	bool getXInputButton(int nIndex, DWORD &dwBind);

	/**
	* Converts a DIK value to string
	* 
	* @date Created March 29, 2006
	* @param[in]	dwDIK		The key to check for
	* @return string of the associated key
	*/
	const char *convertDIKtoString(DWORD dwDIK);

	/**
	* Converts a DIK value to string from joypad
	* 
	* @date Created March 29, 2006
	* @param[in]	dwDIK		The key to check for
	* @return string of the associated key
	*/
	const char *convertDIKtoStringJoypad(DWORD dwDIK);

	/**
	* Converts a string to DIK Value
	* 
	* @date Created May 9, 2006
	* @param[in]	pString		The string to check for
	* @return DWORD of the associated string
	*/
	DWORD convertStringtoDIK(const char *pString);

	/**
	* Converts a string to DIK Value
	* 
	* @date Created May 9, 2006
	* @param[in]	pString		The string to check for
	* @return DWORD of the associated string
	*/
	DWORD convertStringtoXInput(const char *pString);

	/**
	* Converts a string to DIK value
	* 
	* @date Created May 11, 2006
	* @param[in]	pString		The string to check for
	* @return string of the associated key
	*/
	DWORD convertStringToJoypad(const char *pString);

	/**
	* Converts a DIK valueof XInput to string
	* 
	* @date Created March 29, 2006
	* @param[in]	dwDIK		The key to check for
	* @return string of the associated key
	*/
	const char *convertDIKtoStringXInput(DWORD dwDIK);

	/**
	* Checks to see if a key is alphanumeric
	* 
	* @date Created May 24, 2006
	* @param[in]	dwDIK		The key to check for
	* @return string of the associated key
	*/
	bool isAlphaNumeric(DWORD dwDIK);
};


#endif