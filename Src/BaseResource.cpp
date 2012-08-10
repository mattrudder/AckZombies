/**
* @file BaseResource.cpp
* @author Matt Rudder
* @date Modified Mar 30, 2006
*
* Base interface for all engine file resources.
*/


// Local includes
#include "BaseResource.h"

// Engine includes
#include "ResourceManager.h"

/**
* CBaseResource::CBaseResource
* @date Modified Mar 30, 2006
*/
CBaseResource::CBaseResource(void) : m_ulRefCount(0) {}

/**
* CBaseResource::cloneResource
* @date Modified Mar 31, 2006
*/
CBaseResource* CBaseResource::cloneResource(void)
{
	return this;
}

/**
* CBaseResource::toString
* @date Modified Apr 1, 2006
*/
CString CBaseResource::toString(void) const
{
	return "{BaseResource}";
}

/**
* CBaseResource::addRef
* @date Modified Mar 30, 2006
*/
void CBaseResource::addRef(void)
{
	++m_ulRefCount;
}

/**
* CBaseResource::Release
* @date Modified Mar 30, 2006
*/
void CBaseResource::Release(void)
{
	--m_ulRefCount;

#ifdef _DEBUG
	if(m_ulRefCount == ULONG_MAX)
		assert(false && "Resource not addref'd but released!");
#endif
	
	// Notify manager of release, if count is at zero.
	if(!m_ulRefCount)
		CResourceManager::getInstance().deallocResource(this);
}