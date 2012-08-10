/**
* @file Globals.h
* @author Matt Rudder
* @date Created Mar 02, 2006
*
* Details about the file, and what is included.
*/ 

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/**
* Compiler Warnings
*/
// MSVC 7 Ignores C++ exception spec, and feels the need to tell us.
#pragma warning(disable : 4290)
// Using a non-dll accessible data type in a dll accessible type.
#pragma warning(disable : 4251)


/**
* System-Wide Defines
*/
#define COM_RELEASE(x)			if(x) { x->Release(); x = NULL;	}
#define SAFE_DELETE(x)			if(x) { delete x; x = NULL;		}
#define SAFE_ARRAY_DELETE(x)	if(x) { delete [] x; x = NULL;	}
#define UNUSED_PARAM(x)			x;

// Global Requirements
#include "Timer.h"

/**
* String Definition
*/
#include "StringW.h"
#include "StringA.h"
#include <tchar.h>
#ifdef _UNICODE
	typedef CStringW CString;
	typedef wchar_t Char;
#else
	typedef CStringA CString;
	typedef char Char;
#endif

#endif //_GLOBAL_H_