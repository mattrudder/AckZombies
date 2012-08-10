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
using namespace std;

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

CDebugStream::CDebugStream(bool bDebugLog) : m_fbNull(m_fpNull = fopen("nul", "w")),
	m_fpLog(NULL), m_fbLog(NULL), m_bDebugLog(bDebugLog),
	basic_ostream<char_type>(m_fbTwo = new twobufstream<char_type>(&m_fbNull, &m_fbNull))
{

#ifdef _DEBUG
	m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), &m_fbNull);
	init(m_fbTwo);
	setf(ios::unitbuf);
#else
	if(!bDebugLog)
	{
		m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), &m_fbNull);
		init(m_fbTwo);
		setf(ios::unitbuf);
	}
#endif
	
}

CDebugStream::~CDebugStream(void)
{
	closeLog();

	// Close and deallocate console if there are no more streams remaining
	m_fbTwo->setBuffers(&m_fbNull, &m_fbNull);
	init(m_fbTwo);
	setf(ios::unitbuf);
	delete m_fbTwo;

	// delete the bLog if it still remains
	if(m_fbLog)
	{
		delete m_fbLog;
		m_fbLog = NULL;
	}
}

/**
* CDebugStream::openLog
* @date Modified Mar 28, 2006
*/
void CDebugStream::openLog(const char* szFile)
{
	// Only open the log file if its not marked as a debug log, 
	// or it is marked, and we are in debug mode.
	if(!m_fpLog && ((m_bDebugLog && m_bIsDebug) || !m_bDebugLog))
	{
		m_fpLog = fopen(szFile, "w");
		m_fbLog = new buffer_type(m_fpLog);
		
#ifdef _DEBUG
		//m_fbTwo->setBuffers(m_fbConsole, m_fbLog);
		m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), m_fbLog);
#else
		if(!m_bDebugLog)
			m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), &m_fbNull);
		else
			m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), m_fbLog);
#endif
		init(m_fbTwo);
		setf(ios::unitbuf);
	}
}

/**
* CDebugStream::closeLog
* @date Modified Mar 28, 2006
*/
void CDebugStream::closeLog(void)
{
	if(!m_fpLog)
	{
		if(m_fbLog)
			delete m_fbLog;

		if(m_fpLog)
			fclose(m_fpLog);

		m_fpLog = NULL;
		m_fbLog = NULL;

#ifdef _DEBUG
		//m_fbTwo->setBuffers(m_fbConsole, &m_fbNull);
		m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), &m_fbNull);
#else
		if(!m_bDebugLog)
			m_fbTwo->setBuffers(CCommandConsole::getInstancePtr(), &m_fbNull);
		else
			m_fbTwo->setBuffers(&m_fbNull, &m_fbNull);
#endif
		init(m_fbTwo);
		setf(ios::unitbuf);
	}
}

// Global Logging Objects
CDebugStream Log(false);
CDebugStream Debug;