/**
* @file DirectInputWrapper.cpp
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the Direct Input Wrapper class definition
* used for getting keyboard, mouse, and joystick (non XBOX360 controller) input
*/
#include "DirectInputWrapper.h"

#include <windows.h>
#include <oleauto.h>
#include <wbemidl.h>
#include <strsafe.h>
#include <objbase.h>

/**
* CDirectInputWrapper::CDirectInputWrapper
* @date Created March 06, 2006
*/
CDirectInputWrapper::CDirectInputWrapper()
{
	m_pDIObject = 0; m_pDIDKeyboard = 0; m_pDIDMouse = 0;
	for (unsigned int i = 0; i < XI_MAX_CONTROLLERS; ++i)
	{
		m_pDIDJoysticks[i] = 0;
	}
	m_CursorPos.x =	m_CursorPos.y = 0;
	m_bLeftMouseButtonDown = m_bRightMouseButtonDown = false;
	memset(m_ucBufferedKeys, 0, 256 * MAX_SYSTEMS);
	// clear out the key buffer
	memset(m_ucKeyboardBuffer, 0, sizeof(m_ucKeyboardBuffer));
}

/**
* CDirectInputWrapper::~CDirectInputWrapper
* @date Created March 06, 2006
*/
CDirectInputWrapper::~CDirectInputWrapper()
{
	SAFE_RELEASE(m_pDIObject);
	SAFE_RELEASE(m_pDIDKeyboard);
	SAFE_RELEASE(m_pDIDMouse);
	for (unsigned int i = 0; i < XI_MAX_CONTROLLERS; ++i)
	{
		if(m_pDIDJoysticks[i])
		{
			m_pDIDJoysticks[i]->Unacquire();
			SAFE_RELEASE(m_pDIDJoysticks[i]);
		}
	}
}

/**
* CDirectInputWrapper::initializeDirectInput
* @date Created March 06, 2006
*/
bool CDirectInputWrapper::initializeDirectInput(HINSTANCE hInstance, HWND hWnd)
{
	// Create the Direct Input Device
	HRESULT hResult;
	hResult = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pDIObject, NULL);
	// Make sure it didn't fail.
	if (hResult != DI_OK)
		return false;
	// create the keyboard device
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIDKeyboard, NULL);

	// Set cooperation level with the keyboard so that windows can use the keyboard.
	m_pDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pDIDKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// create the mouse device
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIDMouse, NULL);

	// setup Mouse to be exclusive to this program only so that windows doesn't use it
	m_pDIDMouse->SetDataFormat(&c_dfDIMouse);
	m_pDIDMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	m_hWnd = hWnd;

	// enum joysticks
	if (FAILED(m_pDIObject->EnumDevices(DI8DEVCLASS_GAMECTRL, CDirectInputWrapper::EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return false;

	// go through the joysticks and acquire them
	for (int i = 0; i < XI_MAX_CONTROLLERS; ++i)
	{
		if (m_pDIDJoysticks[i])
		{
			hResult = m_pDIDJoysticks[i]->SetDataFormat(&c_dfDIJoystick2);
			hResult = m_pDIDJoysticks[i]->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
			hResult = m_pDIDJoysticks[i]->Acquire();
		}
	}

	memset(m_JoystickStates, 0, sizeof(m_JoystickStates));

	return true;
}

/**
* CDirectInputWrapper::updateInput
* @date Created March 06, 2006
*/
void CDirectInputWrapper::updateInput(void)
{
	// clear out the key buffer
	memset(m_ucKeyboardBuffer, 0, sizeof(m_ucKeyboardBuffer));
	// acquire device & query state of keyboard
	m_pDIDKeyboard->Acquire();
	m_pDIDKeyboard->GetDeviceState(256, m_ucKeyboardBuffer);

	ZeroMemory(&m_MouseChange, sizeof(m_MouseChange));

	m_pDIDMouse->Acquire();
	m_pDIDMouse->GetDeviceState(sizeof(m_MouseChange), &m_MouseChange);

	// move the cursor pos with relative movement
	m_CursorPos.x += m_MouseChange.lX;
	m_CursorPos.y += m_MouseChange.lY;


	// left mouse button is down
	if (m_MouseChange.rgbButtons[0] & 0x80)
	{
		m_bLeftMouseButtonDown = true;
	}
	else
	{
		m_bLeftMouseButtonDown = false;
	}
	// right mouse button is down
	if (m_MouseChange.rgbButtons[1] & 0x80) 
	{ 
		m_bRightMouseButtonDown = true;
	}
	else 
	{
		m_bRightMouseButtonDown = false;
	}

	// update joypads
	m_oXInputWrapper.updateControllers();
	HRESULT hr;

	for (int i = 0; i < XI_MAX_CONTROLLERS; ++i)
	{
		if (m_pDIDJoysticks[i])
		{
			// Poll the joystick for new input
			hr = m_pDIDJoysticks[i]->Poll();
			
			if (FAILED(hr))
			{
				// we lost the device, so keep trying to acquire if we can't get it.
				hr = m_pDIDJoysticks[i]->Acquire();
				while (hr == DIERR_INPUTLOST) { m_pDIDJoysticks[i]->Acquire(); }
				return;
			}

			hr = m_pDIDJoysticks[i]->GetDeviceState(sizeof(DIJOYSTATE2), &m_JoystickStates[i]);
			if (FAILED(hr))
				return;

			// check for dead zone and make the values of the axis in range of -1 to 1.
			m_JoystickStates[i].lX -= 32767;
			m_JoystickStates[i].lY -= 32767;
			m_JoystickStates[i].lZ -= 32767;
			m_JoystickStates[i].lRz -= 32767;
			if (m_JoystickStates[i].lX < 0 && m_JoystickStates[i].lX > -5000)
				m_JoystickStates[i].lX = 0;
			if (m_JoystickStates[i].lX > 0 && m_JoystickStates[i].lX < 5000)
				m_JoystickStates[i].lX = 0;
			if (m_JoystickStates[i].lY < 0 && m_JoystickStates[i].lY > -5000)
				m_JoystickStates[i].lY = 0;
			if (m_JoystickStates[i].lY > 0 && m_JoystickStates[i].lY < 5000)
				m_JoystickStates[i].lY = 0;
			if (m_JoystickStates[i].lZ < 0 && m_JoystickStates[i].lZ > -5000)
				m_JoystickStates[i].lZ = 0;
			if (m_JoystickStates[i].lZ > 0 && m_JoystickStates[i].lZ < 5000)
				m_JoystickStates[i].lZ = 0;
			if (m_JoystickStates[i].lRz < 0 && m_JoystickStates[i].lRz > -5000)
				m_JoystickStates[i].lRz = 0;
			if (m_JoystickStates[i].lRz > 0 && m_JoystickStates[i].lRz < 5000)
				m_JoystickStates[i].lRz = 0;
		}
	}
}

/**
* CDirectInputWrapper::isKeyDown
* @date Created March 06, 2006
*/
bool CDirectInputWrapper::isKeyDown(DWORD dwKey)
{
	if (KEYDOWN(m_ucKeyboardBuffer, dwKey))
	{
		return true;
	}
	
	return false;
}

/**
* CDirectInputWrapper::isKeyDownBuffered
* @date Created March 27, 2006
*/
bool CDirectInputWrapper::isKeyDownBuffered(DWORD dwKey, ESystemInput eSystemInput)
{
	if (isKeyDown(dwKey))
	{
		if (!m_ucBufferedKeys[eSystemInput][dwKey])
		{
			m_ucBufferedKeys[eSystemInput][dwKey] = 1;
			return true;
		}
	}
	else if (!isKeyDown(dwKey))
		m_ucBufferedKeys[eSystemInput][dwKey] = 0;

	return false;
}

/**
* CDirectInputWrapper::IsXInputDevice
* @date Created March 27, 2006
*/
HRESULT CDirectInputWrapper::IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
// Unfortunately this information can not be found by just using DirectInput 
//-----------------------------------------------------------------------------
    IWbemLocator*           pIWbemLocator  = NULL;
    IEnumWbemClassObject*   pEnumDevices   = NULL;
    IWbemClassObject*       pDevices[20]   = {0};
    IWbemServices*          pIWbemServices = NULL;
    BSTR                    bstrNamespace  = NULL;
    BSTR                    bstrDeviceID   = NULL;
    BSTR                    bstrClassName  = NULL;
    DWORD                   uReturned      = 0;
    bool                    bIsXinputDevice= false;
    UINT                    iDevice        = 0;
    VARIANT                 var;
    HRESULT                 hr;

    // CoInit if needed
    hr = CoInitialize(NULL);
    bool bCleanupCOM = SUCCEEDED(hr);

    // Create WMI
    hr = CoCreateInstance( __uuidof(WbemLocator),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof(IWbemLocator),
                           (LPVOID*) &pIWbemLocator);
    if( FAILED(hr) || pIWbemLocator == NULL )
        goto LCleanup;

    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if( bstrNamespace == NULL ) goto LCleanup;        
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if( bstrClassName == NULL ) goto LCleanup;        
    bstrDeviceID  = SysAllocString( L"DeviceID" );          if( bstrDeviceID == NULL )  goto LCleanup;        
    
    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED(hr) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE. 
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );        

    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 
    if( FAILED(hr) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for( ;; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED(hr) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice=0; iDevice<uReturned; iDevice++ )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it's an XInput device
				    // This information can not be found from DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    // Compare the VID/PID to the DInput device
                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );
                    if( dwVidPid == pGuidProductFromDirectInput->Data1 )
                    {
                        bIsXinputDevice = true;
                        goto LCleanup;
                    }
                }
            }   
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if(bstrNamespace)
        SysFreeString(bstrNamespace);
    if(bstrDeviceID)
        SysFreeString(bstrDeviceID);
    if(bstrClassName)
        SysFreeString(bstrClassName);
    for( iDevice=0; iDevice<20; iDevice++ )
        SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );

    if( bCleanupCOM )
        CoUninitialize();

    return bIsXinputDevice;
}


