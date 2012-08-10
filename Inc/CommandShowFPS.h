/**
* @file CommandShowFPS.h
* @author Jordan Leckner
* @date Created May 30, 2006
*
* Command used to Show the FPS counter.
*/

#ifndef _COMMANDSHOWFPS_H_
#define _COMMANDSHOWFPS_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"

/**
* Command used to Show the FPS counter.
*
* @author Jordan Leckner
* @date Created May 30, 2006
* @date Modified May 30, 2006
*/
class CShowFPSCommand : public CCommand
{
public:
	CShowFPSCommand(void) : CCommand("ShowFPS") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created May 30, 2006
	* @param[in]	sError	Error string buffer.
	* @return Success value.
	*/
	bool execute(CString& sError)
	{
		// toggle the FPS bool.
		CTestState::getInstance().m_bDisplayFPS = !CTestState::getInstance().m_bDisplayFPS;
		return true;
	}

	/**
	* Returns the syntax of the command.
	*
	* @date Created May 30, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax)
	{
		sSyntax = "ShowFPS - Toggles the showing of the FPS";
	}
};

#endif //_COMMANDSHOWFPS_H_