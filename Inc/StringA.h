/**
* @file StringW.h
* @author Matt Rudder
* @date Created Mar 02, 2006
*
* Our user-friendly string class.
*/ 

#ifndef _STRINGA_H_
#define _STRINGA_H_

// Engine includes
#include "AcespaceEngine.h"

// System includes
#include <string>
#include <vector>

class CStringW;

/**
* Encapsulation of an STL std::string, to provide additional functionality much like MFC's CString.
*
* @author Matt Rudder
* @version 1.0
* @date Created December 12, 2005
*/
class CStringA
{
private:
	std::string m_strTheString;
public:
	friend class CStringW;

	//! Constant Blank String, used as a testing utility
	static const CStringA Blank;

	/**
	* Default Constructor.
	*
	* @date Created December 12, 2005
	*/
	CStringA(void);

	/**
	* Destructor.
	*
	* @date Created December 12, 2005
	*/
	virtual ~CStringA(void);

	/**
	* Constructs a CStringA from a c-style ASCII string.
	*
	* @date Created December 12, 2005
	*/
	CStringA(const char* str);

	/**
	* Constructs a CStringA from a c-style Unicode string.
	*
	* @date Created December 12, 2005
	*/
	CStringA(const wchar_t* str);

	/**
	* Copy constructor.
	*
	* @date Created December 12, 2005
	*/
	CStringA(const CStringA & str);

	/**
	* Construts a CStringA froma CStringW.
	*
	* @date Created December 12, 2005
	*/
	CStringA(const CStringW & str);

	/**
	* Builds a string using sprintf style syntax.
	*
	* @date Created December 12, 2005
	* @param[in] szFormat Format string used to build the final string.
	* @return The newly built string.
	* @{
	*/
	void Format(const char* szFormat, ...);
	void Format(const wchar_t* szFormat, ...);
	//!@}

	/**
	* Returns the buffer associated with the string.
	*
	* @date Created December 12, 2005
	* @return c-style string.
	*/
	const char* GetBuffer(void) const;

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
	CStringA Left(size_t nChar) const;

	/**
	* Retrieves a substring from the right of the original.
	*
	* @date Created December 12, 2005
	* @param nChar The size of the string to take from the original.
	* @return The new string.
	*/
	CStringA Right(size_t nChar) const;

	/**
	* Retrieves a substring from of the original.
	*
	* @date Created December 12, 2005
	* @param nStart Where to begin within the original string.
	* @param nLen The number of characters to retrieve.
	* @return The new string, of nLen length or the remainder of the string, whichever is first.
	* @remark If nStart is larger or equal to the length of the string, a null string ("") is returned.
	*/
	CStringA Mid(size_t nStart, size_t nLen) const;

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
	char GetChar(size_t nChar) const;

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
	void ToList(std::vector<CStringA>& vList, char* szDelim = ",;\n\t") const;

	//@{
	/**
	* String concatenation.
	*
	* @date Created December 12, 2005
	* @param[in] str The string to concatenate to the end.
	* @return A new copy of the string.
	*/ 
	CStringA operator+(const CStringA & str) const;

	/**
	* String concatenation.
	*
	* @date Created December 12, 2005
	* @param[in] str The c-style string to concatentate to the end.
	* @return A new copy of the string.
	* @{
	*/ 
	CStringA operator+(const char* str) const;
	CStringA operator+(const wchar_t* str) const;
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
	friend CStringA operator+(const char* sz, const CStringA &s);
	friend CStringA operator+(const wchar_t* sz, const CStringA &s);
	//!@}

	/**
	* String concatenation assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The string to concatentate to the end.
	* @return A new copy of the string.
	*/ 
	CStringA & operator+=(const CStringA & str);

	/**
	* String concatenation assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The c-style string to concatentate to the end.
	* @return A new copy of the string.
	* @{
	*/ 
	CStringA & operator+=(const char* str);
	CStringA & operator+=(const wchar_t* str);
	//!@}

	/**
	* String assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The string to replace the current.
	* @return A new copy of the string.
	* @{
	*/
	CStringA & operator=(const CStringA & str);
	CStringA & operator=(const CStringW & str);
	//!@}

	/**
	* String assignment.
	*
	* @date Created December 12, 2005
	* @param[in] str The c-style string to replace the current.
	* @return A new copy of the string.
	* @{
	*/
	CStringA & operator=(const char* str);
	CStringA & operator=(const wchar_t* str);
	//!@}

	/**
	* String repetition. Repeats the current string a given number of times.
	*
	* @date Created December 12, 2005
	* @param[in] uiTimes The number of times to repeat the string.
	* @return A new copy of the string.
	*/
	CStringA operator*(const unsigned int uiTimes);

	/**
	* String repetition and assignment. Repeats the current string a given number of times.
	*
	* @date Created December 12, 2005
	* @param[in] uiTimes The number of times to repeat the string.
	* @return A new copy of the string.
	*/
	CStringA & operator*=(const unsigned int uiTimes);

	/**
	* Test string equality.
	*
	* @date Created December 12, 2005
	* @param str The string to test against.
	* @return The equality of the strings.
	* @{
	*/
	bool operator==(const CStringA& str) const;
	bool operator!=(const CStringA& str) const;
	//!@}

	/**
	* Test string position, in comparison to another string.
	*
	* @date Created December 12, 2005
	* @param str The string to test against.
	* @return true if the operator test value is true.
	* @{
	*/
	bool operator<(const CStringA& str) const;
	bool operator<=(const CStringA& str) const;
	bool operator>(const CStringA& str) const;
	bool operator>=(const CStringA& str) const;
	//@}

	/**
	* Casting Operators.
	*
	* @date Created December 12, 2005
	* @{
	*/
	operator CStringW(void) const;
	operator const char*(void) const;
	operator const wchar_t*(void) const;
	//!@}
};

#endif //_STRINGA_H_