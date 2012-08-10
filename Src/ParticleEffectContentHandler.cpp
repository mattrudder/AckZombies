/**
* @file ParticleEffectContentHandler.cpp
* @author Matt Rudder
* @date Modified May 20, 2006
*
* Content Handler for Particle Effect files.
*/

// Local includes
#include "ParticleEffectContentHandler.h"
#include "ParticleManager.h"

// XML API
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>
/**
* CParticleEffectContentHandler::CParticleEffectContentHandler
* @date Modified May 20, 2006
*/
CParticleEffectContentHandler::CParticleEffectContentHandler(CParticleSystem* pEmitter) : 
	m_pEmitter(pEmitter), m_pCurrentEffect(NULL), m_pCurrentAttrib(NULL)
{
}

/**
* CParticleEffectContentHandler::getElemType
* @date Modified May 20, 2006
*/
CParticleEffectContentHandler::EEffectFlags CParticleEffectContentHandler::getElemType(const wchar_t* pwchLocalName)
{
	if(!wcsicmp(pwchLocalName, L"ParticleSystem"))
		return EFF_SYSTEM;

	if(!wcsicmp(pwchLocalName, L"Effect"))
		return EFF_EFFECT;

	if(!wcsicmp(pwchLocalName, L"MaxVelocity"))
		return EFF_MAXVEL;

	if(!wcsicmp(pwchLocalName, L"MinVelocity"))
		return EFF_MINVEL;

	if(!wcsicmp(pwchLocalName, L"SpawnRadius"))
		return EFF_RADIUS;

	if(!wcsicmp(pwchLocalName, L"AttributeList"))
		return EFF_ATTRLIST;

	if(!wcsicmp(pwchLocalName, L"Attribute"))
		return EFF_ATTRIB;

	if(!wcsicmp(pwchLocalName, L"Keyframes"))
		return EFF_KEYLIST;

	if(!wcsicmp(pwchLocalName, L"Keyframe"))
		return EFF_KEYFRAME;

	return EFF_NONE;
}

