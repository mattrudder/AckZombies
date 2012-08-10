/**
* @file RenderGroup.h
* @author Matt Rudder
* @date Created Apr 25, 2006
*
* Defines a list of organized groups of geometry by material specification.
*/

#ifndef _RENDERGROUP_H_
#define _RENDERGROUP_H_

// Engine includes
#include "Geometry.h"
#include "RenderList.h"
#include "MethodSingleFunctor.h"
#include "MethodBinaryFunctor.h"

#include <d3dx9.h>

/**
* Defines a list of organized groups of geometry by material specification.
*
* @author Matt Rudder
* @date Created Apr 25, 2006
* @date Modified Apr 25, 2006
*/
class CRenderGroup
{
public:
	struct SDepthSort
	{
		bool operator()(SRenderable* pLeft, SRenderable* pRight)
		{
		}
	};
protected:
	//! Geometry grouping.
	CRenderList m_oSolidsStatic, m_oTransparentStatic;
	//! Functors for getting sort values
	CMethodSingleFunctor<CRenderGroup, SRenderable*, float> m_oTransFunc;
	CMethodSingleFunctor<CRenderGroup, SRenderable*, unsigned int> m_oSolidFunc;
	CMethodBinaryFunctor<CRenderGroup, SRenderable*, SRenderable*, bool> m_oSolidLessFunc, m_oTransLessFunc;
	// View position
	D3DXVECTOR3* m_pEye;
public:
	CRenderGroup(DWORD dwMaxSolidSize, DWORD dwMaxTransSize);
	virtual ~CRenderGroup(void);

	float _renderSort_getViewDistance(SRenderable* pRender);
	CMaterial* _renderSort_getMaterialPtr(SRenderable* pRender);
	unsigned int _renderSort_getMaterialID(SRenderable* pRender);
	bool _renderSortLess_byViewDistance(SRenderable* pLeft, SRenderable* pRight);
	bool _renderSortLess_byMaterialID(SRenderable* pLeft, SRenderable* pRight);
	
	/**
	* Adds geometry primitives to the appropriate material group.
	*
	* @date Created Apr 25, 2006
	* @param[in]	pRender		The primitive to store.
	*/
	void addRenderable(SRenderable* pRender);
	void addRenderableArray(SRenderable* pRender, DWORD dwNumPrimitives);

	void clear(void) { m_oSolidsStatic.clear(); m_oTransparentStatic.clear(); }

	/**
	* Renders all items currently queued.
	*
	* @date Created Apr 25, 2006
	*/
	void renderAll(D3DXVECTOR3& vEye, CRadixSort<CRenderList::RenderPrimitiveList, SRenderable*, unsigned int>* pSortSolid, 
		CRadixSort<CRenderList::RenderPrimitiveList, SRenderable*, float>* pSortTrans);
};

#endif //_RENDERGROUP_H_