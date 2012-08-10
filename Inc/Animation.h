#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "d3d9.h"
#include "d3dx9.h"
#include "d3dx9anim.h"
#include "D3dx9effect.h"

class CMaterial;

//! D3DXFRAME DERIVATION
struct D3DXFRAME_ANIM : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};

//! D3DXMESHCONTAINER DERIVATION
struct D3DXMESHCONTAINER_ANIM : public D3DXMESHCONTAINER
{
	// (MR): Changed Animated mesh to load materials instead of just textures
	//LPDIRECT3DTEXTURE9 * ppTextures;
	CMaterial** ppMaterials;
	DWORD dwMatCount;


	//! Skinned Information Variables
	LPD3DXMESH								pOrigMesh;
	LPD3DXATTRIBUTERANGE		pAttributeTable;
	DWORD											NumAttributeGroups;
	DWORD											NumInfl;
	LPD3DXBUFFER						pBoneCombinationBuffer;
	D3DXMATRIX **							ppBoneMatrixPointers;
	D3DXMATRIX *							pBoneOffsetMatrices;
	DWORD											NumPaletteEntries;

};

#define ANIM_INDEX_FAIL 0xffffffff
#define MOVE_TRANSITION_TIME 0.25f
#define IDLE_TRANSITION_TIME 0.125f
#endif //_ANIMATION_H_