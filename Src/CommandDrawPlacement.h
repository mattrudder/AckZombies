/**
* @file CommandDrawPlacement.h
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
*
* Command used to toggle drawing in the placement
*/

#ifndef _COMMANDDRAWPLACEMENT_H_
#define _COMMANDDRAWPLACEMENT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"
#include "PlacementState.h"

/**
* Command used to toggle drawing in the placement state
*
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
* @date Modified June 2, 2006
*/
class CDrawPlacementCommand : public CCommand
{
public:
	CDrawPlacementCommand(void) : CCommand("DrawPlacement") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created June 2, 2006
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

		if (this->m_vParams.empty())
		{
			sError.Format("Bad Syntax");
			return false;
		}

		CCommandParam* poParam = m_vParams.front();
		if (((CString)(*poParam)) == CString("NodesSpawns"))
		{
			CPlacementState::getInstancePtr()->toggleDrawNodesSpawnTriggers();
			return true;
		}
		else if (((CString)(*poParam)) == CString("Objectives"))
		{
			CPlacementState::getInstancePtr()->toggleDrawObjectiveTriggerBarriers();
			return true;
		}
		else if (((CString)(*poParam)) == CString("ColGeo"))
		{
			CPlacementState::getInstancePtr()->toggleCollisionGeometry();
			return true;
		}
		else
		{
			sError.Format("Bad Syntax");
			return false;
		}

		return true;
	}

	/**
	* Returns the syntax of the command.
	*
	* @date Created June 2, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax)
	{
		sSyntax = "DrawPlacement (type) - Toggles Drawing of the type\n\tNodesSpawns\n\tObjectives\n\tColGeo\n\tAll";
	}
};

#endif /*_COMMANDRAWPLACEMENT_H_*/