/**
* CDirectInputWrapper::EnumJoysticksCallback
* @date Created March 27, 2006
*/
BOOL CALLBACK CDirectInputWrapper::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	static int nIndex = 0;

	if(CDirectInputWrapper::getInstancePtr()->IsXInputDevice(&pdidInstance->guidProduct))
        return DIENUM_CONTINUE;

	// Device is verified not XInput, so add it to the list of DInput devices
	CDirectInputWrapper::getInstancePtr()->m_pDIObject->CreateDevice(pdidInstance->guidInstance, 
		&CDirectInputWrapper::getInstancePtr()->m_pDIDJoysticks[nIndex], NULL);

	++nIndex;

	return DIENUM_CONTINUE;
}

/**
* CDirectInputWrapper::getJoystickDirection
* @date Created March 27, 2006
*/
float CDirectInputWrapper::getJoystickDirection(int nIndex, EXInputAxis eAxis)
{
	// make sure index is valid
	if (nIndex < 0 || nIndex > 1)
		return 0.0f;

	switch (eAxis)
	{
	case XIA_LEFT_X_AXIS:
		return (float)(m_JoystickStates[nIndex].lX) / 32767.0f;
	case XIA_LEFT_Y_AXIS:
		return (float)(m_JoystickStates[nIndex].lY) / 32767.0f;
	case XIA_RIGHT_X_AXIS:
		return (float)(m_JoystickStates[nIndex].lZ) / 32767.0f;
	case XIA_RIGHT_Y_AXIS:
		return (float)(m_JoystickStates[nIndex].lRz) / 32767.0f;
	default:
		break;
	}
	return 0.0f;
}

/**
* CDirectInputWrapper::isJoypadButtonDown
* @date Created March 27, 2006
*/
bool CDirectInputWrapper::isJoypadButtonDown(int nIndex, BYTE Button)
{
	// make sure index is valid
	if (nIndex < 0 || nIndex > 1)
		return false;
	// make sure the button is valid
	if (Button < 0 || Button > 127)
		return false;

	if (m_JoystickStates[nIndex].rgbButtons[Button] & 0x80)
		return true;

	return false;
}

/**
* CDirectInputWrapper::isJoypadButtonDownBuffered
* @date Created March 27, 2006
*/
bool CDirectInputWrapper::isJoypadButtonDownBuffered(int nIndex, BYTE Button)
{
	static BYTE sBufferedButton = 0;

	// make sure index is valid
	if (nIndex < 0 || nIndex > 1)
		return false;
	// make sure the button is valid
	if (Button < 0 || Button > 127)
		return false;

	// if the key got lifted, clear it out
	if (!(m_JoystickStates[nIndex].rgbButtons[Button] & 0x80))
	{
		sBufferedButton = 0;
	}

	if (m_JoystickStates[nIndex].rgbButtons[Button] & 0x80)
	{
		if (sBufferedButton != Button)
		{
			sBufferedButton = Button;
			return true;
		}
	}

	return false;
}

