/**
* @file Enemy.cpp
* @author Jonathan "Awesome" Zimmer
* @date Created April 2, 2006
*
* This file contains the implementation of the CEnemy class
*/

#include "Enemy.h"
#include "BaseWeapon.h"
#include "AIStatePathPlan.h"
#include "ObjectManager.h"
#include "coin.h" 
#include "speedpack.h"
#include "healthpack.h"
#include "Acidic.h"
#include "AIStateZombieAttack.h"
#include "ParticleEmitter.h"

/**
* CEnemy::CEnemy
* @date Modified April 2, 2006
*/
CEnemy::CEnemy(EObjectType eType) : CCharacter(eType), m_oAI(eType), m_poDirt(0), m_bSelected(false)
{
	m_pDeathParticle = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pDeathParticle->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "zombiedeath.xml"));

	m_poDirt = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_poDirt->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "enemySpawn.xml"));
}

CEnemy::~CEnemy(void)
{
}

/**
* CEnemy::getStateType
* @date Modified April 3, 2006
*/
CAIState::EAIStateTypes CEnemy::getStateType(void)
{
	return m_oAI.getStateType();
}

/**
* CEnemy::collided
* @date Modified April 23, 2006
*/
void CEnemy::collided(CActor *pActor)
{
	// the StraightToGoal State isn't being used so that check was pointless
	if (pActor)
	{
		if ((pActor->getType() & OBJ_PLAYER) == OBJ_PLAYER)
		{
			if ((this->getType() & OBJ_ENEMY_ZOMBIECITIZEN) == OBJ_ENEMY_ZOMBIECITIZEN)
				this->setAIState(CAIStateZombieAttack::getInstancePtr());
		}
	}
}

/**
* CEnemy::spawnCoins
* @date Modified May 2, 2006
*/
void CEnemy::spawnCoins(int nAmount, float fVelMin, float fVelMax)
{
	// random velocity x,y,z for coins
	int nX, nY, nZ;

	CCoin *pCoin; 

	CObjectManager::ObjectList loPlayers;
	CObjectManager::getInstance().getObjects(OBJ_PLAYER, &loPlayers);

	// loop to create new spawn amount
	while(nAmount >= COPPER_VALUE)
	{
		// create a new coin
		pCoin = (CCoin*)CObjectManager::getInstance().createObject(OBJ_ITEM_MONEY);
		pCoin->setPosition(getPosition());
		pCoin->setBV(getPosition(), 1.0f);
		pCoin->activate();
		pCoin->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "grenade.x"));

		// find the value of the money, but make sure to not release more than the amount
		if(nAmount >= GOLD_VALUE)
		{
			// create a new value for the coin
			switch(rand()%3)
			{
			case 0:
				pCoin->setValue(COPPER_VALUE);
				break;
			case 1:
				pCoin->setValue(SILVER_VALUE);
				break;
			case 2:
				pCoin->setValue(GOLD_VALUE);
				break;
			}
		}
		else if(nAmount >= SILVER_VALUE)
		{
			// create a new value for the coin
			switch(rand()%2)
			{
			case 0:
				pCoin->setValue(COPPER_VALUE);
				break;
			case 1:
				pCoin->setValue(SILVER_VALUE);
				break;
			}
		}
		else
			pCoin->setValue(COPPER_VALUE);

		
		((CPlayer*)(loPlayers.front()))->m_oStats.addMoneySpawned(pCoin->getValue());

		// subtract from the amount
		nAmount -= pCoin->getValue();

		// get a random x,y,z velocity
		nX = rand()%101;
		nY = rand()%101;
		nZ = rand()%101;

		// if the value overlaps 100, bring it back down
		if(nX+nY+nZ > 100)
		{
			// if these values total 100 themselves, bring it back down
			if(nX+nZ > 100)
			{
				if(nX > nZ)
					nZ = 100 - nX;
				else
					nX = 100 - nZ;

				nY = 0;
			}
			else
				nY = 100 - (nX+nZ);
		}
		// if it's under 100, make it 100
		else if(nX+nY+nZ < 100)
		{
			if(nX > nZ)
				nX = 100 - (nY+nZ);
			else
				nZ = 100 - (nY+nX);
		}

		// randomly negate some of them
		if(nX%2)
			nX = -nX;
		if(nY%2)
			nY = -nY;
		if(nZ%2)
			nZ = -nZ;

		// create the new velocity
		D3DXVECTOR3 vel = D3DXVECTOR3(((float)nX)/100.0f, ((float)nY)/100.0f, ((float)nZ)/100.0f);

		// find a new magnitude for that velocity
		float fVel = (float)((rand()%((int)fVelMax*100 + 1 - (int)fVelMin*100)) + (int)fVelMin*100)/100.0f;
		vel *= fVel;
		pCoin->setVelocity(vel);
	}
}

