/**
* @file AnimatedMesh.cpp
* @author Matt Rudder
* @author Jason Williams
* @date Modified May 15, 2006
*
* Encapsulates the loading, updating and rendering of skinned meshes.
*/

// Local includes
#include "AnimatedMesh.h"
#include "AnimAllocator.h"
#include "Material.h"

// System includes
#include <d3dx9anim.h>

/**
* CAnimatedMesh::CAnimatedMesh
* @date Modified May 15, 2006
*/
CAnimatedMesh::CAnimatedMesh(void) : m_pFrameRoot(NULL), m_pAnimController(NULL), 
	m_pBoneMatrices(NULL), m_pFirstMesh(NULL), m_fAnimSpeed(1.0f), 
	m_fCurrentTime(0.0f), m_dwCurrentTrack(-1), m_dwCurrentAnimSet(-1), m_unNumBoneMax(0)
{
	m_fLastEngineTime = CTimer::getInstance().getTime();
}

/**
* CAnimatedMesh::~CAnimatedMesh
* @date Modified May 15, 2006
*/
CAnimatedMesh::~CAnimatedMesh(void)
{
	COM_RELEASE(m_pAnimController);
	SMeshFrame* pFrame = (SMeshFrame*)m_pFrameRoot;
	CAnimAllocator oAlloc(this);
	--pFrame->ulRefCount;
	if(!pFrame->ulRefCount)
		D3DXFrameDestroy(m_pFrameRoot, &oAlloc);
		
}

/**
* CAnimatedMesh::toString
* @date Modified May 15, 2006
*/
CString CAnimatedMesh::toString(void) const
{
	return CMesh::toString();
}

/**
* CAnimatedMesh::cloneResource
* @date Modified May 15, 2006
*/
CBaseResource* CAnimatedMesh::cloneResource(void)
{
	// Create mesh.
	CAnimatedMesh* pMesh = new CAnimatedMesh;

	// Copy data
	((SMeshFrame*)m_pFrameRoot)->ulRefCount++;
	pMesh->m_pFrameRoot = m_pFrameRoot;
	pMesh->m_pBoneMatrices = m_pBoneMatrices;
	pMesh->m_unNumBoneMax = m_unNumBoneMax;
	pMesh->m_dwNumAnimSets = m_dwNumAnimSets;
	pMesh->m_fAnimSpeed = m_fAnimSpeed;
	pMesh->m_dwNumAnimSets = m_dwNumAnimSets;
	pMesh->m_pFirstMesh = m_pFirstMesh;
	pMesh->m_ulRefCount = 1;

	m_pAnimController->CloneAnimationController(m_pAnimController->GetMaxNumAnimationOutputs(), 
		m_pAnimController->GetNumAnimationSets(), m_pAnimController->GetMaxNumTracks(), 
		m_pAnimController->GetMaxNumEvents(), &pMesh->m_pAnimController);

	pMesh->setAnimationSet(0);

	return pMesh;
}

/**
* CAnimatedMesh::createMeshFromFile
* @date Modified May 15, 2006
*/
CBaseResource* CAnimatedMesh::createMeshFromFile(const CString sFilename)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	CAnimatedMesh* pMesh = new CAnimatedMesh;
	pMesh->m_sFilename = sFilename;
	CAnimAllocator oAlloc(pMesh);
	if(FAILED(D3DXLoadMeshHierarchyFromX(sFilename, D3DXMESH_MANAGED, pDev, &oAlloc,
		NULL, &pMesh->m_pFrameRoot, &pMesh->m_pAnimController)))
		return NULL;

	if(pMesh->m_pAnimController)
		pMesh->m_dwNumAnimSets = pMesh->m_pAnimController->GetMaxNumAnimationSets();

	if(pMesh->m_pFrameRoot)
	{
		pMesh->setupBones(pDev, (SMeshFrame*)pMesh->m_pFrameRoot, NULL);
		pMesh->m_pBoneMatrices = new D3DXMATRIX[pMesh->m_unNumBoneMax];
		memset(pMesh->m_pBoneMatrices, 0, sizeof(D3DXMATRIX)*pMesh->m_unNumBoneMax);
	}

	pMesh->setAnimationSet(0);
	return pMesh;
}