/**
* CDirectInputWrapper::getKey
* @date Created March 29, 2006
*/
bool CDirectInputWrapper::getKey(DWORD &dwBind)
{
	for (int i = 0; i < 256; ++i)
		if (isKeyDownBuffered(i, INPUT_SYSTEM))
		{
			dwBind = i;
			return true;
		}
	// Nothing hit
	return false;
}

/**
* CDirectInputWrapper::getJoypadButton
* @date Created March 29, 2006
*/
bool CDirectInputWrapper::getJoypadButton(int nIndex, DWORD &dwBind)
{
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON0))			{ dwBind = DIJOFS_BUTTON0;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON1))			{ dwBind = DIJOFS_BUTTON1;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON2))			{ dwBind = DIJOFS_BUTTON2;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON3))			{ dwBind = DIJOFS_BUTTON3;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON4))			{ dwBind = DIJOFS_BUTTON4;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON5))			{ dwBind = DIJOFS_BUTTON5;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON6))			{ dwBind = DIJOFS_BUTTON6;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON7))			{ dwBind = DIJOFS_BUTTON7;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON8))			{ dwBind = DIJOFS_BUTTON8;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON9))			{ dwBind = DIJOFS_BUTTON9;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON10))		{ dwBind = DIJOFS_BUTTON10;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON11))		{ dwBind = DIJOFS_BUTTON11;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON12))		{ dwBind = DIJOFS_BUTTON12;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON13))		{ dwBind = DIJOFS_BUTTON13;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON14))		{ dwBind = DIJOFS_BUTTON14;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON15))		{ dwBind = DIJOFS_BUTTON15;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON16))		{ dwBind = DIJOFS_BUTTON16;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON17))		{ dwBind = DIJOFS_BUTTON17;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON18))		{ dwBind = DIJOFS_BUTTON18;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON19))		{ dwBind = DIJOFS_BUTTON19;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON20))		{ dwBind = DIJOFS_BUTTON20;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON21))		{ dwBind = DIJOFS_BUTTON21;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON22))		{ dwBind = DIJOFS_BUTTON22;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON23))		{ dwBind = DIJOFS_BUTTON23;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON24))		{ dwBind = DIJOFS_BUTTON24;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON25))		{ dwBind = DIJOFS_BUTTON25;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON26))		{ dwBind = DIJOFS_BUTTON26;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON27))		{ dwBind = DIJOFS_BUTTON27;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON28))		{ dwBind = DIJOFS_BUTTON28;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON29))		{ dwBind = DIJOFS_BUTTON29;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON30))		{ dwBind = DIJOFS_BUTTON30;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_BUTTON31))		{ dwBind = DIJOFS_BUTTON31;		return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_RX))				{ dwBind = DIJOFS_RX;			return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_RY))				{ dwBind = DIJOFS_RY;			return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_RZ))				{ dwBind = DIJOFS_RZ;			return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_X))				{ dwBind = DIJOFS_X;			return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_Y))				{ dwBind = DIJOFS_Y;			return true; }
	if (isJoypadButtonDownBuffered(nIndex, DIJOFS_Z))				{ dwBind = DIJOFS_Z;			return true; }

	// Nothing hit
	return false;
}

/**
* CDirectInputWrapper::getXInputButton
* @date Created March 29, 2006
*/
bool CDirectInputWrapper::getXInputButton(int nIndex, DWORD &dwBind)
{
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_DPAD_UP))			
		{ dwBind = XINPUT_GAMEPAD_DPAD_UP;				return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_DPAD_DOWN))		
		{ dwBind = XINPUT_GAMEPAD_DPAD_DOWN;			return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_DPAD_LEFT))		
		{ dwBind = XINPUT_GAMEPAD_DPAD_LEFT;			return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_DPAD_RIGHT))		
		{ dwBind = XINPUT_GAMEPAD_DPAD_RIGHT;			return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_START))			
		{ dwBind = XINPUT_GAMEPAD_START;				return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_BACK))				
		{ dwBind = XINPUT_GAMEPAD_BACK;					return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_LEFT_THUMB))		
		{ dwBind = XINPUT_GAMEPAD_LEFT_THUMB;			return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_RIGHT_THUMB))		
		{ dwBind = XINPUT_GAMEPAD_RIGHT_THUMB;			return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_LEFT_SHOULDER))	
		{ dwBind = XINPUT_GAMEPAD_LEFT_SHOULDER;		return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_RIGHT_SHOULDER))	
		{ dwBind = XINPUT_GAMEPAD_RIGHT_SHOULDER;		return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_A))				
		{ dwBind = XINPUT_GAMEPAD_A;					return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_B))				
		{ dwBind = XINPUT_GAMEPAD_B;					return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_X))				
		{ dwBind = XINPUT_GAMEPAD_X;					return true; }
	if (m_oXInputWrapper.isButtonDownBuffered(nIndex, XINPUT_GAMEPAD_Y))				
		{ dwBind = XINPUT_GAMEPAD_Y;					return true; }
	if (m_oXInputWrapper.getTriggerData(nIndex, true))
		{ dwBind = XIT_LEFT_TRIGGER;					return true; }
	if (m_oXInputWrapper.getTriggerData(nIndex, false))
		{ dwBind = XIT_RIGHT_TRIGGER;					return true; }
	if (m_oXInputWrapper.getThumbstickDirection(nIndex, XIA_LEFT_X_AXIS) < -0.5f)
		{ dwBind = XID_LEFT;							return true; }
	if (m_oXInputWrapper.getThumbstickDirection(nIndex, XIA_LEFT_X_AXIS) > 0.5f)
		{ dwBind = XID_RIGHT;							return true; }
	if (m_oXInputWrapper.getThumbstickDirection(nIndex, XIA_LEFT_Y_AXIS) < -0.5f)
		{ dwBind = XID_DOWN;							return true; }
	if (m_oXInputWrapper.getThumbstickDirection(nIndex, XIA_LEFT_Y_AXIS) > 0.5f)
		{ dwBind = XID_UP;								return true; }

	// Nothing hit
	return false;
}

