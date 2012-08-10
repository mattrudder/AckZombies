/**
* @file ResourceManager.cpp
* @author Matt Rudder
* @date Modified Mar 30, 2006
*
* Manager class responsible for all engine file resources.
*/

// Local includes
#include "ResourceManager.h"

// Engine includes
#include "AcespaceEngine.h"
#include "GameKernel.h"
#include "BaseResource.h"
#include "ConfigFile.h"

// System includes
#include <tchar.h>

/**
* CResourceManager::deallocResource
* @date Modified Mar 30, 2006
*/
void CResourceManager::deallocResource(CBaseResource* pRemove)
{
	ResourceList::iterator itRes = m_mResources.begin();
	CBaseResource* pRes;
	while(itRes != m_mResources.end())
	{
		pRes = itRes->second;
		if(pRes == pRemove)
		{
			m_mResources.erase(itRes);
			delete pRes;
			return;
		}
		++itRes;
	}
}
/**
* CResourceManager::~CResourceManager
* @date Modified Mar 30, 2006
*/
CResourceManager::~CResourceManager(void)
{
	// Deallocate all resources.
	ResourceList::iterator itRes = m_mResources.begin();
	CBaseResource* pRes;
	while(itRes != m_mResources.end())
	{
		pRes = itRes->second;
#ifdef _DEBUG
		if(pRes && pRes->m_ulRefCount > 0)
		{
			Debug << "WARNING: Resource \"" << pRes->toString()
				  << "\" has " << pRes->m_ulRefCount << " reference(s) remaining!" << endl;
		}
#endif

		delete pRes;
		itRes->second = NULL;
		
		++itRes;
	}
	m_mResources.clear();
}

/**
* CResourceManager::deAllocAllResources
* @date Modified June 2, 2006
*/
void CResourceManager::deAllocAllResources(void)
{
	// Deallocate all resources.
	ResourceList::iterator itRes = m_mResources.begin();
	CBaseResource* pRes;
	while(itRes != m_mResources.end())
	{
		pRes = itRes->second;
#ifdef _DEBUG
		if(pRes && pRes->m_ulRefCount > 0)
		{
			Debug << "WARNING: Resource \"" << pRes->toString()
				  << "\" has " << pRes->m_ulRefCount << " reference(s) remaining!" << endl;
		}
#endif

		delete pRes;
		itRes->second = NULL;
		
		++itRes;
	}
	m_mResources.clear();
}

/**
* CResourceManager::registerResourceTypes
* @date Modified Mar 30, 2006
*/
void CResourceManager::registerResourceTypes(SResourceCreation* pCreate, unsigned int unNumTypes)
{
	// Clear the previous types.
	m_mResourceCreation.clear();

	// Add the new types.
	for(unsigned int i = 0; i < unNumTypes; ++i)
		m_mResourceCreation[pCreate[i].eResourceTypeID] = pCreate[i];
}

/**
* CResourceManager::searchMediaPaths
* @date Modified Mar 30, 2006
*/
bool CResourceManager::searchMediaPaths(EResourceType eResType, CString sFile, CString& sAbsolutePath)
{
	CConfigFile& cfgFile = CGameKernel::getInstance().getConfig();

	// Get file portion only
	std::vector<CString> vFileParse;
	sFile.ToList(vFileParse, "\\/");
	sFile = vFileParse[vFileParse.size()-1];

	// Get media paths from config
	CString sMediaPath = cfgFile.GetValue("General", "MediaPath");
	vector<CString> vPaths;
	if(sMediaPath != CString::Blank)
		sMediaPath.ToList(vPaths, _T(",;\n\t"));

	// Add default media path.
	vPaths.push_back("..\\");

	// Iterate paths, searching for the file.
	HANDLE hFile = INVALID_HANDLE_VALUE;
	for(size_t i = 0; i < vPaths.size(); ++i)
	{
		// Make sure there are trailing slashes
		if(vPaths[i].GetChar(vPaths[i].GetLength() - 1) != _T('\\'))
			vPaths[i] += _T("\\");

		// Get subdirectory
		ResourceCreationList::const_iterator itResCreate = CResourceManager::getInstance().m_mResourceCreation.find(eResType);
		if(itResCreate == CResourceManager::getInstance().m_mResourceCreation.end())
			return NULL;
		vPaths[i] += itResCreate->second.sSubdir + "\\" + sFile;

		WIN32_FIND_DATA fd;
		hFile = FindFirstFile(vPaths[i].GetBuffer(), &fd);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			sAbsolutePath = vPaths[i];
			FindClose(hFile);
			return true;
		}
	}
	return false;
}

/**
* CResourceManager::loadResource
* @date Modified Mar 30, 2006
*/
CBaseResource* CResourceManager::loadResource(EResourceType eResType, CString sFilename, bool bAbsolute)
{
	// Get path if needed
	if(!bAbsolute)
	{
		if(!searchMediaPaths(eResType, sFilename, sFilename))
			return NULL;
	}

	// Determine if the file is already loaded. If so, 'clone' it.
	ResourceList::iterator itRes = m_mResources.find(sFilename);
	if(itRes != m_mResources.end())
	{
		CBaseResource* pRes = itRes->second,
					 * pClone = NULL;

		if(pRes)
			pClone = pRes->cloneResource();

		// Save clone if the clone operation generated a unique copy.
		if(pRes != pClone)
			m_mResources.insert(itRes, ResourceList::value_type(sFilename, pClone));

		pClone->addRef();
		return pClone;
	}

	// Get creation function
	CBaseResource* pRes = NULL;
	ResourceCreationList::const_iterator itResCreate = m_mResourceCreation.find(eResType);
	if(itResCreate == m_mResourceCreation.end())
		return NULL;

	// Create object
	if(itResCreate->second.poCreationFunction)
		pRes = itResCreate->second.poCreationFunction(sFilename);
	
	// Save and return
	if(pRes)
	{
		pRes->addRef();
		m_mResources.insert(ResourceList::value_type(sFilename, pRes));
		return pRes;
	}
	
	return NULL;
}