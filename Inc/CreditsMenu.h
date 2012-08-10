/**
* @file CreditsMenu.h
* @author Jordan Leckner
* @date Created March 31, 2006
*
* This file includes the Credits Menu class
* used for displaying a Credits menu and its buttons.
*/
#ifndef _CREDITSMENU_H_
#define _CREDITSMENU_H_

#include "Menu.h"
#include "MenuManager.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

#include "MainButtons.h"

struct SCredit
{
	string		name;		// Person's name
	string		job;		// What they did
	D3DXVECTOR2 position;	// Position of the credit
};

/**
* Declares the Credits menu class
* used for displaying a Credits menu and its buttons
* 
* @author Jordan Leckner
* @date Created March 31, 2006
* @date Modified April 10, 2006
*/
class CCreditsMenu : public CMenu
{
private:

	//! Our list of credits
	vector<SCredit>		m_Credits;
	//! The text button for displaying the credits
	CTextButton			*m_poTextButton;

	//! Tick timer for moving the credits up the screen
	CTickTimer			m_oTick;

	//! OK Button
	COKButton			*m_pOKButton;

public:
	/**
	* Constructs a menu
	* 
	* @date Created March 31, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	iHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CCreditsMenu() : CMenu(0, 0, CMenuManager::getInstance().getWindowWidth(), 
		CMenuManager::getInstance().getWindowHeight(), NULL, 0, 0, NULL)
	{
		m_nType = MT_CREDITS;

		CTextButton *pText = new CTextButton(128, (CMenuManager::getInstance().getWindowWidth() >> 1), 
			(CMenuManager::getInstance().getWindowWidth() >> 1), 128, NULL, NULL, 0, 0, false, this, "Arial", 16, 32);

		pText->setFormat(DT_LEFT);

		m_pOKButton = new COKButton((CMenuManager::getInstance().getWindowWidth() >> 1) - 128, 
			CMenuManager::getInstance().getWindowHeight() - 96, 256, 32, "../Textures/Menu/ok.tga", 
			"../Textures/Menu/okHover.tga", 256, 32, true, this);

		resetCredits();

		m_oTick.setInterval(0.0000001f);
	}

	/**
	* Resets the credit system
	* 
	* @date Created March 31, 2006
	*/
	inline void resetCredits()
	{
		m_Credits.clear();

		// Create all of the credits
		SCredit tCredit;
		tCredit.name = "Jordan Leckner";
		tCredit.job = "Gameplay Lead";
		tCredit.position.x = 128;
		tCredit.position.y = (float)(-(CMenuManager::getInstance().getWindowHeight()) + 
			(CMenuManager::getInstance().getWindowHeight() >> 2));
		m_Credits.push_back(tCredit);

		tCredit.name = "Matt Rudder";
		tCredit.job = "Tech Lead";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Jason Williams";
		tCredit.job = "Asset Lead";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Tom Williams";
		tCredit.job = "Project Lead";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Jon Zimmer";
		tCredit.job = "Design Lead";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Justin Gallo";
		tCredit.job = "External Producer";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Dasha Bolotina";
		tCredit.job = "Models / Textures";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Brandon Batterson";
		tCredit.job = "Rigging / Animations";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "Chris Barr";
		tCredit.job = "Web / Models";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);
		
		tCredit.name = "Mauro Velez";
		tCredit.job = "Web / Models";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);
		
		tCredit.name = "Gerard Morrison";
		tCredit.job = "Models";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

		tCredit.name = "X";
		tCredit.job = "Logo";
		tCredit.position.x = 128;
		tCredit.position.y = m_Credits[m_Credits.size() - 1].position.y - 128;
		m_Credits.push_back(tCredit);

	}

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created March 31, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
};


#endif