/**
* @file CommandCheat.h
* @author Jonathan "Awesome" Zimmer
* @date Created May 28, 2006
*
* Command used to cheat it up somethin fierce
*/

#ifndef _COMMANDCHEAT_H_
#define _COMMANDCHEAT_H_

// Local includes
#include "Command.h"
#include "hud.h"
#include "GameKernel.h"
#include "ObjectManager.h"
#include "Shotgun.h"
#include "TripleBarrelShotgun.h"
#include "RocketLauncher.h"
#include "PotatoChainGun.h"
#include "DesertEagle50.h"
#include "HolyWaterBalloonCanon.h"
#include "speedpack.h"

/**
* Command used to cheat it up somethin fierce
*
* @author Jonathan "Awesome" Zimmer
* @date Created May 28, 2006
* @date Modified May 28, 2006
*/
class CCheatCommand : public CCommand
{
public:
	CCheatCommand(void) : CCommand("Cheat") { }

	/**
	* Executes a command with given parameters.
	*
	* @date Created April 27, 2006
	* @param[in]	sError	Error string buffer.
	* @return Success value.
	*/
	bool execute(CString& sError)
	{
		if (this->m_vParams.empty())
		{
			sError.Format("Bad Syntax");
			return false;
		}

		CCommandParam* poParam = m_vParams.front();
		if(((CString)(*poParam)) == CString("moosepants"))
		{
			CObjectManager::ObjectList players;
			CObjectManager::getInstance().getObjects(OBJ_PLAYER, &players);
			CObjectManager::ObjectList::iterator playerIter = players.begin();

			while(playerIter != players.end())
			{
				CPlayer *pPlayer = (CPlayer*)(*playerIter);
				pPlayer->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "johnny_vM0053.x"));
				++playerIter;
			}

