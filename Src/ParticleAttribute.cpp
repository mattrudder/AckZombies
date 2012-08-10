/**
* @file ParticleAttribute.cpp
* @author Matt Rudder
* @date Modified May 20, 2006
*
* Defines a single attribute effecting particle behavior.
*/

// Local includes
#include "ParticleAttribute.h"

// Engine includes
#include "ParticleKeyframe.h"
#include "MethodBinaryFunctor.h"

CParticleKeyframe CParticleAttribute::ms_oBeginFrame;
CParticleKeyframe CParticleAttribute::ms_oEndFrame(1.0f);

/**
* CParticleAttribute::SSortFunc::operator()
* @date Modified May 21, 2006
*/
bool CParticleAttribute::SSortFunc::operator()(CParticleKeyframe* pLeft, CParticleKeyframe* pRight)
{
	return pLeft->getTime() < pRight->getTime();
}

/**
* CParticleAttribute::CParticleAttribute
* @date Modified May 20, 2006
*/
CParticleAttribute::CParticleAttribute(void) : m_fMaxValue(1.0f), m_fMinValue(0.0f)
{
}

/**
* CParticleAttribute::CParticleAttribute
* @date Modified May 26, 2006
*/
CParticleAttribute::CParticleAttribute(const CParticleAttribute& attr) : m_eInterpMethod(attr.m_eInterpMethod),
	m_eType(attr.m_eType), m_fMaxValue(attr.m_fMaxValue), m_fMinValue(attr.m_fMinValue)
{
	// Copy keyframes
	for(size_t i = 0; i < attr.m_vKeyList.size(); ++i)
		m_vKeyList.push_back(new CParticleKeyframe(attr.m_vKeyList[i]->getTime(), attr.m_vKeyList[i]->getValue()));
}

/**
* CParticleAttribute::~CParticleAttribute
* @date Modified May 20, 2006
*/
CParticleAttribute::~CParticleAttribute(void)
{
	for(size_t i = 0; i < m_vKeyList.size(); ++i)
		delete m_vKeyList[i];
	m_vKeyList.clear();
}

/**
* CParticleAttribute::getValue
* @date Modified May 20, 2006
*/
float CParticleAttribute::getValue(float fTime)
{
	CParticleKeyframe* pLeft, *pRight;

	if(m_vKeyList.empty())
	{
		// No items; Default keyframes.
		pLeft = &ms_oBeginFrame;
		pRight = &ms_oEndFrame;
	}
	else
	{
		// Find where the frame time lands in the timeline.
		size_t i = 0;
		for(i = 0; i < m_vKeyList.size(); ++i)
		{
			if(m_vKeyList[i]->getTime() >= fTime)
				break;
		}

		// Are we at the beginning of the list?
		if(i == 0)
			pLeft = &ms_oBeginFrame;
		else
			pLeft = (CParticleKeyframe *)m_vKeyList[i-1];

		// Are we at the end of the list?
		if(i >= m_vKeyList.size())
		{
			pRight = &ms_oEndFrame;
		}
		else
			pRight = (CParticleKeyframe *)m_vKeyList[i];
	}

	float fVal = 0.0f;
	if(pRight->getTime() - pLeft->getTime() > 0.0f)
	{
		float fRelTime = (fTime - pLeft->getTime()) / (pRight->getTime() - pLeft->getTime());
		fVal = pRight->getValue(m_eInterpMethod, pLeft, fRelTime);
	}
	else
	{
		fVal = pRight->getValue(m_eInterpMethod, pLeft, 0.0f);
	}
	return (m_fMaxValue - m_fMinValue) * fVal + (float)m_fMinValue;
}


/**
* CParticleAttribute::addKeyframe
* @date Modified May 20, 2006
*/
void CParticleAttribute::addKeyframe(float fTime, float fValue)
{
	m_vKeyList.push_back(new CParticleKeyframe(fTime, fValue));
	//SSortFunc sortFunc;
	//std::stable_sort(m_vKeyList.begin(), m_vKeyList.end(), sortFunc);
}