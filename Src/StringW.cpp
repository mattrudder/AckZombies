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
#include <wchar.h>
#include <conio.h>
#include <stdarg.h>

const CStringW CStringW::Blank("");

/**
* CStringW::CStringW
* @date Modified Mar 02, 2006
*/
CStringW::CStringW(void) { }

/**
* CStringW::~CStringW
* @date Modified Mar 02, 2006
*/
CStringW::~CStringW(void){ }

/**
* CStringW::CStringW
* @date Modified Mar 02, 2006
*/
CStringW::CStringW(const char* str)
{
	CStringA retVal(str);
	(*this) = (CStringW)retVal;
}

/**
* CStringW::CStringW
* @date Modified Mar 02, 2006
*/
CStringW::CStringW(const wchar_t* str) : m_strTheString(str) { }

/**
* CStringW::CStringW
* @date Modified Mar 02, 2006
*/
CStringW::CStringW(const CStringA& str)
{
	CStringW retVal;
	int nStrLen = (int)str.m_strTheString.size();
	wchar_t* wszBuffer = new wchar_t[nStrLen+1];
	memset(wszBuffer, 0, (nStrLen+1) * sizeof(wchar_t));
	mbstowcs(wszBuffer, str.m_strTheString.c_str(), nStrLen);
	retVal = wszBuffer;
	delete [] wszBuffer;
	(*this) = retVal;
}

/**
* CStringW::CStringW
* @date Modified Mar 02, 2006
*/
CStringW::CStringW(const CStringW& str) : m_strTheString(str.m_strTheString) { }

/**
* CStringW::Format
* @date Modified Mar 02, 2006
*/
void CStringW::Format(const wchar_t* wszFormat, ...)
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
	m_strTheString = wszBuffer;

	// Cleanup buffer
	delete [] wszBuffer;

	// Cleanup args.
	va_end(vaList);
}

/**
* CStringW::Format
* @date Modified Mar 02, 2006
*/
void CStringW::Format(const char* szFormat, ...)
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
	CStringA str;
	str.m_strTheString = szBuffer;

	// Cleanup buffer
	delete [] szBuffer;

	// Cleanup args.
	va_end(vaList);

	// Use string conversion to save the final string
	(*this) = str;
}

/**
* CStringW::GetBuffer
* @date Modified Mar 02, 2006
*/
const wchar_t* CStringW::GetBuffer(void) const
{
	return m_strTheString.c_str();
}

/**
* CStringW::GetLength
* @date Modified Mar 02, 2006
*/
size_t CStringW::GetLength(void) const
{
	return m_strTheString.length();
}

/**
* CStringW::Left
* @date Modified Mar 02, 2006
*/
CStringW CStringW::Left(size_t nChar) const
{
	if(nChar < m_strTheString.length())
		return CStringA(m_strTheString.substr(0, nChar).c_str());
	else
		return *this;
}

/**
* CStringW::Right
* @date Modified Mar 02, 2006
*/
CStringW CStringW::Right(size_t nChar) const
{
	if(nChar < m_strTheString.length())
		return CStringA(m_strTheString.substr(m_strTheString.length() - nChar, nChar).c_str());
	else
		return *this;
}

/**
* CStringW::Mid
* @date Modified Mar 02, 2006
*/
CStringW CStringW::Mid(size_t nStart, size_t nLen) const
{
	if(nStart >= m_strTheString.length())
		return CStringA("");

	if(nStart == 0 && nLen == m_strTheString.length())
		return *this;
	else
		return CStringA(m_strTheString.substr(nStart, nLen).c_str());
}

/**
* CStringW::Trim
* @date Modified Mar 02, 2006
*/
void CStringW::Trim(size_t nStart, size_t nLen)
{
	if(nStart >= m_strTheString.length() || nStart + nLen > m_strTheString.length())
		return;

	m_strTheString = m_strTheString.substr(nStart, nLen);
}

/**
* CStringW::Clear
* @date Modified Mar 02, 2006
*/
void CStringW::Clear(void)
{
	m_strTheString.clear();
}

