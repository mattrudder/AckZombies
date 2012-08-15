/**
* @file Player.cpp
* @author Tom Williams
* @date Modified March 29, 2006
*
* declaration of the player class
*/

#include "Player.h"
#include "AIManager.h"
#include "soundmanager.h"
#include "coin.h"
#include "GameKernel.h"
#include "MenuState.h"
#include "VendingMachine.h"
#include "Options.h"
#include "PauseState.h"
#include "VendingState.h"
#include "HUD.h"
#include "GameoverState.h"
#include "TestState.h"

#include "BaseWeapon.h"

float CPlayer::m_fMaxAwesome = 30.0f;

/**
* CPlayer::CPlayer()
* @date Modified March 29, 2006
*/
CPlayer::CPlayer(EObjectType eType)
	: CCharacter(eType)
	, m_oTargeted(&CPlayer::checkTargeted, this)
{
	// set the initial orientation to a unit vector
	m_fMaxRunSpeed = RUNNING_SPEED;
	m_fMaxWalkSpeed = WALKING_SPEED;

	// grab the pointers to the camera and input
	m_poCamera = CCamera::getInstancePtr();
	m_poInput = CDirectInputWrapper::getInstancePtr();
	m_nHealth = 100;
	m_eInputType = PIT_KEYBOARDMOUSE;
	m_unMoney = 0;
	m_pIcon = 0;
	m_fBoostSpeed = 0.0f;

	init();

	memset(m_Controls, 0, sizeof(DWORD) * NUM_BUTTONS);

	m_fHurtTimer = 0;
	m_bAwesome = false;

	m_pDeathParticle = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pDeathParticle->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "blood.xml"));

	m_pAwesomeParticles = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pAwesomeParticles->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "AwesomeTime.xml"));

	m_pSpeedParticles = (CParticleEmitter*)CObjectManager::getInstance().createObject(OBJ_PARTICLE_EMITTER);
	m_pSpeedParticles->setSystem((CParticleSystem*)CResourceManager::getInstance().loadResource(RES_PARTICLEEMITTER, "sparkles.xml"));

	m_pWeaponActor = (CActor*)CObjectManager::getInstance().createObject(OBJ_WEAPON);
	m_pWeaponActor->deactivate();
	m_pWeaponActor->setMesh(NULL);
	m_pWeaponActor->setBV(getPosition(), 0);
}

/**
* CPlayer::~CPlayer()
* @date Modified March 29, 2006
*/
CPlayer::~CPlayer()
{
	if(m_pIcon)
		CObjectManager::getInstance().removeObjectImmediate(m_pIcon);

	// delete all the weapons in the player
	m_CurWeapon = m_WeaponInventory.begin();
	while(m_CurWeapon != m_WeaponInventory.end())
	{
		if(*m_CurWeapon)
		{
			delete *m_CurWeapon;
			*m_CurWeapon = NULL;
		}

		++m_CurWeapon;
	}

	m_CurGrenade = m_GrenadeInventory.begin();
	while(m_CurGrenade != m_GrenadeInventory.end())
	{
		if(*m_CurGrenade)
		{
			delete *m_CurGrenade;
			*m_CurGrenade = NULL;
		}

		++m_CurGrenade;
	}
}

/**
* CPlayer::enterAwesomeTime
* @date Modified March 29, 2006
*/
void CPlayer::enterAwesomeTime()
{
	// set the bool
	m_bAwesome = true;
	m_pAwesomeParticles->setActorMatrix(getOrientation(), D3DXVECTOR3(getPosition().x, getPosition().y + 80.0f, getPosition().z), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pAwesomeParticles->play();
	
	// give the player the desert eagle.
	// if he already has the desert eagle, give him 10 more bullets
	list<CBaseWeapon*>::iterator weapIter = m_WeaponInventory.begin();
	while(weapIter != m_WeaponInventory.end())
	{
		if((*weapIter)->getType() == OBJ_WEAPON_DEAGLE)
		{
			(*weapIter)->setAmmo((*weapIter)->getAmmo()+10);
			break;
		}

		++weapIter;
	}

	// if he doesn't have the deagle, give it to him
	if(weapIter == m_WeaponInventory.end())
	{
		CDesertEagle50Weapon *pNewDesertEagle = new CDesertEagle50Weapon;
		pNewDesertEagle->init();
		pNewDesertEagle->setPlayer(this);
		pNewDesertEagle->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, "desertEagle.x"));
		pNewDesertEagle->setAmmo(10);
		addWeapon(pNewDesertEagle);
	}	

	// double the size of the bounding volume
	m_oBV.fRadius *= 2.0f;

	// store the current amount of enemies killed.
	m_unNumPriorAwesome = m_oStats.getEnemiesKilled();

	// get the number of enemies in the object manager
	CObjectManager::ObjectList loEnemies;
	CObjectManager::getInstance().getObjects(OBJ_ENEMY, &loEnemies);
	m_oStats.addAwesomeEnemiesSpawned((unsigned int)loEnemies.size());

	// start the awesome time timer.
	m_AwesomeTimer.setInterval(30.0f);
	m_fMaxAwesome = 30.0f;
	m_fTotalAwesome = 0.0f;

	// add one to the awesome count
	m_oStats.addNumAwesomes();

	// Play awesome time sound
	CSoundManager::getInstance().playSound(SND_EFFECT_AWESOMETIME);
}

/**
* CPlayer::switchWeapon
* @date Modified March 29, 2006
*/
void CPlayer::switchWeapon(bool bDirection)
{
	// move forward through the inventory
	if(bDirection)
	{
		++m_CurWeapon;
		if(m_CurWeapon == m_WeaponInventory.end())
		{
			m_CurWeapon == m_WeaponInventory.begin();
			++m_CurWeapon;
		}
	}
	// move backward through the inventory
	else
	{
		if(m_CurWeapon == m_WeaponInventory.begin())
		{
			m_CurWeapon = m_WeaponInventory.end();
		}

		--m_CurWeapon;
	}

	CSoundManager::getInstance().playSound(SND_EFFECT_GUNCOCK);
}

