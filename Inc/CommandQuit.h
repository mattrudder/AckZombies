/**
* @file CommandQuit.h
* @author Matt Rudder
* @date Created Apr 05, 2006
*
* Command used to quit the game.
*/

#ifndef _COMMANDQUIT_H_
#define _COMMANDQUIT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"

/**
* Command used to quit the game.
*
* @author Matt Rudder
* @date Created Apr 05, 2006
* @date Modified Apr 05, 2006
*/
class CQuitCommand : public CCommand
{
public:
	CQuitCommand(void) : CCommand("Quit") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created Apr 04, 2006
	* @param[in]	sError	Error string buffer.
	* @return Success value.
	*/
	bool execute(CString& sError)
	{
		// Quit the game, mang.
		CGameKernel::getInstance().changeState(NULL);
		return true;
	}

	/**
	* Returns the syntax of the command.
	*
	* @date Created Apr 05, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax)
	{
		sSyntax = "Quit - quits the game";
	}
};

#endif //_COMMANDQUIT_H_