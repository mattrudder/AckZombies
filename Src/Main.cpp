/**
* @file Main.cpp
* @author Matt Rudder
* @date Created Mar 07, 2006
*
* Game Launchpoint
*/ 

// Visual Leak Detector (http://www.codeproject.com/tools/visualleakdetector.asp)
//#include "vld.h"
// Only include if needed.

// Engine includes
#include "AcespaceEngine.h"
#include "Actor.h"
#include "GameKernel.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CGameKernel* pKernel = CGameKernel::getInstancePtr();
	if(pKernel->initialize(hInstance))
		while(pKernel->tick()) { }
	else
#ifdef _DEBUG
		MessageBox(NULL, "Unable to initialize engine!\nIf running from within Visual Studio, make sure the proper working directory is set in the project settings\n\nGoto \"Project Properties\" > \"Debugging\" and make sure \"Working Directory\" is set to \"$(TargetDir)\".", APP_VERSION, MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, "Unable to initialize engine!", APP_VERSION, MB_OK | MB_ICONERROR);
#endif
	pKernel->deleteInstance();
}