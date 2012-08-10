/**
* @file AIGroup.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 19, 2006
*
* This file contains the definition of the CAIGroup class.
*/

#ifndef _AIGROUP_H_
#define _AIGROUP_H_

// Local Includes
#include "Enemy.h"
#include "Math3D.h"

// System Includes
#include <list>

/**
* A collection of associated AIEntities used for path planning
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 19, 2006
* @date Modified May 30, 2006
*/
class CAIGroup
{
	//! The enemies in the group
	std::list<CEnemy*> m_loEnemies;

	//! The average position of the group
	D3DXVECTOR3 m_vAvgPos;

	//! The leader of this group
	CEnemy* m_poLeader;

public:

	CAIGroup(void);
	~CAIGroup(void);

	//! Access Stuff
	//!@{
	std::list<CEnemy*>* getEnemies(void) { return &m_loEnemies; }
	void setAvgPos(D3DXVECTOR3 vAvgPos) { m_vAvgPos = vAvgPos; }
	D3DXVECTOR3 getAvgPos(void) { return m_vAvgPos; }
	void setLeader(CEnemy* poLeader) { m_poLeader = poLeader; }
	CEnemy* getLeader(void) { return m_poLeader; }
	//!@}

	/**
	* Re-calculates the average position
	*
	* @date Created April 19, 2006
	*/
	void calculateAvgPos(void);

	/**
	* Updates the leader of this group
	*
	* @date Created April 19, 2006
	* @param[in]	bSpawn		is this group just spawned
	*/
	void updateLeader(bool bSpawn = false);

	/**
	* Destroys the group and sets the members state's to path plan
	*
	* @date Created April 20, 2006
	* @param[in]	bDisperse	should they activly move away from each other
	*/
	void disband(bool bDisperse = false);

	/**
	* Removes the enemy from the group
	*
	* @date Created April 30, 2006
	*/
	void remove(CEnemy* poEnemy);
};
#endif