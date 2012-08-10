/**
* @file RenderQueue.cpp
* @author Matt Rudder
* @date Modified Apr 25, 2006
*
* Defines a list of renderable groups of geometry, and prioritizes them for
* rendering.
*/

// Local includes
#include "RenderQueue.h"
#include "RenderGroup.h"

/**
* CRenderQueue::CRenderQueue
* @date Modified Apr 25, 2006
*/
CRenderQueue::CRenderQueue(DWORD dwMaxSolidSize, DWORD dwMaxTransSize) : 
	m_dwSolidSize(dwMaxSolidSize), m_dwTransSize(dwMaxTransSize)
{
}

/**
* CRenderQueue::~CRenderQueue
* @date Modified Apr 25, 2006
*/
CRenderQueue::~CRenderQueue(void)
{
	RenderPriorityList::iterator iter = m_oGroups.begin();
	while(iter != m_oGroups.begin())
	{
		delete iter->second;
		iter->second = NULL;
		++iter;
	}
	m_oGroups.clear();
}

/**
* CRenderQueue::addRenderable
* @date Modified Apr 25, 2006
*/
void CRenderQueue::addRenderable(SRenderable* pRender, ERenderPriority ePriority)
{
	RenderPriorityList::iterator itPriority = m_oGroups.find(ePriority);
	CRenderGroup* pGroup = NULL;
	if(itPriority == m_oGroups.end())
	{
		// Priority does not exist; Create it.
		m_oGroups[ePriority] = pGroup = new CRenderGroup(m_dwSolidSize, m_dwTransSize);
	}
	else
		pGroup = itPriority->second;

	// Store renderable.
	pGroup->addRenderable(pRender);
}

/**
* CRenderQueue::addRenderableArray
* @date Modified Apr 25, 2006
*/
void CRenderQueue::addRenderableArray(SRenderable* pRender, DWORD dwNumPrimitives, ERenderPriority ePriority)
{
	RenderPriorityList::iterator itPriority = m_oGroups.find(ePriority);
	CRenderGroup* pGroup = NULL;
	if(itPriority == m_oGroups.end())
	{
		// Priority does not exist; Create it.
		m_oGroups[ePriority] = pGroup = new CRenderGroup(m_dwSolidSize, m_dwTransSize);
	}
	else
		pGroup = itPriority->second;

	// Store renderable.
	pGroup->addRenderableArray(pRender, dwNumPrimitives);
}

/**
* CRenderQueue::clear
* @date Modified Apr 25, 2006
*/
void CRenderQueue::clear(void)
{
	// Clear internal lists.
	RenderPriorityList::iterator itPriority = m_oGroups.begin();
	for(itPriority; itPriority != m_oGroups.end(); ++itPriority)
		itPriority->second->clear();
}

/**
* CRenderQueue::renderAll
* @date Modified Apr 25, 2006
*/
void CRenderQueue::renderAll(D3DXVECTOR3& vEye)
{
	RenderPriorityList::iterator itPriority = m_oGroups.begin();
	for(itPriority; itPriority != m_oGroups.end(); ++itPriority)
	{
		itPriority->second->renderAll(vEye, &m_oSortSolid, &m_oSortTrans);
		itPriority->second->clear();
	}

}