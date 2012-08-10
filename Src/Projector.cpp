/**
* @file Projector.cpp
* @author Matt Rudder
* @date Modified Jun 05, 2006
*
* Encapsulates the resourced needed to perform texture projection.
*/

// Local includes
#include "Projector.h"

// Engine includes
#include "Texture2D.h"
#include "RenderSystem.h"

D3DXVECTOR3 CProjector::m_vWorldUp(0,1,0);

/**
* CProjector::CProjector
* @date Modified Jun 05, 2006
*/
CProjector::CProjector(void) : m_pTexture(NULL), m_vUp(m_vWorldUp), m_vAt(0,0,1),
m_vPos(0,0,0)
{
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProjection);

	// Calculate Proj. Matrix
	CRenderSystem* pRen = CRenderSystem::getInstancePtr();
	float fAspect = (float)pRen->getDisplayWidth() / (float)pRen->getDisplayHeight();
	D3DXMatrixPerspectiveFovLH(&m_mProjection, D3DX_PI / 4, fAspect, 0.09f, 5.0f);

	// Calculate View matrix, and initialize frustum.
	calcViewMatrix();
}

/**
* CProjector::~CProjector
* @date Modified Jun 05, 2006
*/
CProjector::~CProjector(void)
{
}

/**
* CProjector::calcViewMatrix
* @date Modified Jun 05, 2006
*/
__forceinline void CProjector::calcViewMatrix(void)
{
	// Calculate View Matrix
	D3DXVECTOR3 vEyePt;
	vEyePt = m_vAt + m_vPos * (1.0f / 2.0f);
	D3DXMatrixLookAtLH(&m_mView, &vEyePt, &m_vAt, &m_vUp);
	m_oFrustum.setMatrix(m_mProjection, m_mView, 5.0f);
}

/**
* CProjector::getFrustum
* @date Modified Jun 06, 2006
*/
CFrustum& CProjector::getFrustum(void)
{
	return m_oFrustum; 
}


/**
* CProjector::setTexture
* @date Modified Jun 05, 2006
*/
void CProjector::setTexture(CTexture2D* pTexture)
{
	// Store texture for later
	m_pTexture = pTexture; 
	pTexture->addRef(); 
}

/**
* CProjector::setPosition
* @date Modified Jun 05, 2006
*/
void CProjector::setPosition(D3DXVECTOR3& vPos)
{
	m_vPos = vPos;
	calcViewMatrix();
}

/**
* CProjector::setDirection
* @date Modified Jun 05, 2006
*/
void CProjector::setDirection(D3DXVECTOR3& vDir)
{
	m_vAt = vDir;
	D3DXVECTOR3 vRight;
	D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &m_vWorldUp, &vDir));
	D3DXVec3Normalize(&m_vUp, D3DXVec3Cross(&m_vUp, &vRight, &vDir));
	calcViewMatrix();
}

/**
* CProjector::getTransform
* @date Modified Jun 06, 2006
*/
void CProjector::getTransform(D3DXMATRIX& mProj)
{
	D3DXMATRIX mWorld;
	D3DXMatrixTranslation(&mWorld, m_vPos.x, m_vPos.y, m_vPos.z);
	mProj = mWorld * m_mView * m_mProjection;
}