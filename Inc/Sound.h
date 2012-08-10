/**
* @file Sound.h
* @author Tom Williams
* @date Created April 26, 2006
*
* definition of a sound within the game
*/

#ifndef _SOUND_H_
#define _SOUND_H_

#include "baseresource.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

/**
* Encapsulates a sound for fmod
*
* @author Tom Williams
* @date Created Apr 26, 2006
* @date Modified Apr 26, 2006
*/
class CSound : public CBaseResource
{
	//! pointer to the sound
	FMOD::Sound *m_pSound;

	//! the current channel if the sound is playing
	FMOD::Channel *m_pCurChannel;

public:

	/**
	* constructor for the sound class
	*
	* @date Created April 26, 2006
	*/
	CSound();

	/**
	* destructor for the sound class
	*
	* @date Created April 26, 2006
	*/
	virtual ~CSound();

	/**
	* accessors for the sound class
	*
	* @date Created April 27 2006
	* @return value of member
	* @{
	*/
	FMOD::Sound*	getSoundPtr()	{return m_pSound;}
	FMOD::Channel*	getChannelPtr() {return m_pCurChannel;}
	//!@}

	/**
	* modifiers for the sound class
	*
	* @date Created April 27, 2006
	* @param[in] value to set member to
	* @{
	*/
	void setSoundPtr(FMOD::Sound* pSound) {m_pSound = pSound;}
	void setChannelPtr(FMOD::Channel* pChan) {m_pCurChannel = pChan;}
	//!@}

	/**
	* Loads a sound from disk.
	*
	* @date Created April 26, 2006
	* @param[in]	sFilename	The sound to load
	* @return true if the sound was loaded, false otherwise
	*/
	static CBaseResource* createSoundFromFile(const CString sFileName);

	/**
	* Gets the length of a sound.
	*
	* @date Created May 21, 2006
	* @return length of the sound
	*/
	unsigned int getLength();
};

#endif