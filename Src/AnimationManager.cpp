#include "AnimationManager.h"

CAnimationManager * CAnimationManager::m_poInstance = NULL;

CAnimationManager::CAnimationManager()
{

}

CAnimationManager::~CAnimationManager()
{
		
}

CAnimationManager * CAnimationManager::GetInstance(void)
{
	if( m_poInstance == NULL)
		m_poInstance = new CAnimationManager();

	return m_poInstance;
}

CAnimatedMesh * CAnimationManager::GetAnimatedMesh( unsigned int uiAnimatedMeshIndex)
{
	CAnimatedMesh * pAnimatedMesh = NULL;
	pAnimatedMesh = m_vpAnimatedMeshes[uiAnimatedMeshIndex];
	return pAnimatedMesh;
}

HRESULT CAnimationManager::UpdateAnimationInstances( /*D3DXMATRIXA16 * pWorldMatrix, */ double dElapsedTime, D3DXMATRIX * pViewMatrix)
{
	memcpy( &m_viewMatrix, pViewMatrix, sizeof(D3DXMATRIX));

	if(!m_vpAnimatedMeshes.empty() )
	{
		vector<CAnimatedMesh *>::iterator iterAnimatedMesh = m_vpAnimatedMeshes.begin();
		
		for( ; iterAnimatedMesh != m_vpAnimatedMeshes.end(); iterAnimatedMesh++)
		{
			CAnimatedMesh * pAnimatedMesh;
			pAnimatedMesh = *iterAnimatedMesh;
			vector<CAnimationInstance *>::iterator iterAnimationInstance = pAnimatedMesh->m_vpAnimationInstances.begin();

			for( ; iterAnimationInstance != pAnimatedMesh->m_vpAnimationInstances.end(); iterAnimationInstance++)
			{
				(*iterAnimationInstance)->UpdateAnimation( /*pWorldMatrix,*/ dElapsedTime);
			}
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

D3DXMATRIX CAnimationManager::GetViewMatrix(void)
{
	return m_viewMatrix;
}

CAnimatedMesh * CAnimationManager::CreateAnimatedMesh(void)
{
	CAnimatedMesh * pAnimatedMesh = new CAnimatedMesh();
	m_vpAnimatedMeshes.push_back( pAnimatedMesh);
	return pAnimatedMesh;
}

unsigned int CAnimationManager::GetAnimatedMeshIndex( CAnimatedMesh * pAnimatedMesh)
{
	
	if(	!m_vpAnimatedMeshes.empty() )
	{
		unsigned int size = (unsigned int)m_vpAnimatedMeshes.size();

		for( unsigned int i = 0; i < size; i++)
		{
			CAnimatedMesh * pAnimatedMeshCompare = NULL;
			pAnimatedMeshCompare = m_vpAnimatedMeshes[i];

			if( pAnimatedMeshCompare == pAnimatedMesh)
				return i;
		}
	}

	return ANIM_INDEX_FAIL;
}