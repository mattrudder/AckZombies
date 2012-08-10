/**
* @file HighScoreMenu.h
* @author Jordan Leckner
* @date Created April 5, 2006
*
* This file includes the HighScore Menu class
* used for displaying a HighScore menu and its buttons.
*/
#ifndef _HIGHSCOREMENU_H_
#define _HIGHSCOREMENU_H_

#include "Menu.h"
#include "MenuManager.h"
#include <string>
#include <queue>
using std::priority_queue;
using std::string;

#include "MainButtons.h"

// high score table
struct SHighScore
{
	float fScore;
	char szName[32];

	SHighScore()
	{
		memset(szName, 0, 32);
		fScore = 0.0f;
	}

	// Define the sort for the priority queue
	bool operator < (const SHighScore &score) const
	{
		return (score.fScore > fScore);
	}
};

/**
* Declares the HighScore table class
* used for storing the high score table
* 
* @author Jordan Leckner
* @date Created May 21, 2006
* @date Modified May 21, 2006
*/
class CHighScoreTable : public CSingleton<CHighScoreTable>
{
	friend class CSingleton<CHighScoreTable>;
private:

	//! the top 10 high scores
	priority_queue<SHighScore>		m_HighScoreList;
	//! the lowest score in the list
	float							m_fLowestScore;

	//! Singleton stuff.
	//!@{
	CHighScoreTable(const CHighScoreTable&) {}
	operator=(const CHighScoreTable&){}

	CHighScoreTable()
	{
		// read in the high scores
		loadHighScoreTable("HighScoreTable.bin");
	}

	virtual ~CHighScoreTable()
	{
		saveHighScoreTable("HighScoreTable.bin");
	}
	//!@}

public:

	//! Accessors
	//!@{
	priority_queue<SHighScore> * getHighScoreList()
	{
		return &m_HighScoreList;
	}
	float getLowestScore()
	{
		return m_fLowestScore;
	}
	//!@}

	//! Mutators
	//!@{
	void setLowestScore(float fScore)
	{
		m_fLowestScore = fScore;
	}
	//!@}

	/**
	* Inserts a score in the high score table
	* 
	* @date Created April 8, 2006
	* @param[in]	tHighScore	high score to enter
	* @return true if the score was entered in the high score table
	*/
	bool insertScore(SHighScore tHighScore);

	/**
	* Inserts a score in the high score table
	* 
	* @date Created April 8, 2006
	* @param[in]	szName	name of the high score person
	* @param[in]	nScore	Score of the person 
	* @return true if the score was entered in the high score table
	*/
	bool insertScore(char *szName, float fScore);

	/**
	* Loads in the high score table
	* 
	* @date Created April 5, 2006
	* @param[in]	szFileName			Filename of the high score table
	*/
	void loadHighScoreTable(char *szFileName);

	/**
	* Saves in the high score table
	* 
	* @date Created April 14, 2006
	* @param[in]	szFileName			Filename of the high score table
	*/
	void saveHighScoreTable(char *szFileName);
};

/**
* Declares the HighScore menu class
* used for displaying a HighScore menu and its buttons
* 
* @author Jordan Leckner
* @date Created April 5, 2006
* @date Modified April 14, 2006
*/
class CHighScoreMenu : public CMenu
{
private:

	//! The text button for displaying the credits
	CTextButton						*m_poTextButton;

public:
	/**
	* Constructs a menu
	* 
	* @date Created April 5, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	iHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground		Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CHighScoreMenu() : CMenu(0, 0, CMenuManager::getInstance().getWindowWidth(), 
		CMenuManager::getInstance().getWindowHeight(), NULL, 0, 0, NULL)
	{
		m_nType = MT_HIGHSCORE;

		CTextButton *pText = new CTextButton(128, (CMenuManager::getInstance().getWindowWidth() >> 1), 
			(CMenuManager::getInstance().getWindowWidth() >> 1), 128, NULL, NULL, 0, 0, false, this, 
			"Arial", 16, 32);

		pText->setFormat(DT_LEFT);

		new COKButton((CMenuManager::getInstance().getWindowWidth() >> 1) - 128, 
			CMenuManager::getInstance().getWindowHeight() - 96, 256, 32, "../Textures/Menu/ok.tga", 
			"../Textures/Menu/okHover.tga", 256, 32, true, this);
	}

public:

	/**
	* Draws a menu, its buttons, and its children menus.
	* 
	* @date Created April 5, 2006
	* @param[in]	pMouseCursor			Mouse cursor position
	*/
	virtual void drawMenu(D3DXVECTOR2 *pMouseCursor);
};


#endif