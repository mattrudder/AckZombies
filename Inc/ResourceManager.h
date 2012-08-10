/**
* @file ResourceManager.h
* @author Matt Rudder
* @date Created Mar 30, 2006
*
* Manager class responsible for all engine file resources.
*/

#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

// Local includes
#include "Singleton.h"

// Engine includes
#include "AcespaceEngine.h"
#include "ResourceTypes.h"

// System includes
#include <map>
#include <list>

// Forward declarations
class CBaseResource;

/**
* Responsible for the loading and management of engine resources.
*
* @author Matt Rudder
* @date Created Mar 30, 2006
* @date Modified Mar 30, 2006
*/
class CResourceManager : public CSingleton<CResourceManager>
{
	friend class CSingleton<CResourceManager>;
	friend class CBaseResource;
public:

	// Types
	typedef CBaseResource* (*ResourceCreationFunc)(const CString);
	typedef std::multimap<CString, CBaseResource*> ResourceList;
	struct SResourceCreation
	{
		//! ResourceType Identifier
		EResourceType eResourceTypeID;
		//! Function pointer to the resource creation function.
		ResourceCreationFunc poCreationFunction;
		//! Media subdirectory this resource
		CString sSubdir;
	};
	typedef std::map<EResourceType, CResourceManager::SResourceCreation> ResourceCreationList;

protected:
	//! Singleton stuff.
	//!@{
	CResourceManager(void) {}
	CResourceManager(const CResourceManager&) {}
	operator=(const CResourceManager&){}
	virtual ~CResourceManager(void);
	//!@}

	//! Collection of currently active resources
	ResourceList m_mResources;
	//! Collection of supported resource types, and their creation functions
	std::map<EResourceType, SResourceCreation> m_mResourceCreation;
	
	/**
	* Removes a resource from the internal list, and deallocates it.
	*
	* @date Created Mar 30, 2006
	* @param[in]	pRemove		Pointer to the resource to remove.
	*/
	void deallocResource(CBaseResource* pRemove);

public:

	/**
	* Removes all resources from the internal list, and deallocates it.
	*
	* @date Created June 2, 2006
	*/
	void CResourceManager::deAllocAllResources(void);

	/**
	* Notify the resource manager about the types of available resources in 
	* the game.
	*
	* @date Created Mar 10, 2006
	* @param[in]	pCreate		The array of Resource Creation structures.
	* @param[in]	unNumTypes	Number of items in pCreate.
	*/
	void registerResourceTypes(SResourceCreation* pCreate, unsigned int unNumTypes);

	/**
	* Searches the media directories for the specified file, and returns
	* its absolute path.
	*
	* @date Created Mar 30, 2006
	* @param[in]	eResType		The type of resource. Used to lookup subdirectory.
	* @param[in]	sFile			The file to search for.
	* @param[out]	sAbsolutePath	The path to sFile, if found.
	* @return true if sFile sFile was successfully found, false otherwise.
	* @remark Expects CGameKernel to have loaded the game config file.
	*/
	static bool searchMediaPaths(EResourceType eResType, CString sFile, CString& sAbsolutePath);

	/**
	* Loads the specified resource if it is not currently in memory.
	*
	* @date Created Mar 30, 2006
	* @param[in]	eResType	The type of resource sFilename represents.
	* @param[in]	sFilename	The file path to the resource.
	* @param[in]	bAbsolute	Specifies whether the file path given is 
	*				relative (in a media directory) or absolute.
	* @return Pointer to the new object
	* @remark If the resource was previously loaded, this function increments
	*		  the resource's reference count.
	*/
	CBaseResource* loadResource(EResourceType eResType, CString sFilename, bool bAbsolute = false);
};

#endif //_RESOURCEMANAGER_H_