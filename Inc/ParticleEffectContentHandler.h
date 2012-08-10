/**
* @file ParticleEffectContentHandler.h
* @author Matt Rudder
* @date Created May 20, 2006
*
* Content Handler for Particle Effect files.
*/

#ifndef _PARTICLEXML_H_
#define _PARTICLEXML_H_

// XML API
#include <xercesc/sax2/DefaultHandler.hpp>
XERCES_CPP_NAMESPACE_USE

// Engine includes
#include "ParticleSystem.h"
#include "ParticleEffect.h"
#include "ParticleAttribute.h"

class CParticleEffectContentHandler : public DefaultHandler
{
protected:

	enum EEffectFlags
	{
		EFF_NONE		= 0x00000000,
		EFF_SYSTEM		= 0x00000001,
		EFF_EFFECT 		= 0x00000010,
		EFF_MAXVEL 		= 0x00000020,
		EFF_MINVEL 		= 0x00000040,
		EFF_RADIUS 		= 0x00000080,
		EFF_ATTRLIST	= 0x00000100,
		EFF_ATTRIB		= 0x00000200,
		EFF_KEYLIST		= 0x00000400,
		EFF_KEYFRAME	= 0x00000800,
		EFF_FORCEDWORD	= 0xFFFFFFFF
	};
	CParticleSystem* m_pEmitter;
	DWORD m_dwFlags;
	CParticleEffect* m_pCurrentEffect;
	CParticleAttribute* m_pCurrentAttrib;
public:
	CParticleEffectContentHandler(CParticleSystem* pEmitter);


	EEffectFlags getElemType(const XMLCh* szLocalName);
	void processAttributes(EEffectFlags eType, const Attributes& rAttributes);
	CParticleEffect::EBlendOp getBlendOp(CString& sBlend);
	CParticleEffect::EShape getShape(CString& sShape);
	CParticleEffect::ESpawnMode getSpawnMode(CString& sMode);
	CParticleAttribute::EAttributeType getAttribType(CString& sType);
	CParticleAttribute::EInterpolationType getInterpMethod(CString& sType);

	// -----------------------------------------------------------------------
	//  Handlers for the SAX ContentHandler interface
	// -----------------------------------------------------------------------
	void startElement(const XMLCh* const szURI, const XMLCh* const szLocalName, const XMLCh* const szQName, const Attributes& rAttributes);
	void endElement(const XMLCh* const szURI,	const XMLCh* const szLocalName, const XMLCh* const szQName);
};

#endif //_PARTICLEXML_H_