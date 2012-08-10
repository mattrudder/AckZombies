/**
* @file Timer.cpp
* @author Matt Rudder
* @date Modified Mar 27, 2006
*
* Singleton utility class used for timing operations
*/

// Local includes
#include "Timer.h"

CTimer::CTimer(void) : m_fAvgFPS(60.0f), m_unFrames(0), m_dSecs(0.0), m_nAppLastFrame(0), m_nAppFrame(0), m_nLastFPS(0)
{
	// Start the clock.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nAppStart);
	m_nAppLastFrame = m_nAppStart;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_nCPUFreq);
	m_dInvFreq = 1.0/(double)m_nCPUFreq;
}

/**
* CTimer::tick
* @date Modified Mar 27, 2006
*/
void CTimer::tick(void)
{
	// Update FPS
	++m_unFrames;
	if((double)(m_nAppFrame - m_nLastFPS) * m_dInvFreq > 1.0f)
	{
		m_fAvgFPS += (float)m_unFrames;
		m_fAvgFPS *= 0.5f;
		m_dSecs = 0.0;
		m_unFrames = 0;
		m_nLastFPS = m_nAppFrame;
	}

	m_nAppLastFrame = m_nAppFrame;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nAppFrame);
}

/**
* CTimer::getTime
* @date Modified Mar 27, 2006
*/
float CTimer::getTime(void)
{
	__int64 nCurTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&nCurTime);
	double dDiff = static_cast<double>(nCurTime - m_nAppStart);
	return (float)(dDiff * m_dInvFreq);
}

/**
* CTimer::getLastTime
* @date Modified Mar 27, 2006
*/
float CTimer::getLastTime(void)
{
	double dDiff = static_cast<double>(m_nAppLastFrame - m_nAppStart);
	return (float)(dDiff * m_dInvFreq);
}

/**
* CTimer::getFrameTime
* @date Modified Mar 27, 2006
*/
float CTimer::getFrameTime(void)
{
	double dDiff = static_cast<double>(m_nAppFrame - m_nAppLastFrame);
	return (float)(dDiff * m_dInvFreq);
}

/**
* CTimer::getFPS
* @date Modified Mar 27, 2006
*/
float CTimer::getFPS(void)
{
	return m_fAvgFPS;
}