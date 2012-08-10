/**
* @file Enemy.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 1, 2006
*
* This file contains the definition of the CEnemy class
*/

#ifndef _ENEMY_H_
#define _ENEMY_H_

// Local Includes
#include "Character.h"
#include "AIEntity.h"
#include "AIState.h"
#include "ResourceManager.h"

class CParticleEmitter;

/**
* The Base class encapsulation of an Enemy and AI
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 2, 2006
* @date Modified May 11, 2006
*/
class CEnemy : public CCharacter
{
protected:
	//! The AI Entity that governs behavior
	CAIEntity m_oAI;

	//! dirt spawning effect
	CParticleEmitter* m_poDirt;

	//! If we're being selected by a player
	bool m_bSelected;

	/**
	* Constructor
	*
	* @date Modified April 2, 2006
	*/
	CEnemy(EObjectType eType = OBJ_ENEMY);

public:

	virtual ~CEnemy(void);
	
	//! Mutators
	void setSelected(bool bSelected) { m_bSelected = bSelected; }
	//! Accessors
	bool isSelected(void) { return m_bSelected; }

	/**
	* Update the enemy and its AI
	*
	* @date Created April 2, 2006
	*/
	virtual void update(void) = 0;

	/**
	* get the emitter for the spawn effect
	*
	* @date Created May 30, 2006
	*/
	CParticleEmitter* getDirtEmitter(void) { return m_poDirt; }

	/**
	* Set the AI current state
	*
	* @date Created April 3, 2006
	*/
	inline void setAIState(CAIState* poState)
	{
		if (m_oAI.getState())
			m_oAI.getState()->exit(&m_oAI, this);
		
		m_oAI.setState(poState);

		if (m_oAI.getState())
			m_oAI.getState()->enter(&m_oAI, this);
	}

	/**
	* Get the type of AI state
	* 
	* @date Created April 3, 2006
	* @return The type of the current AI state
	*/
	CAIState::EAIStateTypes getStateType(void);

	/**
	* Get the access to the AI
	* 
	* @date Created April 3, 2006
	* @return The AI
	*/
	inline CAIEntity* getAI(void) { return &m_oAI; }

	/**
	* update the enemy based on the object it collided with
	*
	* @date Created April 23, 2006
	* @param[in]	poActor		the object it collided with
	*/
	virtual void collided(CActor *pActor);

	/**
	* function that creates multiple coins based on the value passed in
	*
	* @date Created May 2, 2006
	* @param[in]	nAmount		combined value of all coins spawned
	* @param[in]	fVelMag		the magnitude of the initial velocity for the coin
	*/
	void spawnCoins(int nAmount, float fVelMin, float fVelMax);

	/**
	* function that creates multiple coins based on the value passed in
	*
	* @date Created May 2, 2006
	* @param[in]	nAmount		combined value of all coins spawned
	* @param[in]	fVelMag		the magnitude of the initial velocity for the coin
	*/
	void spawnPickup(int nAmount, float fVelMin, float fVelMax);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* Used to notify objects of removal from the object manager's active list.
	*
	* @date Created May 30, 2006
	*/
	virtual void destroy(void);
};

#endif /*_ENEMY_H_*/