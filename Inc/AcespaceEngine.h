/**
* @file AcespaceEngine.h
* @author Matt Rudder
* @date Created Mar 02, 2006
*
* Standard definitions for the engine as a whole.
*/ 

#ifndef _ACESPACEENGINE_H_
#define _ACESPACEENGINE_H_

// Version Info
#include "Version.h"
#define APP_NAME "Ack! Zombies"
#ifdef _DEBUG
	#define APP_VERSION APP_NAME " v" STRVERSION_INTERNAL
	#define ACESPACE_VERSION STRVERSION_INTERNAL
#else
	#define APP_VERSION APP_NAME " v" STRVERSION_EXTERNAL
	#define ACESPACE_VERSION STRVERSION_EXTERNAL
#endif

// Lame warnings
#pragma warning(disable : 4355)  // 'this' used in base member initializer list

// Global Includes
#include "Globals.h"
#include "DebugStream.h"
#include <iostream>
#include <assert.h>
using namespace std;

// Global Logging Objects
extern CDebugStream Log;
extern CDebugStream Debug;

// Helper Macros
#ifdef _DEBUG
#	define ACEASSERT(x) if(!x) { Debug << Console::fg_red << "Error:" << Console::def << " Assertion Failed! " << __FILE__ << " in function " << __FUNCSIG__ << " (line " << __LINE__ << "):\n" << #x << endl; } assert(x);
#else
#	define ACEASSERT(x)
#endif

// Render defines.
#define USING_ABT 0

#endif //_ACESPACEENGINE_H_