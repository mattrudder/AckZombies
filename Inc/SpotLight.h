/**
* @file SpotLight.h
* @author Matt Rudder
* @date Created Jun 02, 2006
*
* Defines a spotlight in the game world.
*/

#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

// Local includes
#include "Light.h"

/**
* Defines a spotlight in the game world.
*
* @author Matt Rudder
* @date Created Jun 02, 2006
* @date Modified Jun 02, 2006
*/
class CSpotLight : public CLight
{
protected:
	//! Projection Matrix
	D3DXMATRIX m_mProj;
public:
	CSpotLight(void);
	virtual ~CSpotLight(void);

	static CObject* create(void) { return new CSpotLight; }
};

#endif //_SPOTLIGHT_H_