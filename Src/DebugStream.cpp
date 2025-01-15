/**
* @file DebugStream.cpp
* @author Matt Rudder
* @date Modified Apr 04, 2006
*
* Stream used for debug output
*/

// Local includes
#include "DebugStream.h"

// Engine includes
#include "RenderSystem.h"
#include "CommandConsole.h"

// System includes
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

// Statics
#ifdef _DEBUG
bool CDebugStream::m_bIsDebug = true;
#else
bool CDebugStream::m_bIsDebug = false;
#endif

/**
* CDebugStream::clearConsole
* @date Modified Apr 04, 2006
*/
void CDebugStream::clearConsole(void)
{
	CCommandConsole::getInstance().clearOutput();
}

/**
* CDebugStream::setColor
* @date Modified Apr 04, 2006
*/
void CDebugStream::setColor(WORD wRGBI, WORD Mask)
{
	CCommandConsole::getInstance().setColor(wRGBI);
}

CDebugStream::CDebugStream(bool bDebugLog) : basic_ostream<char_type>(&m_fbTwo), m_bDebugLog(bDebugLog)
{
#ifdef _DEBUG
	m_fbTwo.setBuffers(CCommandConsole::getInstancePtr());
	this->rdbuf(&m_fbTwo);
	setf(std::ios::unitbuf);
#else
	if(!bDebugLog)
	{
		m_fbTwo.setBuffers(CCommandConsole::getInstancePtr());
		this->rdbuf(&m_fbTwo);
		setf(ios::unitbuf);
	}
#endif

}

CDebugStream::~CDebugStream(void)
{
	closeLog();

	// Close and deallocate console if there are no more streams remaining
	m_fbTwo.setBuffers();
	this->rdbuf(&m_fbTwo);
	setf(std::ios::unitbuf);
}

/**
* CDebugStream::openLog
* @date Modified Mar 28, 2006
*/
void CDebugStream::openLog(const char* szFile)
{
	// Only open the log file if its not marked as a debug log,
	// or it is marked, and we are in debug mode.
	if(!m_fbLog.is_open() && ((m_bDebugLog && m_bIsDebug) || !m_bDebugLog))
	{
		m_fbLog.open(szFile, std::ios::out);

#ifdef _DEBUG
		m_fbTwo.setBuffers(CCommandConsole::getInstancePtr());
#else
		if(!m_bDebugLog)
			m_fbTwo.setBuffers(CCommandConsole::getInstancePtr());
		else
			m_fbTwo.setBuffers(CCommandConsole::getInstancePtr(), &m_fbLog);
#endif
		this->rdbuf(&m_fbTwo);
		setf(std::ios::unitbuf);
	}
}

/**
* CDebugStream::closeLog
* @date Modified Mar 28, 2006
*/
void CDebugStream::closeLog(void)
{
	if(m_fbLog.is_open())
	{
		m_fbLog.close();

#ifdef _DEBUG
		m_fbTwo.setBuffers(CCommandConsole::getInstancePtr());
#else
		if(m_bDebugLog)
			m_fbTwo.setBuffers(CCommandConsole::getInstancePtr());
		else
			m_fbTwo.setBuffers();
#endif
		this->rdbuf(&m_fbTwo);
		setf(std::ios::unitbuf);
	}
}

// Global Logging Objects
CDebugStream Log(false);
CDebugStream Debug;