/**
* CParticleEffectContentHandler::processAttributes
* @date Modified May 20, 2006
*/
void CParticleEffectContentHandler::processAttributes(EEffectFlags eType, 
	const Attributes& rAttributes)
{
	switch(eType)
	{
	case EFF_SYSTEM:
		break;
	case EFF_EFFECT:
		{
			m_pCurrentEffect = new CParticleEffect;

			// Initialize Effect
			const XMLCh* pStr = NULL;
			if(pStr = rAttributes.getValue(L"Texture"))
				m_pCurrentEffect->m_pTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, CString(pStr));
			if(pStr = rAttributes.getValue(L"SpawnShape"))
				m_pCurrentEffect->m_eSpawnShape = getShape(CString(pStr));
			if(pStr = rAttributes.getValue(L"BlendMode"))
				m_pCurrentEffect->m_eBlendMode = getBlendOp(CString(pStr));
			if(pStr = rAttributes.getValue(L"EffectLength"))
				m_pCurrentEffect->m_fEffectLength = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"MaxParticles"))
				m_pCurrentEffect->m_nMaxParticles = CString(pStr).ToInt();
			if(pStr = rAttributes.getValue(L"SpawnMode"))
				m_pCurrentEffect->m_eSpawnMode = getSpawnMode(CString(pStr));
			if(pStr = rAttributes.getValue(L"Restitution"))
				m_pCurrentEffect->m_fInvRestitution = 1.0f / (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Bounce"))
				m_pCurrentEffect->m_bBounce = CString(pStr).ToBool();
			
			// Save Effect
			CParticleSystem::SEffectInstance oInst;
			memset(&oInst, 0, sizeof(oInst));
			oInst.pEffect = m_pCurrentEffect;
			m_pEmitter->m_vEffects.push_back(oInst);
			CParticleManager::getInstance().addEffect(m_pCurrentEffect);
		}
		break;
	case EFF_MAXVEL:
		if(m_pCurrentEffect)
		{
			const XMLCh* pStr = NULL;
			if(pStr = rAttributes.getValue(L"X"))
				m_pCurrentEffect->m_vMaxVelocity.x = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Y"))
				m_pCurrentEffect->m_vMaxVelocity.y = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Z"))
				m_pCurrentEffect->m_vMaxVelocity.z = (float)CString(pStr).ToFloat();
		}
		break;
	case EFF_MINVEL:
		if(m_pCurrentEffect)
		{
			const XMLCh* pStr = NULL;
			if(pStr = rAttributes.getValue(L"X"))
				m_pCurrentEffect->m_vMinVelocity.x = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Y"))
				m_pCurrentEffect->m_vMinVelocity.y = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Z"))
				m_pCurrentEffect->m_vMinVelocity.z = (float)CString(pStr).ToFloat();
		}
		break;
	case EFF_RADIUS:
		if(m_pCurrentEffect)
		{
			const XMLCh* pStr = NULL;
			if(pStr = rAttributes.getValue(L"X"))
				m_pCurrentEffect->m_vSpawnRadius.x = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Y"))
				m_pCurrentEffect->m_vSpawnRadius.y = (float)CString(pStr).ToFloat();
			if(pStr = rAttributes.getValue(L"Z"))
				m_pCurrentEffect->m_vSpawnRadius.z = (float)CString(pStr).ToFloat();
		}
		break;
	case EFF_ATTRLIST:
		break;
	case EFF_ATTRIB:
		if(m_pCurrentEffect)
		{
			m_pCurrentAttrib = new CParticleAttribute;

			// Initialize Attribute
			const XMLCh* pStr = NULL;
			if(pStr = rAttributes.getValue(L"Type"))
				m_pCurrentAttrib->setType(getAttribType(CString(pStr)));
			if(pStr = rAttributes.getValue(L"InterpMethod"))
				m_pCurrentAttrib->setInterpolationMethod(getInterpMethod(CString(pStr)));
			if(pStr = rAttributes.getValue(L"MinValue"))
				m_pCurrentAttrib->setMinValue(CString(pStr).ToFloat());
			if(pStr = rAttributes.getValue(L"MaxValue"))
				m_pCurrentAttrib->setMaxValue(CString(pStr).ToFloat());
			
			// Save Attribute
			m_pCurrentEffect->addAttribute(m_pCurrentAttrib);
		}
		break;
	case EFF_KEYLIST:
		break;
	case EFF_KEYFRAME:
		if(m_pCurrentAttrib)
		{
			const XMLCh* pStr = NULL, *pStr2 = NULL;
			if((pStr = rAttributes.getValue(L"Value")) && (pStr2 = rAttributes.getValue(L"Time")))
				m_pCurrentAttrib->addKeyframe((float)CString(pStr2).ToFloat(), (float)CString(pStr).ToFloat());
		}
		break;
	}
}

/**
* CParticleEffectContentHandler::getBlendOp
* @date Modified May 21, 2006
*/
CParticleEffect::EBlendOp CParticleEffectContentHandler::getBlendOp(CString& sBlend)
{
	if(sBlend == CString("Replace"))
		return CParticleEffect::OP_REPLACE;

	if(sBlend == CString("Additive"))
		return CParticleEffect::OP_ADDITIVE;

	if(sBlend == CString("Multiply"))
		return CParticleEffect::OP_MULTIPLY;

	return CParticleEffect::OP_MULTIPLY;
}

/**
* CParticleEffectContentHandler::getShape
* @date Modified May 21, 2006
*/
CParticleEffect::EShape CParticleEffectContentHandler::getShape(CString& sShape)
{
	if(sShape == CString("Square"))
		return CParticleEffect::SH_SQUARE;

	if(sShape == CString("Cube"))
		return CParticleEffect::SH_CUBE;

	if(sShape == CString("Sphere"))
		return CParticleEffect::SH_SPHERE;

	if(sShape == CString("Circle"))
		return CParticleEffect::SH_CIRCLE;

	return CParticleEffect::SH_SQUARE;
}

