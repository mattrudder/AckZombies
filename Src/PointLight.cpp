/**
* @file PointLight.cpp
* @author Matt Rudder
* @date Modified Jun 07, 2006
*
* Light in the world that emits light in all directions.
*/

// Local includes
#include "PointLight.h"

/**
* CPointLight::CPointLight
* @date Modified Jun 07, 2006
*/
CPointLight::CPointLight(void) : CLight(OBJ_LIGHT_POINT), m_fRadius(1.0f)
{
}

/**
* CPointLight::~CPointLight
* @date Modified Jun 07, 2006
*/
CPointLight::~CPointLight(void)
{
}
