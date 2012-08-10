/**
* @file PointLight.h
* @author Matt Rudder
* @date Created Jun 07, 2006
*
* Light in the world that emits light in all directions.
*/

#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

// Local includes
#include "Light.h"

/**
* Light in the world that emits light in all directions.
*
* @author Matt Rudder
* @date Created Jun 07, 2006
* @date Modified Jun 07, 2006
*/
class CPointLight : public CLight
{
protected:
	float m_fRadius;
public:
	CPointLight(void);
	virtual ~CPointLight(void);

	float getRadius(void) { return m_fRadius; }
	void setRadius(float fRadius) { m_fRadius = fRadius; }

	static CObject* create(void) { return new CPointLight; }
};

#endif //_POINTLIGHT_H_