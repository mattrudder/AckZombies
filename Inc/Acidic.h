/**
* @file Acidic.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CAcidic class
*/

#ifndef _ACIDIC_H_
#define _ACIDIC_H_

#include "Enemy.h"

/**
* The Acidic enemy in the game
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified May 11, 2006
*/
class CAcidic : public CEnemy
{
	//! time spent circle - ing
	double m_dCircleTime;

	//! distance at which to attack from
	float m_fAttackDist;

	//! circling on way or the other
	bool m_bWhichWay;

public:

	CAcidic(void);
	~CAcidic(void);

	/**
	* Update the enemy and its AI
	*
	* @date Created April 12, 2006
	*/
	void update(void);

	/**
	* Overloaded collided function
	*
	* @date Created May 9, 2006
	*/
	virtual void collided(CActor* pActor);

	/**
	* create a new acidic enemy
	*
	* @date Created April 23, 2006
	*/
	static CObject* create();

	/**
	* get the time spent circle ing
	*
	* @data Created April 12, 2006
	*/
	inline double getCircleTime(void) { return m_dCircleTime; }

	/**
	* add to the time circleing
	*
	* @date Created April 12, 2006
	*/
	inline void addCircleTime(double dTime) { m_dCircleTime += dTime; }

	/**
	* reset the circle time
	*
	* @date Created April 12, 2006
	*/
	inline void resetCircleTime(void) { m_dCircleTime = 0.0f; }

	/**
	* get the distance to attack at
	*
	* @data Created April 13, 2006
	*/
	inline float getAttackDist(void) { return m_fAttackDist; }

	/**
	* set the distance to attack at
	*
	* @date Created April 13, 2006
	*/
	inline void setAttackDist(float fAttackDist) { m_fAttackDist = fAttackDist; }

	/**
	* sets the way this acidic circles
	*
	* @date Created May 4, 2006
	*/
	inline void setWay(bool bWhichWay) { m_bWhichWay = bWhichWay; }

	/**
	* gets the way this acidic circles
	*
	* @date Created May 4, 2006
	*/
	inline bool getWay(void) { return m_bWhichWay; }

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);
};

#endif /*_ACIDIC_H_*/