/**
* CEnemy::spawnPickup
* @date Modified May 4, 2006
*/
void CEnemy::spawnPickup(int nAmount, float fVelMin, float fVelMax)
{
	int nX, nY, nZ;
	int nProb = rand()%100;	// spawning a random object
	CActor* pPack;

	// 95% chance of spawning coins
	if(nProb < 95)
	{
		spawnCoins(nAmount, fVelMin, fVelMax);
		return;
	}
	// 2% chance of a small health pack
	else if(nProb < 97)
	{
		pPack = (CActor*)CObjectManager::getInstance().createObject(OBJ_ITEM_HEALTH);
		pPack->setPosition(getPosition());
		pPack->setBV(getPosition(), 1.5f);
		pPack->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
		pPack->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "healthpack.x"));
		((CHealthPack*)pPack)->setHealthPackType(EHT_SMALL_PACK);
	}
	// 2% chance of a speed pack
	else if(nProb < 99)
	{
		pPack = (CActor*)CObjectManager::getInstance().createObject(OBJ_ITEM_SLICKSHOES);
		pPack->setPosition(getPosition());
		pPack->setBV(getPosition(), 1.5f);
		pPack->setScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
		pPack->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "speedpack.x"));
	}
	// 1% chance of a large health pack
	else
	{
		pPack = (CActor*)CObjectManager::getInstance().createObject(OBJ_ITEM_HEALTH);
		pPack->setPosition(getPosition());
		pPack->setBV(getPosition(), 1.5f);
		pPack->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
		pPack->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "largehealthpack.x"));
		((CHealthPack*)pPack)->setHealthPackType(EHT_LARGE_PACK);
	}

	// get a random x,y,z velocity
	nX = rand()%101;
	nY = rand()%101;
	nZ = rand()%101;

	// if the value overlaps 100, bring it back down
	if(nX+nY+nZ > 100)
	{
		// if these values total 100 themselves, bring it back down
		if(nX+nZ > 100)
		{
			if(nX > nZ)
				nZ = 100 - nX;
			else
				nX = 100 - nZ;

			nY = 0;
		}
		else
			nY = 100 - (nX+nZ);
	}
	// if it's under 100, make it 100
	else if(nX+nY+nZ < 100)
	{
		if(nX > nZ)
			nX = 100 - (nY+nZ);
		else
			nZ = 100 - (nY+nX);
	}

	// randomly negate some of them
	if(nX%2)
		nX = -nX;
	if(nY%2)
		nY = -nY;
	if(nZ%2)
		nZ = -nZ;

	// create the new velocity
	D3DXVECTOR3 vel = D3DXVECTOR3(((float)nX)/100.0f, ((float)nY)/100.0f, ((float)nZ)/100.0f);

	// find a new magnitude for that velocity
	float fVel = (float)((rand()%((int)fVelMax*100 + 1 - (int)fVelMin*100)) + (int)fVelMin*100)/100.0f;
	vel *= fVel;
	pPack->setVelocity(vel);
}

/**
* CEnemy::init
* @date Modified May 11, 2006
*/
void CEnemy::init(void)
{
	CCharacter::init();
	setVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_bSelected = false;
}

/**
* CEnemy::destroy
* @date Modified May 30, 2006
*/
void CEnemy::destroy(void)
{
	m_poDirt->pause();
	m_pDeathParticle->setPosition(getPosition() + D3DXVECTOR3(0,4.5f,0));
	m_pDeathParticle->play();
}