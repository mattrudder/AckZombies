/**
* @file ConfigFile.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Utility for loading configuration files.
*/ 

#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_

// Engine includes
#include "Globals.h"

// System includes
#include <map>

/**
* Allows for persistant engine settings to be saved and loaded from disk.
*
* @author Matt Rudder
* @version 1.0
*/
class CConfigFile
{
	//! A section in the configuration file; Contains a collection of keys.
	typedef std::map<CString, CString> ConfigSection;
	//! Filename of the current config file.
	CString m_strFileName;
	//! Collection of sections in the config file.
	std::map<CString, ConfigSection> m_mSections;
public:
	CConfigFile(CString strFileName);
	CConfigFile(void);
	virtual ~CConfigFile(void);

	/**
	* Saves the current config file to disk.
	*
	* @created 01/08/2006
	* @return true if the file was saved successfully, false otherwise.
	* @{
	*/
	bool SaveConfig(void);
	bool SaveConfig(CString strFileName);
	/*@}*/

	/**
	* Loads (or reloads) a config file from disk.
	*
	* @created 01/08/2006
	* @return true if the file was loaded successfully, false otherwise.
	* @{
	*/
	bool LoadConfig(void);
	bool LoadConfig(CString strFileName);
	/*@}*/

	/**
	* Checks the currently loaded config file for a given section.
	*
	* @created 01/08/2006
	* @param strSection The section to search for.
	* @return true if the section exists, false otherwise.
	*/
	bool SectionExists(CString strSection);

	/**
	* Sets a value within the config file.
	*
	* @created 01/08/2006
	* @param strSection The section to add to.
	* @param strKey The key to add to.
	* @param strValue The value to store.
	* @remarks The function will add any sections or keys required to add the given value.
	*  It will also overwrite any previous values.
	*/
	void SetValue(CString strSection, CString strKey, CString strValue);

	/**
	* Retrieves a value from the currently loaded config file.
	*
	* @created 01/08/2006
	* @param strSection The section to retrieve the value from.
	* @param strKey The key to retrieve the value from.
	* @return The value requested, or an empty string if the section or key do not exist.
	*/
	CString GetValue(CString strSection, CString strKey);

	/**
	* Retrieves the number of keys in a given section
	*
	* @date Created Mar 28, 2006
	* @param[in]	strSection	The section to check.
	* @return The number of keys in the section.
	*/
	unsigned long getNumKeys(CString strSection);
};

/**
* Utility macros for setting variables from strings
* @param[in]	str		The string to read data from.
* @param[in]	default	The default value to use, if there is nothing in str.
* @{
*/
#define SET_BOOL(str, default) !str.GetLength() ? default : str.ToBool();
#define SET_INT(str, default) !str.GetLength() ? default : str.ToInt();
#define SET_FLOAT(str, default) !str.GetLength() ? default : str.ToFloat();
//!@}
#endif //_CONFIGFILE_H_