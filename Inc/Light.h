/**
* @file Light.h
* @author Matt Rudder
* @date Created Jun 07, 2006
*
* Base class for all lights in the scene.
*/

#ifndef _LIGHT_H_
#define _LIGHT_H_

// Engine includes
#include "Actor.h"


/**
* Base class for all lights in the scene.
*
* @author Matt Rudder
* @date Created Jun 07, 2006
* @date Modified Jun 07, 2006
*/
class CLight : public CActor
{
public:
	CLight(EObjectType eType = OBJ_LIGHT);
	virtual ~CLight(void);
};

#endif //_LIGHT_H_