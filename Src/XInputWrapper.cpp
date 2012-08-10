/**
* @file XInputWrapper.cpp
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the XInput Wrapper class definition
* used for getting the XBOX360 controller input
*/
#include "XInputWrapper.h"

/**
* CXInputWrapper::CXInputWrapper
* @date Created March 06, 2006
*/
CXInputWrapper::CXInputWrapper(void)
{
	for (unsigned int i = 0; i < XI_MAX_CONTROLLERS; ++i)
	{
		memset(&m_tGamePads[i], 0, sizeof(m_tGamePads[i]));
	}
	memset(m_sButtons, 0, sizeof(unsigned short) * XI_MAX_CONTROLLERS);

	m_bLeftTriggerDown = m_bRightTriggerDown = false;
	m_bLeftThumbstick = false;
}

/**
* CXInputWrapper::~CXInputWrapper
* @date Created March 06, 2006
*/
CXInputWrapper::~CXInputWrapper(void)
{
}

/**
* CXInputWrapper::isConnected
* @date Created March 06, 2006
*/
bool CXInputWrapper::isConnected(DWORD dwIndex)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return false;

	return m_tGamePads[dwIndex].bConnected;
}

/**
* CXInputWrapper::updateControllers
* @date Created March 06, 2006
*/
void CXInputWrapper::updateControllers(void)
{
	for (unsigned int i = 0; i < XI_MAX_CONTROLLERS; ++i)
	{
		getGamePadState(i);

		// if this controller is not connected, set its values to zero.
		if (!m_tGamePads[i].bConnected)
		{
			memset(&m_tGamePads[i], 0, sizeof(m_tGamePads[i]));
		}
	}
}

/**
* CXInputWrapper::isButtonDown
* @date Created March 06, 2006
*/
bool CXInputWrapper::isButtonDown(DWORD dwIndex, DWORD dwButton)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return false;

	if (m_tGamePads[dwIndex].Gamepad.wButtons & dwButton)
		return true;

	return false;
}

/**
* CXInputWrapper::getKey
* @date Created June 18, 2006
*/
bool CXInputWrapper::getKey(DWORD dwIndex, DWORD &dwKey)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return false;

	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_DPAD_UP)) { dwKey = XINPUT_GAMEPAD_DPAD_UP; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_DPAD_DOWN)) { dwKey = XINPUT_GAMEPAD_DPAD_DOWN; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_DPAD_LEFT)) { dwKey = XINPUT_GAMEPAD_DPAD_LEFT; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_DPAD_RIGHT)) { dwKey = XINPUT_GAMEPAD_DPAD_RIGHT; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_START)) { dwKey = XINPUT_GAMEPAD_START; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_BACK)) { dwKey = XINPUT_GAMEPAD_BACK; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_LEFT_THUMB)) { dwKey = XINPUT_GAMEPAD_LEFT_THUMB; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_RIGHT_THUMB)) { dwKey = XINPUT_GAMEPAD_RIGHT_THUMB; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_LEFT_SHOULDER)) { dwKey = XINPUT_GAMEPAD_LEFT_SHOULDER; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_RIGHT_SHOULDER)) { dwKey = XINPUT_GAMEPAD_RIGHT_SHOULDER; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_A)) { dwKey = XINPUT_GAMEPAD_A; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_B)) { dwKey = XINPUT_GAMEPAD_B; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_X)) { dwKey = XINPUT_GAMEPAD_X; return true; }
	if (isButtonDown(dwIndex, XINPUT_GAMEPAD_Y)) { dwKey = XINPUT_GAMEPAD_Y; return true; }

	if (getTriggerData(dwIndex, true) >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		dwKey = XIT_LEFT_TRIGGER;
		return true;
	}
	if (getTriggerData(dwIndex, false) >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		dwKey = XIT_RIGHT_TRIGGER;
		return true;
	}

	return false;
}

/**
* CXInputWrapper::isButtonDownBuffered
* @date Created March 06, 2006
*/
bool CXInputWrapper::isButtonDownBuffered(DWORD dwIndex, DWORD dwButton)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return false;

	if (isButtonDown(dwIndex, dwButton))
	{
		if (!(m_sButtons[dwIndex] & dwButton))
		{
			m_sButtons[dwIndex] |= dwButton;
			return true;
		}
	}
	else if (!isButtonDown(dwIndex, dwButton))
		m_sButtons[dwIndex] &= ~dwButton;

	return false;
}

/**
* CXInputWrapper::getTriggerData
* @date Created March 06, 2006
*/
BYTE CXInputWrapper::getTriggerData(DWORD dwIndex, bool bLeftTrigger)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return 0;

	if (bLeftTrigger)
	{
		return m_tGamePads[dwIndex].Gamepad.bLeftTrigger;
	}
	else
	{
		return m_tGamePads[dwIndex].Gamepad.bRightTrigger;
	}
}

