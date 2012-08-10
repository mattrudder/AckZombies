/**
* @file StringW.h
* @author Matt Rudder
* @date Created Mar 02, 2006
*
* Our user-friendly string class.
*/ 

#ifndef _STRINGW_H_
#define _STRINGW_H_

// Engine includes
#include "AcespaceEngine.h"

// System includes
#include <string>
#include <vector>

class CStringA;

/**
* Encapsulation of an STL std::string, to provide additional functionality much like MFC's CString.
*
* @author Matt Rudder
* @version 1.0
* @date Created December 12, 2005
*/
class CStringW
{
private:
	std::wstring m_strTheString;
public:
	friend class CStringA;

	//! Constant Blank String, used as a testing utility
	static const CStringW Blank;

	/**
	* Default Constructor.
	*
	* @date Created December 12, 2005
	*/
	CStringW(void);

	/**
	* Destructor.
	*
	* @date Created December 12, 2005
	*/
	virtual ~CStringW(void);

	/**
	* Constructs a CStringW from a c-style ASCII string.
	*
	* @date Created December 12, 2005
	*/
	CStringW(const char* str);

	/**
	* Constructs a CStringW from a c-style Unicode string.
	*
	* @date Created December 12, 2005
	*/
	CStringW(const wchar_t* str);

	/**
	* Copy constructor.
	*
	* @date Created December 12, 2005
	*/
	CStringW(const CStringW & str);

	/**
	* Construts a CStringW from a CStringA.
	*
	* @date Created December 12, 2005
	*/
	CStringW(const CStringA & str);

	/**
	* Builds a string using sprintf style syntax.
	*
	* @date Created December 12, 2005
	* @param[in] szFormat Format string used to build the final string.
	* @return The newly built string.
	* @{
	*/
	void Format(const wchar_t* wszFormat, ...);
	void Format(const char* wszFormat, ...);
	//!@}

	/**
	* Returns the buffer associated with the string.
	*
	* @date Created December 12, 2005
	* @return c-style string.
	*/
	const wchar_t* GetBuffer(void) const;

	/**
	* Returns the size of the string
	*
	* @date Created December 12, 2005
	* @return Number of characters in the string.
	*/
	size_t GetLength(void) const;

	/**
	* Retrieves a substring from the left of the original.
	*
	* @date Created December 12, 2005
	* @param nChar The size of the string to take from the original.
	* @return The new string.
	*/
	CStringW Left(size_t nChar) const;

	/**
	* Retrieves a substring from the right of the original.
	*
	* @date Created December 12, 2005
	* @param nChar The size of the string to take from the original.
	* @return The new string.
	*/
	CStringW Right(size_t nChar) const;

	/**
	* Retrieves a substring from of the original.
	*
	* @date Created December 12, 2005
	* @param nStart Where to begin within the original string.
	* @param nLen The number of characters to retrieve.
	* @return The new string, of nLen length or the remainder of the string, whichever is first.
	* @remark If nStart is larger or equal to the length of the string, a null string ("") is returned.
	*/
	CStringW Mid(size_t nStart, size_t nLen) const;

	/**
	* Trims characters from the string.
	*
	* @date Created December 12, 2005
	* @param nStart Where to start the new string.
	* @param nLen How long to make the new string from nStart.
	*/
	void Trim(size_t nStart, size_t nLen);

	/**
	* Clears the string to a null string ("").
	*
	* @date Created December 12, 2005
	*/
	void Clear(void);

	/**
	* Character manipulation function.
	*
	* @date Created December 12, 2005
	* @param nChar The index of the character
	* @return The character at a given index.
	*/
	wchar_t GetChar(size_t nChar) const;

	/**
	* Converts the current string to its numeric counter-part
	*
	* @date Created December 12, 2005
	* @return The numeric value of the string
	* @{
	*/
	float ToFloat(void) const;
	double ToDouble(void) const;
	int ToInt(void) const;
	bool ToBool(void) const;
	long ToLongFromHex(void) const;
	unsigned long ToUlongFromHex(void) const;
	//!@}

	/**
	* Converts a string delimited by a given character into a list of strings.
	*
	* @date Created December 12, 2005
	* @param vList A vector to fill out with strings.
	* @param cDelim The character to delimit the string by.
	*/
	void ToList(std::vector<CStringW>& vList, wchar_t* szDelim = L",;\n\t") const;

	/**
	* String concatenation.
	*
	* @date Created December 12, 2005
	* @param[in] str The string to concatenate to the end.
	* @return A new copy of the string.
	*/ 
	CStringW operator+(const CStringW & str) const;

	/**
	* String concatenation.
	*
	* @date Created December 12, 2005
	* @param[in] str The c-style string to concatentate to the end.
	* @return A new copy of the string.
	* @{
	*/ 
	CStringW operator+(const wchar_t* str) const;
	CStringW operator+(const char* str) const;
	//!@}

	/**
	* String concatenation.
	*
	* @date Created December 12, 2005
	* @param[in] sz The c-style string to concatentate to the end.
	* @param[in] s The string to concatentate to the end.
	* @return A new copy of the string.
	* @{
	*/ 
	friend CStringW operator+(const wchar_t* sz, const CStringW &s);
	friend CStringW operator+(const char* sz, const CStringW &s);
	//!@}

	/**
	* String concatenation assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The string to concatentate to the end.
	* @return A new copy of the string.
	*/ 
	CStringW & operator+=(const CStringW & str);

	/**
	* String concatenation assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The c-style string to concatentate to the end.
	* @return A new copy of the string.
	* @{
	*/ 
	CStringW & operator+=(const wchar_t* str);
	CStringW & operator+=(const char* str);
	//!@}

	/**
	* String assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The string to replace the current.
	* @return A new copy of the string.
	* @{
	*/
	CStringW & operator=(const CStringA & str);
	CStringW & operator=(const CStringW & str);
	//!@}

	/**
	* String assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The c-style string to replace the current.
	* @return A new copy of the string.
	* @{
	*/
	CStringW & operator=(const wchar_t* str);
	CStringW & operator=(const char* str);
	//!@}

	/**
	* String repetition. Repeats the current string a given number of times.
	*
	* @date Created December 12, 2005
	* @param[in] uiTimes The number of times to repeat the string.
	* @return A new copy of the string.
	*/
	CStringW operator*(const unsigned int uiTimes);

	/**
	* String repetition and assignment. Repeats the current string a given number of times.
	*
	* @date Created December 12, 2005
	* @param[in] uiTimes The number of times to repeat the string.
	* @return A new copy of the string.
	*/
	CStringW & operator*=(const unsigned int uiTimes);

	/**
	* Test string equality.
	*
	* @date Created December 12, 2005
	* @param str The string to test against.
	* @return The equality of the strings.
	* @{
	*/
	bool operator==(const CStringW& str) const;
	bool operator!=(const CStringW& str) const;
	//!@}

	/**
	* Test string position, in comparison to another string.
	*
	* @date Created December 12, 2005
	* @param str The string to test against.
	* @return true if the operator test value is true.
	* @{
	*/
	bool operator<(const CStringW& str) const;
	bool operator<=(const CStringW& str) const;
	bool operator>(const CStringW& str) const;
	bool operator>=(const CStringW& str) const;
	//!@}

	/**
	* Casting operator to various types.
	* 
	* @date Created December 12, 2005
	* @{
	*/
	operator CStringA(void) const;
	operator const char*(void) const;
	operator const wchar_t*(void) const;
	//!@}
};

#endif //_STRINGW_H_