/**
* @file Player.h
* @author Tom Williams
* @date Created March 27, 2006
*
* declaration of the player class
*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

// local includes/libraries
#include "Character.h"
#include "StatSystem.h"
#include "BaseWeapon.h"
#include "directinputwrapper.h"
#include "menumanager.h"
#include "camera.h"
#include "Grenade.h"
#include "enemy.h"

// system includes/libraries
#include <list>
#include <vector>

// the max number of buttons affected by key binding
#define NUM_BUTTONS		12L
#define WALKING_SPEED	15.0f
#define RUNNING_SPEED	35.0f

//! the type of input the player is using
enum EPlayerInputType
{
	PIT_1ST_XBOX360PAD,
	PIT_2ND_XBOX360PAD,
	PIT_KEYBOARDMOUSE,
	PIT_1ST_GAMEPAD,
	PIT_2ND_GAMEPAD,
	PIT_NUM_OBJECTS
};

/**
* the class which describes an in game player
*
* @author Tom Williams
* @date Created March 27, 2006
* @date Modified June 7, 2006
*/
class CPlayer : public CCharacter
{
	friend class CRenderSystem;

	//! the amount of money the player has
	unsigned int m_unMoney;

	//! the weapons in the inventory
	list<CBaseWeapon*> m_WeaponInventory;

	//! the current weapon
	list<CBaseWeapon*>::iterator m_CurWeapon;

	//! Actor for the weapon to draw
	CActor* m_pWeaponActor;

	//! the grenades in the inventory
	list<CGrenadeWeapon*> m_GrenadeInventory;

	//! the current grenade
	list<CGrenadeWeapon*>::iterator m_CurGrenade;

	//! the current running speed of the player
	float m_fMaxRunSpeed;
	
	//! the current walking speed of the player
	float m_fMaxWalkSpeed;

	//! the amount of speed power up
	float m_fBoostSpeed;

	//! the timer for speed burst to last
	CTickTimer m_oSpeedBoostSpan;

	//! pointer to the camera
	CCamera *m_poCamera;

	//! the icon under the player
	CActor *m_pIcon;

	//! pointer to the input wrapper
	CDirectInputWrapper *m_poInput;

	//! type of controller the player is using
	EPlayerInputType m_eInputType;

	//! use for key binding
	// SEE setControls FUNCTION FOR ORDER OF ASSIGNMENT
	DWORD m_Controls[NUM_BUTTONS];

	//! bounding volume for picking up powerups
	SSphere m_tPickupBV;

	//! timer so switching to gameover state isn't instant
	CTickTimer m_oDeathTimer;

	//! timer for awesome time
	CTickTimer m_AwesomeTimer;

	//! whether or not in awesome time
	bool m_bAwesome;

	//! initial amount of enemies killed prior to entering awesome time
	unsigned int m_unNumPriorAwesome;

	//! maximum amount of allowed time in awesome time (same for all players)
	static float m_fMaxAwesome;

	//! the total amount of time currently spent in awesome time
	float m_fTotalAwesome;

	//! the default actor scale
	D3DXVECTOR3 m_vDefaultScale;

	//! timer to limit the hurt sound
	float m_fHurtTimer;

	//! timer to play the hit animation
	float m_fHitAnimationTimer;

	//! The awesome time transformation
	CParticleEmitter* m_pAwesomeParticles;

	//! played during speed bursting
	CParticleEmitter* m_pSpeedParticles;

	//! functor to test for currently targeted enemy
	CMethodSingleFunctor<CPlayer, CObject*> m_oTargeted;

public:
	//! the time thi
	float m_fTime;

	//! the player's stat system
	// allow outsiders easy access to modify it
	CStatSystem m_oStats;

	/**
	* constructor
	*
	* @date Created March 27, 2006
	*/
	CPlayer(EObjectType eType = OBJ_PLAYER);

	/**
	* destructor
	*
	* @date Created March 27, 2006
	*/
	~CPlayer();

	/**
	* Modifiers for the player class members
	*
	* @date Created March 27, 2006
	* @param[in]	The applicable member variable.
	* @{
	*/
	void setAmtMoney(const unsigned int &unMoney) {m_unMoney = unMoney;}
	void setInputType(const EPlayerInputType eType) {m_eInputType = eType;}
	void setIcon(const char* filename);
	void setDefaultScale(D3DXVECTOR3 &vScale) {m_vDefaultScale = vScale; setScale(vScale);}
	//!@}

	/**
	* Add a passed in amount to the money member
	*
	* @date Created March 27, 2006
	* @param[in]	nMoney	The amount to add.
	*/
	void addMoney(const int &nMoney)
	{
		if(nMoney < 0)
		{
			if(abs(nMoney) > (int)m_unMoney)
			{
				m_unMoney = 0;
				return;
			}
		}
		
		m_unMoney = (int)m_unMoney + nMoney;
	}

	/**
	* called on player death
	*
	* @date Created June 2, 2006
	*/
	void destroy();

	/**
	* Returns the current actor of the weapon for a player
	*
	* @date Created June 5, 2006
	*/
	bool getWeaponActorPos(D3DXVECTOR3 *pvPos);

	/**
	* Add a passed in amount to max speed of run & walk
	*
	* @date Created May 5, 2006
	* @param[in]	fSpeed	The amount to add.
	*/
	void addToMaxSpeed(float fSpeed)
	{
		m_fBoostSpeed = fSpeed;
		m_oSpeedBoostSpan.setInterval(10.0f);

		m_fMaxRunSpeed = RUNNING_SPEED + m_fBoostSpeed;
		m_fMaxWalkSpeed = WALKING_SPEED + m_fBoostSpeed;
	}