/**
* CXInputWrapper::isTriggerDownBuffered
* @date Created June 18, 2006
*/
bool CXInputWrapper::isTriggerDownBuffered(DWORD dwIndex, bool bLeftTrigger, int nPressure)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return false;

	if (bLeftTrigger)
	{
		if (m_tGamePads[dwIndex].Gamepad.bLeftTrigger >= nPressure)
		{
			if (!m_bLeftTriggerDown)
			{
				m_bLeftTriggerDown = true;
				return true;
			}
		}
		else
		{
			m_bLeftTriggerDown = false;
		}
	}
	else
	{
		if (m_tGamePads[dwIndex].Gamepad.bRightTrigger >= nPressure)
		{
			if (!m_bRightTriggerDown)
			{
				m_bRightTriggerDown = true;
				return true;
			}
		}
		else
		{
			m_bRightTriggerDown = false;
		}
	}

	// nope
	return false;
}
/**
* CXInputWrapper::setMotorVibration
* @date Created March 06, 2006
*/
void CXInputWrapper::setMotorVibration(DWORD dwIndex, bool bLeftMotor, WORD wSpeed, unsigned int unMiliseconds)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return;

	if (wSpeed < 0) { wSpeed = 0; }
	if (wSpeed > 65535) { wSpeed = 65535; }

	if (bLeftMotor)
	{
		m_tGamePads[dwIndex].Vibration.wLeftMotorSpeed = wSpeed;
	}
	else
	{
		m_tGamePads[dwIndex].Vibration.wRightMotorSpeed = wSpeed;
	}

	XInputSetState(dwIndex, &m_tGamePads[dwIndex].Vibration);
}

/**
* CXInputWrapper::getThumbstickDirection
* @date Created March 06, 2006
*/
float CXInputWrapper::getThumbstickDirection(DWORD dwIndex, EXInputAxis eAxis)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return 0.0f;

	// make sure the enum is valid
	if (eAxis >= XIA_MAX)
		return 0.0f;

	switch (eAxis)
	{
	case XIA_LEFT_X_AXIS:
		return m_tGamePads[dwIndex].fThumbLX;
	case XIA_LEFT_Y_AXIS:
		return m_tGamePads[dwIndex].fThumbLY;
	case XIA_RIGHT_X_AXIS:
		return m_tGamePads[dwIndex].fThumbRX;
	case XIA_RIGHT_Y_AXIS:
		return m_tGamePads[dwIndex].fThumbRY;
	default:
		return 0.0f;
	}
}

/**
* CXInputWrapper::getThumbstickDirectionBuffered
* @date Created June 18, 2006
*/
float CXInputWrapper::getThumbstickDirectionBuffered(DWORD dwIndex, EXInputAxis eAxis)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return 0.0f;

	// make sure the enum is valid
	if (eAxis >= XIA_MAX)
		return 0.0f;

	switch (eAxis)
	{
	case XIA_LEFT_Y_AXIS:
		if (m_tGamePads[dwIndex].fThumbLY)
		{
			if (!m_bLeftThumbstick)
			{
				m_bLeftThumbstick = true;
				return m_tGamePads[dwIndex].fThumbLY;
			}
		}
		else
			m_bLeftThumbstick = false;
	
		return 0.0f;
	case XIA_LEFT_X_AXIS:
	case XIA_RIGHT_X_AXIS:
	case XIA_RIGHT_Y_AXIS:
	default:
		return 0.0f;
	}
}

/**
* CXInputWrapper::getGamePadState
* @date Created March 06, 2006
*/
void CXInputWrapper::getGamePadState(DWORD dwIndex)
{
	// make sure the index is valid
	if (dwIndex < 0 || dwIndex >= XI_MAX_CONTROLLERS)
		return;

	// The state of the gamepad
	XINPUT_STATE State;
	memset(&State, 0, sizeof(State));

	// Get the state of the gamepad
	DWORD dwResult = XInputGetState(dwIndex, &State);

	// Gamepad is connected
	if (dwResult == ERROR_SUCCESS)
	{
		m_tGamePads[dwIndex].bConnected = true;
	}
	// Gamepad is not connected
	else
	{
		m_tGamePads[dwIndex].bConnected = false;
		return;
	}

	// Zero value if thumbsticks are within the dead zone 
	if( (State.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		State.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
		(State.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		State.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
	{	
		State.Gamepad.sThumbLX = 0;
		State.Gamepad.sThumbLY = 0;
	}

	if( (State.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
		State.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && 
		(State.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
		State.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ) 
	{
		State.Gamepad.sThumbRX = 0;
		State.Gamepad.sThumbRY = 0;
	}

	// copy the state over
	m_tGamePads[dwIndex].Gamepad = State.Gamepad;

	// make all values in range of -1 to 1
	m_tGamePads[dwIndex].fThumbLX = m_tGamePads[dwIndex].Gamepad.sThumbLX / 32767.0f;
	m_tGamePads[dwIndex].fThumbLY = m_tGamePads[dwIndex].Gamepad.sThumbLY / 32767.0f;
	m_tGamePads[dwIndex].fThumbRX = m_tGamePads[dwIndex].Gamepad.sThumbRX / 32767.0f;
	m_tGamePads[dwIndex].fThumbRY = m_tGamePads[dwIndex].Gamepad.sThumbRY / 32767.0f;
}