/**
* CStringW::GetChar
* @date Modified Mar 02, 2006
*/
wchar_t CStringW::GetChar(size_t nChar) const
{
	try
	{
		return m_strTheString.at(nChar);
	}
	catch(std::exception& e)
	{
		UNUSED_PARAM(e);
		return L'\0';
	}
}

/**
* CStringW::ToFloat
* @date Modified May 27, 2006
*/
float CStringW::ToFloat(void) const
{
	return (float)_wtof(m_strTheString.c_str());
}

/**
* CStringW::ToDouble
* @date Modified May 27, 2006
*/
double CStringW::ToDouble(void) const
{
	return _wtof(m_strTheString.c_str());
}

/**
* CStringW::ToInt
* @date Modified Mar 02, 2006
*/
int CStringW::ToInt(void) const
{
	return _wtoi(m_strTheString.c_str());
}

/**
* CStringW::ToBool
* @date Modified Mar 28, 2006
*/
bool CStringW::ToBool(void) const
{
	return !_wcsicmp(m_strTheString.c_str(), L"true");
}

/**
* CStringW::ToLongFromHex
* @date Modified Mar 29, 2006
*/
long CStringW::ToLongFromHex(void) const
{
	wchar_t* pStopString;
	return wcstol(m_strTheString.c_str(), &pStopString, 0);
}

/**
* CStringW::ToUlongFromHex
* @date Modified Mar 29, 2006
*/
unsigned long CStringW::ToUlongFromHex(void) const
{
	wchar_t* pStopString;
	return wcstoul(m_strTheString.c_str(), &pStopString, 0);
}

/**
* CStringW::ToList
* @date Modified Mar 02, 2006
*/
void CStringW::ToList(std::vector<CStringW>& vList, wchar_t* szDelim) const
{
	// Clear the incoming vector.
	vList.clear();
	
	// Copy the string to an outside buffer.
	wchar_t* szString = new wchar_t[m_strTheString.length()+1];
	memset(szString, 0, (m_strTheString.length()+1) * sizeof(wchar_t));
	wcsncpy(szString, m_strTheString.c_str(), m_strTheString.length());

	// Split up the string.
	wchar_t* pTok = wcstok(szString, szDelim);
	while(pTok)
	{
		vList.push_back(CStringW(pTok));
		pTok = wcstok(NULL, szDelim);
	}

	// Cleanup
	delete [] szString;
}

/**
* CStringW::operator+
* @date Modified Mar 02, 2006
*/
CStringW CStringW::operator+(const CStringW & str) const
{
	CStringW retVal;
	retVal.m_strTheString = m_strTheString + str.m_strTheString;
	return retVal;
}

/**
* CStringW::operator+
* @date Modified Mar 02, 2006
*/
CStringW CStringW::operator+(const wchar_t* str) const
{
	CStringW retVal;
	retVal.m_strTheString = m_strTheString + str;
	return retVal;
}

/**
* CStringW::operator+
* @date Modified Mar 02, 2006
*/
CStringW operator+(const wchar_t* sz, const CStringW & s)
{
	CStringW retVal;
	retVal.m_strTheString = sz + s.m_strTheString;
	return retVal;
}

/**
* CStringW::operator+
* @date Modified Mar 02, 2006
*/
CStringW CStringW::operator+(const char* str) const
{
	CStringA cat(str);
	CStringW retVal;
	retVal.m_strTheString = m_strTheString + ((CStringW)cat).m_strTheString;
	return retVal;
}

/**
* CStringW::operator+
* @date Modified Mar 02, 2006
*/
CStringW operator+(const char* sz, const CStringW & s)
{
	CStringA cat(sz);
	CStringW retVal;
	retVal.m_strTheString = ((CStringW)cat).m_strTheString + s.m_strTheString;
	return retVal;
}

/**
* CStringW::operator+=
* @date Modified Mar 02, 2006
*/
CStringW & CStringW::operator+=(const CStringW & str)
{
	m_strTheString += str.m_strTheString;
	return (*this);
}

