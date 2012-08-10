/**
* @file VendingMachineButtons.h
* @author Jordan Leckner
* @date Created April 6, 2006
*
* This file includes the Vending Machine Buttons class
* used for displaying a Vending Machine Button.
*/
#ifndef _VENDINGMACHINEBUTTONS_H_
#define _VENDINGMACHINEBUTTONS_H_

#include "MainButtons.h"
#include "Shotgun.h"
#include "TripleBarrelShotgun.h"
#include "RocketLauncher.h"
#include "PotatoChainGun.h"
#include "DesertEagle50.h"
#include "HolyWaterBalloonCanon.h"
#include "BigCluckingBomb.h"

enum EVendingMachineButtons
{
	VMB_SHOTGUN = 512,
	VMB_ROCKETLAUNCHER,
	VMB_HOLYWATERBALLOONCANON,
	VMB_TRIPLEBARRELSHOTGUN,
	VMB_BIGCLUCKINGBOMB,
	VMB_POTATOCHAINGUN,
	VMB_DESERTEAGLE,
	VMB_MAX
};

class CVendingMachine;

/**
* Declares the Shotgun Button class
* used for the action of a Shotgun Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CShotgunButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CShotgunWeapon	*m_poShotgun;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CShotgunButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_SHOTGUN;
		m_poVendingMachine = poVendingMachine;
		m_poShotgun = new CShotgunWeapon;
		m_poShotgun->init();
	}

	~CShotgunButton()
	{
		delete m_poShotgun;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Rocket Launcher Button class
* used for the action of a Rocket Launcher Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CRocketLauncherButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CRocketLauncherWeapon	*m_poRocketLauncher;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CRocketLauncherButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_ROCKETLAUNCHER;
		m_poVendingMachine = poVendingMachine;
		m_poRocketLauncher = new CRocketLauncherWeapon;
		m_poRocketLauncher->init();
	}

	~CRocketLauncherButton()
	{
		delete m_poRocketLauncher;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Holy Water Balloon Canon Button class
* used for the action of a Holy Water Balloon Canon Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CHolyWaterBalloonCanonButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CHolyWaterBalloonCanonWeapon	*m_poHolyWaterBalloonCanon;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CHolyWaterBalloonCanonButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_HOLYWATERBALLOONCANON;
		m_poVendingMachine = poVendingMachine;
		m_poHolyWaterBalloonCanon = new CHolyWaterBalloonCanonWeapon;
		m_poHolyWaterBalloonCanon->init();
	}

	~CHolyWaterBalloonCanonButton()
	{
		delete m_poHolyWaterBalloonCanon;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Potato Chain Gun Button class
* used for the action of a Potato Chain Gun Canon Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CPotatoChainGunButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CPotatoChainGunWeapon	*m_poPotatoChaingun;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CPotatoChainGunButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_POTATOCHAINGUN;
		m_poVendingMachine = poVendingMachine;
		m_poPotatoChaingun = new CPotatoChainGunWeapon;
		m_poPotatoChaingun->init();
	}

	~CPotatoChainGunButton()
	{
		delete m_poPotatoChaingun;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Desert Eagle Button class
* used for the action of a Desert Eagle Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CDesertEagleButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CDesertEagle50Weapon	*m_poDesertEagle;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CDesertEagleButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_DESERTEAGLE;
		m_poVendingMachine = poVendingMachine;
		m_poDesertEagle = new CDesertEagle50Weapon;
		m_poDesertEagle->init();
	}

	~CDesertEagleButton()
	{
		delete m_poDesertEagle;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Big Clucking Bomb Button class
* used for the action of a Big Clucking Bomb Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CBigCluckingBombButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CBigCluckingBombWeapon	*m_poBigCluckingBomb;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CBigCluckingBombButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_BIGCLUCKINGBOMB;
		m_poVendingMachine = poVendingMachine;
		m_poBigCluckingBomb = new CBigCluckingBombWeapon;
		m_poBigCluckingBomb->init();
	}

	~CBigCluckingBombButton()
	{
		delete m_poBigCluckingBomb;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

/**
* Declares the Triple Barrel Shotgun Button class
* used for the action of a Triple Barrel Shotgun Button.
* 
* @author Jordan Leckner
* @date Created April 6, 2006
* @date Modified April 6, 2006
*/
class CTripleBarrelShotgunButton : public CMenuButton
{
private:

	//! Vending Machine this weapon belongs to
	CVendingMachine *m_poVendingMachine;
	//! Shotgun weapon
	CTripleBarrelShotgunWeapon	*m_poTripleShotgun;

public:

	/**
	* Constructs a menu
	* 
	* @date Created April 6, 2006
	* @param[in]	nPosX					Position X in screen space where the menu resides.
	* @param[in]	nPosY					Position Y in screen space where the menu resides.
	* @param[in]	nWidth					Width in screen space how wide the menu is.
	* @param[in]	nHeight					Height in screen space how wide the menu is.
	* @param[in]	szTextureBackground	Filename of the texture to use for the background of the menu.
	* @param[in]	nWidthTex				Actual width of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	nHeightTex				Actual height of the image in the texture. Texture must be A power of 2. eg.(512x512)
	* @param[in]	poParentMenu			Pointer to parent Menu if available.
	*/
	CTripleBarrelShotgunButton(int nPosX, int nPosY, int nWidth, int nHeight, const char *szTextureBackground, 
		const char *szTextureBackgroundHover, int nWidthTex, int nHeightTex, 
		bool bCheckInput = true, CMenu* poParentMenu = 0, CVendingMachine *poVendingMachine = 0)
		: CMenuButton(nPosX, nPosY, nWidth, nHeight, szTextureBackground, szTextureBackgroundHover, nWidthTex, nHeightTex, bCheckInput, poParentMenu)
	{
		m_unType = VMB_TRIPLEBARRELSHOTGUN;
		m_poVendingMachine = poVendingMachine;
		m_poTripleShotgun = new CTripleBarrelShotgunWeapon;
		m_poTripleShotgun->init();
	}

	~CTripleBarrelShotgunButton()
	{
		delete m_poTripleShotgun;
	}

	/**
	* Process a specified action
	* Single Player Button will close the window and save changes
	* to the menus.
	* 
	* @date Created April 6, 2006
	*/
	virtual void doAction(void);
};

#endif