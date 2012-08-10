/**
* @file StringW.h
* @author Matt Rudder
* @date Modified Mar 02, 2006
*
* Our user-friendly string class.
*/ 

// Engine include
#include "Globals.h"

// Local include
#include "StringA.h"
#include "StringW.h"

// System include
#include <stdio.h>
#include <conio.h>
#include <stdarg.h>

const CStringA CStringA::Blank("");

/**
* CStringA::CStringA
* @date Modified Mar 02, 2006
*/
CStringA::CStringA(void) { }

/**
* CStringA::~CStringA
* @date Modified Mar 02, 2006
*/
CStringA::~CStringA(void){ }

/**
* CStringA::CStringA
* @date Modified Mar 02, 2006
*/
CStringA::CStringA(const char* str) : m_strTheString(str) { }

/**
* CStringA::CStringA
* @date Modified Mar 02, 2006
*/
CStringA::CStringA(const wchar_t* str)
{
	CStringW retVal(str);
	(*this) = (CStringA)retVal;
}

/**
* CStringA::CStringA
* @date Modified Mar 02, 2006
*/
CStringA::CStringA(const CStringA& str) : m_strTheString(str.m_strTheString) { }

/**
* CStringA::CStringA
* @date Modified Mar 02, 2006
*/
CStringA::CStringA(const CStringW& str)
{
	CStringA retVal;
	int nStrLen = (int)str.m_strTheString.size();
	char* szBuffer = new char[nStrLen+1];
	memset(szBuffer, 0, (nStrLen+1) * sizeof(char));
	wcstombs(szBuffer, str.m_strTheString.c_str(), nStrLen);
	retVal = szBuffer;
	delete [] szBuffer;
	(*this) = retVal;
}

/**
* CStringA::Format
* @date Modified Mar 02, 2006
*/
void CStringA::Format(const char* szFormat, ...)
{
	// Get arguments
	va_list vaList;
	va_start(vaList, szFormat);

	// Determine size of full string.
	int nSize = _vscprintf(szFormat, vaList);

	// Allocate buffer.
	char* szBuffer = new char[nSize+1];
	memset(szBuffer, 0, sizeof(char) * (nSize+1));

	// Save string.
	vsprintf(szBuffer, szFormat, vaList);
	m_strTheString = szBuffer;

	// Cleanup buffer
	delete [] szBuffer;

	// Cleanup args.
	va_end(vaList);
}

/**
* CStringA::Format
* @date Modified Mar 02, 2006
*/
void CStringA::Format(const wchar_t* wszFormat, ...)
{
	// Get arguments
	va_list vaList;
	va_start(vaList, wszFormat);

	// Determine size of full string.
	int nSize = _vscwprintf(wszFormat, vaList);

	// Allocate buffer.
	wchar_t* wszBuffer = new wchar_t[nSize+1];
	memset(wszBuffer, 0, sizeof(wchar_t) * (nSize+1));

	// Save string.
	vswprintf(wszBuffer, wszFormat, vaList);
	CStringW str;
	str.m_strTheString = wszBuffer;

	// Cleanup buffer
	delete [] wszBuffer;

	// Cleanup args.
	va_end(vaList);

	// Use string conversion to save the final string
	(*this) = str;
}

/**
* CStringA::GetBuffer
* @date Modified Mar 02, 2006
*/
const char* CStringA::GetBuffer(void) const
{
	return m_strTheString.c_str();
}

/**
* CStringA::GetLength
* @date Modified Mar 02, 2006
*/
size_t CStringA::GetLength(void) const
{
	return m_strTheString.length();
}

/**
* CStringA::Left
* @date Modified Mar 02, 2006
*/
CStringA CStringA::Left(size_t nChar) const
{
	if(nChar < m_strTheString.length())
		return CStringA(m_strTheString.substr(0, nChar).c_str());
	else
		return *this;
}

/**
* CStringA::Right
* @date Modified Mar 02, 2006
*/
CStringA CStringA::Right(size_t nChar) const
{
	if(nChar < m_strTheString.length())
		return CStringA(m_strTheString.substr(m_strTheString.length() - nChar, nChar).c_str());
	else
		return *this;
}