	/**
	* Add a passed in amount to health of player
	*
	* @date Created May 5, 2006
	* @param[in]	unBoost	The amount to add.
	*/
	void addToHealth(int unBoost)
	{
		m_nHealth += unBoost;

		// make sure the health stays between 0 and 100
		if(m_nHealth < 0)
			m_nHealth = 0;
		else if(m_nHealth > 100)
			m_nHealth = 100;
	}

	/**
	* Accessors for the player class members
	*
	* @date Created March 27, 2006
	* @return	The applicable member variable.
	* @{
	*/
	unsigned int getAmtMoney() {return m_unMoney;}
	EPlayerInputType getInputType() {return m_eInputType;}
	CBaseWeapon* getCurrentWeapon() {if (*m_CurWeapon) return *m_CurWeapon; else return NULL;}
	CGrenadeWeapon* getCurrentGrenade() {if (*m_CurGrenade) return *m_CurGrenade; else return NULL;}
	SSphere getPickupSphere(void) { return m_tPickupBV; }
	list<CBaseWeapon*> * getWeaponList() { return &m_WeaponInventory; }
	list<CGrenadeWeapon*> * getGrenadeList() { return &m_GrenadeInventory; }
	bool isAwesome() {return m_bAwesome;}
	//!@}

	/**
	* Accessors for the player class members
	*
	* @date Created March 27, 2006
	* @param[in]	**Fire				the button to use to fire.
	* @param[in]	**Grenade			the button to throw grenade.
	* @param[in]	**RotateCamLeft		the button to rotate camera left.
	* @param[in]	**RotateCamRight	the button to rotate camera right.
	* @param[in]	**SwitchToPrev		the button to switch to the previous weapon.
	* @param[in]	**SwitchToNext		the button to switch to the next weapon.
	* @param[in]	**Action			the button to perform an action.
	* @param[in]	dwMoveLeft			the keyboard button for moving left.
	* @param[in]	dwMoveRight			the keyboard button for moving right.
	* @param[in]	dwMoveUp			the keyboard button for moving up.
	* @param[in]	dwMoveDown			the keyboard button for moving down.
	* @{
	*/
	void setControls(const DWORD dwFire, const DWORD dwGrenade, const DWORD dwRotateCamLeft, const DWORD dwRotateCamRight,
		const DWORD dwSwitchToPrev, const DWORD dwSwitchToNext, const DWORD dwAction, const DWORD dwPause);
	void setControls(const BYTE ucFire, const BYTE ucGrenade, const BYTE ucRotateCamLeft, const BYTE ucRotateCamRight,
		const BYTE ucSwitchToPrev, const BYTE ucSwitchToNext, const BYTE ucAction, const BYTE ucPause);
	void setControls(const DWORD dwFire, const DWORD dwGrenade, const DWORD dwRotateCamLeft, const DWORD dwRotateCamRight,
		const DWORD dwSwitchToPrev, const DWORD dwSwitchToNext, const DWORD dwAction, const DWORD dwMoveLeft, const DWORD dwMoveRight,
		const DWORD dwMoveUp, const DWORD dwMoveDown, const DWORD dwPause);
	//!@}

	/**
	* function to swap the models, stats for awesome time
	*
	* @date Created March 29, 2006
	*/
	void enterAwesomeTime();

	/**
	* function to switch weapons
	*
	* @date Created March 29, 2006
	* @param[in]	bDirection	true moves forward through inventory, false backward 
	*/
	void switchWeapon(bool bDirection);

	/**
	* add a weapon to the inventory list
	*
	* @date Created March 29, 2006
	* @param[in]	the weapon to add
	*/
	void addWeapon(CBaseWeapon *poNewGun);

	/**
	* add a grenade to the inventory list
	*
	* @date Created April 27, 2006
	* @param[in]	the grenade to add
	*/
	void addGrenade(CGrenadeWeapon *poNewGrenade);

	/**
	* remove a weapon to the inventory list
	*
	* @date Created March 29, 2006
	* @param[in]	the weapon to remove
	* @return the pointer to that weapon
	*/
	void dropWeapon(CBaseWeapon *poDropGun);

	/**
	* remove the current weapon from the inventory
	*
	* @date Created March 29, 2006
	*/
	void dropCurrentWeapon();

	/**
	* update the player
	*
	* @date Created March 29, 2006
	*/
	virtual void update();

	/**
	* update the player based on the object it collided with
	*
	* @date Created April 23, 2006
	* @param[in]	poActor		the object it collided with
	*/
	virtual void collided(CActor *poActor);

	/**
	* create a player
	*
	* @date Created April 23, 2006
	*/
	static CObject* create()
	{
		return new CPlayer;
	}

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 18, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* Renders the actor and allows subclasses to 
	* perform specific behavior at render time.
	*
	* @date Created Jun 05, 2006
	*/
	virtual void draw(void);

	/**
	* checks for collsion with the orientation and the enemy
	*
	* @date Created June 7, 2006
	*/
	void checkTargeted(CObject* poObject);

	/**
	* calculate how long until we play a sound
	*
	* @date Created June 8, 2006
	*/
	void calcPlaySoundTime(void);
};

#endif