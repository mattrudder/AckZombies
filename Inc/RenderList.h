/**
* @file RenderList.h
* @author Matt Rudder
* @date Created Apr 25, 2006
*
* Defines a list of sorted renderable primitives.
*/

#ifndef _RENDERLIST_H_
#define _RENDERLIST_H_

// Engine includes
#include "Geometry.h"
#include "MethodFunctor.h"
#include "RadixSort.h"

// System includes
#include <vector>
#include <algorithm>
using std::vector;

class CVertexCache;

/**
* Defines a list of sorted renderable primitives.
*
* @author Matt Rudder
* @date Created Apr 25, 2006
* @date Modified Apr 25, 2006
*/
class CRenderList
{
	friend class CRenderGroup;
public:
	typedef vector<SRenderable*> RenderPrimitiveList;
protected:
	
	RenderPrimitiveList m_oPrimList;
	SRenderable** m_ppPrimList;
	DWORD m_dwCurrentIndex;
	DWORD m_dwMaxIndex;
	CVertexCache* m_poVertCache;
	CMaterial* m_pCurrentMaterial;
	DWORD m_dwCurrentPass;

	template<typename radix_ptr_type, typename radix_sort_func_type, typename sort_func_type>
		void sort(radix_ptr_type pSort, radix_sort_func_type pFunc, sort_func_type pStableFunc)
	{
		// Sort using radix sorter if the list size is greater than 2000, 
		// Otherwise, use a less-than sort
		if(m_oPrimList.size() > 2000)
			pSort->sort(m_oPrimList, pFunc);
		else
			std::stable_sort(m_oPrimList.begin(), m_oPrimList.end(), pStableFunc);
	}

	void calcViewDistance(D3DXVECTOR3& vEye)
	{
		RenderPrimitiveList::iterator itTris;
		for(itTris = m_oPrimList.begin(); itTris != m_oPrimList.end(); ++itTris)
			(*itTris)->calcSquaredViewDistance(vEye);
	}

public:
	CRenderList(DWORD dwMax = 1024);
	virtual ~CRenderList(void);

	/**
	* Adds geometry primitives to the internal group.
	*
	* @date Created Apr 25, 2006
	* @param[in]	pRender		The primitive to store.
	*/
	void addRenderable(SRenderable* pRender);

	void clear(void) { m_dwCurrentIndex = 0; m_oPrimList.clear(); }

	/**
	* Renders all items currently queued.
	*
	* @date Created Apr 25, 2006
	*/
	void renderAll(void);
};

#endif //_RENDERLIST_H_