/**
* @file ParticleEmitter.cpp
* @author Matt Rudder
* @date Modified May 20, 2006
*
* Defines a collection of particle effects and their location in the game world.
*/

// XML API
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
XERCES_CPP_NAMESPACE_USE

// Local includes
#include "ParticleSystem.h"
#include "ParticleEffectContentHandler.h"
#include "ParticleEffect.h"

/**
* CParticleSystem::CParticleSystem
* @date Modified May 20, 2006
*/
CParticleSystem::CParticleSystem(void) : m_bPlaying(false),	m_fCurrentTime(0.0f)
{
}

/**
* CParticleSystem::CParticleSystem
* @date Modified Jun 01, 2006
*/
CParticleSystem::CParticleSystem(const CParticleSystem& rSys) : m_bPlaying(false),
	m_fCurrentTime(0.0f)
{
	SEffectInstance oInst;
	memset(&oInst, 0, sizeof(oInst));
	for(size_t i = 0; i < rSys.m_vEffects.size(); ++i)
	{
		oInst.pEffect = rSys.m_vEffects[i].pEffect;
		m_vEffects.push_back(oInst);
	}
}

/**
* CParticleSystem::~CParticleSystem
* @date Modified May 20, 2006
*/
CParticleSystem::~CParticleSystem(void)
{
	m_vEffects.clear();
}

/**
* CParticleSystem::cloneResource
* @date Modified May 20, 2006
*/
CBaseResource* CParticleSystem::cloneResource(void)
{
	CParticleSystem* pSys = new CParticleSystem(*this);
	return pSys;
}

/**
* CParticleSystem::toString
* @date Modified May 20, 2006
*/
CString CParticleSystem::toString(void) const
{
	return "";
}

/**
* CParticleSystem::createEmitterFromFile
* @date Modified May 20, 2006
*/
CBaseResource* CParticleSystem::createEmitterFromFile(const CString sFilename)
{
	xercesc::SAX2XMLReader* reader;
	xercesc::ContentHandler* handler;
	CParticleSystem* pNewEmitter = new CParticleSystem();
	try
	{
		// Initialize xerces
		XMLPlatformUtils::Initialize();

		// Initialize xml serializer.
		reader = xercesc::XMLReaderFactory::createXMLReader();
		handler = new CParticleEffectContentHandler(pNewEmitter);
		reader->setContentHandler(handler);
		reader->parse(sFilename.GetBuffer());
		SAFE_DELETE(handler);
	}
	catch (const XMLException& toCatch)
	{
		UNUSED_PARAM(toCatch);

		if(handler)
			delete handler;

		XMLPlatformUtils::Terminate();
		delete reader;
		return NULL;
	}

	CString fmt;
	fmt.Format("Particle system \"%s\" loaded with %d effects", sFilename.GetBuffer(), pNewEmitter->m_vEffects.size());
	Log << fmt.GetBuffer() << endl;

	// Cleanup
	delete reader;
	XMLPlatformUtils::Terminate();
	return pNewEmitter;
}

/**
* CParticleSystem::update
* @date Modified May 20, 2006
*/
void CParticleSystem::update(D3DXMATRIX* mOffset)
{
	if(!m_bPlaying)
		return;

	float fTime = CTimer::getInstance().getFrameTime();
	for(size_t i = 0; i < m_vEffects.size(); ++i)
	{
		unsigned int unMaxParticles = m_vEffects[i].pEffect->getMaxParticles();

		// Looping effects
		if(m_vEffects[i].pEffect->getSpawnMode() == CParticleEffect::SM_LOOPING)
		{
			if(m_vEffects[i].unNumActive >= unMaxParticles)
				continue;

			// Make sure to keep a constant flow of particles.
			float fPerSecond = (float)unMaxParticles /  m_vEffects[i].pEffect->getEffectLength();
			static float fParticleFrag = 0.0f;
			float fParticlesThisFrame = (fPerSecond * fTime);
			float fNumSpawned = 0.0f;

			// Fix for high frame rates causing no particles to be spawned.
			if(fParticlesThisFrame < 1.0f)
			{
				fParticleFrag += fParticlesThisFrame;
				if(fParticleFrag > 1.0f)
				{
					fParticlesThisFrame = fParticleFrag;
					fParticleFrag = 0.0f;
				}
			}

			// Spawn particles continuously over time.
			while(m_vEffects[i].unNumActive < unMaxParticles)
			{
				++fNumSpawned;
				if(fNumSpawned > fParticlesThisFrame)
					break;

				CParticleManager::getInstance().addParticle(mOffset, m_vEffects[i].pEffect, this);
				++m_vEffects[i].unNumSpawned;
				++m_vEffects[i].unNumActive;
			}
		}
		else
		{
			// We only want to spawn max particles once.
			while(m_vEffects[i].unNumSpawned < unMaxParticles)
			{
				CParticleManager::getInstance().addParticle(mOffset, m_vEffects[i].pEffect, this);
				++m_vEffects[i].unNumSpawned;
				++m_vEffects[i].unNumActive;
			}
		}
	}
}

/**
* CParticleSystem::reset
* @date Modified May 21, 2006
*/
void CParticleSystem::reset(bool bPlay)
{
	m_bPlaying = bPlay;

	for(size_t i = 0; i < m_vEffects.size(); ++i)
	{
		m_vEffects[i].unNumSpawned = 0;
	}
}

/**
* CParticleSystem::removeParticle
* @date Modified Jun 02, 2006
*/
void CParticleSystem::removeParticle(CParticleEffect* pEffect)
{
	for(size_t i = 0; i < m_vEffects.size(); ++i)
	{
		if(m_vEffects[i].pEffect == pEffect)
		{
			--m_vEffects[i].unNumActive;
			return;
		}
	}
}