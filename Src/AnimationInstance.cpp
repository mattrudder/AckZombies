#include "AnimationInstance.h"
#include "AnimationManager.h"
#include "ShaderManager.h"
#include "Material.h"

CAnimationInstance::CAnimationInstance(CAnimatedMesh * pAnimatedMesh)
{
	m_pAnimatedMesh = pAnimatedMesh;
	m_pAnimationController = NULL;
	m_pFrameRoot = (D3DXFRAME_ANIM *)pAnimatedMesh->m_pFrameRoot;
	m_dwCurrentTrack = 0;
}

CAnimationInstance::~CAnimationInstance()
{
}

void CAnimationInstance::CleanUp(void)
{
	COM_RELEASE( m_pAnimationController);	
}

HRESULT CAnimationInstance::ResetTime(void)
{
	HRESULT hr;
	hr = m_pAnimationController->ResetTime();
	return hr;
}

void CAnimationInstance::SetWorldMatrix( const D3DXMATRIX * pWorldMatrix)
{
	m_WorldMatrix = *pWorldMatrix;
}

D3DXMATRIX CAnimationInstance::GetWorldMatrix(void)
{
	return m_WorldMatrix;
}

CAnimatedMesh * CAnimationInstance::GetAnimatedMeshPointer(void)
{
	return m_pAnimatedMesh;
}

HRESULT CAnimationInstance::InitializeInstance( LPD3DXANIMATIONCONTROLLER pAnimationController, double dCurrentTime )
{
	unsigned int uiTrackCounter;
	unsigned int uiNumTracks;

	if( pAnimationController != NULL)
	{
		m_pAnimationController = pAnimationController;
		uiNumTracks = m_pAnimationController->GetMaxNumTracks();
		
		for( uiTrackCounter = 0; uiTrackCounter < uiNumTracks; uiTrackCounter++)
		{
			m_pAnimationController->SetTrackEnable( uiTrackCounter, false);
		}

		m_dwAnimIndexIdle = GetAnimationIndex("Loiter");
		m_dwAnimIndexWalk = GetAnimationIndex("Walk");
		m_dwAnimIndexJog = GetAnimationIndex("Jog");
		
		if( m_dwAnimIndexIdle == ANIM_INDEX_FAIL || m_dwAnimIndexWalk == ANIM_INDEX_FAIL || m_dwAnimIndexJog == ANIM_INDEX_FAIL)
			return E_FAIL;
		else
		{
			m_dTimeCurrent = dCurrentTime;
			m_pAnimationController->AdvanceTime( m_dTimeCurrent, NULL);
			return S_OK;
		}
	}
	return E_FAIL;
}

LPD3DXANIMATIONCONTROLLER CAnimationInstance::GetAnimationController(void)
{
	m_pAnimationController->AddRef();
	return m_pAnimationController;
}

