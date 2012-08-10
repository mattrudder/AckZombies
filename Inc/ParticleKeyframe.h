/**
* @file ParticleKeyframe.h
* @author Matt Rudder
* @date Created May 20, 2006
*
* Defines a keyframe in the behavior of a particle. This keyframe is generic to 
* work with any values.
*/

#ifndef _PARTICLEKEYFRAME_H_
#define _PARTICLEKEYFRAME_H_

#include "ParticleAttribute.h"

/**
* Defines a keyframe in the behavior of a particle. This keyframe is generic to 
* work with any values.
*
* @author Matt Rudder
* @date Created May 20, 2006
* @date Modified May 20, 2006
*/
class CParticleKeyframe
{
	friend class CParticleEffectContentHandler;
protected:
	float m_fValue;
	float m_fTime;
public:
	CParticleKeyframe(float fTime = 0.0f, float fValue = 0.0f);
	virtual ~CParticleKeyframe(void);

	float getValue(void) { return m_fValue; }
	float getTime(void) { return m_fTime; }
	void setValue(float fValue) { m_fValue = fValue; }
	void setTime(float fTime) { m_fTime = fTime; }

	/**
	* Retrieves the interpolated attribute value of a keyframe, given the 
	* keyframe to the right on the timeline, and the distance between the
	* two keyframes.
	*
	* @date Created May 20, 2006
	* @param[in]	pRightFrame		The keyframe on the right.
	* @param[in]	fScale			The distance in time between this frame and the 
	*								frame on the right, in a range from 0.0 to 1.0.
	*/
	float getValue(CParticleAttribute::EInterpolationType eInterp, CParticleKeyframe* pRightFrame, float fScale);
};

#endif //_PARTICLEKEYFRAME_H_