/**
* @file VertexCache.cpp
* @author Matt Rudder
* @date Modified Apr 25, 2006
*
* Handles the batching of loose primitives, as found in a spatial partitioning
* schemes or particle systems.
*/

// Local includes
#include "VertexCache.h"

// Engine includes
#include "RenderSystem.h"
#include "Functor.h"

/**
* CVertexCache::CVertexCache
* @date Modified Apr 25, 2006
*/
CVertexCache::CVertexCache(DWORD dwMaxVerts, DWORD dwStride, DWORD dwMaxIndex, D3DVERTEXELEMENT9* pDecl, D3DPRIMITIVETYPE d3dpt)
	: m_ePrimType(d3dpt), m_dwMaxVerts(dwMaxVerts), m_dwStride(dwStride), m_dwMaxIndex(dwMaxIndex),
	  m_pVB(NULL), m_pIB(NULL), m_dwNumIndex(0),  m_dwNumVerts(0), m_pVertex(NULL), m_pIndex(NULL),
	  m_pPreRenderFunc(NULL), m_pPostRenderFunc(NULL)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	pDev->CreateVertexBuffer(dwMaxVerts * dwStride, 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_pVB, NULL);
	pDev->CreateIndexBuffer(dwMaxIndex*sizeof(WORD), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
	pDev->CreateVertexDeclaration(pDecl, &m_pVertexDecl);
}

/**
* CVertexCache::~CVertexCache
* @date Modified Apr 25, 2006
*/
CVertexCache::~CVertexCache(void)
{
	COM_RELEASE(m_pVB);
	COM_RELEASE(m_pIB);
	COM_RELEASE(m_pVertexDecl);
}

/**
* CVertexCache::begin
* @date Modified Apr 25, 2006
*/
bool CVertexCache::begin(void)
{
	// Initialize Device data
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	pDev->SetVertexDeclaration(m_pVertexDecl);
	pDev->SetStreamSource(0, m_pVB, 0, m_dwStride);
	pDev->SetIndices(m_pIB);

	// Reset status
	m_dwNumVerts = m_dwNumIndex = 0;

	// Lock buffers
	m_pVertex = NULL;
	if(FAILED(m_pVB->Lock(0, 0, (LPVOID*)&m_pVertex, D3DLOCK_DISCARD)))
		return false;
	m_pIndex = NULL;
	if(FAILED(m_pIB->Lock(0, 0, (LPVOID*)&m_pIndex, D3DLOCK_DISCARD)))
		return false;
	return true;
}

/**
* CVertexCache::flush
* @date Modified Apr 25, 2006
*/
bool CVertexCache::flush(void)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	// Unlock buffers
	if(m_pVertex)
	{
		m_pVB->Unlock();
		m_pVertex = NULL;
	}
	if(m_pIndex)
	{
		m_pIB->Unlock();
		m_pIndex = NULL;
	}

	bool bContinueRendering = true;
	HRESULT hr = S_OK;
	while(bContinueRendering)
	{
		// Call pre-render function, if set
		if(m_pPreRenderFunc)
			(*m_pPreRenderFunc)();

		// Check for tris
		if(m_dwNumIndex && m_dwNumVerts)
		{
			// Draw correct primitive type
			switch(m_ePrimType)
			{
			case D3DPT_TRIANGLELIST:
				hr = pDev->DrawIndexedPrimitive(m_ePrimType, 0, 0, m_dwNumVerts, 0, m_dwNumIndex/3);
				break;
			case D3DPT_TRIANGLESTRIP:
			case D3DPT_TRIANGLEFAN:
				hr = pDev->DrawIndexedPrimitive(m_ePrimType, 0, 0, m_dwNumVerts, 0, m_dwNumIndex-2);
				break;
			case D3DPT_LINELIST:
				hr = pDev->DrawIndexedPrimitive(m_ePrimType, 0, 0, m_dwNumVerts, 0, m_dwNumIndex/2);
				break;
			case D3DPT_LINESTRIP:
				hr = pDev->DrawIndexedPrimitive(m_ePrimType, 0, 0, m_dwNumVerts, 0, m_dwNumIndex-1);
				break;
			case D3DPT_POINTLIST:
				hr = pDev->DrawPrimitive(m_ePrimType, 0, m_dwNumIndex);
				break;
			}
		}

		// Call post-render function, if set
		if(m_pPostRenderFunc)
			bContinueRendering = (*m_pPostRenderFunc)();
		else
			bContinueRendering = false;
	}


	// Reset counts
	m_dwNumIndex = m_dwNumVerts = 0;

	// Return status
	return FAILED(hr);
}

/**
* CVertexCache::insert
* @date Modified Apr 25, 2006
*/
bool CVertexCache::insert(LPVOID pVerts, DWORD dwNumVerts, unsigned short* pIndex, DWORD dwNumIndex)
{	
	// Flush if the buffers are full.
	if(m_dwNumVerts + dwNumVerts > m_dwMaxVerts || m_dwNumIndex + dwNumIndex > m_dwMaxIndex)
	   flush();

	// Lock buffers
	if(!m_pVertex)
		if(FAILED(m_pVB->Lock(0, 0, (LPVOID*)&m_pVertex, D3DLOCK_DISCARD)))
			return false;
	if(!m_pIndex)
		if(FAILED(m_pIB->Lock(0, 0, (LPVOID*)&m_pIndex, D3DLOCK_DISCARD)))
			return false;

	// Copy vertex data
	memcpy(m_pVertex + (m_dwStride*m_dwNumVerts), pVerts, dwNumVerts * m_dwStride);

	// Copy index data
	for(DWORD i = 0; i < dwNumIndex; ++i)
		m_pIndex[m_dwNumIndex++] = (unsigned short)((pIndex ? pIndex[i] : i) + m_dwNumVerts);

	// Add to vertex count
	m_dwNumVerts += dwNumVerts;

	// Return success.
	return true;
}