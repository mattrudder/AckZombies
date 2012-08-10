/**
* @file ResourceTypes.h
* @author Matt Rudder
* @date Created Mar 30, 2006
*
* Listing of all supported engine file resource types.
*/

#ifndef _RESOURCETYPES_H_
#define _RESOURCETYPES_H_

enum EResourceType
{
	RES_TEXTURE2D,
	RES_TEXTURE3D,
	RES_TEXTURECUBE,
	RES_FONT,
	RES_MESH,
	RES_ANIMATEDMESH,
	RES_SOUND,
	RES_PARTICLEEMITTER,
	RES_MAX
};

// Includes for all above types
#include "Texture2D.h"
#include "Mesh.h"
#include "AnimatedMesh.h"
#include "BitmapFont.h"
#include "sound.h"
#include "ParticleSystem.h"


#endif //_RESOURCETYPES_H_