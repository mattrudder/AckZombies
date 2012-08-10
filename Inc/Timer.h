/**
* @file Timer.h
* @author Matt Rudder
* @date Created Mar 27, 2006
*
* Singleton utility class used for timing operations
*/

#ifndef _TIMER_H_
#define _TIMER_H_

// Engine includes
#include "Singleton.h"

// System includes
#include <windows.h>

/**
* Singleton utility class used for timing operations
*
* @author Matt Rudder
* @date Created Mar 27, 2006
* @date Modified Mar 27, 2006
*/
class CTimer : public CSingleton<CTimer>
{
	friend class CSingleton<CTimer>;
protected:
	//! Singleton stuff.
	//!@{
	CTimer(void);
	CTimer(const CTimer&) {}
	operator=(const CTimer&){}
	virtual ~CTimer(void) {}
	//!@}

	//! Tick count at the beginning of the application
	__int64 m_nAppStart;
	//! Tick count at the start of the current frame
	__int64 m_nAppFrame;
	//! Tick count at the start of the last frame
	__int64 m_nAppLastFrame;
	//! CPU Frequency; Used in timing calculations
	__int64 m_nCPUFreq;
	//! Last time an FPS check was done
	__int64 m_nLastFPS;
	//! Fraction of seconds; used for FPS calculation
	double m_dSecs;
	//! Inverse of the CPU frequency
	double m_dInvFreq;
	//! Frames this second so far
	unsigned int m_unFrames;
	//! Average frames per second
	float m_fAvgFPS;

public:

	/**
	* Ticks the global timer.
	*
	* @date Created Mar 27, 2006
	*/
	void tick(void);

	/**
	* Gets the time in seconds since the beginning of the application.
	*
	* @date Created Mar 27, 2006
	* @return Seconds since the beginning of the application.
	*/
	float getTime(void);
	float getLastTime(void);

	/**
	* Gets the time in seconds passed since the last frame.
	*
	* @date Created Mar 27, 2006
	* @return Seconds since the last frame.
	*/
	float getFrameTime(void);

	/**
	* Gets the current average frames per second
	*
	* @date Created Mar 27, 2006
	*/
	float getFPS(void);
};

#endif // _TIMER_H_