/**
* @file ConfigFile.cpp
* @author Matt Rudder
* @date Modified Mar 28, 2006
*
* Utility for loading configuration files.
*/ 

// Local includes
#include "ConfigFile.h"

// System includes
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <crtdbg.h>

// Engine includes
#include "AcespaceEngine.h"

/**
* CConfigFile::CConfigFile
* @date Modified Mar 06, 2006
*/
CConfigFile::CConfigFile(void) {}

/**
* CConfigFile::CConfigFile
* @date Modified Mar 06, 2006
*/
CConfigFile::CConfigFile(CString strFileName)
{
	LoadConfig(strFileName);
}

/**
* CConfigFile::~CConfigFile
* @date Modified Mar 06, 2006
*/
CConfigFile::~CConfigFile(void) {}

/**
* CConfigFile::SaveConfig
* @date Modified Mar 06, 2006
*/
bool CConfigFile::SaveConfig(void)
{
	if(m_strFileName == CString::Blank)
		return false;

	// Open file
	ofstream ofFile;
	ofFile.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	try
	{
		ofFile.open(m_strFileName.GetBuffer());

		if(m_mSections.begin() == m_mSections.end())
		{
			ofFile.close();
			return true;
		}

		// Iterate config collection and write to file.
		std::map<CString, ConfigSection>::iterator itConfig = m_mSections.begin();
		ConfigSection::iterator itSection;
		ConfigSection& cfgSec = m_mSections.begin()->second;
		CStringA str;
		while(itConfig != m_mSections.end())
		{
			// Write section header.
			str = itConfig->first;
			ofFile << '[' << str.GetBuffer() << ']' << std::endl;

			// Iterate section entries
			cfgSec = itConfig->second;
			itSection = cfgSec.begin();
			while(itSection != cfgSec.end())
			{
				// Write key info
				str = itSection->first;
				ofFile << str.GetBuffer() << " = ";
				str = itSection->second;
				ofFile << str.GetBuffer() << std::endl;

				// Iterate section entries
				++itSection;
			}

			// End of section line break
			ofFile << std::endl;

			// Iterate sections
			++itConfig;
		}

		// Cleanup 
		ofFile.close();
	}
	catch(std::exception &e)
	{
		UNUSED_PARAM(e);

		if(ofFile.is_open())
			ofFile.close();

		return false;
	}

	// Return success
	return true;
}

/**
* CConfigFile::SaveConfig
* @date Modified Mar 06, 2006
*/
bool CConfigFile::SaveConfig(CString strFileName)
{
	m_strFileName = strFileName;
	return SaveConfig();
}

/**
* CConfigFile::LoadConfig
* @date Modified Mar 28, 2006
*/
bool CConfigFile::LoadConfig(void)
{
	if(m_strFileName == CString::Blank)
		return false;

	// Open file
	ifstream ifFile;
	CString strCurrentSection;
	ifFile.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	try
	{
		ifFile.open(m_strFileName.GetBuffer());

		// Clear current config
		m_mSections.clear();

		char cBuffer[512];
		char* pTestChar;
		while(!ifFile.eof())
		{
			ifFile.getline(cBuffer, 512);
			if(cBuffer[0] == '[')
			{
				// Process Section
				// Search CString for the ending bracket, and replace with NULL.
				pTestChar = cBuffer + 1;
				while(*pTestChar != ']') { ++pTestChar; }
				*pTestChar = 0;

				// Save current section
				strCurrentSection = cBuffer + 1;
			}
			else
			{
				// Process Key
				char* cKey, *cVal;
				char cDelim[] = " =\n";
				if(strCurrentSection != CString::Blank)
				{
					// Split Key/Value and save.
					cKey = strtok(cBuffer, " =\n");
					cVal = strtok(NULL, cDelim);
					
					// Continue Parsing until a value is found, then save.
					if(cVal)
					{
						m_mSections[strCurrentSection][cKey] = cVal;
					}
				}
			}
		}

		// Cleanup 
		ifFile.close();
	}
	catch(std::exception &e)
	{
		UNUSED_PARAM(e);

		bool rVal = ifFile.eof();

		if(ifFile.is_open())
			ifFile.close();

		return rVal;
	}

	// Return success
	return true;
}

/**
* CConfigFile::LoadConfig
* @date Modified Mar 06, 2006
*/
bool CConfigFile::LoadConfig(CString strFileName)
{
	m_strFileName = strFileName;
	return LoadConfig();
}

/**
* CConfigFile::SectionExists
* @date Modified Mar 06, 2006
*/
bool CConfigFile::SectionExists(CString strSection)
{
	return (m_mSections.find(strSection) != m_mSections.end());
}

/**
* CConfigFile::SetValue
* @date Modified Mar 06, 2006
*/
void CConfigFile::SetValue(CString strSection, CString strKey, CString strValue)
{
	m_mSections[strSection][strKey] = strValue;
}

/**
* CConfigFile::GetValue
* @date Modified Mar 06, 2006
*/
CString CConfigFile::GetValue(CString strSection, CString strKey)
{
	if(m_mSections.begin() == m_mSections.end())
		return CString::Blank;
	
	std::map<CString, ConfigSection>::iterator itSec = m_mSections.find(strSection);
	ConfigSection* cfgSec = 0;
	ConfigSection::iterator itConf;
	if(itSec != m_mSections.end())
	{
		cfgSec = &(itSec->second);
		itConf = cfgSec->find(strKey);
		if(itConf != itSec->second.end())
			return itConf->second;
	}

	return CString::Blank;
}

/**
* CConfigFile::getNumKeys
* @date Modified Mar 28, 2006
*/
unsigned long CConfigFile::getNumKeys(CString strSection)
{
	std::map<CString, ConfigSection>::iterator itSec = m_mSections.find(strSection);
	if(itSec == m_mSections.end())
		return 0;
	else
		return (unsigned long)itSec->second.size();
}