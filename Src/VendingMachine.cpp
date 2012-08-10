/**
* @file VendingMachine.cpp
* @author Jordan Leckner
* @date Created May 15, 2006
*
* Definition of the vending Machine
*/
#include "VendingMachine.h"
#include "MenuManager.h"

/**
* CVendingMachine::addWeapon
* @date Modified May 15, 2006
*/
void CVendingMachine::addWeapon(EVendingMachineButtons eWeaponButton)
{
	if (eWeaponButton < VMB_SHOTGUN || eWeaponButton >= VMB_MAX)
		return;

	CMenuButton *pButton;
	int nNum = m_VendingButtons.size();
	int nWindowWidth = CMenuManager::getInstance().getWindowWidth();
	int nWindowHeight = CMenuManager::getInstance().getWindowHeight();

	if (eWeaponButton == VMB_SHOTGUN)
	{
		pButton = new CShotgunButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/shotgunButton.tga", NULL,
			256, 128, true, NULL, this);
	}
	else if (eWeaponButton == VMB_ROCKETLAUNCHER)
	{
		pButton = new CRocketLauncherButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/rocketLauncherButton.tga", NULL,
			256, 128, true, NULL, this);
	}
	else if (eWeaponButton == VMB_HOLYWATERBALLOONCANON)
	{
		pButton = new CHolyWaterBalloonCanonButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/HolyWaterBalloonCanonButton.tga", NULL,
			256, 128, true, NULL, this);
	}
	else if (eWeaponButton == VMB_TRIPLEBARRELSHOTGUN)
	{
		pButton = new CTripleBarrelShotgunButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/tripleShotgunButton.tga", NULL,
			256, 128, true, NULL, this);
	}
	else if (eWeaponButton == VMB_BIGCLUCKINGBOMB)
	{
		pButton = new CBigCluckingBombButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/BigCluckingBombButton.tga", NULL,
			256, 128, true, NULL, this);
	}
	else if (eWeaponButton == VMB_POTATOCHAINGUN)
	{
		pButton = new CPotatoChainGunButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/potatoChaingunButton.tga", NULL,
			256, 128, true, NULL, this);
	}
	else if (eWeaponButton == VMB_DESERTEAGLE)
	{
		pButton = new CDesertEagleButton(nWindowWidth - 296,
			(nWindowHeight >> 1) - (nNum * 160), 256, 128, "../Textures/Menu/deagleButton.tga", NULL,
			256, 128, true, NULL, this);
	}

	m_VendingButtons.push_back(pButton);
}