			givePlayersAllWeapons();
			return true;
		}
		else if (((CString)(*poParam)) == CString("iownfaces"))
		{
			givePlayersAllWeapons();
			return true;
		}
		else if(((CString)(*poParam)) == CString("imnotreallyawesome"))
		{
			CMenuManager::getInstance().getHUD()->addToAwesomeMeter(0);
			CMenuManager::getInstance().getHUD()->addToAwesomeMeter(MAX_AWESOME_TEX-1);
			return true;
		}
		else if(((CString)(*poParam)) == CString("ludicrousspeed"))
		{
			CObjectManager::ObjectList players;
			CObjectManager::getInstance().getObjects(OBJ_PLAYER, &players);
			CObjectManager::ObjectList::iterator playerIter = players.begin();

			while(playerIter != players.end())
			{
				CPlayer *pPlayer = ((CPlayer*)*playerIter);
				CSpeedPack* pPack = (CSpeedPack*)CObjectManager::getInstance().createObject(OBJ_ITEM_SLICKSHOES);
				pPack->setPosition(pPlayer->getPosition());
				pPack->setBV(pPlayer->getBV());
				pPack->setScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
				pPack->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "speedpack.x"));
				++playerIter;
			}
		}
		else if(((CString)(*poParam)) == CString("needalillovin"))
		{
			CObjectManager::ObjectList players;
			CObjectManager::getInstance().getObjects(OBJ_PLAYER, &players);
			CObjectManager::ObjectList::iterator playerIter = players.begin();

			while(playerIter != players.end())
			{
				CPlayer *pPlayer = ((CPlayer*)*playerIter);
				CHealthPack* pPack = (CHealthPack*)CObjectManager::getInstance().createObject(OBJ_ITEM_HEALTH);
				pPack->setPosition(pPlayer->getPosition());
				pPack->setBV(pPlayer->getBV());
				pPack->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
				pPack->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "healthpack.x"));
				++playerIter;
			}
		}
		else if(((CString)(*poParam)) == CString("needalottalovin"))
		{
			CObjectManager::ObjectList players;
			CObjectManager::getInstance().getObjects(OBJ_PLAYER, &players);
			CObjectManager::ObjectList::iterator playerIter = players.begin();

			while(playerIter != players.end())
			{
				CPlayer *pPlayer = ((CPlayer*)*playerIter);
				CHealthPack* pPack = (CHealthPack*)CObjectManager::getInstance().createObject(OBJ_ITEM_HEALTH);
				pPack->setPosition(pPlayer->getPosition());
				pPack->setBV(pPlayer->getBV());
				pPack->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
				pPack->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "largehealthpack.x"));
				pPack->setHealthPackType(EHT_LARGE_PACK);
				++playerIter;
			}
		}
		else if(((CString)(*poParam)) == CString("skipobjective"))
		{
			CAIManager::getInstance().skipCurrentObjective();
		}
		else
		{
			sError.Format("Invalid Cheat");
			return false;
		}

		return true;
	}

	/**
	* Returns the syntax of the command.
	*
	* @date Created April 27, 2006
	* @param[out]	sSyntax	Syntax string buffer.
	*/
	virtual void getSyntax(CString& sSyntax)
	{
		sSyntax = "Cheat - use codes to do awesome stuff";
	}

	/**
	* Gives all players all weapons
	*
	* @date Created May 28, 2006
	*/
	void givePlayersAllWeapons(void)
	{
		CObjectManager::ObjectList loPlayers;
		CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);
		CObjectManager::ObjectList::iterator oPlayerIter = loPlayers.begin();
		while (oPlayerIter != loPlayers.end())
		{
			// if they already have the weapon, just give them the ammo for it.
			list<CBaseWeapon*> *pWeaponList = ((CPlayer*)(*oPlayerIter))->getWeaponList();
			bool bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_SHOTGUN) == OBJ_WEAPON_SHOTGUN)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + 100);
				}
			}

			if (!bHasWeapon)
			{
				CShotgunWeapon *pNewShotgun = new CShotgunWeapon;
				pNewShotgun->init();
				pNewShotgun->setPlayer(((CPlayer*)(*oPlayerIter)));
				pNewShotgun->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "shotgun.x"));
				((CPlayer*)(*oPlayerIter))->addWeapon(pNewShotgun);
			}

			// if they already have the weapon, just give them the ammo for it.
			pWeaponList = ((CPlayer*)(*oPlayerIter))->getWeaponList();
			bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_ROCKETLAUNCHER) == OBJ_WEAPON_ROCKETLAUNCHER)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + 100);
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CRocketLauncherWeapon *pNewRocketLauncher = new CRocketLauncherWeapon;
				pNewRocketLauncher->init();
				pNewRocketLauncher->setPlayer(((CPlayer*)(*oPlayerIter)));
				pNewRocketLauncher->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "bazooka.x"));
				((CPlayer*)(*oPlayerIter))->addWeapon(pNewRocketLauncher);
			}

			// if they already have the weapon, just give them the ammo for it.
			pWeaponList = ((CPlayer*)(*oPlayerIter))->getWeaponList();
			bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_TRIPLESHOTGUN) == OBJ_WEAPON_TRIPLESHOTGUN)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + 100);
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CTripleBarrelShotgunWeapon *pNewTripleShotgun = new CTripleBarrelShotgunWeapon;
				pNewTripleShotgun->init();
				pNewTripleShotgun->setPlayer(((CPlayer*)(*oPlayerIter)));
				pNewTripleShotgun->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "tripleshotgun.x"));
				((CPlayer*)(*oPlayerIter))->addWeapon(pNewTripleShotgun);
			}

			// if they already have the weapon, just give them the ammo for it.
			pWeaponList = ((CPlayer*)(*oPlayerIter))->getWeaponList();
			bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_HOLYWATERCANNON) == OBJ_WEAPON_HOLYWATERCANNON)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + 100);
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CHolyWaterBalloonCanonWeapon *pNewWaterBalloonCanon = new CHolyWaterBalloonCanonWeapon;
				pNewWaterBalloonCanon->init();
				pNewWaterBalloonCanon->setPlayer(((CPlayer*)(*oPlayerIter)));
				pNewWaterBalloonCanon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "balonzooka.x"));
				((CPlayer*)(*oPlayerIter))->addWeapon(pNewWaterBalloonCanon);
			}

			// if they already have the weapon, just give them the ammo for it.
			pWeaponList = ((CPlayer*)(*oPlayerIter))->getWeaponList();
			bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_DEAGLE) == OBJ_WEAPON_DEAGLE)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + 100);
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CDesertEagle50Weapon *pNewDesertEagle = new CDesertEagle50Weapon;
				pNewDesertEagle->init();
				pNewDesertEagle->setPlayer(((CPlayer*)(*oPlayerIter)));
				pNewDesertEagle->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "desertEagle.x"));
				((CPlayer*)(*oPlayerIter))->addWeapon(pNewDesertEagle);
			}

			// if they already have the weapon, just give them the ammo for it.
			pWeaponList = ((CPlayer*)(*oPlayerIter))->getWeaponList();
			bHasWeapon = false;
			for (list<CBaseWeapon*>::iterator iter = pWeaponList->begin(); iter != pWeaponList->end(); ++iter)
			{
				CBaseWeapon *pWeapon = (CBaseWeapon*)*iter;
				if ((pWeapon->getType() & OBJ_WEAPON_POTATOGUN) == OBJ_WEAPON_POTATOGUN)
				{
					bHasWeapon = true;
					pWeapon->setAmmo(pWeapon->getAmmo() + 1000);
				}
			}

			if (!bHasWeapon)
			{
				// make a new shotgun to give to the player
				CPotatoChainGunWeapon *pNewPotatoChainGun = new CPotatoChainGunWeapon;
				pNewPotatoChainGun->init();
				pNewPotatoChainGun->setPlayer(((CPlayer*)(*oPlayerIter)));
				pNewPotatoChainGun->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "tatergun.x"));
				((CPlayer*)(*oPlayerIter))->addWeapon(pNewPotatoChainGun);
			}

			oPlayerIter++;
		}
	}
};

#endif /*_COMMANDSTARTPLACEMENT_H_*/