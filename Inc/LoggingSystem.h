/**
* @file LoggingSystem.h
* @author Matt Rudder
* @date Created Mar 27, 2006
*
* Utility manager used to write logging information to file.
*/

#ifndef _LOGGINGSYSTEM_H_
#define _LOGGINGSYSTEM_H_

// Engine includes
#include "AcespaceEngine.h"
#include "Singleton.h"

// System includes
#include <fstream>

/**
* Utility manager used to write logging information to file.
*
* @author Matt Rudder
* @date Created Mar 27, 2006
* @date Modified Mar 27, 2006
*/
class CLoggingSystem : public CSingleton<CLoggingSystem>
{
protected:
	friend class CSingleton<CLoggingSystem>;

	//! Singleton stuff.
	//!@{
	CLoggingSystem(void);
	CLoggingSystem(const CLoggingSystem&) {}
	operator=(const CLoggingSystem&){}
	virtual ~CLoggingSystem(void);
	//!@}

	//! Global log file
	std::ofstream m_ofLogFile;
	//! Current File
	CString m_sCurFile;
	//! Current Function
	CString m_sCurFunc;
public:

	/**
	* Adds a line to the specified log.
	*
	* @date Created Mar 27, 2006
	* @param[in]	unLine	The line number the entry is on
	* @param[in]	sText	The text to write to file
	*/
	void addEntry(unsigned int unLine, CString sText);

	/**
	* Begins a new section within a log file.
	*
	* @date Created Mar 27, 2006
	* @param[in]	sFile	The name of the file the entry is in
	* @param[in]	sFunc	The name of the function the entry is in
	*/
	void beginSection(CString sFile, CString sFunc);

	/**
	* Exits a current section within the log
	*
	* @date Created Mar 27, 2006
	*/
	void endSection(void);
};

//! Debug helpers
#define BEGIN_LOG()\
	CLoggingSystem::getInstance().beginSection(__FILE__, __FUNCDNAME__);

#define END_LOG()\
	CLoggingSystem::getInstance().endSection();

#ifdef _DEBUG
#	define DEBUGLOG(x)\
	CLoggingSystem::getInstance().addEntry(__LINE__, x);
#else
#	define DEBUGLOG(x)
#endif

#define STATUSLOG(x)\
	CLoggingSystem::getInstance().addEntry(__LINE__, x);

#endif // _LOGGINGSYSTEM_H_