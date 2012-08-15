/**
* @file RenderGroup.cpp
* @author Matt Rudder
* @date Modified Apr 25, 2006
*
* Defines a list of organized groups of geometry by material specification.
*/

// Local includes
#include "RenderGroup.h"

// Engine includes
#include "RenderSystem.h"
#include "Material.h"

/**
* CRenderGroup::CRenderGroup
* @date Modified Apr 25, 2006
*/
CRenderGroup::CRenderGroup(DWORD dwMaxSolidSize, DWORD dwMaxTransSize) : 
	m_oSolidsStatic(dwMaxSolidSize), m_oTransparentStatic(dwMaxTransSize),
	m_oTransFunc(&CRenderGroup::_renderSort_getViewDistance, this),
	m_oSolidFunc(&CRenderGroup::_renderSort_getMaterialID, this),
	m_oSolidLessFunc(&CRenderGroup::_renderSortLess_byMaterialID, this), 
	m_oTransLessFunc(&CRenderGroup::_renderSortLess_byViewDistance, this)
{
}

/**
* CRenderGroup::~CRenderGroup
* @date Modified Apr 25, 2006
*/
CRenderGroup::~CRenderGroup(void)
{
}

/**
* CRenderGroup::_renderSort_getViewDistance
* @date Modified Apr 28, 2006
*/
float CRenderGroup::_renderSort_getViewDistance(SRenderable* pRender)
{
	return -pRender->getSquaredViewDistance();
}

/**
* CRenderGroup::_renderSort_getMaterialPtr
* @date Modified Apr 28, 2006
*/
CMaterial* CRenderGroup::_renderSort_getMaterialPtr(SRenderable* pRender)
{
	return ((SRenderTriangle*)pRender)->m_pMaterial;
}

/**
* CRenderGroup::_renderSort_getMaterialID
* @date Modified Apr 28, 2006
*/
unsigned int CRenderGroup::_renderSort_getMaterialID(SRenderable* pRender)
{
	return ((SRenderTriangle*)pRender)->m_pMaterial->getHash();
}

/**
* CRenderGroup::_renderSortLess_byViewDistance
* @date Modified Apr 29, 2006
*/
bool CRenderGroup::_renderSortLess_byViewDistance(SRenderable* pLeft, SRenderable* pRight)
{
	float fLeft = pLeft->getSquaredViewDistance(),
		  fRight = pRight->getSquaredViewDistance();

	if(fLeft == fRight)
	{
		return false;
	}
	else
	{
		// Sort descending.
		return fLeft > fRight;
	}
}

/**
* CRenderGroup::_renderSortLess_byMaterialID
* @date Modified Apr 29, 2006
*/
bool CRenderGroup::_renderSortLess_byMaterialID(SRenderable* pLeft, SRenderable* pRight)
{
	unsigned int nLeft = ((SRenderTriangle*)pLeft)->m_pMaterial->getHash(),
				 nRight = ((SRenderTriangle*)pRight)->m_pMaterial->getHash();

	if(nLeft == nRight)
	{
		return false;
	}
	else
	{
		return nLeft < nRight;
	}
}

/**
* CRenderGroup::addRenderable
* @date Modified Apr 25, 2006
*/
void CRenderGroup::addRenderable(SRenderable* pRender)
{
	// Add to the appropriate list.
	if(pRender->isTransparent())
		m_oTransparentStatic.addRenderable(pRender);
	else
		m_oSolidsStatic.addRenderable(pRender);
}

/**
* CRenderGroup::addRenderableArray
* @date Modified Apr 25, 2006
*/
void CRenderGroup::addRenderableArray(SRenderable* pRender, DWORD dwNumPrimitives)
{
	SRenderTriangle* pTri = (SRenderTriangle*)pRender;
	for(DWORD i = 0; i < dwNumPrimitives; ++i)
	{
		// Add to the appropriate list.
		if(pTri[i].isTransparent())
			m_oTransparentStatic.addRenderable(pTri+i);
		else
			m_oSolidsStatic.addRenderable(pTri+i);
	}
}

/**
* CRenderGroup::renderAll
* @date Modified Apr 25, 2006
*/
void CRenderGroup::renderAll(D3DXVECTOR3& vEye, CRadixSort<CRenderList::RenderPrimitiveList, SRenderable*, unsigned int>* pSortSolid, CRadixSort<CRenderList::RenderPrimitiveList, SRenderable*, float>* pSortTrans)
{
	m_pEye = &vEye;
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_oSolidsStatic.sort(pSortSolid, m_oSolidFunc, m_oSolidLessFunc);
	m_oSolidsStatic.renderAll();
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_oTransparentStatic.calcViewDistance(vEye);
	m_oTransparentStatic.sort(pSortTrans, m_oTransFunc, m_oTransLessFunc);
	m_oTransparentStatic.renderAll();
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}