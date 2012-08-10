/**
* @file CommandStringPlacement.h
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
*
* Command used to create a tooltip for the objective
*/

#ifndef _COMMANDSTRINGPLACEMENT_H_
#define _COMMANDSTRINGPLACEMENT_H_

// Local includes
#include "Command.h"
#include "GameKernel.h"
#include "PlacementState.h"

/**
* Command used to create a tooltip for the objective
*
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
* @date Modified June 2, 2006
*/
class CStringPlacementCommand : public CCommand
{
public:
	CStringPlacementCommand(void) : CCommand("StringPlacement") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created June 2, 2006
	* @param[in]	sError	Error string buffer.
	* @return Success value.
	*/
	bool execute(CString& sError)
	{
		CPlacementState* poPlacement = CPlacementState::getInstancePtr();
		if (CGameKernel::getInstance().getCurrentState() != poPlacement)
		{
			sError.Format("Placement State Not Active");
			return false;
		}

		if (this->m_vParams.empty())
		{
			sError.Format("Bad Syntax");
			return false;
		}

		CString string;
		std::vector<CCommandParam*>::iterator oParamIter = m_vParams.begin();
		while (oParamIter != m_vParams.end())
		{
			string += ((CString)(*(*oParamIter)));
			string += CString(" ");
			oParamIter++;
		}

		string.Trim(0, string.GetLength() - 1);
		poPlacement->m_strToolTip = string;

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
		sSyntax = "StringPlacement (tooltip) - Type the tooltip out as a sentence";
	}
};

#endif /*_COMMANDSTRINGPLACEMENT_H_*/