/**
* CAnimatedMesh::setupBones
* @date Modified May 16, 2006
*/
void CAnimatedMesh::setupBones(LPDIRECT3DDEVICE9 pDev, SMeshFrame* pFrame, LPD3DXMATRIX pParent)
{
	SMeshContainer* pMesh = (SMeshContainer*)pFrame->pMeshContainer;
	if(pMesh)
	{
		if(!m_pFirstMesh)
			m_pFirstMesh = pMesh;

		// Setup bone matrices if skin info exists.
		if(pMesh->pSkinInfo && pMesh->MeshData.pMesh)
		{
			// Clone mesh.
			D3DVERTEXELEMENT9 oDecl[MAX_FVF_DECL_SIZE];
			if(FAILED(pMesh->MeshData.pMesh->GetDeclaration(oDecl)))
				return;

			pMesh->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED,
				oDecl, pDev, &pMesh->m_pSkinMesh);

			// Get max bone count
			m_unNumBoneMax = max(m_unNumBoneMax, pMesh->pSkinInfo->GetNumBones());

			// Setup matrices
			for(UINT i = 0; i < pMesh->pSkinInfo->GetNumBones(); ++i)
			{
				SMeshFrame* pTempFrame = (SMeshFrame*)D3DXFrameFind(m_pFrameRoot, pMesh->pSkinInfo->GetBoneName(i));
				pMesh->m_ppFrameMatrices[i] = &pTempFrame->m_mCombinedWorld;
			}
		}
	}

	if(pFrame->pFrameSibling)
		setupBones(pDev, (SMeshFrame*)pFrame->pFrameSibling, pParent);

	if(pFrame->pFrameFirstChild)
		setupBones(pDev, (SMeshFrame*)pFrame->pFrameFirstChild, &pFrame->m_mCombinedWorld);

}

/**
* CAnimatedMesh::updateBones
* @date Modified May 16, 2006
*/
void CAnimatedMesh::updateBones(SMeshFrame* pFrame, const D3DXMATRIX* pParent)
{
	if(pParent)
		D3DXMatrixMultiply(&pFrame->m_mCombinedWorld, &pFrame->TransformationMatrix, pParent);
	else
		pFrame->m_mCombinedWorld = pFrame->TransformationMatrix;

	if(pFrame->pFrameSibling)
		updateBones((SMeshFrame*)pFrame->pFrameSibling, pParent);

	if(pFrame->pFrameFirstChild)
		updateBones((SMeshFrame*)pFrame->pFrameFirstChild, &pFrame->m_mCombinedWorld);
}

/**
* CAnimatedMesh::drawFrame
* @date Modified May 16, 2006
*/
void CAnimatedMesh::drawFrame(LPDIRECT3DDEVICE9 pDev, bool bTextured, SMeshFrame* pFrame) const
{
	LPD3DXMESHCONTAINER pMesh = pFrame->pMeshContainer;
	while(pMesh)
	{
		drawMesh(pDev, bTextured, (SMeshContainer*)pMesh, pFrame);
		pMesh = pMesh->pNextMeshContainer;
	}

	if(pFrame->pFrameSibling)
		drawFrame(pDev, bTextured, (SMeshFrame*)pFrame->pFrameSibling);

	if(pFrame->pFrameFirstChild)
		drawFrame(pDev, bTextured, (SMeshFrame*)pFrame->pFrameFirstChild);
}

