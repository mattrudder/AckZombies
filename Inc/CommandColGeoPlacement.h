/**
* @file CommandColGeoPlacement.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 29, 2006
*
* Command used to toggle drawing the collision geometry in the placement
*/

#ifndef _COMMANDCOLGEOPLACEMENT_H_
#define _COMMANDCOLGEOPLACEMENT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"
#include "PlacementState.h"

/**
* Command used to toggle drawing the collision geometry in the placement state
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 29, 2006
* @date Modified April 29, 2006
*/
class CColGeoPlacementCommand : public CCommand
{
public:
	CColGeoPlacementCommand(void) : CCommand("ColGeoPlacement") { }

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

		CPlacementState::getInstancePtr()->toggleCollisionGeometry();
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
		sSyntax = "ColGeoPlacement - toggle drawing collision geometry during actor placement";
	}
};

#endif /*_COMMANDCOLGEOPLACEMENT_H_*/