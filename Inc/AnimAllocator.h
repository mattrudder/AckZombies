/**
* @file AnimAllocator.h
* @author Matt Rudder
* @date Created May 15, 2006
*
* Defines all interfaces required for loading .x format meshes using 
* D3DXLoadMeshHierarchyFromFile.
*/

#ifndef _ANIMALLOCATOR_H_
#define _ANIMALLOCATOR_H_

// Local includes
#include "AnimatedMesh.h"

// System includes
#include <vector>
using std::vector;

class CMaterial;
struct SMeshContainer : public D3DXMESHCONTAINER 
{
	//! Material collection
	vector<CMaterial*> m_vMaterials;
	//! Skin mesh
	LPD3DXMESH m_pSkinMesh;
	//! Bone matrix offsets
	LPD3DXMATRIX m_pBoneOffsets;
	//! Array of frame matrices
	LPD3DXMATRIX* m_ppFrameMatrices;
	//! Bone Combination Buffer
	LPD3DXBUFFER m_pBoneCombinationBuffer;
	//! # of Attribute Groups
	DWORD m_dwNumAttributeGroups;
	//! # of Influences
	DWORD m_dwNumInfl;
	//!  Split Attribute groups for SW or HW rendering
	DWORD m_dwAttributeSoftware;
};

struct SMeshFrame : public D3DXFRAME
{
	D3DXMATRIX m_mCombinedWorld;
	unsigned long ulRefCount;
};

/**
* Defines all interfaces required for loading .x format meshes using 
* D3DXLoadMeshHierarchyFromFile.
*
* @author Matt Rudder
* @date Created May 15, 2006
* @date Modified May 15, 2006
*/
class CAnimAllocator : public ID3DXAllocateHierarchy
{
	CAnimatedMesh* m_pAnimMesh;
public:
	/**
	* Creates a D3DXFRAME derived object, and initializes it.
	*
	* @date Created May 15, 2006
	* @param[in]	szName		Name of the frame.
	* @param[out]	ppNewFrame	New D3DXFRAME instance.
	* @return Result handle
	*/
	STDMETHOD(CreateFrame)(THIS_ LPCSTR szName, LPD3DXFRAME *ppNewFrame);

	/**
	* Creates a D3DXMESHCONTAINER derived object, and initializes it.
	*
	* @date Created May 15, 2006
	* @param[in]	szName				Name of the container.
	* @param[in]	pMeshData			Mesh data structure. (see D3DXMESHDATA)
	* @param[in]	pMaterials			Material/Texture array.
	* @param[in]	pEffectInstances	Shader (.fx) array.
	* @param[in]	dwNumMaterials		Number of items in pMaterials and pEffectInstances.
	* @param[in]	pAdjacency			Adjacency array.
	* @param[in]	pSkinInfo			Skin data, if found. (see ID3DXSkinInfo)
	* @param[out]	ppNewMeshContainer	Output mesh container.
	* @return Result handle.
	*/
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR szName, CONST D3DXMESHDATA * pMeshData, 
		CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
		DWORD dwNumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER * ppNewMeshContainer);

	/**
	* Destroys a previously created D3DXFRAME derived object.
	*
	* @date Created May 15, 2006
	* @param[in]	pFrameToFree	The frame to destroy.
	* @return Result handle
	*/
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);

	/**
	* Destroys a previously created D3DXMESHCONTAINER derived object.
	*
	* @date Created May 15, 2006
	* @param[in]	pMeshContainerToFree	The container to destroy.
	* @return Result handle
	*/
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

	/**
	* Constructor
	*
	* @date Created May 15, 2006
	* @param[in]	pMesh	The parent mesh; Used for during allocation.
	*/
	CAnimAllocator(CAnimatedMesh* pMesh);

	/**
	* Creates a HW friendly version of the Skinned Mesh
	* 
	* @date Created May 28, 2006
	* @param[in] pMeshContainer
	* @return Result handle
	*/
	HRESULT GenerateSkinnedMesh( SMeshContainer * pMeshContainer);

};

#endif //_ANIMALLOCATOR_H_
