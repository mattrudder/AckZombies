/**
* @file VendingMachine.h
* @author Jordan Leckner
* @date Created May 15, 2006
*
* This file includes the Vending Machine class
* used for displaying a Vending Machine and its buttons.
*/
#ifndef VENDINGMACHINE_H_
#define VENDINGMACHINE_H_

#include "Actor.h"
#include "VendingMachineButtons.h"

/**
* Declares the Vending Machine class
* used for displaying a Vending Machine and its buttons.
* 
* @author Jordan Leckner
* @date Created May 15, 2006
* @date Modified May 18, 2006
*/
class CVendingMachine : public CActor
{
	friend class CVendingState;
private:

	//! Our list of vending machine buttons
	vector<CMenuButton*> m_VendingButtons;
	//! Player that activated this vending machine
	CPlayer *m_poPlayer;
	//! Sphere to check with players hitting action
	SSphere m_ActionBounds;

public:

	CVendingMachine() : m_poPlayer(NULL)
	{
		m_eType = OBJ_VENDINGMACHINE;
	}

	~CVendingMachine()
	{
	}

	/**
	* Adds weapons to the vending machine
	* 
	* @date Created May 15, 2006
	* @param[in]	eWeaponButton		Weapon to add.
	*/
	void addWeapon(EVendingMachineButtons eWeaponButton);

	/**
	* Sets the player that activated this
	* 
	* @date Created May 18, 2006
	* @param[in]	poPlayer		Player to add.
	*/
	void setPlayer(CPlayer *poPlayer)
	{
		m_poPlayer = poPlayer;
	}

	/**
	* Gets the player that activated this
	* 
	* @date Created May 18, 2006
	*/
	CPlayer * getPlayer()
	{
		return m_poPlayer;
	}

	/**
	* Gets the action bounds for the players
	* 
	* @date Created May 30, 2006
	*/
	SSphere getActionBounds()
	{
		return m_ActionBounds;
	}

	/**
	* Gets the action bounds for the players
	* 
	* @date Created May 30, 2006
	*/
	void setActionBounds(SSphere &tSphere)
	{
		m_ActionBounds.centerPt = tSphere.centerPt;
		m_ActionBounds.fRadius = tSphere.fRadius;
	}

	/**
	* Creates the VendingMachine.
	* 
	* @date Created May 15, 2006	
	*/
	static CObject* create(void)
	{
		return new CVendingMachine;
	}
};

#endif



