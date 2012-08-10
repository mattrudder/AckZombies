#include "AllocateHierarchy.h"
#include "Globals.h"
#include <stdio.h>
#include "Material.h"

CAllocateHierarchy::CAllocateHierarchy()
{
}

CAllocateHierarchy::~CAllocateHierarchy()
{
}

HRESULT CAllocateHierarchy::AllocateName( LPCSTR Name, LPSTR * pNewName)
{
	unsigned int uiByteLength;

	if( Name != NULL)
	{
		uiByteLength = (unsigned int)strlen(Name) + 1;
		*pNewName = new char[uiByteLength];

		if( *pNewName == NULL)
			return E_OUTOFMEMORY;
		
		memcpy( *pNewName, Name, uiByteLength*sizeof(char));
	}
	else
		*pNewName = NULL;
    
	return S_OK;
}

HRESULT CAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{	
	D3DXFRAME_ANIM * pFrame;
	*ppNewFrame = NULL;
	pFrame = new D3DXFRAME_ANIM;
	
	if(!pFrame)
		return E_OUTOFMEMORY;

	if( FAILED( AllocateName( Name, &pFrame->Name)))
	{
		delete pFrame;
		return E_OUTOFMEMORY;
	}

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	delete pFrame;
	return S_OK;
}

HRESULT CAllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData,
																								CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
																								DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo,
																								LPD3DXMESHCONTAINER * ppNewMeshContainer)
{

	HRESULT hr;
	D3DXMESHCONTAINER_ANIM * pMeshContainer = NULL;
	unsigned int uiNumFaces;
	unsigned int uiMaterialIndex;
	unsigned int uiBoneIndex;
	unsigned int uiNumBones;

	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	LPD3DXMESH pMesh = NULL;
	*ppNewMeshContainer = NULL;

	if( pMeshData->Type != D3DXMESHTYPE_MESH)
		return E_FAIL;

	//if( pMesh->GetFVF() == 0)
	//	return E_FAIL;

	pMeshContainer = new D3DXMESHCONTAINER_ANIM;

	if(!pMeshContainer)
		return E_OUTOFMEMORY;

	memset( pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_ANIM));

	if( FAILED( AllocateName( Name, &pMeshContainer->Name)))
		return E_OUTOFMEMORY;

	pMesh = pMeshData->pMesh;
	pMesh->GetDevice( &pD3DDevice);
	uiNumFaces = pMesh->GetNumFaces();

	pMesh->CloneMesh(0, CMesh::VertexElem, pD3DDevice, &pMeshContainer->MeshData.pMesh);
	D3DXComputeTangent(pMeshContainer->MeshData.pMesh, 0, 0, 0, 1, (const DWORD*)pAdjacency);

	//! Check to ensure normals are in the mesh. If they are not present generate them.
	//if( pMesh->GetFVF() & D3DFVF_NORMAL)
	//{
	//	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//	//! Clone Mesh to add in Normals
	//	hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, pD3DDevice, &pMeshContainer->MeshData.pMesh);
	//	
	//	if( FAILED( hr))
	//	{
	//		//! TODO : Add in HR handler here.
	//		return E_FAIL;
	//	}

	//	pMesh = pMeshContainer->MeshData.pMesh;
	//	//! Generate Normals
	//	D3DXComputeNormals( pMesh, NULL);
	//}
	//else
	//{
	//	pMeshContainer->MeshData.pMesh = pMesh;
	//	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//	pMesh->AddRef();
	//}

	pMeshContainer->NumMaterials = max( 1, NumMaterials);
	pMeshContainer->ppMaterials = new CMaterial*[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[ uiNumFaces * 3];

	if( (!pMeshContainer->pAdjacency) || (!pMeshContainer->ppMaterials))
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}

	memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * uiNumFaces * 3);
	memset( pMeshContainer->ppMaterials, 0, sizeof(CMaterial*) * pMeshContainer->NumMaterials);
	for( uiMaterialIndex = 0; uiMaterialIndex < pMeshContainer->NumMaterials; uiMaterialIndex++)
	{
		// (MR): Changed from texture loading to material loading.
		pMeshContainer->ppMaterials[uiMaterialIndex] = new CMaterial((const LPD3DXMATERIAL)&pMaterials[uiMaterialIndex], (const LPD3DXEFFECTINSTANCE)&pEffectInstances[uiMaterialIndex]);
	}

	//! Save Skinning Information
	if( pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		//! Get Array of Offset Matrices to transform vertices from figure space to bone space.
		uiNumBones = pSkinInfo->GetNumBones();
		//! Allocate Memory for the Bone Matrices.
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[uiNumBones];
		//! If memory allocation failed then return.
		if( pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			DestroyMeshContainer(pMeshContainer);
			return E_OUTOFMEMORY;
		}

		//! Acquire the Offset Matrices.
		for( uiBoneIndex = 0; uiBoneIndex < uiNumBones; uiBoneIndex++)
		{
			//! Get the Offset Matrix from the Skin. Save it.
			pMeshContainer->pBoneOffsetMatrices[uiBoneIndex] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(uiBoneIndex));
		}

		//! Convert Skinning Information to HW friendly version.
		//! Call hr = GenerateSkinnedMesh( pD3DDevice, pMeshContainer) returns HRESULT. Exit if FAILED.
		hr = GenerateSkinnedMesh( pD3DDevice, pMeshContainer);
		if( FAILED( hr))
		{
			return E_FAIL;
		}
	}// pSkinInfo(END)

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

	//! Release the DX Device.
	COM_RELEASE(pD3DDevice);
	
	return S_OK;
}

