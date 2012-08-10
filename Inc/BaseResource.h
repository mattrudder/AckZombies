/**
* @file BaseResource.h
* @author Matt Rudder
* @date Created Mar 30, 2006
*
* Base interface for all engine file resources.
*/

#ifndef _BASERESOURCE_H_
#define _BASERESOURCE_H_

// Engine includes
#include "AcespaceEngine.h"

/**
* Base class for all engine file resources. Implements reference counting for
* object reuse.
*
* @author Matt Rudder
* @date Created Mar 30, 2006
* @date Modified Mar 30, 2006
*/
class CBaseResource
{
protected:
	friend class CResourceManager;

	//! Reference counter
	unsigned long m_ulRefCount;

public:
	CBaseResource(void);
	virtual ~CBaseResource(void) {}

	/**
	* Called by the Resource Manager when a previously loaded resource is 
	* requested again. Derived classes may override the default behavior for
	* any special needs cases, where the same object should not be given out
	* multiple times.
	*
	* @date Created Mar 31, 2006
	* @return The original object.
	*/
	virtual CBaseResource* cloneResource(void);

	/**
	* Allows a resource to output information about itself for debugging purposes
	*
	* @date Created Apr 01, 2006
	* @return Description string
	*/
	virtual CString toString(void) const;

	/**
	* Adds to the internal reference count.
	*
	* @date Created Mar 30, 2006
	*/
	void addRef(void);

	/**
	* Decrements the internal reference count and deallocates the object if 
	* it is no longer in use.
	*
	* @date Created Mar 30, 2006
	*/
	void Release(void);

	/**
	* Returns the current reference count of the resource.
	*
	* @date Created Jun 09, 2006
	* @return The resource reference count.
	*/
	unsigned long getRefCount(void) { return m_ulRefCount; }
};

#endif //_BASERESOURCE_H_