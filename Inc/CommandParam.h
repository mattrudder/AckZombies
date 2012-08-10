/**
* @file CommandParam.h
* @author Matt Rudder
* @date Created Apr 04, 2006
*
* Parameter used by a command.
*/

#ifndef _COMMANDPARAM_H_
#define _COMMANDPARAM_H_

/**
* Parameter used by a command.
*
* @author Matt Rudder
* @date Created Apr 04, 2006
* @date Modified Apr 04, 2006
*/
class CCommandParam
{
	//! Parameter data
	CString m_sData;
public:
	CCommandParam(CString sData) : m_sData(sData) { }
	virtual ~CCommandParam(void) {}

	operator int(void) { return m_sData.ToInt(); }
	operator short(void) { return m_sData.ToInt(); }
	operator long(void) { return m_sData.ToInt(); }
	operator float(void) { return (float)m_sData.ToFloat(); }
	operator double(void) { return m_sData.ToFloat(); }
	operator bool(void) { return m_sData.ToBool(); }
	operator CString(void) { return m_sData; }
	operator const char*(void) { return (const char*)m_sData; }
	operator const wchar_t*(void) { return (const wchar_t*)m_sData; }
};

#endif //_COMMANDPARAM_H_