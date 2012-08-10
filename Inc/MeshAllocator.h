/**
* @file MeshAllocator.h
* @author Matt Rudder
* @date Created Apr 02, 2006
*
* Class used by DirectX to allocate mesh skin information.
*/

#ifndef _MESHALLOCATOR_H_
#define _MESHALLOCATOR_H_

// System includes
#include <d3dx9.h>

class CMaterial;

/**
* Class used by DirectX to allocate mesh skin information.
*
* @author Matt Rudder
* @date Created Apr 02, 2006
* @date Modified Apr 02, 2006
*/
class CMeshAllocator : public ID3DXAllocateHierarchy
{
public:

	struct SMeshContainer : public D3DXMESHCONTAINER
	{
		CMaterial* pMaterials;
		LPD3DXMESH pMesh;
		bool bSkinned;
	};

	STDMETHOD(CreateFrame)(LPCSTR Name,	LPD3DXFRAME * ppNewFrame) { return S_OK; }
	STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree) { return S_OK; }
	STDMETHOD(CreateMeshContainer)(
		LPCSTR Name,
		CONST D3DXMESHDATA * pMeshData,
		CONST D3DXMATERIAL * pMaterials,
		CONST D3DXEFFECTINSTANCE * pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD * pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER * ppNewMeshContainer
		);
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerToFree);
};

#endif //_MESHALLOCATOR_H_