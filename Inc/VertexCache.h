/**
* @file VertexCache.h
* @author Matt Rudder
* @date Created Apr 25, 2006
*
* Handles the batching of loose primitives, as found in a spatial partitioning
* schemes or particle systems.
*/

#ifndef _VERTEXCACHE_H_
#define _VERTEXCACHE_H_

// System includes
#include <d3d9.h>

// Engine includes
#include "Functor.h"

/**
* Handles the batching of loose primitives, as found in a spatial partitioning
* schemes or particle systems.
*
* @author Matt Rudder
* @date Created Apr 25, 2006
* @date Modified Apr 25, 2006
* @remark Does not handle the switching of texture/shader values.
*/
class CVertexCache
{
	//! Vertex buffer for use in batching.
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	//! Index buffer for use in batching.
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	//! Maximum number of vertices in a batch.
	DWORD m_dwMaxVerts;
	//! Maximum number of indices in a batch.
	DWORD m_dwMaxIndex;
	//! Current number of vertices in this batch.
	DWORD m_dwNumVerts;
	//! Current number of indices in this batch.
	DWORD m_dwNumIndex;
	//! Size of a single vertex
	DWORD m_dwStride;
	//! Pointers for buffer access
	unsigned short* m_pIndex;
	unsigned char* m_pVertex;
	//! Type of primitives contained within the cache
	D3DPRIMITIVETYPE m_ePrimType;
	//! Vertex declaration for the current buffer
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;
	//! Reference to a pre-render functor.
	CFunctor<void>* m_pPreRenderFunc;
	//! Reference to a post-render functor.
	CFunctor<bool>* m_pPostRenderFunc;
public:
	CVertexCache(DWORD dwMaxVerts, DWORD dwStride, DWORD dwMaxIndex, D3DVERTEXELEMENT9* pDecl, D3DPRIMITIVETYPE d3dpt = D3DPT_TRIANGLELIST);
	virtual ~CVertexCache(void);

	void setPreRenderFunc(CFunctor<void>* pFunc) { m_pPreRenderFunc = pFunc; }
	void setPostRenderFunc(CFunctor<bool>* pFunc) { m_pPostRenderFunc = pFunc; }

	/**
	* Notifies the cache that batching has begun. Allows for the locking of 
	* buffers and initialization of cache parameters.
	*
	* @date Created Apr 25, 2006
	* @return true if the operation completed successfully, false otherwise.
	*/
	bool begin(void);

	/**
	* Forces the render of any vertices currently in the cache.
	*
	* @date Created Apr 25, 2006
	* @return true if the operation completed successfully, false otherwise.
	*/
	bool flush(void);

	/**
	* Adds a collection of vertices to the cache.
	*
	* @date Created Apr 25, 2006
	* @param[in]	pVerts		Array of vertex data.
	* @param[in]	dwNumVerts	Number of items in pVerts.
	* @param[in]	pIndex		Array of index data.
	* @param[in]	dwNumIndex	Number of items in pIndex.
	* @remark NULL can be specified for the index list (this will tell the cache
	*	to treat the data as it is non-indexed) but dwNumIndex must be the same 
	*	as dwNumVerts in this case.
	*/
	bool insert(LPVOID pVerts, DWORD dwNumVerts, unsigned short* pIndex, DWORD dwNumIndex);
};

#endif //_VERTEXCACHE_H_