CStringW & CStringW::operator+=(const wchar_t* str)
{
	m_strTheString += str;
	return (*this);
}

/**
* CStringW::operator+=
* @date Modified Mar 02, 2006
*/
CStringW & CStringW::operator+=(const char* str)
{
	CStringA cat(str);
	m_strTheString += ((CStringW)cat).m_strTheString;
	return (*this);
}

/**
* CStringW::operator=
* @date Modified Mar 02, 2006
*/
CStringW & CStringW::operator=(const CStringW & str)
{
	m_strTheString = str.m_strTheString;
	return (*this);
}

/**
* CStringW::operator=
* @date Modified Mar 02, 2006
*/
CStringW & CStringW::operator=(const CStringA & str)
{
	(*this) = (CStringW)str;
	return (*this);
}

/**
* CStringW::operator=
* @date Modified Mar 02, 2006
*/
CStringW & CStringW::operator=(const wchar_t* str)
{
	if(str)
		m_strTheString = str;
	return (*this);
}

/**
* CStringW::operator=
* @date Modified Mar 02, 2006
*/
CStringW & CStringW::operator=(const char* str)
{
	if(str)
	{
		CStringA cat(str);
		m_strTheString = ((CStringW)cat).m_strTheString;
	}
	return (*this);
}

CStringW CStringW::operator*(const unsigned int uiTimes)
{
	CStringW retVal;
	for(unsigned int i = 0; i < uiTimes; ++i)
		retVal += (*this);
	return retVal;
}
CStringW & CStringW::operator*=(const unsigned int uiTimes)
{
	CStringW retVal;
	for(unsigned int i = 0; i < uiTimes; ++i)
		retVal += (*this);
	(*this) = retVal;
	return (*this);
}

/**
* CStringW::operator==
* @date Modified Mar 02, 2006
*/
bool CStringW::operator==(const CStringW& str) const
{
	return wcsicmp(m_strTheString.c_str(), str.m_strTheString.c_str()) == 0;
}

/**
* CStringW::operator!=
* @date Modified Mar 02, 2006
*/
bool CStringW::operator!=(const CStringW& str) const
{
	return wcsicmp(m_strTheString.c_str(), str.m_strTheString.c_str()) != 0;
}

/**
* CStringW::operator<
* @date Modified Mar 02, 2006
*/
bool CStringW::operator<(const CStringW& str) const
{
	return wcsicmp(m_strTheString.c_str(), str.m_strTheString.c_str()) < 0;
}

/**
* CStringW::operator<=
* @date Modified Mar 02, 2006
*/
bool CStringW::operator<=(const CStringW& str) const
{
	return wcsicmp(m_strTheString.c_str(), str.m_strTheString.c_str()) <= 0;
}

/**
* CStringW::operator>
* @date Modified Mar 02, 2006
*/
bool CStringW::operator>(const CStringW& str) const
{
	return wcsicmp(m_strTheString.c_str(), str.m_strTheString.c_str()) > 0;
}

/**
* CStringW::operator>=
* @date Modified Mar 02, 2006
*/
bool CStringW::operator>=(const CStringW& str) const
{
	return wcsicmp(m_strTheString.c_str(), str.m_strTheString.c_str()) >= 0;
}

/**
* CStringW::operator CStringA
* @date Modified Mar 02, 2006
*/
CStringW::operator CStringA(void) const
{
	CStringA retVal;
	int nStrLen = (int)m_strTheString.size();
	char* szBuffer = new char[nStrLen+1];
	memset(szBuffer, 0, (nStrLen+1) * sizeof(char));
	wcstombs(szBuffer, m_strTheString.c_str(), nStrLen);
	retVal = szBuffer;
	delete [] szBuffer;
	return retVal;
}

/**
* CStringW::operator const char*
* @date Modified Mar 02, 2006
*/
CStringW::operator const char*(void) const
{
	return ((CStringA)*this).GetBuffer();
}

/**
* CStringW::operator const wchar_t*
* @date Modified Mar 02, 2006
*/
CStringW::operator const wchar_t*(void) const
{
	return m_strTheString.c_str();
}