/**
* CPlayer::setIcon()
* @date Modified May 12, 2006
*/
void CPlayer::setIcon(const char *filename)
{
	if(!m_pIcon)
		m_pIcon = (CActor*)CObjectManager::getInstance().createObject(OBJ_ACTOR);

	m_pIcon->setPosition(getPosition());
	m_pIcon->setBV(m_pIcon->getPosition(), 1.0f);
	m_pIcon->setScale(D3DXVECTOR3(1.5f,1.5f,1.5f));
	m_pIcon->setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, filename));
	m_pIcon->deactivate();	
}

/**
* CPlayer::addWeapon()
* @date Modified March 29, 2006
*/
void CPlayer::addWeapon(CBaseWeapon *poNewGun)
{
	// add the weapon to the inventory
	m_WeaponInventory.push_back(poNewGun);

	m_CurWeapon = m_WeaponInventory.end();
	m_CurWeapon--;
}

/**
* CPlayer::addGrenade()
* @date Modified April 27, 2006
*/
void CPlayer::addGrenade(CGrenadeWeapon *poNewGrenade)
{
	// add the weapon to the inventory
	m_GrenadeInventory.push_back(poNewGrenade);

	m_CurGrenade = m_GrenadeInventory.end();
	m_CurGrenade--;
}

/**
* CPlayer::dropWeapon()
* @date Modified March 29, 2006
*/
void CPlayer::dropWeapon(CBaseWeapon *poDropGun)
{
	// loop through all the weapons in the inventory
	list<CBaseWeapon*>::iterator weapIter = m_WeaponInventory.begin();
	while(weapIter != m_WeaponInventory.end())
	{
		// if the weapon is in the inventory, remove it from the list
		if(*weapIter == poDropGun)
		{
			// if the weapon to drop is the current, move the current 
			// to another weapon in the inventory
			if(weapIter == m_CurWeapon)
			{
				++m_CurWeapon;
				if(m_CurWeapon == m_WeaponInventory.end())
					m_CurWeapon = m_WeaponInventory.begin();
			}

			m_WeaponInventory.erase(weapIter);
			return;
		}

		// keep checking
		++weapIter;
	}
}

