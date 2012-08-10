/**
* @file StatSystem.h
* @author Tom Williams
* @date Created March 5, 2006
*
* System to track a player's in game stats
*/
#ifndef _STATSYSTEM_H_
#define _STATSYSTEM_H_

/**
* defines the system to track the player's stats
*
* @author Tom Williams
* @date Created March 5, 2006
* @date Modified March 5, 2006
*/
class CStatSystem
{
	//! the number of shots that hit enemies
	unsigned int m_unShotsHit;
	//! the total number of shots fired
	unsigned int m_unShotsFired;
	//! the total number of enemies killed
	unsigned int m_unEnemiesKilled;
	//! the total number of enemies spawned
	unsigned int m_unEnemiesSpawned;
	//! the total number of enemies spawned during "Awesome Time"
	unsigned int m_unAwesomeEnemiesSpawned;
	//! the total number of kills during "Awesome Time"
	unsigned int m_unAwesomeKills;
	//! the total number of times in "Awesome Time"
	unsigned int m_unNumAwesomes;
	//! the total amount of money spawned in the level
	unsigned int m_unMoneySpawned;
	//! the total amount of money the player earned
	unsigned int m_unTotalMoneyEarned;
	//! the total amount of money the player spent
	unsigned int m_unMoneySpent;
	//! the accuracy of the player (shotshit/shotsfired)
	float m_fAccuracy;
	//! the total amount of time taken to complete a level
	float m_fTimeTaken;
	//! the par time to complete the level
	float m_fParTime;
	//! the amount of time in "Awesome Time"
	float m_fAwesomeTime;
	//! the final percentage grade of the player
	float m_fFinalGrade;
	
public:
	/**
	* constructor for the stat system
	*
	* @date Created March 5, 2006
	*/
	CStatSystem(void);

	/**
	* destructor for the stat system
	*
	* @date Created March 5, 2006
	*/
	~CStatSystem(void){}

	/**
	* Accessors for the stat system class members
	*
	* @date Created March 5, 2006
	* @return	The applicable member variable.
	* @{
	*/
	inline unsigned int getShotsHit()				{return m_unShotsHit;}
	inline unsigned int getShotsFired()				{return m_unShotsFired;}
	inline unsigned int getEnemiesKilled()			{return m_unEnemiesKilled;}
	inline unsigned int getEnemiesSpawned()			{return m_unEnemiesSpawned;}
	inline unsigned int getAwesomeEnemiesSpawned()	{return m_unAwesomeEnemiesSpawned;}
	inline unsigned int getNumAwesomes()			{return m_unNumAwesomes;}
	inline unsigned int getAwesomeKills()			{return m_unAwesomeKills;}
	inline unsigned int getMoneySpawned()			{return m_unMoneySpawned;}
	inline unsigned int getTotalMoneyEarned()		{return m_unTotalMoneyEarned;}
	inline unsigned int getMoneySpent()				{return m_unMoneySpent;}
	inline float getAccuracy()						{return m_fAccuracy;}
	inline float getTimeTaken()						{return m_fTimeTaken;}
	inline float getAwesomeTime()					{return m_fAwesomeTime;}
	inline float getParTime()						{return m_fParTime;}
	inline float getFinalGrade()					{return m_fFinalGrade;}
	//!@}

	/**
	* Modifiers for the stat system class members
	*
	* @date Created March 5, 2006
	* @param[in]	The variable to set the member to.
	* @{
	*/
	inline void setShotsHit(unsigned int unShotsHit)					{m_unShotsHit = unShotsHit;}
	inline void setShotsFired(unsigned int unShotsFired)				{m_unShotsFired = unShotsFired;}
	inline void setEnemiesKilled(unsigned int unEnemiesKilled)			{m_unEnemiesKilled = unEnemiesKilled;}
	inline void setEnemiesSpawned(unsigned int unEnemiesSpawned)		{m_unEnemiesSpawned = unEnemiesSpawned;}
	inline void setAwesomeEnemiesSpawned(unsigned int unNumSpawn)		{m_unAwesomeEnemiesSpawned = unNumSpawn;}
	inline void setNumAwesomes(unsigned int unNumAwesomes)				{m_unNumAwesomes = unNumAwesomes;}
	inline void setAwesomeKills(unsigned int unAwesomeKills)			{m_unAwesomeKills = unAwesomeKills;}
	inline void setMoneySpawned(unsigned int unMoneySpawned)			{m_unMoneySpawned = unMoneySpawned;}
	inline void setTotalMoneyEarned(unsigned int unTotalMoneyEarned)	{m_unTotalMoneyEarned = unTotalMoneyEarned;}
	inline void setMoneySpent(unsigned int unMoneySpent)				{m_unMoneySpent = unMoneySpent;}
	inline void setAccuracy(float fAccuracy)							{m_fAccuracy = fAccuracy;}
	inline void setTimeTaken(float fTimeTaken)							{m_fTimeTaken = fTimeTaken;}
	inline void setAwesomeTime(float fAwesomeTime)						{m_fAwesomeTime = fAwesomeTime;}
	inline void setParTime(float fParTime)								{m_fParTime = fParTime;}
	inline void setFinalGrade(float fFinalGrade)						{m_fFinalGrade = fFinalGrade;}
	//!@}

	/**
	* functions to increment the member variables by a passed in amount
	*
	* @date Created March 5, 2006
	* @param[in]	The variable to increment the member by (default to 1)
	* @{
	*/
	inline void addShotsHit(unsigned int unNum = 1)				{m_unShotsHit += unNum;}
	inline void addShotsFired(unsigned int unNum = 1)			{m_unShotsFired += unNum;}
	inline void addEnemiesKilled(unsigned int unNum = 1)		{m_unEnemiesKilled += unNum;}
	inline void addEnemiesSpawned(unsigned int unNum = 1)		{m_unEnemiesSpawned += unNum;}
	inline void addAwesomeEnemiesSpawned(unsigned int unNum = 1){m_unAwesomeEnemiesSpawned += unNum;}
	inline void addNumAwesomes(unsigned int unNum = 1)			{m_unNumAwesomes += unNum;}
	inline void addAwesomeKills(unsigned int unNum = 1)			{m_unAwesomeKills += unNum;}
	inline void addMoneySpawned(unsigned int unNum = 1)			{m_unMoneySpawned += unNum;}
	inline void addTotalMoneyEarned(unsigned int unNum = 1)		{m_unTotalMoneyEarned += unNum;}
	inline void addMoneySpent(unsigned int unNum = 1)			{m_unMoneySpent += unNum;}
	inline void addAccuracy(float fNum = 1.0f)					{m_fAccuracy += fNum;}
	inline void addTimeTaken(float fNum = 1.0f)					{m_fTimeTaken += fNum;}
	inline void addAwesomeTime(float fNum = 1.0f)				{m_fAwesomeTime += fNum;}
	inline void addFinalGrade(float fNum = 1.0f)				{m_fFinalGrade += fNum;}
	//!@}

	/**
	* clear all the member variables to zero
	*
	* @date Created March 5, 2006
	*/
	void clearStats();

	/**
	* compute the accuracy based on the ShotsHit and ShotsFired members
	*
	* @date Created March 5, 2006
	*/
	void computeAccuracy();

	/**
	* compute the player's final grade based on all the members
	*
	* @date Created June 10, 2006
	* @param[in]	bDead	did the player die
	*/
	void computeFinalGrade(bool bDead);
};

#endif