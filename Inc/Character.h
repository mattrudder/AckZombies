/**
* @file Character.h
* @author Tom Williams
* @date Created March 27, 2006
*
* declaration of the charcter class
*/
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// local includes/libraries
#include "Actor.h"
#include "TickTimer.h"
#include "Math3d.h"
#include "particleemitter.h"

// system includes/libraries

/**
* the class which describes any character in the game
*
* @author Tom Williams
* @date Created March 27, 2006
* @date Modified  June 7, 2006
*/
class CCharacter : public CActor
{
protected:
	//! the character's health/ability to take damage
	int m_nHealth;

	//! The death particle for a character
	CParticleEmitter* m_pDeathParticle;

	//! Timer to check against animation length
	float m_fAnimationTime;

	//! Temporary velocity to hold on to while being shot.
	D3DXVECTOR3 m_vTempVel;

public:

	//! Mutators
	void setAnimationTime(float fTime) { m_fAnimationTime = fTime; }
	void setTempVel(D3DXVECTOR3 &vel) { m_vTempVel = vel; }
	//! Accessors
	float getAnimationTime(void) { return m_fAnimationTime; }
	D3DXVECTOR3 getTempVel(void) { return m_vTempVel; }

	/**
	* constructor for the character class
	*
	* @date Created March 27, 2006
	*/
	CCharacter(EObjectType eType = OBJ_CHARACTER);

	/**
	* destructor for the character class
	*
	* @date Created March 27, 2006
	*/
	virtual ~CCharacter() {}

	/**
	* Modifiers for the character members
	*
	* @date Created March 27, 2006
	* @param[in]	The variable to set the member to.
	* @{
	*/
	void setHealth(const int nHealth) {m_nHealth=nHealth;}
	//!@}

	/**
	* Accessors for the character class members
	*
	* @date Created March 27, 2006
	* @return	The applicable member variable.
	* @{
	*/
	int getHealth() {return m_nHealth;}
	//!@}

	/**
	* update the character based on the object it collided with
	*
	* @date Created April 23, 2006
	* @param[in]	poActor		the object it collided with
	*/
	virtual void collided(CActor *poActor);

	/**
	* create a new character
	*
	* @date Created April 23, 2006
	*/
	static CObject* create()
	{
		return new CCharacter;
	}

	/**
	* updates a character
	*
	* @date Created April 28, 2006
	*/
	virtual void update();

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);
};


#endif