/**
* CDirectInputWrapper::convertDIKtoString
* @date Created March 29, 2006
*/
const char *CDirectInputWrapper::convertDIKtoString(DWORD dwDIK)
{
	if (dwDIK == DIK_0)					{ return "0"; }
	else if (dwDIK == DIK_1)			{ return "1"; }
	else if (dwDIK == DIK_2)			{ return "2"; }
	else if (dwDIK == DIK_3)			{ return "3"; }
	else if (dwDIK == DIK_4)			{ return "4"; }
	else if (dwDIK == DIK_5)			{ return "5"; }
	else if (dwDIK == DIK_6)			{ return "6"; }
	else if (dwDIK == DIK_7)			{ return "7"; }
	else if (dwDIK == DIK_8)			{ return "8"; }
	else if (dwDIK == DIK_9)			{ return "9"; }
	else if (dwDIK == DIK_A)			{ return "A"; }
	else if (dwDIK == DIK_B)			{ return "B"; }
	else if (dwDIK == DIK_C)			{ return "C"; }
	else if (dwDIK == DIK_D)			{ return "D"; }
	else if (dwDIK == DIK_E)			{ return "E"; }
	else if (dwDIK == DIK_F)			{ return "F"; }
	else if (dwDIK == DIK_G)			{ return "G"; }
	else if (dwDIK == DIK_H)			{ return "H"; }
	else if (dwDIK == DIK_I)			{ return "I"; }
	else if (dwDIK == DIK_J)			{ return "J"; }
	else if (dwDIK == DIK_K)			{ return "K"; }
	else if (dwDIK == DIK_L)			{ return "L"; }
	else if (dwDIK == DIK_M)			{ return "M"; }
	else if (dwDIK == DIK_N)			{ return "N"; }
	else if (dwDIK == DIK_O)			{ return "O"; }
	else if (dwDIK == DIK_P)			{ return "P"; }
	else if (dwDIK == DIK_Q)			{ return "Q"; }
	else if (dwDIK == DIK_R)			{ return "R"; }
	else if (dwDIK == DIK_S)			{ return "S"; }
	else if (dwDIK == DIK_T)			{ return "T"; }
	else if (dwDIK == DIK_U)			{ return "U"; }
	else if (dwDIK == DIK_V)			{ return "V"; }
	else if (dwDIK == DIK_W)			{ return "W"; }
	else if (dwDIK == DIK_X)			{ return "X"; }
	else if (dwDIK == DIK_Y)			{ return "Y"; }
	else if (dwDIK == DIK_Z)			{ return "Z"; }
	else if (dwDIK == DIK_LSHIFT)		{ return "LSHIFT"; }
	else if (dwDIK == DIK_LCONTROL)		{ return "LCONTROL"; }
	else if (dwDIK == DIK_LWIN)			{ return "LWIN"; }
	else if (dwDIK == DIK_LALT)			{ return "LALT"; }
	else if (dwDIK == DIK_RALT)			{ return "RALT"; }
	else if (dwDIK == DIK_RSHIFT)		{ return "RSHIFT"; }
	else if (dwDIK == DIK_RCONTROL)		{ return "RCONTROL"; }
	else if (dwDIK == DIK_ESCAPE)		{ return "ESC"; }
	else if (dwDIK == DIK_MINUS)		{ return "MINUS"; }
	else if (dwDIK == DIK_EQUALS)		{ return "EQUALS"; }
	else if (dwDIK == DIK_BACK)			{ return "BACK"; }
	else if (dwDIK == DIK_TAB)			{ return "TAB"; }
	else if (dwDIK == DIK_LBRACKET)		{ return "LBRACKET"; }
	else if (dwDIK == DIK_RBRACKET)		{ return "RBRACKET"; }
	else if (dwDIK == DIK_RETURN)		{ return "RETURN"; }
	else if (dwDIK == DIK_SEMICOLON)	{ return "SEMICOLON"; }
	else if (dwDIK == DIK_APOSTROPHE)	{ return "APOSTROPHE"; }
	else if (dwDIK == DIK_GRAVE)		{ return "TILDE"; }
	else if (dwDIK == DIK_BACKSLASH)	{ return "BACKSLASH"; }
	else if (dwDIK == DIK_COMMA)		{ return "COMMA"; }
	else if (dwDIK == DIK_PERIOD)		{ return "PERIOD"; }
	else if (dwDIK == DIK_SLASH)		{ return "SLASH"; }
	else if (dwDIK == DIK_MULTIPLY)		{ return "ASTERISK"; }
	else if (dwDIK == DIK_SPACE)		{ return "SPACE"; }
	else if (dwDIK == DIK_CAPITAL)		{ return "CAPS"; }
	else if (dwDIK == DIK_F1)			{ return "F1"; }
	else if (dwDIK == DIK_F2)			{ return "F2"; }
	else if (dwDIK == DIK_F3)			{ return "F3"; }
	else if (dwDIK == DIK_F4)			{ return "F4"; }
	else if (dwDIK == DIK_F5)			{ return "F5"; }
	else if (dwDIK == DIK_F6)			{ return "F6"; }
	else if (dwDIK == DIK_F7)			{ return "F7"; }
	else if (dwDIK == DIK_F8)			{ return "F8"; }
	else if (dwDIK == DIK_F9)			{ return "F9"; }
	else if (dwDIK == DIK_F10)			{ return "F10"; }
	else if (dwDIK == DIK_F11)			{ return "F11"; }
	else if (dwDIK == DIK_F12)			{ return "F12"; }
	else if (dwDIK == DIK_F13)			{ return "F13"; }
	else if (dwDIK == DIK_F14)			{ return "F14"; }
	else if (dwDIK == DIK_F15)			{ return "F15"; }
	else if (dwDIK == DIK_NUMPAD0)		{ return "NUM0"; }
	else if (dwDIK == DIK_NUMPAD1)		{ return "NUM1"; }
	else if (dwDIK == DIK_NUMPAD2)		{ return "NUM2"; }
	else if (dwDIK == DIK_NUMPAD3)		{ return "NUM3"; }
	else if (dwDIK == DIK_NUMPAD4)		{ return "NUM4"; }
	else if (dwDIK == DIK_NUMPAD5)		{ return "NUM5"; }
	else if (dwDIK == DIK_NUMPAD6)		{ return "NUM6"; }
	else if (dwDIK == DIK_NUMPAD7)		{ return "NUM7"; }
	else if (dwDIK == DIK_NUMPAD8)		{ return "NUM8"; }
	else if (dwDIK == DIK_NUMPAD9)		{ return "NUM9"; }
	else if (dwDIK == DIK_ADD)			{ return "ADD"; }
	else if (dwDIK == DIK_SUBTRACT)		{ return "SUBTRACT"; }
	else if (dwDIK == DIK_DECIMAL)		{ return "DECIMAL"; }
	else if (dwDIK == DIK_PAUSE)		{ return "PAUSE"; }
	else if (dwDIK == DIK_HOME)			{ return "HOME"; }
	else if (dwDIK == DIK_END)			{ return "END"; }
	else if (dwDIK == DIK_PRIOR)		{ return "PGUP"; }
	else if (dwDIK == DIK_NEXT)			{ return "PGDOWN"; }
	else if (dwDIK == DIK_UP)			{ return "UP"; }
	else if (dwDIK == DIK_DOWN)			{ return "DOWN"; }
	else if (dwDIK == DIK_LEFT)			{ return "LEFT"; }
	else if (dwDIK == DIK_RIGHT)		{ return "RIGHT"; }
	else if (dwDIK == DIK_INSERT)		{ return "INSERT"; }
	else if (dwDIK == DIK_DELETE)		{ return "DEL"; }
	else if (dwDIK == LMOUSE)			{ return "LMOUSE"; }
	else if (dwDIK == RMOUSE)			{ return "RMOUSE"; }
	else
		return "ERROR";
}

