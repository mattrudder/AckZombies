/**
* @file SpotLight.cpp
* @author Matt Rudder
* @date Modified Jun 02, 2006
*
* Defines a spotlight in the game world.
*/

// Local includes
#include "Spotlight.h"

// Engine includes
#include "ObjectTypes.h"
#include "ResourceManager.h"

/**
* CSpotLight::CSpotLight
* @date Modified Jun 02, 2006
*/
CSpotLight::CSpotLight(void) : CLight(OBJ_LIGHT_SPOT)
{
}

/**
* CSpotLight::~CSpotLight
* @date Modified Jun 02, 2006
*/
CSpotLight::~CSpotLight(void)
{
}