/**
* CAnimatedMesh::drawMesh
* @date Modified May 16, 2006
*/
void CAnimatedMesh::drawMesh(LPDIRECT3DDEVICE9 pDev, bool bTextured, SMeshContainer* pMesh, SMeshFrame* pFrame) const
{
	HRESULT hr;
	UNREFERENCED_PARAMETER(hr);
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneCombination;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pDev->GetDeviceCaps(&d3dCaps);

	AttribIdPrev = UNUSED32;
	pBoneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(pMesh->m_pBoneCombinationBuffer->GetBufferPointer());

	UINT NumBlend;
	//! Draw using default vertex processing on HW
	for(iAttrib = 0; iAttrib < pMesh->m_dwNumAttributeGroups; iAttrib++)
	{
		NumBlend = 0;
		for(DWORD i = 0; i < pMesh->m_dwNumInfl; i++)
		{
			if(pBoneCombination[iAttrib].BoneId[i] != UINT_MAX) 
				NumBlend = i;
		}

		if(d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
		{	
			DWORD dwCull, dwBlend, dwSrcBlend, dwDestBlend;
			bool bBlend = false;

			if(pMesh->m_vMaterials[pBoneCombination[iAttrib].AttribId]->isTransparent())
			{
				pDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
				pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

				pDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwBlend);
				if(!dwBlend)
					pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

				// Blend states
				pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
				pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				pDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
				pDev->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
				pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}

			if(bTextured)
				pMesh->m_vMaterials[pBoneCombination[iAttrib].AttribId]->begin();

			for( DWORD i = 0; i < pMesh->m_dwNumInfl; i++)
			{
				iMatrixIndex = pBoneCombination[iAttrib].BoneId[i];
				if( iMatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply(&matTemp, &pMesh->m_pBoneOffsets[iMatrixIndex], pMesh->m_ppFrameMatrices[iMatrixIndex]);
					pDev->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
				}
			}

			pDev->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

			LPD3DXMESH pDrawMesh = ( pMesh->pSkinInfo) ? pMesh->m_pSkinMesh : pMesh->MeshData.pMesh;
			pDrawMesh->DrawSubset(iAttrib);
			
			if(bTextured)
				pMesh->m_vMaterials[pBoneCombination[iAttrib].AttribId]->end();


			// Turn off vertex blending
			pDev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);


		    // Reset blend modes for alpha-blended materials
			if(pMesh->m_vMaterials[pBoneCombination[iAttrib].AttribId]->isTransparent())
			{
				if(!dwBlend)
					pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				pDev->SetRenderState(D3DRS_CULLMODE, dwCull);
				pDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
				pDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
			}

		}
	}
}

/**
* CAnimatedMesh::updateFrame
* @date Modified May 16, 2006
*/
void CAnimatedMesh::updateFrame(const D3DXMATRIX* mWorld, SMeshFrame* pFrame)
{
	if(!pFrame)
	{
		float fCurTime = CTimer::getInstance().getTime();
		float fElapsed = fCurTime - m_fLastEngineTime;
		m_fLastEngineTime = fCurTime;

		// Adjust animation speed.
		fElapsed *= m_fAnimSpeed;

		// Update timestep
		if(m_pAnimController)
			m_pAnimController->AdvanceTime(fElapsed, NULL);
		m_fCurrentTime += fElapsed;

		// Update hierarchy
		updateBones((SMeshFrame*)m_pFrameRoot, mWorld);
	}

	SMeshContainer* pMesh = m_pFirstMesh;
	if(pMesh && pMesh->pSkinInfo)
	{
		UINT unBones = pMesh->pSkinInfo->GetNumBones();

		// Create bone matrices: bone space -> model space.
		for(UINT i = 0; i < unBones; ++i)
			D3DXMatrixMultiply(&m_pBoneMatrices[i], &pMesh->m_pBoneOffsets[i], pMesh->m_ppFrameMatrices[i]);

	}
}

/**
* CAnimatedMesh::draw
* @date Modified May 16, 2006
*/
void CAnimatedMesh::setAnimationSet(UINT unIndex)
{
	if(unIndex >= m_dwNumAnimSets || unIndex == m_dwCurrentAnimSet)
		return;

	m_dwCurrentAnimSet = unIndex;

	// Get the requested anim set.
	LPD3DXANIMATIONSET pSet;
	m_pAnimController->GetAnimationSet(m_dwCurrentAnimSet, &pSet);

	// Alternate tracks for transitions.
	DWORD dwNewTrack = (m_dwCurrentTrack == 0) ? 1 : 0;

	// Assign new track.
	m_pAnimController->SetTrackAnimationSet(dwNewTrack, pSet);
	pSet->Release();

	// Clear track events
	m_pAnimController->UnkeyAllTrackEvents(m_dwCurrentTrack);
	m_pAnimController->UnkeyAllTrackEvents(dwNewTrack);

	// Disable the current track in the future (should make the transition time variable)
	m_pAnimController->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_fCurrentTime + 0.25f);
	// Change anim speed
	m_pAnimController->KeyTrackSpeed(m_dwCurrentTrack, 0.0f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);
	// Change current track weight (to enable blending with the next track)
	m_pAnimController->KeyTrackWeight(m_dwCurrentTrack, 0.0f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);

	// Enable new track
	m_pAnimController->SetTrackEnable(dwNewTrack, TRUE);
	// Set speed
	m_pAnimController->KeyTrackSpeed(dwNewTrack, 1.0f, m_fAnimSpeed, 0.25f, D3DXTRANSITION_LINEAR);
	// Change the weighting of the current track over time
	m_pAnimController->KeyTrackWeight(dwNewTrack, 1.0f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;
}

