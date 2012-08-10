/**
* @file CommandExportPlacement.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 30, 2006
*
* Command used to export actors from the object manager in the placement
*/

#ifndef _COMMANDEXPORTPLACEMENT_H_
#define _COMMANDEXPORTPLACEMENT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"
#include "PlacementState.h"

/**
* Command used to export actors from the object manager in the placement state
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 30, 2006
* @date Modified April 30, 2006
*/
class CExportPlacementCommand : public CCommand
{
public:
	CExportPlacementCommand(void) : CCommand("ExportPlacement") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created April 30, 2006
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
		if (((CString)(*poParam)) == CString("Nodes"))
		{
			CPlacementState::getInstancePtr()->exportNodes();
			return true;
		}
		else if (((CString)(*poParam)) == CString("SpawnTriggers"))
		{
			CPlacementState::getInstancePtr()->exportSpawnTriggers();
			return true;
		}
		else if (((CString)(*poParam)) == CString("Spawns"))
		{
			CPlacementState::getInstancePtr()->exportEnemySpawns();
			return true;
		}
		else if (((CString)(*poParam)) == CString("Objectives"))
		{
			CPlacementState::getInstancePtr()->exportObjectiveTriggers();
			return true;
		}
		else if (((CString)(*poParam)) == CString("Barriers"))
		{
			CPlacementState::getInstancePtr()->exportObjectiveBarriers();
			return true;
		}
		else if (((CString)(*poParam)) == CString("All"))
		{
			CPlacementState::getInstancePtr()->exportNodes();
			CPlacementState::getInstancePtr()->exportEnemySpawns();
			CPlacementState::getInstancePtr()->exportSpawnTriggers();
			CPlacementState::getInstancePtr()->exportObjectiveTriggers();
			CPlacementState::getInstancePtr()->exportObjectiveBarriers();
			return true;
		}
		else
		{
			sError.Format("Bad Syntax");
			return false;
		}
	}

	/**
	* Returns the syntax of the command.
	*
	* @date Created April 30, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax)
	{
		//sSyntax = "ExportPlacement - Writes type to creation files";
		sSyntax = "ExportPlacement (type) - Writes type to creation file\n\tNodes\n\tSpawns\n\tSpawnTriggers\n\tObjectives\n\tBarriers\n\tAll";
	}
};

#endif /*_COMMANDEXPORTPLACEMENT_H_*/