/**
* CPlayer::collided()
* @date Modified April 23, 2006
*/
void CPlayer::collided(CActor *poActor)
{
	if (!poActor)
		return;

	EObjectType eType = poActor->getType();

	// if the object it collided with was an item, pick it up
	if(eType== OBJ_ITEM_MONEY)
	{
		// if there was collision, increase the player's money
		addMoney(((CCoin*)poActor)->getValue());
		m_oStats.addTotalMoneyEarned(((CCoin*)poActor)->getValue());
	}

	// check for collision with enemies
	if((eType & OBJ_ENEMY) == OBJ_ENEMY && !m_bAwesome)
	{
		//***TODO:
		// deal the player damage
		m_nHealth -= 1;

		// play the hit animation
		m_fHitAnimationTimer = CTimer::getInstance().getTime();
		((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(1.0f);
		((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Shot");

		// Play a hurt sound
		int nRand = rand() % 3;
		switch (nRand)
		{
		case 0:
			{
				// limit the hurt sound based on the length of the sound.
				unsigned int unLength = CSoundManager::getInstance().getLength(SND_EFFECT_JOHNNYTAKEDAMAGE);
				// get the amount of seconds.. not miliseconds.
				float fSoundLength = (float)unLength * 0.001f;
				if (CTimer::getInstance().getTime() - m_fHurtTimer >= fSoundLength)
				{
					m_fHurtTimer = CTimer::getInstance().getTime();
				}
				CSoundManager::getInstance().playSound(SND_EFFECT_JOHNNYTAKEDAMAGE);
			}
			break;
		case 1:
			{
				// limit the hurt sound based on the length of the sound.
				unsigned int unLength = CSoundManager::getInstance().getLength(SND_EFFECT_JOHNNYTAKEDAMAGE1);
				// get the amount of seconds.. not miliseconds.
				float fSoundLength = (float)unLength * 0.001f;
				if (CTimer::getInstance().getTime() - m_fHurtTimer >= fSoundLength)
				{
					m_fHurtTimer = CTimer::getInstance().getTime();
				}
				CSoundManager::getInstance().playSound(SND_EFFECT_JOHNNYTAKEDAMAGE1);
			}
			break;
		case 2:
			{
				// limit the hurt sound based on the length of the sound.
				unsigned int unLength = CSoundManager::getInstance().getLength(SND_EFFECT_JOHNNYTAKEDAMAGE2);
				// get the amount of seconds.. not miliseconds.
				float fSoundLength = (float)unLength * 0.001f;
				if (CTimer::getInstance().getTime() - m_fHurtTimer >= fSoundLength)
				{
					m_fHurtTimer = CTimer::getInstance().getTime();
				}
				CSoundManager::getInstance().playSound(SND_EFFECT_JOHNNYTAKEDAMAGE2);
			}
			break;
		default:
			break;
		}
	}
}

/**
* CPlayer::setControls()
* @date Modified April 9, 2006
*/
void CPlayer::setControls(const DWORD dwFire, const DWORD dwGrenade, const DWORD dwRotateCamLeft, const DWORD dwRotateCamRight,
		const DWORD dwSwitchToPrev, const DWORD dwSwitchToNext, const DWORD dwAction, const DWORD dwPause)
{
	// set the new controls
	m_Controls[0] = dwFire;
	m_Controls[1] = dwGrenade;
	m_Controls[2] = dwRotateCamLeft;
	m_Controls[3] = dwRotateCamRight;
	m_Controls[4] = dwSwitchToPrev;
	m_Controls[5] = dwSwitchToNext;
	m_Controls[6] = dwAction;
	m_Controls[11] = dwPause;
}

/**
* CPlayer::setControls()
* @date Modified April 9, 2006
*/
void CPlayer::setControls(const BYTE ucFire, const BYTE ucGrenade, const BYTE ucRotateCamLeft, const BYTE ucRotateCamRight,
		const BYTE ucSwitchToPrev, const BYTE ucSwitchToNext, const BYTE ucAction, const BYTE ucPause)
{
	// set the new controls
	m_Controls[0] = ucFire;
	m_Controls[1] = ucGrenade;
	m_Controls[2] = ucRotateCamLeft;
	m_Controls[3] = ucRotateCamRight;
	m_Controls[4] = ucSwitchToPrev;
	m_Controls[5] = ucSwitchToNext;
	m_Controls[6] = ucAction;
	m_Controls[11] = ucPause;
}

/**
* CPlayer::setControls()
* @date Modified April 9, 2006
*/
void CPlayer::setControls(const DWORD dwFire, const DWORD dwGrenade, const DWORD dwRotateCamLeft, const DWORD dwRotateCamRight,
		const DWORD dwSwitchToPrev, const DWORD dwSwitchToNext, const DWORD dwAction, const DWORD dwMoveLeft, const DWORD dwMoveRight,
		const DWORD dwMoveUp, const DWORD dwMoveDown, const DWORD dwPause)
{
	// set the new controls
	m_Controls[0] = dwFire;
	m_Controls[1] = dwGrenade;
	m_Controls[2] = dwRotateCamLeft;
	m_Controls[3] = dwRotateCamRight;
	m_Controls[4] = dwSwitchToPrev;
	m_Controls[5] = dwSwitchToNext;
	m_Controls[6] = dwAction;
	m_Controls[7] = dwMoveLeft;
	m_Controls[8] = dwMoveRight;
	m_Controls[9] = dwMoveUp;
	m_Controls[10] = dwMoveDown;
	m_Controls[11] = dwPause;
}


/**
* CPlayer::dropCurrentWeapon
* @date Modified March 29, 2006
*/
void CPlayer::dropCurrentWeapon()
{
	// remove the current weapon from the inventory
	m_CurWeapon = m_WeaponInventory.erase(m_CurWeapon);

	// check if the iterator was set to the end, if so, set it to the beginning
	if(m_CurWeapon == m_WeaponInventory.end())
		m_CurWeapon = m_WeaponInventory.begin();
}

/**
* CPlayer::update()
* @date Modified April 9, 2006
*/
void CPlayer::update()
{
	if (getHealth() <= 0)
	{
		setHealth(0);

		// blow up the player (play only once)
		destroy();
		
		if (m_oDeathTimer.tick())
		{
			CGameKernel::getInstance().changeState(CGameoverState::getInstancePtr());
		}
		return;
	}

	// FREEROME NOT MOVE PLAYER HACK!
	////////////////////////////////////
	if (CTestState::getInstance().m_bFreeRoam)
		return;

	// CONSOLE NOT MOVE PLAYER HACK!
	////////////////////////////////////
	if (CCommandConsole::getInstance().isVisible())
		return;
	////////////////////////////////////
	////////////////////////////////////

	// if the player earned a speed powerup, check if it's time to go
	if(m_fBoostSpeed != 0.0f)
	{
		// check if it's time for speed boost love to leave
		if(m_oSpeedBoostSpan.tick())
		{
			m_pSpeedParticles->pause();
			addToMaxSpeed(0.0f);
			m_oSpeedBoostSpan.setInterval(0.0f);
		}
		// if the speed boost is still going, keep playing it yo
		else
		{
			m_pSpeedParticles->setActorMatrix(getOrientation(), getPosition(), D3DXVECTOR3(1,1,1));
			m_pSpeedParticles->play(false);
		}
	}

	m_fTime += CTimer::getInstance().getFrameTime();

		// change the player's velocity/orientation based on input
	// the x and z axis of movement
	float fVelX = 0.0f, fVelZ = 0.0f, fOrientX = 0.0f, fOrientZ = 0.0f;
	bool bFire = false, bGrenade = false, bRCamLeft = false, bRCamRight = false, bSwitchToPrev = false,
		bSwitchToNext = false, bAction = false, bPause = false;

	D3DXVECTOR3 cameraRight, cameraAt, xVelocity, zVelocity;
	cameraRight = CCamera::getInstance().getCameraRight();
	cameraAt = CCamera::getInstance().getCameraAt();

	switch(m_eInputType)
	{
	case PIT_1ST_XBOX360PAD:
	case PIT_2ND_XBOX360PAD:
		{
			DWORD dwIndex = PIT_1ST_XBOX360PAD;
			if (m_eInputType == PIT_2ND_XBOX360PAD && m_poInput->m_oXInputWrapper.isConnected(PIT_2ND_XBOX360PAD))
			{
				dwIndex = PIT_2ND_XBOX360PAD;
			}
			else if (m_eInputType == PIT_2ND_XBOX360PAD)
			{
				if (COptions::getInstance().getControlScheme(true) == CS_XBOX360)
				{
					return;
				}
			}
			// grab the orientation from the joypad
			fOrientX = m_poInput->m_oXInputWrapper.getThumbstickDirection(dwIndex, XIA_RIGHT_X_AXIS);
			fOrientZ = m_poInput->m_oXInputWrapper.getThumbstickDirection(dwIndex, XIA_RIGHT_Y_AXIS);

			// set the orientation based on the camera orientation
			// orient the player in the x and z direction
			// (reuse the xVelocity and zVelocity vector to save allocation time)
			xVelocity = fOrientX * cameraRight;
			zVelocity = fOrientZ * cameraAt;

			// get the combination of the two vectors
			xVelocity += zVelocity;

			// set the orientation
			fOrientX = xVelocity.x;
			fOrientZ = xVelocity.z;

			// grab the velocity from the joypad
			fVelX = m_poInput->m_oXInputWrapper.getThumbstickDirection(dwIndex, XIA_LEFT_X_AXIS);
			fVelZ = m_poInput->m_oXInputWrapper.getThumbstickDirection(dwIndex, XIA_LEFT_Y_AXIS);

			// check whether to fire the weapon or not
			if(m_Controls[0] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bFire = true : bFire = false;
			}
			else if(m_Controls[0] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bFire = true : bFire = false;
			}
			else
				bFire = m_poInput->m_oXInputWrapper.isButtonDownBuffered(dwIndex, m_Controls[0]);

			// check whether to use a grenade or not
			if(m_Controls[1] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bGrenade = true : bGrenade = false;
			}
			else if(m_Controls[1] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bGrenade = true : bGrenade = false;
			}
			else
				bGrenade = m_poInput->m_oXInputWrapper.isButtonDownBuffered(dwIndex, m_Controls[1]);

			// check whether to rotate camera right or not
			if(m_Controls[2] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bRCamLeft = true : bRCamLeft = false;
			}
			else if(m_Controls[2] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bRCamLeft = true : bRCamLeft = false;
			}
			else
				bRCamLeft = m_poInput->m_oXInputWrapper.isButtonDown(dwIndex, m_Controls[2]);

			// check whether to rotate camera right or not
			if(m_Controls[3] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bRCamRight = true : bRCamRight = false;
			}
			else if(m_Controls[3] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bRCamRight = true : bRCamRight = false;
			}
			else
				bRCamRight = m_poInput->m_oXInputWrapper.isButtonDown(dwIndex, m_Controls[3]);

			// check whether to switch to previous weapon or not
			if(m_Controls[4] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bSwitchToPrev = true : bSwitchToPrev = false;
			}
			else if(m_Controls[4] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bSwitchToPrev = true : bSwitchToPrev = false;
			}
			else
				bSwitchToPrev = m_poInput->m_oXInputWrapper.isButtonDownBuffered(dwIndex, m_Controls[4]);

			// check whether to switch to next weapon or not
			if(m_Controls[5] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bSwitchToNext = true : bSwitchToNext = false;
			}
			else if(m_Controls[5] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bSwitchToNext = true : bSwitchToNext = false;
			}
			else
				bSwitchToNext = m_poInput->m_oXInputWrapper.isButtonDownBuffered(dwIndex, m_Controls[5]);

			// check whether to do an action or not
			if(m_Controls[6] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bAction = true : bAction = false;
			}
			else if(m_Controls[6] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bAction = true : bAction = false;
			}
			else
				bAction = m_poInput->m_oXInputWrapper.isButtonDownBuffered(dwIndex, m_Controls[6]);

			// check whether to do a pause or not
			if(m_Controls[11] == XIT_RIGHT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, false) > 10 ? bPause = true : bPause = false;
			}
			else if(m_Controls[11] == XIT_LEFT_TRIGGER)
			{
				m_poInput->m_oXInputWrapper.getTriggerData(dwIndex, true) > 10 ? bPause = true : bPause = false;
			}
			else
				bPause = m_poInput->m_oXInputWrapper.isButtonDownBuffered(dwIndex, m_Controls[11]);
		}
		break;

	case PIT_KEYBOARDMOUSE:
		{
			// set the velocity component
			if(m_poInput->isKeyDown(m_Controls[8]))
				fVelX += 1.0f;
			if(m_poInput->isKeyDown(m_Controls[7]))
				fVelX += -1.0f;

			if(m_poInput->isKeyDown(m_Controls[9]))
				fVelZ += 1.0f;
			else if(m_poInput->isKeyDown(m_Controls[10]))
				fVelZ += -1.0f;

			// get the orientation component by transforming the mouse pos to screen space
			D3DXVECTOR2 mouse(CMenuManager::getInstance().checkMouseInput(32,32));
			// HACK to shoot through the reticle on the HUD
			mouse.x += 16;
			mouse.y += 16;
			
			// transform the point from screen to world space
			D3DVIEWPORT9 viewPort;

			// get the viewport
			IDirect3DDevice9* renderDev;
			renderDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
			renderDev->GetViewport(&viewPort);

			// get the projection matrix
			D3DXMATRIX projMat;
			renderDev->GetTransform(D3DTS_PROJECTION, &projMat);

			// get the upper left of the screen
			mouse.x += viewPort.Width*0.5f;
			mouse.y += viewPort.Height*0.5f;

			// get the offset scaled
			D3DXVECTOR3 v, rayDir, rayPt;
			v.x = ((2.0f * mouse.x)/(float)viewPort.Width - 1.0f) / projMat._11;
			v.y = -((2.0f * mouse.y)/(float)viewPort.Height - 1.0f ) / projMat._22;
			v.z =  1.0f;

			// use picking to find the ray passing through the mouse point
			D3DXMATRIX viewMat;
			CCamera::getInstance().getViewMatrix(&viewMat);
			D3DXMatrixInverse(&viewMat, NULL, &viewMat);

			// Transform the screen space: pick ray into 3D space
			rayDir.x = v.x*viewMat._11 + v.y*viewMat._21 + v.z*viewMat._31;
			rayDir.y = v.x*viewMat._12 + v.y*viewMat._22 + v.z*viewMat._32;
			rayDir.z = v.x*viewMat._13 + v.y*viewMat._23 + v.z*viewMat._33;
			rayPt = D3DXVECTOR3(viewMat._41, viewMat._42, viewMat._43);

			// find the ray from this point to the player's location
			float fT = (getPosition().y - rayPt.y)/rayDir.y;
			fOrientX = (rayPt.x+rayDir.x*fT) - getPosition().x;
			fOrientZ = (rayPt.z+rayDir.z*fT) - getPosition().z;

			// check whether to fire or not
			if (m_Controls[0] == LMOUSE)
				bFire = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[0] == RMOUSE)
				bFire = m_poInput->isRightMouseButtonDown();
			else
				bFire = m_poInput->isKeyDown(m_Controls[0]);
			if (m_Controls[1] == LMOUSE)
				bGrenade = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[1] == RMOUSE)
				bGrenade = m_poInput->isRightMouseButtonDown();
			else
				bGrenade = m_poInput->isKeyDownBuffered(m_Controls[1], PLAYER_SYSTEM);
			if (m_Controls[2] == LMOUSE)
				bRCamLeft = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[2] == RMOUSE)
				bRCamLeft = m_poInput->isRightMouseButtonDown();
			else
				bRCamLeft = m_poInput->isKeyDown(m_Controls[2]);
			if (m_Controls[3] == LMOUSE)
				bRCamRight = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[3] == RMOUSE)
				bRCamRight = m_poInput->isRightMouseButtonDown();
			else
				bRCamRight = m_poInput->isKeyDown(m_Controls[3]);
			if (m_Controls[4] == LMOUSE)
				bSwitchToPrev = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[4] == RMOUSE)
				bSwitchToPrev = m_poInput->isRightMouseButtonDown();
			else
				bSwitchToPrev = m_poInput->isKeyDownBuffered(m_Controls[4], PLAYER_SYSTEM);
			if (m_Controls[5] == LMOUSE)
				bSwitchToNext = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[5] == RMOUSE)
				bSwitchToNext = m_poInput->isRightMouseButtonDown();
			else
				bSwitchToNext = m_poInput->isKeyDownBuffered(m_Controls[5], PLAYER_SYSTEM);
			if (m_Controls[6] == LMOUSE)
				bAction = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[6] == RMOUSE)
				bAction = m_poInput->isRightMouseButtonDown();
			else
				bAction = m_poInput->isKeyDownBuffered(m_Controls[6], PLAYER_SYSTEM);
			if (m_Controls[11] == LMOUSE)
				bPause = m_poInput->isLeftMouseButtonDown();
			else if (m_Controls[11] == RMOUSE)
				bPause = m_poInput->isRightMouseButtonDown();
			else
				bPause = m_poInput->isKeyDownBuffered(m_Controls[11], PLAYER_SYSTEM);
		}
		break;

	case PIT_1ST_GAMEPAD:
	case PIT_2ND_GAMEPAD:
		{
			// set the velocity components
			fVelX = m_poInput->getJoystickDirection(m_eInputType - PIT_1ST_GAMEPAD, XIA_LEFT_X_AXIS);
			fVelZ = m_poInput->getJoystickDirection(m_eInputType - PIT_1ST_GAMEPAD, XIA_LEFT_Y_AXIS);

			// set the orientation components
			fOrientX = m_poInput->getJoystickDirection(m_eInputType - PIT_1ST_GAMEPAD, XIA_RIGHT_X_AXIS);
			fOrientZ = m_poInput->getJoystickDirection(m_eInputType - PIT_1ST_GAMEPAD, XIA_RIGHT_Y_AXIS);

			// set the orientation based on the camera orientation
			// orient the player in the x and z direction
			// (reuse the xVelocity and zVelocity vector to save allocation time)
			xVelocity = fOrientX * cameraRight;
			zVelocity = fOrientZ * cameraAt;

			// get the combination of the two vectors
			xVelocity += zVelocity;

			// set the orientation
			fOrientX = xVelocity.x;
			fOrientZ = xVelocity.z;

			// check whether fire was pressed or not
			bFire = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[0]);
			bGrenade = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[1]);
			bRCamLeft = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[2]);
			bRCamRight = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[3]);
			bSwitchToPrev = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[4]);
			bSwitchToNext = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[5]);
			bAction = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[6]);

			bPause = m_poInput->isJoypadButtonDown(m_eInputType - PIT_1ST_GAMEPAD, (BYTE)m_Controls[11]);
		}
		break;
	}

	// now that input has been checked, update the player information based on that
	// if the player only wanted to move a little, set to walk
	// get the coordinates based off the position of the camera
	// (keeps controls relative to the position of the camera)
	// move the player in the x direction
	// change the player's velocity
	xVelocity = fVelX * cameraRight;

	// change the player's velocity to 0 along the camera's z
	zVelocity = fVelZ * cameraAt;
	
	// get the unit vector of the velocity
	xVelocity += zVelocity;

	// if it's a zero vector, don't normalize
	if(xVelocity.x == 0.0f && xVelocity.z == 0.0f)
	{
		setVelocity(xVelocity);
	}
	// otherwise, normalize and scale the velocity
	else
	{
		D3DXVec3Normalize(NULL, &xVelocity, &xVelocity);

		// scale to the speed of running and set the velocity
		if(abs(fVelX) <= 0.5f && abs(fVelZ) <= 0.5f)
			setVelocity(xVelocity*m_fMaxWalkSpeed);
		else
			setVelocity(xVelocity*m_fMaxRunSpeed);
	}

	// orient the player in the x and z direction
	// (reuse the xVelocity and zVelocity vector to save allocation time)
	zVelocity = D3DXVECTOR3(fOrientX, 0.0f,  fOrientZ);

	// if it's a zero vector, don't update the orientation
	if(zVelocity.z != 0.0f || zVelocity.x != 0.0f)
	{
		// if there was a vector, update the orientation to that vector
		// (after normalizing it)
		D3DXVec3Normalize(NULL, &zVelocity, &zVelocity);
		setOrientation(zVelocity);
	}

	// TESTING -- Sample animation swaping.
	// if the player is moving, set the player to a moving animation
	if(fVelX != 0.0f || fVelZ != 0.0f)
	{
		// check the velocity against the orientation of the player to find which animation to play. 
		// get the x axis of the orientation (the right vector out of the actor matrix without the scale)
		// get the angle between the velocity and the right vector from the actor matrix
		float fTheta = acosf(D3DXVec3Dot(&D3DXVECTOR3(m_ActorMatrix._11/m_vScale.x, 0.0f, m_ActorMatrix._13/m_vScale.z), &xVelocity));

		// set the velocity based on the angle (0 - pi)
		// (0 - pi/4 == Strafe right) 
		if(fTheta <= D3DX_PI/4.0f)
		{
			((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(3.0f);
			((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("StrafeRight");
		}
		// (pi/4 - 3pi/4 == walk)
		else if(fTheta <= 3.0f*D3DX_PI/4.0f)
		{
			((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Walk");
			// check whether to make the player walk forward or backward
			// forward -> velocity and orientation point in same direction
			// backward -> velocity and orientation point in opposite direction
			if(D3DXVec3Dot(&xVelocity, &zVelocity) <= 0.0f)
				((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(2.0f);
			else
				((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(2.0f);
		}
		// (3pi/4 - pi == strafe left)
		else
		{
			((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(3.0f);
			((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("StrafeLeft");
		}
		
	}
	// if the player is idle and not hit, set to the idle animation
	else if(((CAnimatedMesh*)m_pMesh)->getAnimationSetByName("Shot") != ((CAnimatedMesh*)m_pMesh)->getCurrentAnimationIndex())
	{
		((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(1.0f);
		((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Idle");
	}
	// if the player is idle, but being hit, leave as the hit animation
	else
	{
		if(CTimer::getInstance().getTime() - m_fHitAnimationTimer >= (float)((CAnimatedMesh*)m_pMesh)->getAnimationLength(true, "Shot"))
		{
			((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Idle");
		}
	}

	// get the elapsed time since the last frame
	float fElapsedTime = CTimer::getInstance().getFrameTime();

	// update based on the button presses
	// if fire was pressed, fire the current weapon
	if(bFire)
	{
		(*m_CurWeapon)->fireWeapon();

		// throw away the weapon if the ammo is gone.
		if ((*m_CurWeapon)->getAmmo() == 0)
		{
			delete *m_CurWeapon;
			m_CurWeapon = m_WeaponInventory.erase(m_CurWeapon);
			if (m_CurWeapon == m_WeaponInventory.end())
				--m_CurWeapon;
		}
	}
	// if the grenade button was pressed, update the number of grenades
	if(bGrenade)
	{
		// throw a grenade
		if ((*m_CurGrenade)->fireWeapon())
		{
			CSoundManager::getInstance().playSound(SND_EFFECT_JOHNNYTHROW);
		}

		// throw away the weapon if the ammo is gone.
		if ((*m_CurGrenade)->getAmmo() == 0)
		{
			delete *m_CurGrenade;
			m_GrenadeInventory.erase(m_CurGrenade);
			// Get the back weapon.
			m_CurGrenade = m_GrenadeInventory.end();
			--m_CurGrenade;
		}
	}
	// if the rotate camera button was pressed, rotate the camera
	if(bRCamLeft)
	{
		// rotate the camera left
		CCamera::getInstance().rotateCamera(-2.0f*fElapsedTime);
	}
	if(bRCamRight)
	{
		// rotate the camera right
		CCamera::getInstance().rotateCamera(2.0f*fElapsedTime);
	}
	if(bSwitchToPrev)
	{
		// switch to the previous weapon
		switchWeapon(false);
	}
	if(bSwitchToNext)
	{
		// switch to the next weapon
		switchWeapon(true);
	}
	if(bAction)
	{
		// Check collision with the vending machines
		CObjectManager::ObjectList lVends;
		CObjectManager::getInstance().getObjects(OBJ_VENDINGMACHINE, &lVends);

		CVendingMachine *pVend;
		for (CObjectManager::ObjectList::iterator iter = lVends.begin(); iter != lVends.end(); ++iter)
		{
			pVend = (CVendingMachine*)*iter;
			D3DXVECTOR3 ptOfCol;
			// if the player is near a vending machine..
			if (checkCol_SphereSphere(&ptOfCol, getBV(), pVend->getActionBounds()))
			{
				CVendingState::getInstance().setVendingMachine(pVend);
				pVend->setPlayer(this);
				CGameKernel::getInstance().changeState(CVendingState::getInstancePtr());
			}
		}
	}
	if(bPause)
	{
		// pause the game
		CGameKernel::getInstance().changeState(CPauseState::getInstancePtr());
	}

	if (m_vVelocity != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		CAIManager::getInstancePtr()->updateGoalLinks();

	// check if the player is in awesome time
	if(m_bAwesome)
	{
		float fDT;
		if(m_AwesomeTimer.tick(&fDT))
		{
			m_bAwesome = false;
			setScale(m_vDefaultScale);
			// give the player the last little bit
			m_oStats.addAwesomeTime(fDT);
			// restore the size of the bounding volume
			m_oBV.fRadius *= 0.5f;
			CMenuManager::getInstance().getHUD()->addToAwesomeMeter(0);
			// set the model back
			setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Johnny.x"));
		}
		else
		{
			// add to the amount of time spent in awesome time
			m_oStats.addAwesomeTime(fDT);

			// update the player's awesome attributes
			unsigned int unKills = m_oStats.getEnemiesKilled()-m_unNumPriorAwesome;
			m_unNumPriorAwesome += unKills;
			m_oStats.addAwesomeKills(unKills);

			// add awesome time for kills
			m_fMaxAwesome += (float)(unKills)*0.1f;
			if(m_fMaxAwesome > 45.0f) m_fMaxAwesome = 45.0f;
			m_fTotalAwesome += fDT;
			fDT = m_fMaxAwesome - m_fTotalAwesome;
			if(fDT < 0.0f) fDT = 0.0f;
			m_AwesomeTimer.setInterval(fDT);

			int nNum = (int)(((float)MAX_AWESOME_TEX) * (fDT/m_fMaxAwesome));

			CMenuManager::getInstance().getHUD()->addToAwesomeMeter(0);
			CMenuManager::getInstance().getHUD()->addToAwesomeMeter(nNum);

			if(m_fTotalAwesome > 0.77f)
			{
				// check if the rodge mesh was set, if not, set it.
				if(((CAnimatedMesh*)m_pMesh)->getAnimationSetByName("Roar") == -1)
					setMesh((CMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Rodge.x"));

				// make the rodge yell
				if(m_fTotalAwesome < ((CAnimatedMesh*)m_pMesh)->getAnimationLength(true, "Roar") + 0.77)
				{
					((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Roar");
					((CAnimatedMesh*)m_pMesh)->setAnimationSpeed(1.0f);
					setVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));
				}
				else
				{
					// check if the model was already expanded
					if(getScale() != m_vDefaultScale*1.02f)
					{
						// blow up the current model if it wasn't
						setScale(m_vDefaultScale*1.02f);
					}
				}
			}
			else
				// don't let the player move for half a sec
				setVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}

	// update within the base classes
	CActor::update();

	// grab the list of pickups
	CObjectManager::ObjectList lPickups;
	CActor *pPickup;
	CObjectManager::getInstance().getObjects(OBJ_WORLD, &lPickups);
	CObjectManager::ObjectList::iterator pickupIter = lPickups.begin();
	// check for collision b/w the player and the pickup
	while(pickupIter != lPickups.end())
	{
		pPickup = (CActor*)(*pickupIter);

		// make the pickup volume between the player's bv and the ground
		m_tPickupBV.centerPt = m_oBV.centerPt;
		m_tPickupBV.fRadius = 2.0f*m_oBV.fRadius;
		if(m_tPickupBV.centerPt.y > pPickup->getBV().centerPt.y)
			m_tPickupBV.centerPt.y = pPickup->getBV().centerPt.y;

		// check for collision
		if(checkCol_SphereSphere(NULL, pPickup->getBV(), m_tPickupBV))
		{
			pPickup->collided(this);
			collided(pPickup);
		}
		++pickupIter;
	}

	// update the position of the icon (if the player has one)
	if(m_pIcon)
	{
		xVelocity = getPosition();
		if(!m_bAwesome)
			m_pIcon->setPosition(D3DXVECTOR3(xVelocity.x,xVelocity.y +17.0f, xVelocity.z));
		else
			m_pIcon->setPosition(D3DXVECTOR3(xVelocity.x,xVelocity.y +19.0f, xVelocity.z));
		m_pIcon->setBV(m_pIcon->getPosition(), 1.0f);
		m_pIcon->rotateActorY(fElapsedTime);
	}

	// update the targeted enemy
	CObjectManager::getInstance().iterateObjects(OBJ_ENEMY, &m_oTargeted);

	if ((*m_CurGrenade))
	{
		// woo.
		(*m_CurGrenade)->spectaculupdate();
	}
}

/**
* CPlayer::destroy
* @date Modified May 18, 2006
*/
void CPlayer::destroy()
{
	// if the player doesn't have a mesh, then he's already dead
	if(m_pMesh)
	{
		// play the player death sound.
		CSoundManager::getInstance().playSound(SND_EFFECT_DEATH);
		m_pDeathParticle->setPosition(getPosition() + D3DXVECTOR3(0,4.5f,0));
		m_pDeathParticle->play();
		deactivate();
		m_pMesh = NULL;
	}
}

/**
* CPlayer::init
* @date Modified May 18, 2006
*/
void CPlayer::init(void)
{
	m_fTime = 0.0f;
	m_fHitAnimationTimer = 0.0f;
	setMesh((CAnimatedMesh*)CResourceManager::getInstance().loadResource(RES_ANIMATEDMESH, "Johnny.x"));
	if(m_pMesh)
		((CAnimatedMesh*)m_pMesh)->setAnimationSetByName("Idle");
	// set the initial orientation to a unit vector
	m_oDeathTimer.setInterval(3.0f);
	m_oSpeedBoostSpan.setInterval(0);
	m_fBoostSpeed = 0.0f;
	m_oStats.clearStats();
	m_fMaxRunSpeed = RUNNING_SPEED;
	m_fMaxWalkSpeed = WALKING_SPEED;
	m_nHealth = 100;
	m_unMoney = 0;
	CCharacter::init();
}

/**
* CPlayer::draw
* @date Modified Jun 05, 2006
*/
void CPlayer::draw(void)
{
	CActor::draw();

	if (!getMesh())
		return;

	// Draw the player's weapon at their hand.
	if (*m_CurWeapon)
	{
		CBaseWeapon* pWeapon = *m_CurWeapon;
		m_pWeaponActor->setMesh(pWeapon->getMesh());
		D3DXMATRIXA16 boneMatrix;
		D3DXVECTOR3 vScale;

		if (!m_bAwesome)
		{
			// PISTOL
			if ((pWeapon->getType() & OBJ_WEAPON_PISTOL) == OBJ_WEAPON_PISTOL)
			{
				vScale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// SHOTGUN
			else if ((pWeapon->getType() & OBJ_WEAPON_SHOTGUN) == OBJ_WEAPON_SHOTGUN)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// TRIPLE SHOTGUN
			else if ((pWeapon->getType() & OBJ_WEAPON_TRIPLESHOTGUN) == OBJ_WEAPON_TRIPLESHOTGUN)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// DEAGLE
			else if ((pWeapon->getType() & OBJ_WEAPON_DEAGLE) == OBJ_WEAPON_DEAGLE)
			{
				vScale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// POTATO CHAINGUN
			else if ((pWeapon->getType() & OBJ_WEAPON_POTATOGUN) == OBJ_WEAPON_POTATOGUN)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// ROCKET LAUNCHER
			else if ((pWeapon->getType() & OBJ_WEAPON_ROCKETLAUNCHER) == OBJ_WEAPON_ROCKETLAUNCHER)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// HOLY WATER BALLOON CANNON
			else if ((pWeapon->getType() & OBJ_WEAPON_HOLYWATERCANNON) == OBJ_WEAPON_HOLYWATERCANNON)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Wrist", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
		}
		else // Awesome Time
		{
			// PISTOL
			if ((pWeapon->getType() & OBJ_WEAPON_PISTOL) == OBJ_WEAPON_PISTOL)
			{
				vScale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// SHOTGUN
			else if ((pWeapon->getType() & OBJ_WEAPON_SHOTGUN) == OBJ_WEAPON_SHOTGUN)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// TRIPLE SHOTGUN
			else if ((pWeapon->getType() & OBJ_WEAPON_TRIPLESHOTGUN) == OBJ_WEAPON_TRIPLESHOTGUN)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// DEAGLE
			else if ((pWeapon->getType() & OBJ_WEAPON_DEAGLE) == OBJ_WEAPON_DEAGLE)
			{
				vScale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// POTATO CHAINGUN
			else if ((pWeapon->getType() & OBJ_WEAPON_POTATOGUN) == OBJ_WEAPON_POTATOGUN)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// ROCKET LAUNCHER
			else if ((pWeapon->getType() & OBJ_WEAPON_ROCKETLAUNCHER) == OBJ_WEAPON_ROCKETLAUNCHER)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}
			// HOLY WATER BALLOON CANNON
			else if ((pWeapon->getType() & OBJ_WEAPON_HOLYWATERCANNON) == OBJ_WEAPON_HOLYWATERCANNON)
			{
				vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
				if (((CAnimatedMesh*)m_pMesh)->getBoneByName("Rt_Hand", boneMatrix))
					m_pWeaponActor->setActorMatrix(getOrientation(), D3DXVECTOR3(boneMatrix._41, boneMatrix._42, boneMatrix._43), 
					vScale);
				else
					m_pWeaponActor->setActorMatrix(getOrientation(), getBV().centerPt, vScale);
			}

		}

		m_pWeaponActor->setBV(getBV());
	}
}

/**
* CPlayer::getWeaponActorPos
* @date Modified Jun 05, 2006
*/
bool CPlayer::getWeaponActorPos(D3DXVECTOR3 *pvPos)
{
	if (!m_pWeaponActor)
	{
		*pvPos = D3DXVECTOR3(0,0,0);
		return false;
	}

	pvPos->x = m_pWeaponActor->getActorMatrix()._41;
	pvPos->y = m_pWeaponActor->getActorMatrix()._42;
	pvPos->z = m_pWeaponActor->getActorMatrix()._43;
	return true;
}

/**
* CPlayer::checkTargeted
* @date Modified June 7, 2006
*/
void CPlayer::checkTargeted(CObject* poObject)
{
	D3DXVECTOR3 vPoint;
	getWeaponActorPos(&vPoint);
	float fDist = FLT_MAX;
	D3DXVECTOR3 ptOfCol;

	D3DCOLOR prevColor = ((CActor*)poObject)->getColor();

	if ((*m_CurWeapon)->getType() != OBJ_WEAPON_SHOTGUN &&
		(*m_CurWeapon)->getType() != OBJ_WEAPON_TRIPLESHOTGUN)
	{
		// test for the collision
		if (!((CEnemy*)poObject)->isSelected())
		{
			if(checkCol_RaySphere(&ptOfCol, vPoint, getOrientation(), ((CEnemy*)poObject)->getBV(), &fDist))
			{
				// change their color
				((CActor*)poObject)->setColor(0xff005500);
				((CEnemy*)poObject)->setSelected(true);
			}
			else
			{
				if (prevColor == 0xff005500)
					((CActor*)poObject)->setColor(0xff000000);
			}			
		}
		
	}
	else
	{
		// test for all the collisions
		D3DXVECTOR3 Spread[3]; // the spread of the shotgun made into a triangle
		float fLength = 20.0f; // Max length of distance to shoot
		float fAngle = acosf(D3DXVec3Dot(&getOrientation(), &CCamera::getInstance().getCameraRight()));

		Spread[0].x = vPoint.x + cosf(fAngle - M_PI/12.0f) * fLength;
		Spread[0].y = vPoint.y;
		Spread[0].z = vPoint.z + sinf(fAngle - M_PI/12.0f) * fLength;
		Spread[1].x = vPoint.x + cosf(fAngle + M_PI/12.0f) * fLength;
		Spread[1].y = vPoint.y;
		Spread[1].z = vPoint.z + sinf(fAngle + M_PI/12.0f) * fLength;
		Spread[2] = getOrientation();

		Spread[0] = Spread[0] - vPoint;
		Spread[1] = Spread[1] - vPoint;

		D3DXVec3Normalize(NULL, &Spread[0], &Spread[0]);
		D3DXVec3Normalize(NULL, &Spread[1], &Spread[1]);
		for (unsigned int i = 0; i < 3; i++)
		{
			if (checkCol_RaySphere(&ptOfCol, vPoint, Spread[i], ((CEnemy*)poObject)->getBV(), &fDist))
			{
				// change their color
				((CActor*)poObject)->setColor(0xff005500);
			}
			else
			{
				if (prevColor == 0xff005500)
					((CActor*)poObject)->setColor(0xff000000);
			}
		}
	}
}

