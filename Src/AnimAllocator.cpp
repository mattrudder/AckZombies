/**
* @file AnimAllocator.h
* @author Matt Rudder
* @author Jason Williams
* @date Created May 15, 2006
*
* Defines all interfaces required for loading .x format meshes using 
* D3DXLoadMeshHierarchyFromFile.
*/

// Local includes
#include "AnimAllocator.h"
#include "RenderSystem.h"
#include "Material.h"

/**
* CAnimAllocator::CAnimAllocator
* @date Modified May 16, 2006
*/
CAnimAllocator::CAnimAllocator(CAnimatedMesh* pMesh) : m_pAnimMesh(pMesh)
{
}

/**
* CAnimAllocator::CreateFrame
* @date Modified May 16, 2006
*/
HRESULT CAnimAllocator::CreateFrame(THIS_ LPCSTR szName, LPD3DXFRAME *ppNewFrame)
{
	*ppNewFrame = NULL;

	SMeshFrame* pNewFrame = new SMeshFrame;
	memset(pNewFrame, 0, sizeof(SMeshFrame));

	pNewFrame->ulRefCount = 1;

	// Copy name.
	size_t nLen = 0;
	if(szName)
	{
		nLen = strlen(szName)+1;
		pNewFrame->Name = new char[nLen];
		memcpy(pNewFrame->Name, szName, nLen);
	}
	else
		pNewFrame->Name = NULL;

	// Init matrices
	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pNewFrame->m_mCombinedWorld);
	
	// Return pointer and success.
	*ppNewFrame = pNewFrame;
	return S_OK;
}

/**
* CAnimAllocator::CreateMeshContainer
* @date Modified May 16, 2006
*/
HRESULT CAnimAllocator::CreateMeshContainer(THIS_ LPCSTR szName, CONST D3DXMESHDATA * pMeshData, 
	CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
	DWORD dwNumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	SMeshContainer* pNewMesh = new SMeshContainer;
	memset(pNewMesh, 0, sizeof(SMeshContainer));

	*ppNewMeshContainer = NULL;

	// Copy mesh name
	size_t nLen = 0;
	if(szName)
	{
		nLen = strlen(szName)+1;
		pNewMesh->Name = new char[nLen];
		memcpy(pNewMesh->Name, szName, nLen);
	}
	else
		pNewMesh->Name = NULL;

	// We only support plain meshs.
	if(pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		DestroyMeshContainer(pNewMesh);
		return E_FAIL;
	}
	pNewMesh->MeshData.Type = D3DXMESHTYPE_MESH;

	// Copy adjacencies.
	DWORD dwFaceCount = pMeshData->pMesh->GetNumFaces();
	pNewMesh->pAdjacency = new DWORD[dwFaceCount*3];
	memcpy(pNewMesh->pAdjacency, pAdjacency, sizeof(DWORD) * dwFaceCount * 3);

	// Get mesh's vertex declaration and clone mesh.
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	D3DVERTEXELEMENT9 oDecl[MAX_FVF_DECL_SIZE];
	if(FAILED(pMeshData->pMesh->GetDeclaration(oDecl)))
		return E_FAIL;

	pMeshData->pMesh->CloneMesh(D3DXMESH_MANAGED, oDecl, pDev, &pNewMesh->MeshData.pMesh);

	// Process materials
	
	pNewMesh->NumMaterials = max(dwNumMaterials, 1);
	if(dwNumMaterials > 0)
	{
		for(DWORD i = 0; i < dwNumMaterials; ++i)
			pNewMesh->m_vMaterials.push_back(new CMaterial((LPD3DXMATERIAL)&pMaterials[i], (LPD3DXEFFECTINSTANCE)&pEffectInstances[i]));
	}
	else
	{
		// Default material
		pNewMesh->m_vMaterials.push_back(new CMaterial);
	}

	// Process skin info, if any.
	if(pSkinInfo)
	{
		// Save for later.
		pNewMesh->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pNewMesh->m_pSkinMesh = pMeshData->pMesh;
		pMeshData->pMesh->AddRef();

		// Create bone matrices
		UINT unNumBones = pSkinInfo->GetNumBones();
		pNewMesh->m_pBoneOffsets = new D3DXMATRIX[unNumBones];
		pNewMesh->m_ppFrameMatrices = new D3DXMATRIX*[unNumBones];

		// Get bone offsets for later
		for(UINT i = 0; i < unNumBones; ++i)
			pNewMesh->m_pBoneOffsets[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));

		// TODO: Generate hardware version of mesh info.
		HRESULT hr;
		hr = GenerateSkinnedMesh( pNewMesh);
		if( FAILED(hr))
			return hr;
	}
	else
	{
		pNewMesh->pSkinInfo = NULL;
		pNewMesh->m_pBoneOffsets = NULL;
		pNewMesh->m_pSkinMesh = NULL;
		pNewMesh->m_ppFrameMatrices = NULL;
	}

	// Return pointer and success.
	*ppNewMeshContainer = pNewMesh;
	return S_OK;
}

