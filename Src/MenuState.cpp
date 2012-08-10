/**
* @file MenuState.cpp
* @author Jordan Leckner
* @date Created May 9, 2006
*
* the menu state
*/

#include "MenuState.h"
#include "TestState.h"

/**
* CMenuState::tick
* @date Modified May 9, 2006
*/
void CMenuState::tick()
{
	if (m_bLoadingState)
	{
		CGameKernel::getInstance().changeState(CLoadState::getInstancePtr());

		m_bLoadingState = false;
	}
}