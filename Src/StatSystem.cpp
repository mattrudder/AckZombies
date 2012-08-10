/**
* @file StatSystem.cpp
* @author Tom Williams
* @date Modified March 5, 2006
*
* System to track a player's in game stats
*/
#include ".\statsystem.h"

/**
* CStatSystem::CStatSystem
* date Modified March 5, 2006
*/
CStatSystem::CStatSystem(void)
{
	clearStats();	
}

/**
* CStatSystem::clearStats
* date Modified March 5, 2006
*/
void CStatSystem::clearStats()
{
	// set all the stats to zero initially
	m_unShotsHit = m_unShotsFired = m_unEnemiesKilled = m_unAwesomeKills = m_unTotalMoneyEarned = m_unMoneySpent = 0;
	m_unEnemiesSpawned = m_unAwesomeEnemiesSpawned = m_unNumAwesomes = m_unMoneySpawned = 0;
	m_fAccuracy = m_fTimeTaken = m_fFinalGrade = m_fParTime = m_fAwesomeTime = 0.0f;
}

/**
* CStatSystem::computeAccuracy
* date Modified March 5, 2006
*/
void CStatSystem::computeAccuracy()
{
	// don't do this if the player did not fire.
	if(m_unShotsFired)
	{
		// accuracy is a percentage of the player's shooting skill
		m_fAccuracy = (float)m_unShotsHit/(float)m_unShotsFired * 100.0f;
	}
	else
		m_fAccuracy = 0;
}

/**
* CStatSystem::computeFinalGrade
* date Modified March 5, 2006
*/
void CStatSystem::computeFinalGrade(bool bDead)
{
	if (m_fTimeTaken > 0.0f)
		// weight the time taken to be 20% of the final score (beating par time = full percent)
		m_fFinalGrade = m_fParTime/m_fTimeTaken;

	if(m_fFinalGrade > 1.0f) m_fFinalGrade = 1.0f;
	m_fFinalGrade *= 20.0f;

	if (bDead)
		m_fFinalGrade = 0.0f;

	if (m_unEnemiesSpawned > 0)
		// weight the enemies killed percentage to be 30% of the final score
		m_fFinalGrade += (float)m_unEnemiesKilled/(float)m_unEnemiesSpawned * 30.0f;

	// weight the accuracy of the player to be 25% of the final score
	m_fFinalGrade += m_fAccuracy/100.0f * 25.0f;

	if (m_unNumAwesomes > 0)
		// weight the amount of gained awesome time to 10% of the final score
		m_fFinalGrade += m_fAwesomeTime/(float)(m_unNumAwesomes * 45.0f) * 10.0f;

	if (m_unAwesomeEnemiesSpawned > 0)
		// weight the amount of "Awesome Time" kills by the number of enemies spawned during that period to 10%
		m_fFinalGrade += (float)m_unAwesomeKills/(float)m_unAwesomeEnemiesSpawned * 10.0f;

	if (m_unMoneySpawned > 0)
		// weight the percent of money gained by 15%
		m_fFinalGrade += (float)m_unTotalMoneyEarned/(float)m_unMoneySpawned * 15.0f;

	if (m_unTotalMoneyEarned > 0)
		// take away the percent of the money spent by the player, weighted 10%
		m_fFinalGrade -= (float)m_unMoneySpent/(float)m_unTotalMoneyEarned * 10.0f;
}