/**
* CDirectInputWrapper::convertStringtoDIK
* @date Created May 9, 2006
*/
DWORD CDirectInputWrapper::convertStringtoDIK(const char *pString)
{
	if (!pString)
		return 0;

	if (!strcmp(pString, "0"))				{ return DIK_0; }
	else if (!strcmp(pString, "1"))			{ return DIK_1; }
	else if (!strcmp(pString, "2"))			{ return DIK_2; }
	else if (!strcmp(pString, "3"))			{ return DIK_3; }
	else if (!strcmp(pString, "4"))			{ return DIK_4; }
	else if (!strcmp(pString, "5"))			{ return DIK_5; }
	else if (!strcmp(pString, "6"))			{ return DIK_6; }
	else if (!strcmp(pString, "7"))			{ return DIK_7; }
	else if (!strcmp(pString, "8"))			{ return DIK_8; }
	else if (!strcmp(pString, "9"))			{ return DIK_9; }
	else if (!strcmp(pString, "A"))			{ return DIK_A; }
	else if (!strcmp(pString, "B"))			{ return DIK_B; }
	else if (!strcmp(pString, "C"))			{ return DIK_C; }
	else if (!strcmp(pString, "D"))			{ return DIK_D; }
	else if (!strcmp(pString, "E"))			{ return DIK_E; }
	else if (!strcmp(pString, "F"))			{ return DIK_F; }
	else if (!strcmp(pString, "G"))			{ return DIK_G; }
	else if (!strcmp(pString, "H"))			{ return DIK_H; }
	else if (!strcmp(pString, "I"))			{ return DIK_I; }
	else if (!strcmp(pString, "J"))			{ return DIK_J; }
	else if (!strcmp(pString, "K"))			{ return DIK_K; }
	else if (!strcmp(pString, "L"))			{ return DIK_L; }
	else if (!strcmp(pString, "M"))			{ return DIK_M; }
	else if (!strcmp(pString, "N"))			{ return DIK_N; }
	else if (!strcmp(pString, "O"))			{ return DIK_O; }
	else if (!strcmp(pString, "P"))			{ return DIK_P; }
	else if (!strcmp(pString, "Q"))			{ return DIK_Q; }
	else if (!strcmp(pString, "R"))			{ return DIK_R; }
	else if (!strcmp(pString, "S"))			{ return DIK_S; }
	else if (!strcmp(pString, "T"))			{ return DIK_T; }
	else if (!strcmp(pString, "U"))			{ return DIK_U; }
	else if (!strcmp(pString, "V"))			{ return DIK_V; }
	else if (!strcmp(pString, "W"))			{ return DIK_W; }
	else if (!strcmp(pString, "X"))			{ return DIK_X; }
	else if (!strcmp(pString, "Y"))			{ return DIK_Y; }
	else if (!strcmp(pString, "Z"))			{ return DIK_Z; }
	else if (!strcmp(pString, "LSHIFT"))	{ return DIK_LSHIFT; }
	else if (!strcmp(pString, "LCONTROL"))	{ return DIK_LCONTROL; }
	else if (!strcmp(pString, "LWIN"))		{ return DIK_LWIN; }
	else if (!strcmp(pString, "LALT"))		{ return DIK_LALT; }
	else if (!strcmp(pString, "RALT"))		{ return DIK_RALT; }
	else if (!strcmp(pString, "RSHIFT"))	{ return DIK_RSHIFT; }
	else if (!strcmp(pString, "RCONTROL"))	{ return DIK_RCONTROL; }
	else if (!strcmp(pString, "ESC"))		{ return DIK_ESCAPE; }
	else if (!strcmp(pString, "MINUS"))		{ return DIK_MINUS; }
	else if (!strcmp(pString, "EQUALS"))	{ return DIK_EQUALS; }
	else if (!strcmp(pString, "BACK"))		{ return DIK_BACK; }
	else if (!strcmp(pString, "TAB"))		{ return DIK_TAB; }
	else if (!strcmp(pString, "LBRACKET"))	{ return DIK_LBRACKET; }
	else if (!strcmp(pString, "RBRACKET"))	{ return DIK_RBRACKET; }
	else if (!strcmp(pString, "RETURN"))	{ return DIK_RETURN; }
	else if (!strcmp(pString, "SEMICOLON"))	{ return DIK_SEMICOLON; }
	else if (!strcmp(pString, "APOSTROPHE")){ return DIK_APOSTROPHE; }
	else if (!strcmp(pString, "TILDE"))		{ return DIK_GRAVE; }
	else if (!strcmp(pString, "BACKSLASH"))	{ return DIK_BACKSLASH; }
	else if (!strcmp(pString, "COMMA"))		{ return DIK_COMMA; }
	else if (!strcmp(pString, "PERIOD"))	{ return DIK_PERIOD; }
	else if (!strcmp(pString, "SLASH"))		{ return DIK_SLASH; }
	else if (!strcmp(pString, "ASTERISK"))	{ return DIK_MULTIPLY; }
	else if (!strcmp(pString, "SPACE"))		{ return DIK_SPACE; }
	else if (!strcmp(pString, "CAPS"))		{ return DIK_CAPITAL; }
	else if (!strcmp(pString, "F1"))		{ return DIK_F1; }
	else if (!strcmp(pString, "F2"))		{ return DIK_F2; }
	else if (!strcmp(pString, "F3"))		{ return DIK_F3; }
	else if (!strcmp(pString, "F4"))		{ return DIK_F4; }
	else if (!strcmp(pString, "F5"))		{ return DIK_F5; }
	else if (!strcmp(pString, "F6"))		{ return DIK_F6; }
	else if (!strcmp(pString, "F7"))		{ return DIK_F7; }
	else if (!strcmp(pString, "F8"))		{ return DIK_F8; }
	else if (!strcmp(pString, "F9"))		{ return DIK_F9; }
	else if (!strcmp(pString, "F10"))		{ return DIK_F10; }
	else if (!strcmp(pString, "F11"))		{ return DIK_F11; }
	else if (!strcmp(pString, "F12"))		{ return DIK_F12; }
	else if (!strcmp(pString, "F13"))		{ return DIK_F13; }
	else if (!strcmp(pString, "F14"))		{ return DIK_F14; }
	else if (!strcmp(pString, "F15"))		{ return DIK_F15; }
	else if (!strcmp(pString, "NUM0"))		{ return DIK_NUMPAD0; }
	else if (!strcmp(pString, "NUM1"))		{ return DIK_NUMPAD1; }
	else if (!strcmp(pString, "NUM2"))		{ return DIK_NUMPAD2; }
	else if (!strcmp(pString, "NUM3"))		{ return DIK_NUMPAD3; }
	else if (!strcmp(pString, "NUM4"))		{ return DIK_NUMPAD4; }
	else if (!strcmp(pString, "NUM5"))		{ return DIK_NUMPAD5; }
	else if (!strcmp(pString, "NUM6"))		{ return DIK_NUMPAD6; }
	else if (!strcmp(pString, "NUM7"))		{ return DIK_NUMPAD7; }
	else if (!strcmp(pString, "NUM8"))		{ return DIK_NUMPAD8; }
	else if (!strcmp(pString, "NUM9"))		{ return DIK_NUMPAD9; }
	else if (!strcmp(pString, "ADD"))		{ return DIK_ADD; }
	else if (!strcmp(pString, "SUBTRACT"))	{ return DIK_SUBTRACT; }
	else if (!strcmp(pString, "DECIMAL"))	{ return DIK_DECIMAL; }
	else if (!strcmp(pString, "PAUSE"))		{ return DIK_PAUSE; }
	else if (!strcmp(pString, "HOME"))		{ return DIK_HOME; }
	else if (!strcmp(pString, "END"))		{ return DIK_END; }
	else if (!strcmp(pString, "PGUP"))		{ return DIK_PRIOR; }
	else if (!strcmp(pString, "PGDOWN"))	{ return DIK_NEXT; }
	else if (!strcmp(pString, "UP"))		{ return DIK_UP; }
	else if (!strcmp(pString, "DOWN"))		{ return DIK_DOWN; }
	else if (!strcmp(pString, "LEFT"))		{ return DIK_LEFT; }
	else if (!strcmp(pString, "RIGHT"))		{ return DIK_RIGHT; }
	else if (!strcmp(pString, "INSERT"))	{ return DIK_INSERT; }
	else if (!strcmp(pString, "DEL"))		{ return DIK_DELETE; }
	else if (!strcmp(pString, "LMOUSE"))	{ return LMOUSE; }
	else if (!strcmp(pString, "RMOUSE"))	{ return RMOUSE; }
	else
		return -1;
}