HRESULT CAllocateHierarchy::DestroyMeshContainer( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{

	unsigned int iMaterial;																										 //! Counter for Materials (Textures)
	D3DXMESHCONTAINER_ANIM * pMeshContainer;													 //! Mesh Container to cycle through and clear out.
	pMeshContainer = (D3DXMESHCONTAINER_ANIM *)pMeshContainerToFree;	 //! Assign the Mesh Container to Free.

	//! Delete Arrays within the Mesh Container
	if( pMeshContainer->Name)
	{
		delete [] pMeshContainer->Name;
		pMeshContainer->Name = NULL;
	}

	if( pMeshContainer->pAdjacency)
	{
		delete [] pMeshContainer->pAdjacency;
		pMeshContainer->pAdjacency = NULL;
	}

	if( pMeshContainer->ppMaterials)
	{
		for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			delete pMeshContainer->ppMaterials[iMaterial];
		}
		delete [] pMeshContainer->pMaterials;
		pMeshContainer->ppMaterials = NULL;
	}

	if( pMeshContainer->pBoneOffsetMatrices)
	{
		delete [] pMeshContainer->pBoneOffsetMatrices;
		pMeshContainer->pBoneOffsetMatrices = NULL;
	}

	if( pMeshContainer->ppBoneMatrixPointers)
	{
		delete [] pMeshContainer->ppBoneMatrixPointers;
		pMeshContainer->ppBoneMatrixPointers = NULL;
	}

	if( pMeshContainer->pBoneCombinationBuffer)
	{
		pMeshContainer->pBoneCombinationBuffer->Release();
		pMeshContainer->pBoneCombinationBuffer = NULL;
	}

	if( pMeshContainer->MeshData.pMesh)
	{
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = NULL;
	}

	if( pMeshContainer->pSkinInfo)
	{
		pMeshContainer->pSkinInfo->Release();
		pMeshContainer->pSkinInfo = NULL;

	}

	if( pMeshContainer->pOrigMesh)
	{
		pMeshContainer->pOrigMesh->Release();
		pMeshContainer->pOrigMesh = NULL;
	}

	if( pMeshContainer)
	{
		delete pMeshContainer;
		pMeshContainer = NULL;
	}

	return S_OK;
}

HRESULT CAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	//! If there is a Frame Name.
	if( pFrameToFree->Name)
	{
		delete [] pFrameToFree->Name;			//! Delete the Frame Name from Memory.
		pFrameToFree->Name = NULL;				//! Set the name to NULL;
	}

	//If there is a Frame to Free
	if( pFrameToFree)
	{
		delete pFrameToFree;								//! Free the Frame
		pFrameToFree = NULL;							//! Set the frame to NULL;
	}

	return S_OK;
}

HRESULT CAllocateHierarchy::GenerateSkinnedMesh( IDirect3DDevice9 * pD3DDevice, D3DXMESHCONTAINER_ANIM * pMeshContainer)
{
	HRESULT hr;
	D3DCAPS9 d3dCaps;
	pD3DDevice->GetDeviceCaps(&d3dCaps);

	if( pMeshContainer->pSkinInfo == NULL)
		return S_OK;

	COM_RELEASE( pMeshContainer->MeshData.pMesh);
	COM_RELEASE( pMeshContainer->pBoneCombinationBuffer);

	//! Generate a Mesh for IndexedVertexShader or HLSL VertexShader
	unsigned int uiMaxMatrices = 26;
	pMeshContainer->NumPaletteEntries = min( uiMaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
	if( d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
	{	
		Flags |= D3DXMESH_MANAGED;
	}

	COM_RELEASE( pMeshContainer->MeshData.pMesh);

	hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh( pMeshContainer->pOrigMesh,
										Flags,
										pMeshContainer->NumPaletteEntries,
										pMeshContainer->pAdjacency,
										NULL, NULL, NULL,
										&pMeshContainer->NumInfl,
										&pMeshContainer->NumAttributeGroups,
										&pMeshContainer->pBoneCombinationBuffer,
										&pMeshContainer->MeshData.pMesh);

	if( FAILED( hr))
	{
		//! TODO : Add in HR results here.
	}

	//! FVF has to match the declaration for VS.
	DWORD NewFVF = ( pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

	if( NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		LPD3DXMESH pMesh;

		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF( pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pD3DDevice, &pMesh);
		if( !FAILED( hr))
		{
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMesh;
			pMesh = NULL;
		}
	}

	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;

	hr = pMeshContainer->MeshData.pMesh->GetDeclaration( pDecl);

	//! Vertex Shader will interpret UBYTE4 as D3DCOLOR.
	pDeclCur =	 pDecl;

	while( pDeclCur->Stream != 0xff)
	{
		if( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && ( pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = pMeshContainer->MeshData.pMesh->UpdateSemantics( pDecl);
	
	if( FAILED( hr))
	{
		return hr;
	}
	
	//! Allocate a Buffer for Bone Matrices, but only if another Mesh hasn't done one of greater size yet.
	unsigned int uiNumBoneMatricesMax = 0;

	if( uiNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
	{
		uiNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
		
		delete [] m_poAnimatedMesh->m_pBoneMatrices;
		m_poAnimatedMesh->m_pBoneMatrices = new D3DXMATRIXA16[uiNumBoneMatricesMax];
		
		if( m_poAnimatedMesh->m_pBoneMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			return hr;
		}
	}

	return hr;
}

void CAllocateHierarchy::SetAnimatedMeshPointer( CAnimatedMesh * poAnimatedMesh)
{
	m_poAnimatedMesh = poAnimatedMesh;
}