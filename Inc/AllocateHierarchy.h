#ifndef _ALLOCATE_HIERARCHY_H_
#define _ALLOCATE_HIERARCHY_H_

//! Animation Includes
#include "Animation.h"
#include "AnimatedMesh.h"

//! Normal DirectX Includes
#include "d3d9.h"
#include "d3dx9.h"

//! Included to overload the ID3DXAllocateHierarchy Class
#include "d3dx9anim.h"

class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME * ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData,
																		CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
																		DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo,
																		LPD3DXMESHCONTAINER * ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

	CAllocateHierarchy();
	~CAllocateHierarchy();

	void SetAnimatedMeshPointer( CAnimatedMesh * poAnimatedMesh);
private:
	HRESULT AllocateName( LPCSTR Name, LPSTR * pNewName);
	HRESULT GenerateSkinnedMesh( IDirect3DDevice9 * pD3DDevice, D3DXMESHCONTAINER_ANIM * pMeshContainer);

	CAnimatedMesh * m_poAnimatedMesh;
};

#endif //_ALLOCATE_HIERARCHY_H_


