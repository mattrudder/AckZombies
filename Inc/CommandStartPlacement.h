/**
* @file CommandStartPlacement.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
*
* Command used to start the placement state
*/

#ifndef _COMMANDSTARTPLACEMENT_H_
#define _COMMANDSTARTPLACEMENT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"
#include "PlacementState.h"

/**
* Command used to start the placement state
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
* @date Modified April 27, 2006
*/
class CStartPlacementCommand : public CCommand
{
public:
	CStartPlacementCommand(void) : CCommand("StartPlacement") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created April 27, 2006
	* @param[in]	sError	Error string buffer.
	* @return Success value.
	*/
	bool execute(CString& sError)
	{
		CGameKernel::getInstance().changeState(CPlacementState::getInstancePtr());
		CCommandConsole::getInstance().toggleConsole();
		return true;
	}

	/**
	* Returns the syntax of the command.
	*
	* @date Created April 27, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax)
	{
		sSyntax = "StartPlacement - starts the Actor Placement";
	}
};

#endif /*_COMMANDSTARTPLACEMENT_H_*/