/**
* CStringA::Mid
* @date Modified Mar 02, 2006
*/
CStringA CStringA::Mid(size_t nStart, size_t nLen) const
{
	if(nStart >= m_strTheString.length())
		return CStringA("");

	if(nStart == 0 && nLen == m_strTheString.length())
		return *this;
	else
		return CStringA(m_strTheString.substr(nStart, nLen).c_str());
}

/**
* CStringA::Trim
* @date Modified Mar 02, 2006
*/
void CStringA::Trim(size_t nStart, size_t nLen)
{
	if(nStart >= m_strTheString.length() || nStart + nLen > m_strTheString.length())
		return;

	m_strTheString = m_strTheString.substr(nStart, nLen);
}

/**
* CStringA::Clear
* @date Modified Mar 02, 2006
*/
void CStringA::Clear(void)
{
	m_strTheString.clear();
}

/**
* CStringA::GetChar
* @date Modified Mar 02, 2006
*/
char CStringA::GetChar(size_t nChar) const
{
	try
	{
		return m_strTheString.at(nChar);
	}
	catch(std::exception& e)
	{
		UNUSED_PARAM(e);
		return '\0';
	}
}

/**
* CStringA::ToFloat
* @date Modified May 27, 2006
*/
float CStringA::ToFloat(void) const
{
	return (float)atof(m_strTheString.c_str());
}

/**
* CStringA::ToDouble
* @date Modified May 27, 2006
*/
double CStringA::ToDouble(void) const
{
	return atof(m_strTheString.c_str());
}

/**
* CStringA::ToInt
* @date Modified Mar 02, 2006
*/
int CStringA::ToInt(void) const
{
	return atoi(m_strTheString.c_str());
}

/**
* CStringA::ToBool
* @date Modified Mar 28, 2006
*/
bool CStringA::ToBool(void) const
{
	return !stricmp(m_strTheString.c_str(), "true");
}

/**
* CStringA::ToLongFromHex
* @date Modified Mar 29, 2006
*/
long CStringA::ToLongFromHex(void) const
{
	char* pStopString;
	return strtol(m_strTheString.c_str(), &pStopString, 0);
}
/**
* CStringA::ToUlongFromHex
* @date Modified Mar 29, 2006
*/
unsigned long CStringA::ToUlongFromHex(void) const
{
	char* pStopString;
	return strtoul(m_strTheString.c_str(), &pStopString, 0);
}

/**
* CStringA::ToList
* @date Modified Mar 02, 2006
*/
void CStringA::ToList(std::vector<CStringA>& vList, char* szDelim) const
{
	// Clear the incoming vector.
	vList.clear();
	
	// Copy the string to an outside buffer.
	char* szString = new char[m_strTheString.length()+1];
	memset(szString, 0, m_strTheString.length()+1);
	strncpy(szString, m_strTheString.c_str(), m_strTheString.length());

	// Split up the string.
	char* pTok = strtok(szString, szDelim);
	while(pTok)
	{
		vList.push_back(CStringA(pTok));
		pTok = strtok(NULL, szDelim);
	}

	// Cleanup
	delete [] szString;
}

/**
* CStringA::operator+
* @date Modified Mar 02, 2006
*/
CStringA CStringA::operator+(const CStringA & str) const
{
	CStringA retVal;
	retVal.m_strTheString = m_strTheString + str.m_strTheString;
	return retVal;
}

/**
* CStringA::operator+
* @date Modified Mar 02, 2006
*/
CStringA CStringA::operator+(const char* str) const
{
	CStringA retVal;
	retVal.m_strTheString = m_strTheString + str;
	return retVal;
}

/**
* CStringA::operator+
* @date Modified Mar 02, 2006
*/
CStringA operator+(const char* sz, const CStringA & s)
{
	CStringA retVal;
	retVal.m_strTheString = sz + s.m_strTheString;
	return retVal;
}

/**
* CStringA::operator+
* @date Modified Mar 02, 2006
*/
CStringA CStringA::operator+(const wchar_t* str) const
{
	CStringW cat(str);
	CStringA retVal;
	retVal.m_strTheString = m_strTheString + ((CStringA)cat).m_strTheString;
	return retVal;
}

