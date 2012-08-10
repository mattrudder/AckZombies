/**
* @file SoundManger.cpp
* @author Tom Williams
* @date Modified April 26, 2006
*
* definition of class that handles sound within the game
*/
#include ".\soundmanager.h"
#include "resourcemanager.h"

/**
* CSoundManager::CSoundManager()
* @date Modified April 26, 2006
*/
CSoundManager::CSoundManager(void)
{
	m_pFmodSys = NULL;
	memset(&m_atSounds[0], 0, SND_TOTAL_NUMBER*sizeof(m_atSounds[0]));
}

/**
* CSoundManager::~CSoundManager()
* @date Modified April 26, 2006
*/
CSoundManager::~CSoundManager(void)
{
	//deinitialize();
}

/**
* CSoundManager::initialize()
* @date Modified April 26, 2006
*/
void CSoundManager::initialize()
{
	// if already initialized, deinit first
	if(m_pFmodSys)
		deinitialize();

	// attempt to create the system
	FMOD_RESULT res = FMOD::System_Create(&m_pFmodSys);

	// if it was successfully created, init fmod
	if(res == FMOD_OK)
	{
		res = m_pFmodSys->init(100, FMOD_INIT_NORMAL, 0);
	}

	// load all the sounds within the game
	m_atSounds[SND_MUSIC_MAINMENU] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "MUSIC Credits (Deep & Noisy FS).mp3");
	m_atSounds[SND_MUSIC_GAME] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "ingame.mp3");
	m_atSounds[SND_MUSIC_AWESOMETIME] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "MUSIC (Random Battle).mp3");
	m_atSounds[SND_MUSIC_PAUSE] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "MUSIC (In Game Play Two).mp3");
	m_atSounds[SND_EFFECT_ROCKETLAUNCH] =	(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "rocketFire.mp3");
	m_atSounds[SND_EFFECT_PLAYERRUN] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "Step Player.wav");
	m_atSounds[SND_EFFECT_DEATH] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "Player Death.wav");
	m_atSounds[SND_EFFECT_MENUCLICK] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "Menu Click.wav");
	m_atSounds[SND_EFFECT_MENUCANCEL] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "Menu Cancel.wav");
	m_atSounds[SND_EFFECT_MENUHOVER] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "MouseHover.mp3");
	m_atSounds[SND_EFFECT_AWESOMETIME] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "awesome time2.mp3");

	// Gun Selections
	m_atSounds[SND_SELECT_SHOTGUN] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "shotgun.mp3");
	m_atSounds[SND_SELECT_TIPLESHOTGUN] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "tripleshotgun.mp3");
	m_atSounds[SND_SELECT_POTATOCHAINGUN] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "potatochaingun.mp3");
	m_atSounds[SND_SELECT_HOLYWATERBALLOONBATMAN] =	(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "holywaterballooncanonbatman.mp3");
	m_atSounds[SND_SELECT_ROCKETLAUNCHER] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "rocketlauncher.mp3");
	m_atSounds[SND_SELECT_DEAGLE] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "deserteagle.mp3");
	m_atSounds[SND_SELECT_BIGCLUCKINGBOMB] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "bigcluckinbomb.mp3");

	// Voices
	m_atSounds[SND_EFFECT_BIGCLUCKINGBOMB_MOVE] =		(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "bigcluckinbombMove.mp3");
	m_atSounds[SND_EFFECT_BIGCLUCKINGBOMB_EXPLODE] =	(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "bigcluckinbombExplode.mp3");
	m_atSounds[SND_EFFECT_GASEOUS_READY] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "gaseousReadyTo.mp3");
	m_atSounds[SND_EFFECT_GUNCOCK] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "gunCock.mp3");
	m_atSounds[SND_EFFECT_IHATEZOMBIES] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyHateZombies.mp3");
	m_atSounds[SND_EFFECT_NEEDCASH] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyNeedCash.mp3");
	m_atSounds[SND_EFFECT_NOTENOUGHCASH] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyNoCash.mp3");
	m_atSounds[SND_EFFECT_SODAWITHSHOTGUN] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnySodaShotgun.mp3");
	m_atSounds[SND_EFFECT_JOHNNYTAKEDAMAGE] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyTakeDamage.mp3");
	m_atSounds[SND_EFFECT_JOHNNYTAKEDAMAGE1] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyTakeDamage1.mp3");
	m_atSounds[SND_EFFECT_JOHNNYTAKEDAMAGE2] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyTakeDamage2.mp3");
	m_atSounds[SND_EFFECT_JOHNNYTHROW] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "johnnyThrow.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEATTACK] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieAttack.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEBRAINS] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieBrains.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEDEATH] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieDeath.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEDEATH1] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieDeath1.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEDEATH2] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieDeath2.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEDEATH3] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieDeath3.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEDEATH4] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieDeath4.mp3");
	m_atSounds[SND_EFFECT_ZOMBIEDEATH5] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieDeath5.mp3");
	m_atSounds[SND_EFFECT_ZOMBIESPAWN] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieSpawn.mp3");
	m_atSounds[SND_EFFECT_ZOMBIETAKEDAMAGE] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "zombieTakeDamage.mp3");
	m_atSounds[SND_EFFECT_EXPLOSION] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "explosion.mp3");
	m_atSounds[SND_EFFECT_EXPLOSION1] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "explosion1.mp3");
	m_atSounds[SND_EFFECT_EXPLOSION2] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "explosion2.mp3");
	m_atSounds[SND_EFFECT_EXPLOSION3] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "explosion3.mp3");
	m_atSounds[SND_EFFECT_PISTOLFIRE] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "pistolFire.mp3");
	m_atSounds[SND_EFFECT_SHOTGUNFIRE] =				(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "shotgunFire.mp3");
	m_atSounds[SND_EFFECT_POTATOFIRE] =					(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "potatoeFire.mp3");
	m_atSounds[SND_EFFECT_WATERCANNONFIRE] =			(CSound*)CResourceManager::getInstance().loadResource(RES_SOUND, "waterCannonFire.mp3");

}

