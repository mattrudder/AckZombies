/**
* @file RenderList.cpp
* @author Matt Rudder
* @date Modified Apr 25, 2006
*
* Defines a list of sorted renderable primitives.
*/

// Local includes
#include "RenderList.h"

// Engine includes
#include "VertexCache.h"
#include "MethodFunctor.h"
#include "Material.h"

/**
* CRenderList::CRenderList
* @date Modified Apr 25, 2006
*/
CRenderList::CRenderList(DWORD dwMax) : m_dwMaxIndex(dwMax), m_dwCurrentIndex(0), 
m_pCurrentMaterial(NULL), m_dwCurrentPass(0)
{
	m_ppPrimList = new SRenderable*[m_dwMaxIndex];
	m_oPrimList.reserve(m_dwMaxIndex);
	m_poVertCache = new CVertexCache(4096, sizeof(CMesh::SVertex), 4096, CMesh::VertexElem);
}

/**
* CRenderList::~CRenderList
* @date Modified Apr 25, 2006
*/
CRenderList::~CRenderList(void)
{
	delete m_poVertCache;
	delete [] m_ppPrimList;
}

/**
* CRenderList::addRenderable
* @date Modified Apr 25, 2006
*/
void CRenderList::addRenderable(SRenderable* pRender)
{
	m_oPrimList.push_back(pRender);
	m_ppPrimList[m_dwCurrentIndex++] = pRender;
}

/**
* CRenderList::renderAll
* @date Modified Apr 25, 2006
*/
void CRenderList::renderAll(void)
{
	// Render Geomtery
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	m_poVertCache->begin();
	SRenderTriangle* pTri;
	CMaterial* pTriMat = NULL;
	for(DWORD i = 0; i < m_dwCurrentIndex; ++i)
	{
		pTri = (SRenderTriangle*)m_oPrimList[i];

		// Check for material change.
		pTriMat = pTri->m_pMaterial;
		if(pTriMat != m_pCurrentMaterial)
		{
			// Change material
			if(m_pCurrentMaterial) 
			{
				// Render previous if the new material has effect on the shader or texture
				if(pTriMat->getDiffuse() != m_pCurrentMaterial->getDiffuse())
				{
					// Make sure the straggler triangles get rendered.
					m_poVertCache->flush();
					m_dwCurrentPass = 0;

					m_pCurrentMaterial->end();
				};
			}
			m_pCurrentMaterial = pTriMat;
			m_pCurrentMaterial->begin();
		}

		// Add triangles
		m_poVertCache->insert(pTri->m_oVerts, 3, NULL, 3);
	}

	// Clean up last material
	if(m_pCurrentMaterial)
	{
		m_poVertCache->flush();
		m_dwCurrentPass = 0;
		pDev->SetTexture(0, NULL);
		m_pCurrentMaterial->end();
		m_pCurrentMaterial = NULL;
	}
}