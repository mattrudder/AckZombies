/**
* @file Grenade.cpp
* @author Jordan Leckner
* @date Created April 7, 2006
*
* This file includes the definition of the Grenade Weapon class
*/
#include "Grenade.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ResourceManager.h"
#include "ParticleEmitter.h"
#include "SoundManager.h"

/**
* CGrenadeWeapon::CGrenadeWeapon
* @date Modified April 27, 2006
*/
CGrenadeWeapon::CGrenadeWeapon(int nDamage, int nCost, int nAmmo, int nAOE, CCharacter *poCharacter) :
	CProjectileWeapon(nDamage, nCost, nAmmo, nAOE, poCharacter)
{
	m_eType = OBJ_WEAPON_GRENADE;
	m_oBV.centerPt = D3DXVECTOR3(0,0,0);
	m_oBV.fRadius = 1.0f;
	m_vVelocity = D3DXVECTOR3(0,0,0);
	m_fTimer = 0;
	m_fFireTimer = 0.0f;
	m_unGrenadeCounter = 1;
	m_unKillCounter = m_unKillsLastFrame = 0;

	m_poGrenadeMesh = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "waterballoon.x");
	m_pMesh = m_poGrenadeMesh;

	// Create the explosion particle effect
	m_pExplosion = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pExplosion->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "Explosion.xml"));
}

/**
* CGrenadeWeapon::~CGrenadeWeapon
* @date Modified April 7, 2006
*/
CGrenadeWeapon::~CGrenadeWeapon()
{
	CObjectManager::getInstance().removeObject(m_pExplosion);
}

/**
* CGrenadeWeapon::fireWeapon
* @date Modified April 27, 2006
*/
bool CGrenadeWeapon::fireWeapon()
{
	// limit the amount of shooting per second
	if (m_unGrenadeCounter && CTimer::getInstance().getTime() - m_fFireTimer > 0.2f)
	{
		CGrenadeWeapon *poGrenade = (CGrenadeWeapon*)CObjectManager::getInstance().createObject(OBJ_WEAPON_GRENADE);
		poGrenade->m_pMesh = m_poGrenadeMesh;
		// set the color to the grenade to a random one
		poGrenade->setColor(getRandomColor());
		// increase reference count
		m_poGrenadeMesh->addRef();
		poGrenade->setPosition(m_poCharacter->getBV().centerPt + m_poCharacter->getOrientation() * 5);
		poGrenade->m_oBV.centerPt = poGrenade->getPosition();
		poGrenade->m_oBV.fRadius = 1.0f;
		poGrenade->setOrientation(m_poCharacter->getOrientation());
		poGrenade->m_vVelocity = poGrenade->getOrientation() * 25.0f;
		poGrenade->m_vVelocity.y += 25.0f;
		poGrenade->m_fTimer = CTimer::getInstance().getTime();
		poGrenade->activate();
		poGrenade->m_nDamage = m_nDamage;
		poGrenade->m_nAreaOfEffect = m_nAreaOfEffect;
		poGrenade->setPlayer((CPlayer*)m_poCharacter);
		m_fFireTimer = CTimer::getInstance().getTime();
		poGrenade->setScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
		--m_unGrenadeCounter;
		return true;
	}
	return false;
}

/**
* CGrenadeWeapon::collided
* @date Modified May 30, 2006
*/
void CGrenadeWeapon::collided(CActor *poActor)
{
	if (!poActor)
	{
		return;
	}

	// Enemy collision
	if (poActor && (poActor->getType() & OBJ_ENEMY) == OBJ_ENEMY)
	{
		CObjectManager::getInstance().removeObject((CObject*)this);
	}

	CProjectileWeapon::collided(poActor);
}

/**
* CGrenadeWeapon::update
* @date Modified May 30, 2006
*/
void CGrenadeWeapon::update()
{
	if (!this)// eh
		return;

	if (m_fTimer && CTimer::getInstance().getTime() - m_fTimer >= 5.0f)
	{
		CObjectManager::getInstance().removeObject((CObject*)this);
	}

	CProjectileWeapon::update();
}

/**
* CGrenadeWeapon::destroy
* @date Modified May 26, 2006
*/
void CGrenadeWeapon::destroy(void)
{
	m_pExplosion->setActorMatrix(getOrientation(), getPosition(), D3DXVECTOR3(1,1,1));
	m_pExplosion->play();
	CObjectManager::getInstance().iterateObjects(OBJ_ENEMY, &m_oSplashFunctor);
	// Explosion sounds
	if ((getType() & OBJ_WEAPON_GRENADE) == OBJ_WEAPON_GRENADE)
	{
		int nRand = rand() % 3;
		switch (nRand)
		{
		case 0:
			CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION1);
			break;
		case 1:
			CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION2);
			break;
		case 2:
			CSoundManager::getInstance().playSound(SND_EFFECT_EXPLOSION3);
			break;
		default:
			break;
		}
	}
	else // Big clucking bomb
	{
		CSoundManager::getInstance().stopSound(SND_EFFECT_BIGCLUCKINGBOMB_MOVE);
		CSoundManager::getInstance().playSound(SND_EFFECT_BIGCLUCKINGBOMB_EXPLODE);
	}
}

/**
* CGrenadeWeapon::getRandomColor
* @date Modified May 30, 2006
*/
DWORD CGrenadeWeapon::getRandomColor(void)
{
	char r,g,b;
	r = rand() % 192;
	g = rand() % 204;
	b = rand() % 160;

	DWORD dwColor = D3DCOLOR_XRGB(r, g, b);

	return dwColor;
}

/**
* CGrenadeWeapon::spectaculupdate
* @date Modified June 10, 2006
*/
void CGrenadeWeapon::spectaculupdate()
{
	// get the difference in kills to see if the player gets another grenade
	if(m_poCharacter && m_poCharacter->getType() == OBJ_PLAYER && getType() == OBJ_WEAPON_GRENADE)
	{
		// check how many kills the player has
		m_unKillCounter += (((CPlayer*)m_poCharacter)->m_oStats.getEnemiesKilled() - m_unKillsLastFrame);
		m_unKillsLastFrame = ((CPlayer*)m_poCharacter)->m_oStats.getEnemiesKilled();

		// check the number of kills... every 25 give the player a grenade
		while(m_unKillCounter > 25)
		{
			m_unKillCounter -= 25;
			m_unGrenadeCounter++;
		}
	}
}