/**
* CSoundManager::deinitialize()
* @date Modified April 26, 2006
*/
void CSoundManager::deinitialize()
{
	// attempt to release all the sounds
	for(int i = 0; i < SND_TOTAL_NUMBER; ++i)
	{
		// attempt to release the FMOD::Sound pointers
		if(m_atSounds[i] && m_atSounds[i]->getSoundPtr())
		{
			m_atSounds[i]->getSoundPtr()->release();
			m_atSounds[i]->setSoundPtr(NULL);
		}
	}

	m_pFmodSys->release();
	m_pFmodSys = NULL;
}

/**
* CSoundManager::playSound()
* @date Modified 
*/
void CSoundManager::playSound(ESoundType eToPlay, const int nLoopCount, bool bRestart, bool bOverLapSound)
{
	// check if the sound is already playing
	FMOD::Channel* pChannel;
	// verify that there is even a channel
	if(m_atSounds[eToPlay]->getChannelPtr())
	{
		// if the channel is playing, don't keep going.
		bool bPlaying = false;
		m_atSounds[eToPlay]->getChannelPtr()->isPlaying(&bPlaying);
		// if the sounds already playing, stop it and play it again
		if(bPlaying)
		{
			if(bRestart)
				m_atSounds[eToPlay]->getChannelPtr()->stop();
			else if(!bOverLapSound)
				return;
		}

	}

	// set up the channel if there's looping
	m_atSounds[eToPlay]->getSoundPtr()->setLoopCount(nLoopCount);
	FMOD_RESULT res = m_pFmodSys->playSound(FMOD_CHANNEL_FREE, m_atSounds[eToPlay]->getSoundPtr(), false, &pChannel);
	m_atSounds[eToPlay]->setChannelPtr(pChannel);
}

/**
* CSoundManager::update()
* @date Modified April 30, 2007
*/
void CSoundManager::update()
{
	// update the system
	m_pFmodSys->update();
}

