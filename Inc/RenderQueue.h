/**
* @file RenderQueue.h
* @author Matt Rudder
* @date Created Apr 25, 2006
*
* Defines a list of renderable groups of geometry, and prioritizes them for
* rendering.
*/

#ifndef _RENDERQUEUE_H_
#define _RENDERQUEUE_H_

// Engine includes
#include "Geometry.h"
#include "RenderGroup.h"

// System includes
#include <map>
using std::map;

// Forward Declarations
class CRenderGroup;

/**
* Defines a list of renderable groups of geometry, and prioritizes them for
* rendering.
*
* @author Matt Rudder
* @date Created Apr 25, 2006
* @date Modified Apr 25, 2006
*/
class CRenderQueue
{
public:
	enum ERenderPriority
	{
		PRI_STATICWORLD = 10,
	};
protected:
	typedef std::less<unsigned short> ShortCmp;
	typedef map<unsigned short, CRenderGroup*, ShortCmp> RenderPriorityList;
	RenderPriorityList m_oGroups;
	//! 
	DWORD m_dwSolidSize, m_dwTransSize; 
	//! 
	CRadixSort<CRenderList::RenderPrimitiveList, SRenderable*, unsigned int> m_oSortSolid;
	CRadixSort<CRenderList::RenderPrimitiveList, SRenderable*, float> m_oSortTrans;
public:
	CRenderQueue(DWORD dwMaxSolidSize, DWORD dwMaxTransSize);
	virtual ~CRenderQueue(void);

	/**
	* Adds geometry primitives to the appropriate priority group.
	*
	* @date Created Apr 25, 2006
	* @param[in]	pRender		The primitive/model to add.
	* @param[in]	ePriority	The priority group to add the geometry to.
	*/
	void addRenderable(SRenderable* pRender, ERenderPriority ePriority = PRI_STATICWORLD);
	void addRenderableArray(SRenderable* pRender, DWORD dwNumPrimitives, ERenderPriority ePriority = PRI_STATICWORLD);

	/**
	* Clears primitives from the internal lists, but leaves the outer structure.
	*
	* @date Created Apr 25, 2006
	*/
	void clear(void);

	/**
	* Renders all items currently queued.
	*
	* @date Created Apr 25, 2006
	*/
	void renderAll(D3DXVECTOR3& vEye);
};

#endif //_RENDERQUEUE_H_