/**
* CDirectInputWrapper::convertDIKtoStringJoypad
* @date Created March 29, 2006
*/
const char *CDirectInputWrapper::convertDIKtoStringJoypad(DWORD dwDIK)
{
	if (dwDIK == DIJOFS_BUTTON0)			{ return "JOYPAD_BUTTON0"; }
	else if (dwDIK == DIJOFS_BUTTON1)		{ return "JOYPAD_BUTTON1"; }
	else if (dwDIK == DIJOFS_BUTTON2)		{ return "JOYPAD_BUTTON2"; }
	else if (dwDIK == DIJOFS_BUTTON3)		{ return "JOYPAD_BUTTON3"; }
	else if (dwDIK == DIJOFS_BUTTON4)		{ return "JOYPAD_BUTTON4"; }
	else if (dwDIK == DIJOFS_BUTTON5)		{ return "JOYPAD_BUTTON5"; }
	else if (dwDIK == DIJOFS_BUTTON6)		{ return "JOYPAD_BUTTON6"; }
	else if (dwDIK == DIJOFS_BUTTON7)		{ return "JOYPAD_BUTTON7"; }
	else if (dwDIK == DIJOFS_BUTTON8)		{ return "JOYPAD_BUTTON8"; }
	else if (dwDIK == DIJOFS_BUTTON9)		{ return "JOYPAD_BUTTON9"; }
	else if (dwDIK == DIJOFS_BUTTON10)		{ return "JOYPAD_BUTTON10"; }
	else if (dwDIK == DIJOFS_BUTTON11)		{ return "JOYPAD_BUTTON11"; }
	else if (dwDIK == DIJOFS_BUTTON12)		{ return "JOYPAD_BUTTON12"; }
	else if (dwDIK == DIJOFS_BUTTON13)		{ return "JOYPAD_BUTTON13"; }
	else if (dwDIK == DIJOFS_BUTTON14)		{ return "JOYPAD_BUTTON14"; }
	else if (dwDIK == DIJOFS_BUTTON15)		{ return "JOYPAD_BUTTON15"; }
	else if (dwDIK == DIJOFS_BUTTON16)		{ return "JOYPAD_BUTTON16"; }
	else if (dwDIK == DIJOFS_BUTTON17)		{ return "JOYPAD_BUTTON17"; }
	else if (dwDIK == DIJOFS_BUTTON18)		{ return "JOYPAD_BUTTON18"; }
	else if (dwDIK == DIJOFS_BUTTON19)		{ return "JOYPAD_BUTTON19"; }
	else if (dwDIK == DIJOFS_BUTTON20)		{ return "JOYPAD_BUTTON20"; }
	else if (dwDIK == DIJOFS_BUTTON21)		{ return "JOYPAD_BUTTON21"; }
	else if (dwDIK == DIJOFS_BUTTON22)		{ return "JOYPAD_BUTTON22"; }
	else if (dwDIK == DIJOFS_BUTTON23)		{ return "JOYPAD_BUTTON23"; }
	else if (dwDIK == DIJOFS_BUTTON24)		{ return "JOYPAD_BUTTON24"; }
	else if (dwDIK == DIJOFS_BUTTON25)		{ return "JOYPAD_BUTTON25"; }
	else if (dwDIK == DIJOFS_BUTTON26)		{ return "JOYPAD_BUTTON26"; }
	else if (dwDIK == DIJOFS_BUTTON27)		{ return "JOYPAD_BUTTON27"; }
	else if (dwDIK == DIJOFS_BUTTON28)		{ return "JOYPAD_BUTTON28"; }
	else if (dwDIK == DIJOFS_BUTTON29)		{ return "JOYPAD_BUTTON29"; }
	else if (dwDIK == DIJOFS_BUTTON30)		{ return "JOYPAD_BUTTON30"; }
	else if (dwDIK == DIJOFS_BUTTON31)		{ return "JOYPAD_BUTTON31"; }
	else if (dwDIK == DIJOFS_RX)			{ return "JOYPAD_RX"; }
	else if (dwDIK == DIJOFS_RY)			{ return "JOYPAD_RY"; }
	else if (dwDIK == DIJOFS_RZ)			{ return "JOYPAD_RZ"; }
	else if (dwDIK == DIJOFS_X)				{ return "JOYPAD_X"; }
	else if (dwDIK == DIJOFS_Y)				{ return "JOYPAD_Y"; }
	else if (dwDIK == DIJOFS_Z)				{ return "JOYPAD_Z"; }
	else
		return "ERROR";
}