/**
* CStringA::operator+
* @date Modified Mar 02, 2006
*/
CStringA operator+(const wchar_t* sz, const CStringA & s)
{
	CStringW cat(sz);
	CStringA retVal;
	retVal.m_strTheString = ((CStringA)cat).m_strTheString + s.m_strTheString;
	return retVal;
}

/**
* CStringA::operator+=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator+=(const CStringA & str)
{
	m_strTheString += str.m_strTheString;
	return (*this);
}

/**
* CStringA::operator+=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator+=(const char* str)
{
	m_strTheString += str;
	return (*this);
}

/**
* CStringA::operator+=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator+=(const wchar_t* str)
{
	CStringW cat(str);
	m_strTheString += ((CStringA)cat).m_strTheString;
	return (*this);
}

/**
* CStringA::operator=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator=(const CStringA & str)
{
	m_strTheString = str.m_strTheString;
	return (*this);
}

/**
* CStringA::operator=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator=(const CStringW & str)
{
	(*this) = (CStringA)str;
	return (*this);
}

/**
* CStringA::operator=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator=(const char* str)
{
	if(str)
		m_strTheString = str;
	return (*this);
}

/**
* CStringA::operator=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator=(const wchar_t* str)
{
	if(str)
	{
		CStringW cat(str);
		m_strTheString = ((CStringA)cat).m_strTheString;
	}
	return (*this);
}

/**
* CStringA::operator*
* @date Modified Mar 02, 2006
*/
CStringA CStringA::operator*(const unsigned int uiTimes)
{
	CStringA retVal;
	for(unsigned int i = 0; i < uiTimes; ++i)
		retVal += (*this);
	return retVal;
}

/**
* CStringA::operator*=
* @date Modified Mar 02, 2006
*/
CStringA & CStringA::operator*=(const unsigned int uiTimes)
{
	CStringA retVal;
	for(unsigned int i = 0; i < uiTimes; ++i)
		retVal += (*this);
	(*this) = retVal;
	return (*this);
}

/**
* CStringA::operator==
* @date Modified Mar 02, 2006
*/
bool CStringA::operator==(const CStringA& str) const
{
	return stricmp(m_strTheString.c_str(), str.m_strTheString.c_str()) == 0;
}

/**
* CStringA::operator!=
* @date Modified Mar 02, 2006
*/
bool CStringA::operator!=(const CStringA& str) const
{
	return stricmp(m_strTheString.c_str(), str.m_strTheString.c_str()) != 0;
}

/**
* CStringA::operator<
* @date Modified Mar 02, 2006
*/
bool CStringA::operator<(const CStringA& str) const
{
	return stricmp(m_strTheString.c_str(), str.m_strTheString.c_str()) < 0;
}

/**
* CStringA::operator<=
* @date Modified Mar 02, 2006
*/
bool CStringA::operator<=(const CStringA& str) const
{
	return stricmp(m_strTheString.c_str(), str.m_strTheString.c_str()) <= 0;
}

/**
* CStringA::operator>
* @date Modified Mar 02, 2006
*/
bool CStringA::operator>(const CStringA& str) const
{
	return stricmp(m_strTheString.c_str(), str.m_strTheString.c_str()) > 0;
}

/**
* CStringA::operator>=
* @date Modified Mar 02, 2006
*/
bool CStringA::operator>=(const CStringA& str) const
{
	return stricmp(m_strTheString.c_str(), str.m_strTheString.c_str()) >= 0;
}

/**
* CStringA::operator CStringW
* @date Modified Mar 02, 2006
*/
CStringA::operator CStringW(void) const
{
	CStringW retVal;
	int nStrLen = (int)m_strTheString.size();
	wchar_t* wszBuffer = new wchar_t[nStrLen+1];
	memset(wszBuffer, 0, (nStrLen+1) * sizeof(wchar_t));
	mbstowcs(wszBuffer, m_strTheString.c_str(), nStrLen);
	retVal = wszBuffer;
	delete [] wszBuffer;
	return retVal;
}
/**
* CStringA::operator const char*
* @date Modified Mar 02, 2006
*/
CStringA::operator const char*(void) const
{
	return m_strTheString.c_str();
}
/**
* CStringA::operator const wchar_t*
* @date Modified Mar 02, 2006
*/
CStringA::operator const wchar_t*(void) const
{
	return ((CStringW)*this).GetBuffer();
}