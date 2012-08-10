/**
* @file ParticleAttribute.h
* @author Matt Rudder
* @date Created May 20, 2006
*
* Defines a single attribute effecting particle behavior.
*/

#ifndef _PARTICLEATTRIBUTE_H_
#define _PARTICLEATTRIBUTE_H_

// System includes
#include <vector>
#include <map>
using std::vector;
using std::map;

// Engine includes
#include "ParticleSystem.h"

class CParticleKeyframe;

/**
* Defines a single attribute effecting particle behavior.
*
* @author Matt Rudder
* @date Created May 20, 2006
* @date Modified May 20, 2006
*/
class CParticleAttribute
{
	friend class CParticleEffectContentHandler;
public:
	enum EAttributeType
	{
		ATR_COLORRED, ATR_COLORGREEN, ATR_COLORBLUE, ATR_COLORALPHA,
		ATR_SIZE,
		ATR_ROTATION,
		ATR_ACCELX, ATR_ACCELY, ATR_ACCELZ,
	};

	enum EInterpolationType
	{
		INTERP_LINEAR,
		INTERP_COSINE,
		INTERP_CUBIC,
		INTERP_HERMITE,
	};
protected:
	EInterpolationType m_eInterpMethod;
	EAttributeType m_eType;
	vector<CParticleKeyframe*> m_vKeyList;
	float m_fMaxValue;
	float m_fMinValue;

	static CParticleKeyframe ms_oBeginFrame;
	static CParticleKeyframe ms_oEndFrame;
	struct SSortFunc
	{
		bool operator()(CParticleKeyframe* pLeft, CParticleKeyframe* pRight);
	};
public:
	CParticleAttribute(void);
	CParticleAttribute(const CParticleAttribute&);
	virtual ~CParticleAttribute(void);

	EInterpolationType getInterpolationMethod(void) { return m_eInterpMethod; }
	EAttributeType getType(void) { return m_eType; }
	void setInterpolationMethod(EInterpolationType eInterpMethod) { m_eInterpMethod = eInterpMethod; }
	void setType(EAttributeType eType) { m_eType = eType; }
	void setMinValue(float fValue) { m_fMinValue = fValue; }
	void setMaxValue(float fValue) { m_fMaxValue = fValue; }

	float getValue(float fTime);
	void addKeyframe(float fTime, float fValue);
};

#endif //_PARTICLEATTRIBUTE_H_