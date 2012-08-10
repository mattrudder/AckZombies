/**
* @file XInputWrapper.h
* @author Jordan Leckner
* @date Created March 06, 2006
*
* This file includes the XInput Wrapper class
* used for getting the XBOX360 controller input
*/
#ifndef _XINPUTWRAPPER_H_
#define _XINPUTWRAPPER_H_

#include <windows.h>

#include <XInput.h>

#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "dxguid.lib")

struct SXInput
{
	XINPUT_GAMEPAD		Gamepad;	// Gamepad 
	XINPUT_VIBRATION	Vibration;	// Vibration
	float				fThumbLX;	// Left thumbstick X Axis
	float				fThumbLY;	// Left thumbstick Y Axis
	float				fThumbRX;	// Right thumbstick X Axis
	float				fThumbRY;	// Right thumbstick Y Axis
	bool				bConnected;	// Whether or not the controller is connected
};

enum EXInput
{
	XI_MAX_CONTROLLERS = 2,
	XI_MAX
};

enum EXInputAxis
{
	XIA_LEFT_X_AXIS,
	XIA_LEFT_Y_AXIS,
	XIA_RIGHT_X_AXIS,
	XIA_RIGHT_Y_AXIS,
	XIA_MAX
};

enum EXInputDirection
{
	XID_LEFT = 137,
	XID_RIGHT,
	XID_UP,
	XID_DOWN,
	XID_MAX = XID_DOWN - 136
};

enum EXInputTriggers
{
	XIT_LEFT_TRIGGER = 354,
	XIT_RIGHT_TRIGGER,
	XIT_MAX = XIT_RIGHT_TRIGGER - 353
};

/**
* Declares the XInput Wrapper class
* used for retrieving data from the XBOX360 controller
* 
* @author Jordan Leckner
* @date Created March 06, 2006
* @date Modified March 06, 2006
*/
class CXInputWrapper
{
private:

	//! Our gamepads
	SXInput	m_tGamePads[XI_MAX_CONTROLLERS];

	//! Buffered input
	unsigned short m_sButtons[XI_MAX_CONTROLLERS];

	//! is left trigger down
	bool m_bLeftTriggerDown;

	//! is right trigger down
	bool m_bRightTriggerDown;

	//! thumbstick left up buffered
	bool m_bLeftThumbstick;

public:
	CXInputWrapper(void);
	~CXInputWrapper(void);

	/**
	* Checks to see if a controller is connected
	* 
	* @date Created March 06, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	* @return true if the controller is connected
	*/
	bool isConnected(DWORD dwIndex);

	/**
	* Polls the system for all input
	* 
	* @date Created March 06, 2006
	*/
	void updateControllers(void);

	/**
	* Checks to see if a button is down
	* 
	* @date Created March 06, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	* @param[in]	dwButton	The button to check for
	*/
	bool isButtonDown(DWORD dwIndex, DWORD dwButton);

	/**
	* Checks to see if a button is down (buffered)
	* 
	* @date Created March 27, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	* @param[in]	dwButton	The button to check for
	*/
	bool isButtonDownBuffered(DWORD dwIndex, DWORD dwButton);

	/**
	* Gets the trigger data
	* 
	* @date Created March 06, 2006
	* @param[in]	dwIndex			Index of the controller to check for
	* @param[in]	bLeftTrigger	Whether or not to get the left trigger data
	* @return the value of the trigger in the range of 0-255
	*/
	BYTE getTriggerData(DWORD dwIndex, bool bLeftTrigger);

	/**
	* checks if the trigger is down.
	* 
	* @date Created June 18, 2006
	* @param[in]	dwIndex			Index of the controller to check for
	* @param[in]	bLeftTrigger	Whether or not to get the left trigger data
	* @param[in]	nPressure		How much for the trigger to be down to count
	* @return true if the trigger is down, buffered
	*/
	bool isTriggerDownBuffered(DWORD dwIndex, bool bLeftTrigger, int nPressure);

	/**
	* Sets the motor vibration value and a time span
	* 
	* @date Created March 06, 2006
	* @param[in]	dwIndex			Index of the controller to check for
	* @param[in]	bLeftMotor		Whether or not to set for the left motor
	* @param[in]	wSpeed			The speed of the motor in the range of 0-65535
	* @param[in]	unMiliseconds	How long the vibration should last
	*/
	void setMotorVibration(DWORD dwIndex, bool bLeftMotor, WORD wSpeed, unsigned int unMiliseconds = 1000);

	/**
	* Gets the thumb stick data in a certain direction
	* 
	* @date Created March 06, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	* @param[in]	eAxis		Which axis to get data for
	*/
	float getThumbstickDirection(DWORD dwIndex, EXInputAxis eAxis);

	/**
	* Gets the thumb stick data in a certain direction, buffered
	* 
	* @date Created June 18, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	* @param[in]	eAxis		Which axis to get data for
	*/
	float getThumbstickDirectionBuffered(DWORD dwIndex, EXInputAxis eAxis);

	/**
	* Gets any button pressed
	* 
	* @date Created June 18, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	* @param[in]	dwKey		key to set to
	*/
	bool getKey(DWORD dwIndex, DWORD &dwKey);

private:

	/**
	* Gets the game pad state
	* 
	* @date Created March 06, 2006
	* @param[in]	dwIndex		Index of the controller to check for
	*/
	void getGamePadState(DWORD dwIndex);
};

#endif