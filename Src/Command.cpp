/**
* @file Command.cpp
* @author Matt Rudder
* @date Modified Apr 04, 2006
*
* Commands that wrap up functionality of the game engine for use from console.
*/

// Local includes
#include "Command.h"

CCommand::CCommand(CString sFunc) : m_sCommandName(sFunc)
{
}

CCommand::~CCommand(void)
{
	clearParams();
}


void CCommand::addParam(CCommandParam* pParam)
{
	m_vParams.push_back(pParam);
}

void CCommand::clearParams(void)
{
	for(size_t i = 0; i < m_vParams.size(); ++i)
		delete m_vParams[i];
	m_vParams.clear();
}