/**
* @file VendingMachineButtons.cpp
* @author Jordan Leckner
* @date Created April 6, 2006
*
* This file includes all the Vending Machine classes definitions
*/

#include "VendingMachineButtons.h"
#include "VendingMachine.h"
#include "Player.h"
#include "ObjectManager.h"
#include "SoundManager.h"

/**
* CShotgunButton::doAction
* @date Modified May 18, 2006
*/
void CShotgunButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poShotgun->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = m_poVendingMachine->getPlayer()->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_SHOTGUN) == OBJ_WEAPON_SHOTGUN)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poShotgun->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CShotgunWeapon *pNewShotgun = new CShotgunWeapon;
				pNewShotgun->init();
				pNewShotgun->setPlayer(m_poVendingMachine->getPlayer());
				pNewShotgun->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "shotgun.x"));
				m_poVendingMachine->getPlayer()->addWeapon(pNewShotgun);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poShotgun->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_SHOTGUN);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CRocketLauncherButton::doAction
* @date Modified May 18, 2006
*/
void CRocketLauncherButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poRocketLauncher->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = m_poVendingMachine->getPlayer()->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_ROCKETLAUNCHER) == OBJ_WEAPON_ROCKETLAUNCHER)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poRocketLauncher->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CRocketLauncherWeapon *pNewRocketLauncher = new CRocketLauncherWeapon;
				pNewRocketLauncher->init();
				pNewRocketLauncher->setPlayer(m_poVendingMachine->getPlayer());
				pNewRocketLauncher->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "bazooka.x"));
				m_poVendingMachine->getPlayer()->addWeapon(pNewRocketLauncher);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poRocketLauncher->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_ROCKETLAUNCHER);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CTripleBarrelShotgunButton::doAction
* @date Modified May 18, 2006
*/
void CTripleBarrelShotgunButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poTripleShotgun->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = m_poVendingMachine->getPlayer()->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_TRIPLESHOTGUN) == OBJ_WEAPON_TRIPLESHOTGUN)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poTripleShotgun->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CTripleBarrelShotgunWeapon *pNewTripleShotgun = new CTripleBarrelShotgunWeapon;
				pNewTripleShotgun->init();
				pNewTripleShotgun->setPlayer(m_poVendingMachine->getPlayer());
				pNewTripleShotgun->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "tripleshotgun.x"));
				m_poVendingMachine->getPlayer()->addWeapon(pNewTripleShotgun);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poTripleShotgun->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_TIPLESHOTGUN);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CHolyWaterBalloonCanonButton::doAction
* @date Modified May 18, 2006
*/
void CHolyWaterBalloonCanonButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poHolyWaterBalloonCanon->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = m_poVendingMachine->getPlayer()->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_HOLYWATERCANNON) == OBJ_WEAPON_HOLYWATERCANNON)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poHolyWaterBalloonCanon->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CHolyWaterBalloonCanonWeapon *pNewWaterBalloonCanon = new CHolyWaterBalloonCanonWeapon;
				pNewWaterBalloonCanon->init();
				pNewWaterBalloonCanon->setPlayer(m_poVendingMachine->getPlayer());
				pNewWaterBalloonCanon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "balonzooka.x"));
				m_poVendingMachine->getPlayer()->addWeapon(pNewWaterBalloonCanon);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poHolyWaterBalloonCanon->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_HOLYWATERBALLOONBATMAN);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CBigCluckingBombButton::doAction
* @date Modified May 18, 2006
*/
void CBigCluckingBombButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poBigCluckingBomb->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CGrenadeWeapon*> *pGrenadeList = m_poVendingMachine->getPlayer()->getGrenadeList();
			bool bHasWeapon = false;
			for (list<CGrenadeWeapon*>::iterator iter = pGrenadeList->begin(); iter != pGrenadeList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_BIGCLUCKINGBOMB) == OBJ_WEAPON_BIGCLUCKINGBOMB)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poBigCluckingBomb->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CBigCluckingBombWeapon *pNewBigCluckingBomb = new CBigCluckingBombWeapon;
				pNewBigCluckingBomb->init();
				pNewBigCluckingBomb->setPlayer(m_poVendingMachine->getPlayer());
				pNewBigCluckingBomb->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "BigCluckingBomb.x"));
				m_poVendingMachine->getPlayer()->addGrenade(pNewBigCluckingBomb);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poBigCluckingBomb->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_BIGCLUCKINGBOMB);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CDesertEagleButton::doAction
* @date Modified May 18, 2006
*/
void CDesertEagleButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poDesertEagle->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = m_poVendingMachine->getPlayer()->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_DEAGLE) == OBJ_WEAPON_DEAGLE)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poDesertEagle->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CDesertEagle50Weapon *pNewDesertEagle = new CDesertEagle50Weapon;
				pNewDesertEagle->init();
				pNewDesertEagle->setPlayer(m_poVendingMachine->getPlayer());
				pNewDesertEagle->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "desertEagle.x"));
				m_poVendingMachine->getPlayer()->addWeapon(pNewDesertEagle);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poDesertEagle->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_DEAGLE);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}

/**
* CPotatoChainGunButton::doAction
* @date Modified May 18, 2006
*/
void CPotatoChainGunButton::doAction()
{
	if (m_poVendingMachine && m_poVendingMachine->getPlayer())
	{
		CObjectManager::ObjectList lPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &lPlayers);
		CPlayer *pFirstPlayer = (CPlayer*)lPlayers.front();
		// make sure the player has enough dough
		// only take money from the first player in the list.
		if (pFirstPlayer->getAmtMoney() >= (unsigned int)m_poPotatoChaingun->getCost())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = m_poVendingMachine->getPlayer()->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_POTATOGUN) == OBJ_WEAPON_POTATOGUN)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + m_poPotatoChaingun->getAmmo());
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CPotatoChainGunWeapon *pNewPotatoChainGun = new CPotatoChainGunWeapon;
				pNewPotatoChainGun->init();
				pNewPotatoChainGun->setPlayer(m_poVendingMachine->getPlayer());
				pNewPotatoChainGun->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "tatergun.x"));
				m_poVendingMachine->getPlayer()->addWeapon(pNewPotatoChainGun);
			}
			// subtract from the players money
			pFirstPlayer->setAmtMoney(pFirstPlayer->getAmtMoney() - (unsigned int)m_poPotatoChaingun->getCost());
			CSoundManager::getInstance().playSound(SND_SELECT_POTATOCHAINGUN);
			// fade it in and back out.
			m_dwAlpha = 0;
		}
		else
		{
			// play a no money sound
			int nRand = rand() % 2;
			switch (nRand)
			{
			case 0:
				CSoundManager::getInstance().playSound(SND_EFFECT_NEEDCASH);
				break;
			case 1:
				CSoundManager::getInstance().playSound(SND_EFFECT_NOTENOUGHCASH);
				break;
			default:
				break;
			}
		}
	}
}