/**
* @file ParticleKeyframe.cpp
* @author Matt Rudder
* @date Modified May 20, 2006
*
* Defines a keyframe in the behavior of a particle. This keyframe is generic to 
* work with any values.
*/

// Local include
#include "ParticleKeyframe.h"

/**
* CParticleKeyframe::CParticleKeyframe
* @date Modified May 20, 2006
*/
CParticleKeyframe::CParticleKeyframe(float fTime, float fValue) : 
	m_fTime(fTime), m_fValue(fValue) { }

/**
* CParticleKeyframe::~CParticleKeyframe
* @date Modified May 20, 2006
*/
CParticleKeyframe::~CParticleKeyframe(void)
{
}

/**
* CParticleKeyframe::getValue
* @date Modified May 20, 2006
*/
float CParticleKeyframe::getValue(CParticleAttribute::EInterpolationType eInterp, CParticleKeyframe* pRightFrame, float fScale)
{
	float fScale2;
	switch(eInterp)
	{
	case CParticleAttribute::INTERP_LINEAR:
		return (m_fValue * fScale) + ((1.0f - fScale) * pRightFrame->m_fValue);
	case CParticleAttribute::INTERP_COSINE:
		fScale2 = (float)(1.0f - cos(fScale * D3DX_PI)) / 2.0f;
		return m_fValue * fScale2 + ((1.0f - fScale2) * pRightFrame->m_fValue);
	//case CParticleAttribute::INTERP_CUBIC:
	//	break;
	//case CParticleAttribute::INTERP_HERMITE:
	//	break;
	default:
		// TODO: Cubic/Hermite interpolation; Do Cosine for now.
		fScale2 = (float)(1.0f - cos(fScale * D3DX_PI)) / 2.0f;
		return m_fValue * fScale2 + ((1.0f - fScale2) * pRightFrame->m_fValue);
	}
}