/**
* CParticleEffectContentHandler::getSpawnMode
* @date Modified May 26, 2006
*/
CParticleEffect::ESpawnMode CParticleEffectContentHandler::getSpawnMode(CString& sMode)
{
	if(sMode == CString("Instant"))
		return CParticleEffect::SM_INSTANT;

	if(sMode == CString("Looping"))
		return CParticleEffect::SM_LOOPING;

	return CParticleEffect::SM_LOOPING;
}

/**
* CParticleEffectContentHandler::getAttribType
* @date Modified May 21, 2006
*/
CParticleAttribute::EAttributeType CParticleEffectContentHandler::getAttribType(CString& sType)
{
	if(sType == CString("ColorAlpha"))
		return CParticleAttribute::ATR_COLORALPHA;
	if(sType == CString("ColorRed"))
		return CParticleAttribute::ATR_COLORRED;
	if(sType == CString("ColorGreen"))
		return CParticleAttribute::ATR_COLORGREEN;
	if(sType == CString("ColorBlue"))
		return CParticleAttribute::ATR_COLORBLUE;
	if(sType == CString("Rotation"))
		return CParticleAttribute::ATR_ROTATION;
	if(sType == CString("Size"))
		return CParticleAttribute::ATR_SIZE;
	if(sType == CString("AccelerationX"))
		return CParticleAttribute::ATR_ACCELX;
	if(sType == CString("AccelerationY"))
		return CParticleAttribute::ATR_ACCELY;
	if(sType == CString("AccelerationZ"))
		return CParticleAttribute::ATR_ACCELZ;

	return CParticleAttribute::ATR_SIZE;
}

/**
* CParticleEffectContentHandler::getInterpMethod
* @date Modified May 21, 2006
*/
CParticleAttribute::EInterpolationType CParticleEffectContentHandler::getInterpMethod(CString& sType)
{
	if(sType == CString("Linear"))
		return CParticleAttribute::INTERP_LINEAR;
	if(sType == CString("Cosine"))
		return CParticleAttribute::INTERP_COSINE;
	if(sType == CString("Cubic"))
		return CParticleAttribute::INTERP_CUBIC;
	if(sType == CString("Hermite"))
		return CParticleAttribute::INTERP_HERMITE;

	return CParticleAttribute::INTERP_LINEAR;
}

/**
* CParticleEffectContentHandler::startElement
* @date Modified May 20, 2006
*/
void CParticleEffectContentHandler::startElement(const XMLCh* const szURI, 
	const XMLCh* const szLocalName, const XMLCh* const szQName, 
	const Attributes& rAttributes)
{
	// Get element type
	EEffectFlags eFlags = getElemType(szLocalName);
	m_dwFlags |= eFlags;

	// Process type.
	processAttributes(eFlags, rAttributes);
}

/**
* CParticleEffectContentHandler::endElement
* @date Modified May 20, 2006
*/
void CParticleEffectContentHandler::endElement(const XMLCh* const szURI,
	const XMLCh* const szLocalName, const XMLCh* const szQName)
{
	// Get element type.
	EEffectFlags eFlags = getElemType(szLocalName);
	m_dwFlags &= ~eFlags;

	// Process type.
	switch(eFlags)
	{
	case EFF_SYSTEM:
		break;
	case EFF_EFFECT:
		break;
	case EFF_MAXVEL:
		break;
	case EFF_MINVEL:
		break;
	case EFF_RADIUS:
		break;
	case EFF_ATTRLIST:
		break;
	case EFF_ATTRIB:
		break;
	case EFF_KEYLIST:
		break;
	case EFF_KEYFRAME:
		break;
	}
}