/**
* CAnimAllocator::DestroyFrame
* @date Modified May 16, 2006
*/
HRESULT CAnimAllocator::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	SMeshFrame* pFrame = (SMeshFrame*)pFrameToFree;

	if(pFrame)
	{
		if(pFrame->Name)
			delete [] pFrame->Name;

		delete pFrame;
	}

	return S_OK;
}

/**
* CAnimAllocator::DestroyMeshContainer
* @date Modified May 16, 2006
*/
HRESULT CAnimAllocator::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	SMeshContainer* pCont = (SMeshContainer*)pMeshContainerToFree;

	if(pCont)
	{
		if(pCont->Name)
			delete [] pCont->Name;

		size_t nNumMaterials = pCont->m_vMaterials.size();
		for(size_t i = 0; i < nNumMaterials; ++i)
			delete pCont->m_vMaterials[i];
		
		if(pCont->pAdjacency)
			delete [] pCont->pAdjacency;

		if(pCont->m_pBoneOffsets)
			delete [] pCont->m_pBoneOffsets;

		if(pCont->m_ppFrameMatrices)
			delete [] pCont->m_ppFrameMatrices;

		COM_RELEASE(pCont->m_pSkinMesh);
		COM_RELEASE(pCont->MeshData.pMesh);
		COM_RELEASE(pCont->pSkinInfo);

		delete pCont;
	}

	return S_OK;
}

