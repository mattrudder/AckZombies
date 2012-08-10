/**
* @file TickTimer.h
* @author Matt Rudder
* @date Created Mar 27, 2006
*
* Utility class used to time out regular occuring events.
*/

#ifndef _TICKTIMER_H_
#define _TICKTIMER_H_

// Engine includes
#include "Timer.h"

/**
* Utility class used to time out regular occuring events.
*
* @author Matt Rudder
* @date Created Mar 29, 2006
* @date Modified Mar 29, 2006
*/
class CTickTimer
{
protected:
	//! Interval which the timer sets off.
	float m_fInterval;
	//! Time passed since the last tick
	float m_fSinceLastTick;
	//! Times tick() was called in the last second
	unsigned int m_unFramesPerSecond;
	//! Avg. Frames per second
	float m_fFPS;
	//! Time since last second
	float m_fSinceLastFPS;
public:

	CTickTimer() : m_fInterval(-1.0f), m_fSinceLastTick(0.0f), m_fSinceLastFPS(0),
	m_fFPS(0), m_unFramesPerSecond(0) {}

	/**
	* Sets the interval at which the timer will go off.
	*
	* @date Created Mar 29, 2006
	* @param[in]	fInterval	The interval at which the timer will go off.
	* @remakr	The interval can be set to -1 to pause the timer.
	*/
	void setInterval(float fInterval)
	{
		m_fInterval = fInterval; 
		m_fSinceLastTick = 0.0f; 
	}

	/**
	* Updates the internal clock and alerts the user when an interval has 
	* passed.
	*
	* @date Created Mar 29, 2006
	* @return true if an interval passed this frame, false otherwise.
	*/
	bool tick(void) 
	{ 
		if(m_fInterval == -1.0f)
			return false;

		m_fSinceLastTick += CTimer::getInstance().getFrameTime(); 
		if(m_fSinceLastTick >= m_fInterval)
		{
			// Calculate 'ticks' per second.
			m_fSinceLastFPS += m_fSinceLastTick;
			++m_unFramesPerSecond;
			if(m_fSinceLastFPS >= 1.0f)
			{
				// Scale frames for intervals that take longer than one second
				m_fFPS += m_unFramesPerSecond / m_fSinceLastFPS;
				m_fFPS *= 0.5f;
				m_unFramesPerSecond = 0;
				m_fSinceLastFPS = 0.0f;
			}

			m_fSinceLastTick = 0.0f;
			return true;
		}
		else
			return false;
	}

	/**
	* Updates the internal clock and alerts the user when an interval has 
	* passed.
	*
	* @date Created Mar 29, 2006
	* @return true if an interval passed this frame, false otherwise.
	*/
	bool tick(float *fElapsedTime) 
	{ 
		if(m_fInterval == -1.0f)
			return false;

		m_fSinceLastTick += CTimer::getInstance().getFrameTime();
		*fElapsedTime = m_fSinceLastTick;
		if(m_fSinceLastTick >= m_fInterval)
		{
			// Calculate 'ticks' per second.
			m_fSinceLastFPS += m_fSinceLastTick;
			++m_unFramesPerSecond;
			if(m_fSinceLastFPS >= 1.0f)
			{
				// Scale frames for intervals that take longer than one second
				m_fFPS += m_unFramesPerSecond / m_fSinceLastFPS;
				m_fFPS *= 0.5f;
				m_unFramesPerSecond = 0;
				m_fSinceLastFPS = 0.0f;
			}

			m_fSinceLastTick = 0.0f;
			return true;
		}
		else
			return false;
	}

	/**
	* Returns the average ticks per second on this timer.
	*
	* @date Created Mar 29, 2006
	* @return Ticks per second.
	*/
	float getTicksPerSecond(void)
	{
		return m_fFPS;
	}
};

#endif