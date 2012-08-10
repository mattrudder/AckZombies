/**
* @file CommandQuitPlacement.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
*
* Command used to quit the placement state
*/

#ifndef _COMMANDQUITPLACEMENT_H_
#define _COMMANDQUITPLACEMENT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"
#include "TestState.h"
#include "PlacementState.h"
/**
* Command used to quit the placement state
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
* @date Modified April 27, 2006
*/
class CQuitPlacementCommand : public CCommand
{
public:
	CQuitPlacementCommand(void) : CCommand("Back") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created April 27, 2006
	* @param[in]	sError	Error string buffer.
	* @return Success value.
	*/
	bool execute(CString& sError)
	{
		if (CGameKernel::getInstance().getCurrentState() != CPlacementState::getInstancePtr())
		{
			sError.Format("Placement State Not Active");
			return false;
		}

		CGameKernel::getInstance().changeState(CTestState::getInstancePtr());
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
		sSyntax = "Back - quits the Actor Placement back to the Game";
	}
};

#endif /*_COMMANDQUITPLACEMENT_H_*/