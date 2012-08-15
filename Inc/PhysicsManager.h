/**
* @file PhysicsManger.h
* @author Tom Williams
* @date Created Mar 29, 2006
*
* Update and check collision between actors and world
*/

#ifndef _PHYSICSMANAGER_H_
#define _PHYSICSMANAGER_H_

// local includes/libraries
#include "Singleton.h"
#include "TickTimer.h"
#include "ObjectManager.h"
#include "Player.h"
#include "CollisionGeometry.h"
#include "Enemy.h"
#include "methodsinglefunctor.h"

// system includes/libraries
#include <vector>

// define the initial values of friction/gravity
#define INIT_FRICTION	5.0f
#define INIT_IMPACT		500.0f
#define INIT_GRAVITY	50.0f

/**
* Checks for actor collision, checks players against the view frustum, and applies gravity/friction if necessary
*
* @author Tom Williams
* @date Created March 27, 2006
* @date Modified June 10, 2006
*/
class CPhysicsManager : public CSingleton<CPhysicsManager>
{
	//! necessary for correct operation of template singleton class
	friend class CSingleton<CPhysicsManager>;
	friend class CLoadState;

	//! the actor currently being updated
	CActor* m_poCurrentActor;

	//! instance of the object manager
	CObjectManager *m_poObjManager;

	//! instance of the collision geometry
	CCollisionGeometry *m_poColGeom;

	//! functors to move through the collision functions
	CMethodSingleFunctor<CPhysicsManager, CObject*> m_oCheckCol;
	CMethodSingleFunctor<CPhysicsManager, CObject*> m_oCheckFrustum;

	//! gravity to apply to falling objects
	float m_fGravity;

	//! friction to apply to bouncing objects
	float m_fFriction;

	// privatize constructor/copy constructor/destructor/assignment
	/**
	* necessary for Singleton class
	*
	* @date Created March 29, 2006
	* @{
	*/
	CPhysicsManager(void);
	~CPhysicsManager(void);
	CPhysicsManager(const CPhysicsManager &r)
		: m_oCheckCol(&CPhysicsManager::checkCollision, this)
		, m_oCheckFrustum(&CPhysicsManager::checkFrustumCol, this)
	{
	}

	CPhysicsManager &operator =(const CPhysicsManager &r) {}
	//!@}
public:
	/**
	* Updates actor based on physics & checks collision
	*
	* @date Created March 29, 2006
	* @param[in]	poActor		the new current actor
	*/
	void update(CActor *poActor, float fDT);

	/**
	* checks collision between actors
	*
	* @date Created May 26, 2006
	* @param[in]	poObj	the actor to test against current actor
	*/
	void checkCollision(CObject *poObj);

	/**
	* check collision w/ players and the view frustum
	*
	* @date Created May 26, 2006
	* @param[in]	poObj	the player to check the current actor against
	*/
	void checkFrustumCol(CObject *poObj);

	/**
	* Modifiers for the CPhysicsManager members
	*
	* @date Created March 29, 2006
	* @param[in]	the variable to set the member to
	* @{
	*/
	void setGravity(float fGrav) {m_fGravity = fGrav;}
	void setFriction(float fFric) {m_fFriction = fFric;}
	//!@}

	/**
	* Accessors for the CPhysicsManager members
	*
	* @date Created March 29, 2006
	* @return	the member value
	* @{
	*/
	float getGravity() {return m_fGravity;}
	float getFriction(){return m_fFriction;}
	//!@}

	/**
	* init the collision geometry
	*
	* @date Created May 8, 2006
	*/
	void initCollisionGeometry(const char *filename);
};

#endif