/**
* CDirectInputWrapper::convertDIKtoStringXInput
* @date Created March 29, 2006
*/
const char *CDirectInputWrapper::convertDIKtoStringXInput(DWORD dwDIK)
{
	if (dwDIK == XINPUT_GAMEPAD_DPAD_UP)				{ return "XBOX360_UP"; }
	else if (dwDIK == XINPUT_GAMEPAD_DPAD_DOWN)			{ return "XBOX360_DOWN"; }
	else if (dwDIK == XINPUT_GAMEPAD_DPAD_LEFT)			{ return "XBOX360_LEFT"; }
	else if (dwDIK == XINPUT_GAMEPAD_DPAD_RIGHT)		{ return "XBOX360_RIGHT"; }
	else if (dwDIK == XINPUT_GAMEPAD_START)				{ return "XBOX360_START"; }
	else if (dwDIK == XINPUT_GAMEPAD_BACK)				{ return "XBOX360_BACK"; }
	else if (dwDIK == XINPUT_GAMEPAD_LEFT_THUMB)		{ return "XBOX360_LTHUMB"; }
	else if (dwDIK == XINPUT_GAMEPAD_RIGHT_THUMB)		{ return "XBOX360_RTHUMB"; }
	else if (dwDIK == XINPUT_GAMEPAD_LEFT_SHOULDER)		{ return "XBOX360_LSHOULDER"; }
	else if (dwDIK == XINPUT_GAMEPAD_RIGHT_SHOULDER)	{ return "XBOX360_RSHOULDER"; }
	else if (dwDIK == XINPUT_GAMEPAD_A)					{ return "XBOX360_A"; }
	else if (dwDIK == XINPUT_GAMEPAD_B)					{ return "XBOX360_B"; }
	else if (dwDIK == XINPUT_GAMEPAD_X)					{ return "XBOX360_X"; }
	else if (dwDIK == XINPUT_GAMEPAD_Y)					{ return "XBOX360_Y"; }
	else if (dwDIK == XIT_LEFT_TRIGGER)					{ return "XBOX360_LTRIGGER"; }
	else if (dwDIK == XIT_RIGHT_TRIGGER)				{ return "XBOX360_RTRIGGER"; }
	else if (dwDIK == XID_LEFT)							{ return "XBOX360_LEFTANALOG"; }
	else if (dwDIK == XID_RIGHT)						{ return "XBOX360_RIGHTANALOG"; }
	else if (dwDIK == XID_UP)							{ return "XBOX360_UPANALOG"; }
	else if (dwDIK == XID_DOWN)							{ return "XBOX360_DOWNANALOG"; }
	else
		return "ERROR";
}

/**
* CDirectInputWrapper::convertStringtoXInput
* @date Created May 9, 2006
*/
DWORD CDirectInputWrapper::convertStringtoXInput(const char *pString)
{
	if (!pString)
		return 0;

	if (!strcmp(pString, "0"))							{ return DIK_0; }
	else if (!strcmp(pString, "XBOX360_UP"))			{ return XINPUT_GAMEPAD_DPAD_UP; }
	else if (!strcmp(pString, "XBOX360_DOWN"))			{ return XINPUT_GAMEPAD_DPAD_DOWN; }
	else if (!strcmp(pString, "XBOX360_LEFT"))			{ return XINPUT_GAMEPAD_DPAD_LEFT; }
	else if (!strcmp(pString, "XBOX360_RIGHT"))			{ return XINPUT_GAMEPAD_DPAD_RIGHT; }
	else if (!strcmp(pString, "XBOX360_START"))			{ return XINPUT_GAMEPAD_START; }
	else if (!strcmp(pString, "XBOX360_BACK"))			{ return XINPUT_GAMEPAD_BACK; }
	else if (!strcmp(pString, "XBOX360_LTHUMB"))		{ return XINPUT_GAMEPAD_LEFT_THUMB; }
	else if (!strcmp(pString, "XBOX360_RTHUMB"))		{ return XINPUT_GAMEPAD_RIGHT_THUMB; }
	else if (!strcmp(pString, "XBOX360_LSHOULDER"))		{ return XINPUT_GAMEPAD_LEFT_SHOULDER; }
	else if (!strcmp(pString, "XBOX360_RSHOULDER"))		{ return XINPUT_GAMEPAD_RIGHT_SHOULDER; }
	else if (!strcmp(pString, "XBOX360_A"))				{ return XINPUT_GAMEPAD_A; }
	else if (!strcmp(pString, "XBOX360_B"))				{ return XINPUT_GAMEPAD_B; }
	else if (!strcmp(pString, "XBOX360_X"))				{ return XINPUT_GAMEPAD_X; }
	else if (!strcmp(pString, "XBOX360_Y"))				{ return XINPUT_GAMEPAD_Y; }
	else if (!strcmp(pString, "XBOX360_LTRIGGER"))		{ return XIT_LEFT_TRIGGER; }
	else if (!strcmp(pString, "XBOX360_RTRIGGER"))		{ return XIT_RIGHT_TRIGGER; }
	else if (!strcmp(pString, "XBOX360_LEFTANALOG"))	{ return XID_LEFT; }
	else if (!strcmp(pString, "XBOX360_RIGHTANALOG"))	{ return XID_RIGHT; }
	else if (!strcmp(pString, "XBOX360_UPANALOG"))		{ return XID_UP; }
	else if (!strcmp(pString, "XBOX360_DOWNANALOG"))	{ return XID_DOWN; }
	else
		return -1;
}