/**
/* CAnimAllocator::GenerateSkinnedMesh
/* @date Modified May 28, 2006
*/
HRESULT CAnimAllocator::GenerateSkinnedMesh(SMeshContainer * pMeshContainer)
{
	HRESULT hr = S_OK;
	//! Get a pointer to the D3D Device
	LPDIRECT3DDEVICE9 pDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	D3DCAPS9 d3dCaps;
	//! Get information about the device capabilities
	pDevice->GetDeviceCaps( &d3dCaps);

	//! If the pSkinInfo is NULL there is no need to generate a HW friendly skinned mesh.
	if( pMeshContainer->pSkinInfo == NULL)
		return hr;

	COM_RELEASE( pMeshContainer->MeshData.pMesh);
	COM_RELEASE( pMeshContainer->m_pBoneCombinationBuffer);
	


#if SKIN_VS == 0
	//!  Generate a NonIndexed Blended Mesh
	hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh( pMeshContainer->m_pSkinMesh,
		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
		pMeshContainer->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainer->m_dwNumInfl,
		&pMeshContainer->m_dwNumAttributeGroups,
		&pMeshContainer->m_pBoneCombinationBuffer,
		&pMeshContainer->MeshData.pMesh);

	//! Return if the conversion failed
	if( FAILED( hr))
		return hr;

	//! Check to see how many blends the device is capable of doing
	LPD3DXBONECOMBINATION rgBoneCombinations = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->m_pBoneCombinationBuffer->GetBufferPointer());
	//! Look for bones that do not fit the devices matrix capacity.
	for( pMeshContainer->m_dwAttributeSoftware = 0; pMeshContainer->m_dwAttributeSoftware < pMeshContainer->m_dwNumAttributeGroups; pMeshContainer->m_dwAttributeSoftware++)
	{
		DWORD dwInflCounter = 0;
		for( DWORD dwInfl = 0; dwInfl < pMeshContainer->m_dwNumInfl; dwInfl++)
		{
			if( rgBoneCombinations[pMeshContainer->m_dwAttributeSoftware].BoneId[dwInfl] != UINT_MAX)
				dwInflCounter++;
		}

		if( dwInflCounter > d3dCaps.MaxVertexBlendMatrices)
			break;
	}

	//! If there needs to be HW and SW Vertex Processing, add the SW Flag
	if( pMeshContainer->m_dwAttributeSoftware < pMeshContainer->m_dwNumAttributeGroups)
	{
		LPD3DXMESH pMeshTemp;

		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF( D3DXMESH_SOFTWAREPROCESSING | pMeshContainer->MeshData.pMesh->GetOptions(),
			pMeshContainer->MeshData.pMesh->GetFVF(), pDevice, &pMeshTemp);

		if( FAILED(hr))
			return hr;

		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = pMeshTemp;
		pMeshTemp = NULL;
	}	
#else

	// Get palette size
	// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
	// (96 - 9) /3 i.e. Maximum constant count - used constants 
	UINT unMaxMatrices = 26; 
	pMeshContainer->m_dwNumPaletteEntries = min(unMaxMatrices , pMeshContainer->pSkinInfo->GetNumBones());

	DWORD dwFlags = D3DXMESHOPT_VERTEXCACHE;
	if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
	{
		pMeshContainer->m_bSoftwareVP = false;
		dwFlags |= D3DXMESH_MANAGED;
	}
	else
	{
		pMeshContainer->m_bSoftwareVP = true;
		CAnimatedMesh::ms_bUseSoftwareVP = true;
		dwFlags |= D3DXMESH_SYSTEMMEM;
	}

	COM_RELEASE(pMeshContainer->MeshData.pMesh);

	hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(
		pMeshContainer->m_pSkinMesh,
		dwFlags, 
		pMeshContainer->m_dwNumPaletteEntries, 
		pMeshContainer->pAdjacency, 
		NULL, NULL, NULL,             
		&pMeshContainer->m_dwNumInfl,
		&pMeshContainer->m_dwNumAttributeGroups,
		&pMeshContainer->m_pBoneCombinationBuffer,
		&pMeshContainer->MeshData.pMesh);
	if (FAILED(hr))
		return hr;


	// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	LPD3DXMESH pMesh;
	hr = pMeshContainer->MeshData.pMesh->CloneMesh(pMeshContainer->MeshData.pMesh->GetOptions(), CAnimatedMesh::VertexElem, pDevice, &pMesh);
	COM_RELEASE(pMeshContainer->MeshData.pMesh);
	pMeshContainer->MeshData.pMesh = pMesh;
	pMesh = NULL;

	D3DXComputeTangentFrame(pMeshContainer->MeshData.pMesh, NULL);

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	if( CAnimatedMesh::ms_dwNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
	{
		CAnimatedMesh::ms_dwNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		// Allocate space for blend matrices
		delete [] CAnimatedMesh::ms_pBoneMatrices; 
		CAnimatedMesh::ms_pBoneMatrices  = new D3DXMATRIXA16[CAnimatedMesh::ms_dwNumBoneMatricesMax];
		if(CAnimatedMesh::ms_pBoneMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			return hr;
		}
	}

#endif
	return hr;
}
