/**
* @file LoggingSystem.cpp
* @author Matt Rudder
* @date Modified Mar 27, 2006
*
* Utility manager used to write logging information to file.
*/

#include "LoggingSystem.h"
#include <tchar.h>
#include <ctime>

/**
* CLoggingSystem::CLoggingSystem
* @date Modified Mar 27, 2006
*/
CLoggingSystem::CLoggingSystem(void)
{
	m_ofLogFile.exceptions(std::ios::badbit | std::ios::failbit);
	try
	{
		m_ofLogFile.open("AckZombies.log");

		// Get Date
		char dbuffer[9];
		_strdate(dbuffer);

		CString sFmt = "-",
				sTitle;
		sFmt *= 80;
		sTitle.Format("%-69s %8s ", "Ack! Zombies - Cypher Edge Interactive",  dbuffer);
		m_ofLogFile << sFmt << "\n ";
		m_ofLogFile << sTitle << '\n';
		m_ofLogFile << sFmt << '\n';

	}
	catch(...)
	{
		if(m_ofLogFile.is_open())
			m_ofLogFile.close();
	}
}

/**
* CLoggingSystem::~CLoggingSystem
* @date Modified Mar 27, 2006
*/
CLoggingSystem::~CLoggingSystem(void)
{
	try
	{
		m_ofLogFile.flush();
		if(m_ofLogFile.is_open())
			m_ofLogFile.close();
	}
	catch(...) { }
}

/**
* CLoggingSystem::addEntry
* @date Modified Mar 27, 2006
*/
void CLoggingSystem::addEntry(unsigned int unLine, CString sText)
{
	CString sPrefix;
	if(m_sCurFunc.GetLength() && m_sCurFile.GetLength())
		sPrefix = "   ";

	try
	{
		if(unLine != UINT_MAX)
			m_ofLogFile << sPrefix << sText << '\n';
		else
			m_ofLogFile << sPrefix << '(' << unLine << "): " << sText << '\n';
	}
	catch(...)
	{
		if(m_ofLogFile.is_open())
			m_ofLogFile.close();

		m_ofLogFile.open("AckZombies.log");
	}
}

/**
* CLoggingSystem::beginSection
* @date Modified Mar 27, 2006
*/
void CLoggingSystem::beginSection(CString sFile, CString sFunc)
{
	std::vector<CString> vFile;
	sFile.ToList(vFile, _TEXT("\\/"));
	m_sCurFile = vFile[vFile.size() - 1];
	m_sCurFunc = sFunc;
	
	try
	{
		if(m_sCurFile.GetLength())
			m_ofLogFile << "[" << m_sCurFile;

		if(sFunc.GetLength())
		{
			if(m_sCurFile.GetLength())
				m_ofLogFile << ", ";
			else
				m_ofLogFile << '[';
			m_ofLogFile << sFunc << "]: \n";
		}	
	}
	catch (...)
	{
		if(m_ofLogFile.is_open())
			m_ofLogFile.close();

		m_ofLogFile.open("AckZombies.log");
	}
}

/**
* CLoggingSystem::endSection
* @date Modified Mar 27, 2006
*/
void CLoggingSystem::endSection(void)
{
	m_sCurFile = m_sCurFunc = "";
}