/**
* CSoundManager::setMusicVolume()
* @date Modified April 30, 2007
*/
void CSoundManager::setMusicVolume(float fVolume)
{
	// get all the defaults of the sound
	float fFreq, fOldVol, fPan;
	int nPriority;

	// make sure that the volume is in range
	if(fVolume < 0.0f)
		fVolume = abs(fVolume);
	if(fVolume > 1.0f)
		fVolume = 1.0f;
	
	// loop through, get the defaults, and reset the volume
	for(int i = 0; i < SND_NUM_MUSIC_TRACKS; ++i)
	{
		m_atSounds[i]->getSoundPtr()->getDefaults(&fFreq, &fOldVol, &fPan, &nPriority);
		m_atSounds[i]->getSoundPtr()->setDefaults(fFreq, fVolume, fPan, nPriority);

		// if the music is also playing, change the channel volume
		if(m_atSounds[i]->getChannelPtr())
		{
			bool bPlaying;
			m_atSounds[i]->getChannelPtr()->isPlaying(&bPlaying);
			if(bPlaying)
				m_atSounds[i]->getChannelPtr()->setVolume(fVolume);
		}
	}
}

/**
* CSoundManager::setEffectVolume()
* @date Modified April 30, 2007
*/
void CSoundManager::setEffectVolume(float fVolume)
{
	// get all the defaults of the sound
	float fFreq, fOldVol, fPan;
	int nPriority;

	// make sure that the volume is in range
	if(fVolume < 0.0f)
		fVolume = abs(fVolume);
	if(fVolume > 1.0f)
		fVolume = 1.0f;
	
	// loop through, get the defaults, and reset the volume
	for(int i = SND_FIRST_EFFECT; i < SND_TOTAL_NUMBER; ++i)
	{
		m_atSounds[i]->getSoundPtr()->getDefaults(&fFreq, &fOldVol, &fPan, &nPriority);
		m_atSounds[i]->getSoundPtr()->setDefaults(fFreq, fVolume, fPan, nPriority);
	}
}

/**
* CSoundManager::fadeSound()
* @date Modified May 23, 2006
*/
void CSoundManager::fadeSound(ESoundType eSound, float fScale)
{
	// get the current defaults
	float fFreq, fOldVol, fPan;
	int nPriority;

	// make sure that the volume is in range
	m_atSounds[eSound]->getSoundPtr()->getDefaults(&fFreq, &fOldVol, &fPan, &nPriority);

	// change the volume
	fOldVol *= fScale;
	if(fOldVol > 1.0f)
		fOldVol = 1.0f;
	else if(fOldVol < 0.0f)
		fOldVol = 0.0f;

	// reset the volume
	m_atSounds[eSound]->getSoundPtr()->setDefaults(fFreq, fOldVol, fPan, nPriority);
}

/**
* CSoundManager::playSound()
* @date Modified April 29, 2006
*/
void CSoundManager::stopSound(ESoundType eToStop)
{
	FMOD::Channel* pChannel = m_atSounds[eToStop]->getChannelPtr();

	// verify that there is even a channel
	if(pChannel)
	{
		// if the channel is playing, stop it.
		bool bPlaying;
		pChannel->isPlaying(&bPlaying);
		if(bPlaying)
		{
			pChannel->stop();
			m_atSounds[eToStop]->setChannelPtr(NULL);
		}
	}
}

/**
* CSoundManager::playSound()
* @date Modified April 30, 2006
*/
void CSoundManager::pauseSound(ESoundType eToPause, bool bPause)
{
	FMOD::Channel* pChannel = m_atSounds[eToPause]->getChannelPtr();

	// verify that there is even a channel
	if(pChannel)
	{
		// if the channel is playing, stop it.
		pChannel->setPaused(bPause);
	}
}

/**
* CSoundManager::getLength()
* @date Modified May 21, 2006
*/
unsigned int CSoundManager::getLength(ESoundType eSound)
{
	return m_atSounds[eSound]->getLength();
}