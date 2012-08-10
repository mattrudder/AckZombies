/**
* @file Sound.cpp
* @author Tom Williams
* @date Modified April 27, 2006
*
* definition of class a sound
*/
#include "Sound.h"
#include "SoundManager.h"

/**
* CSound::CSound()
* @date Modified April 27, 2006
*/
CSound::CSound(void)
{
	m_pCurChannel = NULL;
	m_pSound = NULL;
}

/**
* CSound::~CSound()
* @date Modified April 27, 2006
*/
CSound::~CSound(void)
{
}

/**
* CSound::createSoundFromFile()
* @date Modified April 27, 2006
*/
CBaseResource* CSound::createSoundFromFile(const CString sFileName)
{
	// create a new sound
	CSound *pSound = new CSound;

	// load the sound via FMOD
	CSoundManager::getInstance().m_pFmodSys->createSound(sFileName.GetBuffer(), FMOD_DEFAULT, NULL, &pSound->m_pSound);

	// if the sound is longer than 10 seconds, unload it and load it as a stream
	unsigned int unLength;
	pSound->m_pSound->getLength(&unLength, FMOD_TIMEUNIT_MS);
	if(unLength > 10000)
	{
		pSound->m_pSound->release();
		// load the sound via FMOD
		CSoundManager::getInstance().m_pFmodSys->createStream(sFileName.GetBuffer(), FMOD_LOOP_NORMAL|FMOD_2D|FMOD_HARDWARE,
			NULL, &pSound->m_pSound);
	}

	return pSound;
}

/**
* CSound::getLength()
* @date Modified May 21, 2006
*/
unsigned int CSound::getLength()
{
	if (m_pSound)
	{
		unsigned int unLength;
		m_pSound->getLength(&unLength, FMOD_TIMEUNIT_MS);
		return unLength;
	}
	else
		return 0;
}