/**
* CDirectInputWrapper::convertStringToJoypad
* @date Created May 11, 2006
*/
DWORD CDirectInputWrapper::convertStringToJoypad(const char *pString)
{
	if (!pString)
		return 0;

	if (!strcmp(pString, "JOYPAD_BUTTON0"))				{ return DIJOFS_BUTTON0; }
	else if (!strcmp(pString, "JOYPAD_BUTTON1"))		{ return DIJOFS_BUTTON1; }
	else if (!strcmp(pString, "JOYPAD_BUTTON2"))		{ return DIJOFS_BUTTON2; }
	else if (!strcmp(pString, "JOYPAD_BUTTON3"))		{ return DIJOFS_BUTTON3; }
	else if (!strcmp(pString, "JOYPAD_BUTTON4"))		{ return DIJOFS_BUTTON4; }
	else if (!strcmp(pString, "JOYPAD_BUTTON5"))		{ return DIJOFS_BUTTON5; }
	else if (!strcmp(pString, "JOYPAD_BUTTON6"))		{ return DIJOFS_BUTTON6; }
	else if (!strcmp(pString, "JOYPAD_BUTTON7"))		{ return DIJOFS_BUTTON7; }
	else if (!strcmp(pString, "JOYPAD_BUTTON8"))		{ return DIJOFS_BUTTON8; }
	else if (!strcmp(pString, "JOYPAD_BUTTON9"))		{ return DIJOFS_BUTTON9; }
	else if (!strcmp(pString, "JOYPAD_BUTTON10"))		{ return DIJOFS_BUTTON10; }
	else if (!strcmp(pString, "JOYPAD_BUTTON11"))		{ return DIJOFS_BUTTON11; }
	else if (!strcmp(pString, "JOYPAD_BUTTON12"))		{ return DIJOFS_BUTTON12; }
	else if (!strcmp(pString, "JOYPAD_BUTTON13"))		{ return DIJOFS_BUTTON13; }
	else if (!strcmp(pString, "JOYPAD_BUTTON14"))		{ return DIJOFS_BUTTON14; }
	else if (!strcmp(pString, "JOYPAD_BUTTON15"))		{ return DIJOFS_BUTTON15; }
	else if (!strcmp(pString, "JOYPAD_BUTTON16"))		{ return DIJOFS_BUTTON16; }
	else if (!strcmp(pString, "JOYPAD_BUTTON17"))		{ return DIJOFS_BUTTON17; }
	else if (!strcmp(pString, "JOYPAD_BUTTON18"))		{ return DIJOFS_BUTTON18; }
	else if (!strcmp(pString, "JOYPAD_BUTTON19"))		{ return DIJOFS_BUTTON19; }
	else if (!strcmp(pString, "JOYPAD_BUTTON20"))		{ return DIJOFS_BUTTON20; }
	else if (!strcmp(pString, "JOYPAD_BUTTON21"))		{ return DIJOFS_BUTTON21; }
	else if (!strcmp(pString, "JOYPAD_BUTTON22"))		{ return DIJOFS_BUTTON22; }
	else if (!strcmp(pString, "JOYPAD_BUTTON23"))		{ return DIJOFS_BUTTON23; }
	else if (!strcmp(pString, "JOYPAD_BUTTON24"))		{ return DIJOFS_BUTTON24; }
	else if (!strcmp(pString, "JOYPAD_BUTTON25"))		{ return DIJOFS_BUTTON25; }
	else if (!strcmp(pString, "JOYPAD_BUTTON26"))		{ return DIJOFS_BUTTON26; }
	else if (!strcmp(pString, "JOYPAD_BUTTON27"))		{ return DIJOFS_BUTTON27; }
	else if (!strcmp(pString, "JOYPAD_BUTTON28"))		{ return DIJOFS_BUTTON28; }
	else if (!strcmp(pString, "JOYPAD_BUTTON29"))		{ return DIJOFS_BUTTON29; }
	else if (!strcmp(pString, "JOYPAD_BUTTON30"))		{ return DIJOFS_BUTTON30; }
	else if (!strcmp(pString, "JOYPAD_BUTTON31"))		{ return DIJOFS_BUTTON31; }
	else if (!strcmp(pString, "JOYPAD_RX"))				{ return DIJOFS_RX; }
	else if (!strcmp(pString, "JOYPAD_RY"))				{ return DIJOFS_RY; }
	else if (!strcmp(pString, "JOYPAD_RZ"))				{ return DIJOFS_RZ; }
	else if (!strcmp(pString, "JOYPAD_X"))				{ return DIJOFS_X; }
	else if (!strcmp(pString, "JOYPAD_Y"))				{ return DIJOFS_Y; }
	else if (!strcmp(pString, "JOYPAD_Z"))				{ return DIJOFS_Z; }
	else
		return -1;
}

/**
* CDirectInputWrapper::isAlphaNumeric
* @date Created May 11, 2006
*/
bool CDirectInputWrapper::isAlphaNumeric(DWORD dwDIK)
{
	if ( (dwDIK >= DIK_1 && dwDIK <= DIK_0)
		|| (dwDIK >= DIK_Q && dwDIK <= DIK_P)
		|| (dwDIK >= DIK_A && dwDIK <= DIK_L)
		|| (dwDIK >= DIK_Z && dwDIK <= DIK_M) )
		return true;

	return false;
}
