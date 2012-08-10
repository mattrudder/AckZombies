/**
* @file SoundManger.h
* @author Tom Williams
* @date Created April 26, 2006
*
* definition of class that handles sound within the game
*/
#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "singleton.h"
#include "soundtypes.h"
#include <vector>
#include "baseresource.h"
#include "sound.h"

/**
* the class which handles all sounds within the game
*
* @author Tom Williams
* @date Created April 26, 2006
* @date Modified April 26, 2006
*/
class CSoundManager : public CSingleton<CSoundManager>
{
	friend class CSingleton<CSoundManager>;
	friend class CSound;

protected:

	//! instance to FMOD
	FMOD::System *m_pFmodSys;

	//! all the tracks in the game
	CSound *m_atSounds[SND_TOTAL_NUMBER];

	/**
	* necessary protected trilogy of evil for singleton
	*
	* @date Created April 24, 2006
	* @{
	*/
	CSoundManager(void);
	~CSoundManager(void);
	CSoundManager(const CSoundManager &o){}
	CSoundManager &operator =(const CSoundManager &o){}
	//!@}

public:

	/**
	* initialize the sound manager
	*
	* @date Created April 26, 2006
	*/
	void initialize();

	/**
	* deinitialize the sound manager
	*
	* @date Created April 26, 2006
	*/
	void deinitialize();

	/**
	* update the system
	*
	* @date Created June 11, 2006
	*/
	void update();

	/**
	* play the sound
	*
	* @date Created April 26, 2006
	* @param[in]		eType		the sound to play
	* @param[in]		nLoopCnt	the number of times to loop(-1 for infinite)
	* @param[in]		bRestart	whether or not to restart the sound
	*/
	void playSound(ESoundType eToPlay, const int nLoopCount = 0, bool bRestart = false, bool bOverLapSound = true);

	/**
	* stop a playing sound
	*
	* @date Created April 26, 2006
	* @param[in]		eType		the sound to stop
	*/
	void stopSound(ESoundType eToStop);

	/**
	* pause/unpause a playing sound
	*
	* @date Created April 26, 2006
	* @param[in]	eType		the sound to stop
	* @param[in]	bPause		whether to pause/unpause the sound	
	*/
	void pauseSound(ESoundType eToPause, bool bPause);

	/**
	* set the music volume of sounds within game
	*
	* @date Created April 30, 2006
	* @param[in]	fVolume		the volume to set to(0.0 - 1.0)
	*/
	void setMusicVolume(float fVolume);

	/**
	* set the music volume of sounds within game
	*
	* @date Created April 30, 2006
	* @param[in]	fVolume		the volume to set to(0.0 - 1.0)
	*/
	void setEffectVolume(float fVolume);

	/**
	* fade a sound
	*
	* @date Created May 23, 2006
	* @param[in]	eSound		the sound to fade
	* @param[in]	fScale		the amount to scale the sound by
	*/
	void fadeSound(ESoundType eSound, float fScale);

	/**
	* gets the length of a sound
	*
	* @date Created May 21, 2006
	* @param[in]		eType		the sound to get the length of
	*/
	unsigned int getLength(ESoundType eSound);
	
};
#endif