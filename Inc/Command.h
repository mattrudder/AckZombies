/**
* @file Command.h
* @author Matt Rudder
* @date Created Apr 04, 2006
*
* Commands that wrap up functionality of the game engine for use from console.
*/

#ifndef _COMMAND_H_
#define _COMMAND_H_

// System includes
#include <vector>
#include <stdarg.h>

// Engine includes
#include "AcespaceEngine.h"
#include "CommandParam.h"

/**
* Commands that wrap up functionality of the game engine for use from console.
*
* @author Matt Rudder
* @date Created Apr 04, 2006
* @date Modified Apr 04, 2006
*/
class CCommand
{
	friend class CCommandConsole;
protected:
	//! Command Name
	CString m_sCommandName;
	//! Parameters
	std::vector<CCommandParam*> m_vParams;
public:
	CCommand(CString sFunc);
	virtual ~CCommand(void);

	/**
	* Adds a command parameter to the internal list.
	*
	* @date Created Apr 05, 2006
	* @param[in]	pParam	Parameter to pass the executing command.
	* @remark Should be done before calling execute.
	*/
	void addParam(CCommandParam* pParam);

	/**
	* Removes all parameters from the command.
	*
	* @date Created Apr 05, 2006
	* @remark Should be called after the call to execute.
	*/
	void clearParams(void);

	/**
	* Executes a command with given parameters.
	*
	* @date Created Apr 04, 2006
	* @param[out]	sError	Error string buffer.
	* @return Success value.
	*/
	virtual bool execute(CString& sError) = 0;

	/**
	* Returns the syntax of the command.
	*
	* @date Created Apr 05, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax) = 0;
};


#endif //_COMMAND_H_