/**
* CAnimatedMesh::draw
* @date Modified May 16, 2006
*/
void CAnimatedMesh::draw(bool bTextured, D3DCOLOR* dwColor)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	if(dwColor)
	{
		SetTextureColorStage(pDev, 1, D3DTA_CURRENT, D3DTOP_ADD, D3DTA_TFACTOR);
		pDev->SetRenderState(D3DRS_TEXTUREFACTOR, *dwColor);
	}

	if(m_pFrameRoot)
		drawFrame(pDev, bTextured, (SMeshFrame*)m_pFrameRoot);

	if(dwColor)
	{
		DisableColorStage(pDev, 1);
		pDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0);
	}
}

/**
* CAnimatedMesh::getBoneByName
* @date Modified May 17, 2006
*/
bool CAnimatedMesh::getBoneByName(const char* szName, D3DXMATRIX& mBoneTransform)
{
	// Search model hierarchy for bone name.
	SMeshFrame* pFrame = (SMeshFrame*)D3DXFrameFind(m_pFrameRoot, szName);
	if(!pFrame)
		return false;

	// Copy matrix and return success.
	mBoneTransform = pFrame->m_mCombinedWorld;
	return true;
}

/**
* CAnimatedMesh::getAnimationSetByName
* @date Modified May 17, 2006
*/
UINT CAnimatedMesh::getAnimationSetByName(const char* szName)
{
	LPD3DXANIMATIONSET pAnimSet;
	UINT unNumSets = m_pAnimController->GetNumAnimationSets();
	UINT i;
	for(i = 0; i < unNumSets; ++i)
	{
		m_pAnimController->GetAnimationSet(i, &pAnimSet);
		if(!stricmp(pAnimSet->GetName(), szName))
		{
			pAnimSet->Release();
			return i;
		}

		pAnimSet->Release();
	}
	return -1;
}

/**
* CAnimatedMesh::setAnimationSetByName
* @date Modified May 17, 2006
*/
void CAnimatedMesh::setAnimationSetByName(const char* szName)
{
	if (szName)
		m_oAnimationName = szName;

	// Get the requested anim set.
	LPD3DXANIMATIONSET pSet;
	UINT unNumSets = m_pAnimController->GetNumAnimationSets();
	UINT i;
	for(i = 0; i < unNumSets; ++i)
	{
		m_pAnimController->GetAnimationSet(i, &pSet);
		if(!stricmp(pSet->GetName(), szName))
			break;

		pSet->Release();
		pSet = NULL;
	}
	if(!pSet || i == m_dwCurrentAnimSet)
		return;

	m_dwCurrentAnimSet = i;

	// Alternate tracks for transitions.
	DWORD dwNewTrack = (m_dwCurrentTrack == 0) ? 1 : 0;

	// Assign new track.
	m_pAnimController->SetTrackAnimationSet(dwNewTrack, pSet);
	pSet->Release();

	// Clear track events
	m_pAnimController->UnkeyAllTrackEvents(m_dwCurrentTrack);
	m_pAnimController->UnkeyAllTrackEvents(dwNewTrack);

	// Disable the current track in the future (should make the transition time variable)
	m_pAnimController->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_fCurrentTime + 0.25f);
	// Change anim speed
	m_pAnimController->KeyTrackSpeed(m_dwCurrentTrack, 0.0f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);
	// Change current track weight (to enable blending with the next track)
	m_pAnimController->KeyTrackWeight(m_dwCurrentTrack, 0.0f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);

	// Enable new track
	m_pAnimController->SetTrackEnable(dwNewTrack, TRUE);
	// Set speed
	m_pAnimController->KeyTrackSpeed(dwNewTrack, 1.0f, m_fAnimSpeed, 0.25f, D3DXTRANSITION_LINEAR);
	// Change the weighting of the current track over time
	m_pAnimController->KeyTrackWeight(dwNewTrack, 1.0f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;
}

/**
* CAnimatedMesh::getAnimationLength
* @date Modified May 21, 2006
*/
double CAnimatedMesh::getAnimationLength(bool bScaled, const char* szName)
{
	// Get the animation set.
	LPD3DXANIMATIONSET pSet = NULL;
	if(szName)
		m_pAnimController->GetAnimationSetByName(szName, &pSet);
	else
		m_pAnimController->GetAnimationSet(m_dwCurrentAnimSet, &pSet);

	// Get the length of the animation set.
	if(bScaled)
		return pSet->GetPeriod() / (double)m_fAnimSpeed;
	else
		return pSet->GetPeriod();
}
