/**
* @file Gaseous.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
*
* This file contains the definition of the CGaseous class
*/

#ifndef _GASEOUS_H_
#define _GASEOUS_H_

#include "Enemy.h"

/**
* The Gaseous enemy in the game
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 7, 2006
* @date Modified June 7, 2006
*/
class CGaseous : public CEnemy
{
private:

	//! Timer to flash red and then explode
	float	m_fFlashTimer;
	//! Should we update?
	bool m_bUpdate;
	//! old color
	D3DCOLOR m_dwOldColor;
	//! red color
	D3DCOLOR m_dwRedColor;

public:

	CGaseous(void);
	~CGaseous(void);

	/**
	* Update the enemy and its AI
	*
	* @date Created April 12, 2006
	*/
	void update(void);

	/**
	* damage characters for the explosion
	*
	* @date Created May 4, 2006
	*/
	void doDamage(void);

	/**
	* create a new gaseous enemy
	*
	* @date Created April 23, 2006
	*/
	static CObject* create();

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);
};

#endif /*_GASEOUS_H_*/