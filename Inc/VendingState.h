/**
* @file VendingState.h
* @author Jordan Leckner
* @date Created May 18, 2006
*
* This file includes the Vending game state
*/
#ifndef _VENDINGSTATE_H_
#define _VENDINGSTATE_H_

#include "VendingMachine.h"
#include "GameState.h"

class CVendingState : public CGameState<CVendingState>
{
protected:

	//! The vending machine menu
	CMenu *m_poVendingMenu;
	//! The vending machine
	CVendingMachine *m_poVendingMachine;
	//! Text for showing the player their money
	CTextButton *m_poPlayerMoneyText;
	//! How many times this state has been activated
	unsigned int m_unActivateTimes;

public:

	CVendingState() : m_poVendingMenu(NULL), m_poPlayerMoneyText(NULL), m_unActivateTimes(0)
	{
	}

	~CVendingState();

	//! Accessors
	unsigned int getTimesActivated() { return m_unActivateTimes; }
	//! Mutators
	void setVendingMachine(CVendingMachine *pVendingMachine)
	{
		m_poVendingMachine = pVendingMachine;
	}

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created May 18, 2006
	*/
	virtual void onActivate(void);

	/**
	* Called by the kernel when the active state is switched away from this 
	* state. This allows the state to perform any cleanup needed from its 
	* onActivate call.
	*
	* @date Created May 18, 2006
	*/
	virtual void onDeactivate(void);

	/**
	* Called each frame within the kernel's tick function, while this is the 
	* active state.
	*
	* @date Created May 18, 2006
	*/
	virtual void tick(void);

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created May 18, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice);

	/**
	* Called each frame within the render system, just after world/actor 
	* geometry is rendered but before the scene is presented while this 
	* is the active state.
	*
	* @date Created May 18, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void postRender(CRenderDevice& oRenderDevice);
};

#endif