void CAnimationInstance::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pTransformMatrix)
{
	//! Make a frame pointer to the base.
	D3DXFRAME_ANIM * pFrame = (D3DXFRAME_ANIM *)pFrameBase;

	if( pTransformMatrix != NULL)
	{
		//! Multiply the Frame Transformation Matrix with the World Transformation Matrix to get the Combind Transformation Matrix.
		D3DXMatrixMultiply( &pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pTransformMatrix);
	}
	else
	{
		//! We are on the parent matrix. So it's combined is equivalent to it's Transformation Matrix.
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if( pFrame->pFrameSibling != NULL)
	{
		//! If there is a sibling frame update it.
		UpdateFrameMatrices( pFrame->pFrameSibling, pTransformMatrix);
	}

	if( pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices( pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}

}

void CAnimationInstance::UpdateAnimation(/*D3DXMATRIXA16 * pWorldMatrix,*/ double dElapsedTime)
{
	m_dTimePrev = m_dTimeCurrent;
	m_dTimeCurrent = m_dTimeCurrent + dElapsedTime;

	if( m_pAnimationController != NULL)
	{
		//! Update the elapsed time for the animation controller so it can interpolate.
		m_pAnimationController->AdvanceTime( dElapsedTime, NULL);
	}
	//! Traverse the Frame Hierarchy recursively to update them.
	UpdateFrameMatrices( m_pFrameRoot, &m_WorldMatrix);
}

void CAnimationInstance::DrawFrames( D3DXFRAME_ANIM * pFrame)
{
	if( pFrame->pMeshContainer)
		DrawMeshFrame( pFrame);

	if( pFrame->pFrameSibling)
		DrawFrames( (D3DXFRAME_ANIM *)pFrame->pFrameSibling);

	if( pFrame->pFrameFirstChild)
		DrawFrames( (D3DXFRAME_ANIM *) pFrame->pFrameFirstChild);
}

void CAnimationInstance::DrawMeshFrame( D3DXFRAME_ANIM * pFrame)
{
	//! TODO: Insert code to draw the mesh here.
	HRESULT hr = S_OK;
	D3DXFRAME_ANIM * pAnimFrame = pFrame;
	D3DXMESHCONTAINER_ANIM * pMeshContainer = (D3DXMESHCONTAINER_ANIM *)pFrame->pMeshContainer;
	CShader* pShader;
	CMaterial* pMat;

	unsigned int uiAttrib;
	LPD3DXBONECOMBINATION pBoneCombination;

	unsigned int uiMatrixIndex;
	unsigned int uiPaletteEntry;
	D3DXMATRIXA16 matrixTemp;
	D3DCAPS9 d3dCaps;
	m_pAnimatedMesh->m_pD3DDevice->GetDeviceCaps(&d3dCaps);

	//! Check to see that there is Skinning Information
	if( pFrame->pMeshContainer->pSkinInfo != NULL)
	{
		//! Draw via Indexed HLSL VertexShader
		pBoneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuffer->GetBufferPointer());

		for( uiAttrib = 0; uiAttrib < pMeshContainer->NumAttributeGroups; uiAttrib++)
		{
			//! Calculate all the world matrices.
			for( uiPaletteEntry = 0; uiPaletteEntry < pMeshContainer->NumPaletteEntries; uiPaletteEntry++)
			{
				uiMatrixIndex = pBoneCombination[uiAttrib].BoneId[uiPaletteEntry];

				if( uiMatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply( &m_pAnimatedMesh->m_pBoneMatrices[uiPaletteEntry], &pMeshContainer->pBoneOffsetMatrices[uiMatrixIndex], pMeshContainer->ppBoneMatrixPointers[uiMatrixIndex]);
				}
			}

			// Draw Mesh
			// (MR): Modified rendering code to work with shader management
			if(uiAttrib < pMeshContainer->NumMaterials)
				pMat = pMeshContainer->ppMaterials[uiAttrib];
			else
				pMat = pMeshContainer->ppMaterials[0];
			pShader = pMat->getShader();
			CShaderParam& oMatWorldArray = pShader->getParamBySemantic("WorldMatrixArray");
			if(oMatWorldArray.isValid())
				oMatWorldArray.setMatrixArray(m_pAnimatedMesh->m_pBoneMatrices, pMeshContainer->NumPaletteEntries);
			CShaderParam& oBoneCount = pShader->getParamBySemantic("BlendIndexCount");
			if(oBoneCount.isValid())
				oBoneCount = pMeshContainer->NumInfl - 1;

			//! Start Effect.  All parameters should be updated at this point.
			CShaderManager::getInstance().beginObjectRender();

			pMat->begin();
			for(unsigned int uiPass = 0; uiPass < pMat->getNumPasses(); uiPass++)
			{
				CShaderManager::getInstance().beginPass();
				pMat->beginPass(uiPass);
				//! Draw the subset with the current material and world matrix palette
				pMeshContainer->MeshData.pMesh->DrawSubset(uiAttrib);
				pMat->endPass();
			}
			pMat->end();
		}
	}
	
}

void CAnimationInstance::Draw(void)
{
	  DrawFrames( (D3DXFRAME_ANIM *)m_pAnimatedMesh->m_pFrameRoot);
}

void CAnimationInstance::SetAnimationIdle(void)
{
	DWORD dwNewTrack = ( m_dwCurrentTrack == 0 ? 1 : 0);
	LPD3DXANIMATIONSET pAnimSet;

	m_pAnimationController->GetAnimationSet( m_dwAnimIndexIdle, &pAnimSet);
	m_pAnimationController->SetTrackAnimationSet( dwNewTrack, pAnimSet);
	pAnimSet->Release();

	m_pAnimationController->UnkeyAllTrackEvents( m_dwCurrentTrack);
	m_pAnimationController->UnkeyAllTrackEvents( dwNewTrack);

	m_pAnimationController->KeyTrackEnable( m_dwCurrentTrack, FALSE, m_dTimeCurrent + IDLE_TRANSITION_TIME);
	m_pAnimationController->KeyTrackSpeed( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, IDLE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackWeight( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, IDLE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->SetTrackEnable( dwNewTrack, TRUE);
	m_pAnimationController->KeyTrackSpeed( dwNewTrack, 1.0f, m_dTimeCurrent, IDLE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackWeight( dwNewTrack, 1.0f, m_dTimeCurrent, IDLE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;

}

void CAnimationInstance::SetAnimationWalk(void)
{
	DWORD dwNewTrack = ( m_dwCurrentTrack == 0 ?1 : 0);
	LPD3DXANIMATIONSET pAnimSet;

	m_pAnimationController->GetAnimationSet( m_dwAnimIndexWalk, &pAnimSet);

	m_pAnimationController->SetTrackAnimationSet( dwNewTrack, pAnimSet);
	pAnimSet->Release();

	m_pAnimationController->UnkeyAllTrackEvents( m_dwCurrentTrack);
	m_pAnimationController->UnkeyAllTrackEvents( dwNewTrack);

	m_pAnimationController->KeyTrackEnable( m_dwCurrentTrack, FALSE, m_dTimeCurrent + MOVE_TRANSITION_TIME);
	m_pAnimationController->KeyTrackSpeed( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackWeight( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->SetTrackEnable( dwNewTrack, TRUE);
	m_pAnimationController->KeyTrackSpeed( dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackWeight( dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;
}

void CAnimationInstance::SetAnimationRun(void)
{
	DWORD dwNewTrack = ( m_dwCurrentTrack == 0 ? 1 : 0);
	LPD3DXANIMATIONSET pAnimSet;

	m_pAnimationController->GetAnimationSet( m_dwAnimIndexJog, &pAnimSet);
	m_pAnimationController->SetTrackAnimationSet( dwNewTrack, pAnimSet);
	pAnimSet->Release();

	m_pAnimationController->UnkeyAllTrackEvents( m_dwCurrentTrack);
	m_pAnimationController->UnkeyAllTrackEvents( dwNewTrack);

	m_pAnimationController->KeyTrackEnable( m_dwCurrentTrack, FALSE, m_dTimeCurrent + MOVE_TRANSITION_TIME);
	m_pAnimationController->KeyTrackSpeed( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackWeight( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->SetTrackEnable( dwNewTrack, TRUE);
	m_pAnimationController->KeyTrackSpeed( dwNewTrack, 1.0f,  m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackWeight( dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;
}

DWORD CAnimationInstance::GetAnimationIndex(const char * szAnimationName)
{
	HRESULT hr;
	
	LPD3DXANIMATIONSET pAnimSet;
	DWORD dwReturnIndex = ANIM_INDEX_FAIL;

	for( DWORD i = 0; i < m_pAnimationController->GetNumAnimationSets(); ++i)
	{
		hr = m_pAnimationController->GetAnimationSet( i, &pAnimSet);
		if( FAILED( hr))
			continue;

		if( pAnimSet->GetName() && !strncmp( pAnimSet->GetName(), szAnimationName, min( strlen( pAnimSet->GetName() ), strlen( szAnimationName))))
		{
			dwReturnIndex = i;
			pAnimSet->Release();
			break;
		}

		pAnimSet->Release();
	}
	return dwReturnIndex;
}