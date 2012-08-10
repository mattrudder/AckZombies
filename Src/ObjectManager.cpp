/**
* @file ObjectManager.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Details about the file, and what is included.
*/ 

// Local includes
#include "ObjectManager.h"

/**
* CObjectManager::registerObjectTypes
* @date Modified Mar 13, 2006
*/
void CObjectManager::registerObjectTypes(SObjectCreation* pCreate, unsigned int unNumTypes)
{
	// Clear the previous types.
	m_mObjectCreation.clear();

	// Add the new types.
	for(unsigned int i = 0; i < unNumTypes; ++i)
		m_mObjectCreation[pCreate[i].eObjectTypeID] = pCreate[i].poCreationFunction;
}

/**
* CObjectManager::~CObjectManager
* @date Modified May 17
*/
CObjectManager::~CObjectManager(void)
{
	deallocAllObjectsPlusDeadList();
}

/**
* CObjectManager::createObject
* @date Modified Mar 13, 2006
*/
CObject* CObjectManager::createObject(EObjectType eType)
{
	if(m_mObjectCreation[eType])
	{
		CObject* pNewObj = newObject(eType);
		if(m_mObjects.find(eType) != m_mObjects.end())
		{
			m_mObjects[eType].push_back(pNewObj);
		}
		else
		{
			std::list<CObject*> lNewObjs;
			lNewObjs.push_back(pNewObj);
			m_mObjects[eType] = lNewObjs;
		}
		return pNewObj;
	}
	return NULL;
}

/**
* CObjectManager::deallocAllObjects
* @date Modified May 17, 2006
*/
void CObjectManager::deallocAllObjects(void)
{
	// Clear all items from the object list
	TypedObjectList::iterator itObjectMap = m_mObjects.begin();
	ObjectList::iterator itObjectList;
	while(itObjectMap != m_mObjects.end())
	{
		itObjectList = itObjectMap->second.begin();
		while(itObjectList != itObjectMap->second.end())
		{
			deleteObject((*itObjectList));
			++itObjectList;
		}
		itObjectMap->second.resize(0);
		itObjectMap++;
	}
}

/**
* CObjectManager::deallocAllObjectsPlusDeadList
* @date Modified June 2, 2006
*/
void CObjectManager::deallocAllObjectsPlusDeadList(void)
{
	TypedObjectList::iterator itObjectMap = m_mObjects.begin();
	ObjectList::iterator itObjectList;
	while(itObjectMap != m_mObjects.end())
	{
		itObjectList = itObjectMap->second.begin();
		while(itObjectList != itObjectMap->second.end())
		{
			delete (*itObjectList);
			(*itObjectList) = NULL;
			++itObjectList;
		}
		itObjectMap->second.resize(0);
		++itObjectMap;
	}

	itObjectMap = m_mDeadObjects.begin();
	while(itObjectMap != m_mDeadObjects.end())
	{
		itObjectList = itObjectMap->second.begin();
		while(itObjectList != itObjectMap->second.end())
		{
			delete (*itObjectList);
			(*itObjectList) = NULL;
			++itObjectList;
		}
		itObjectMap->second.resize(0);
		++itObjectMap;
	}
}

/**
* CObjectManager::getObjects
* @date Modified Mar 13, 2006
*/
void CObjectManager::getObjects(EObjectType eType, ObjectList* pObjectList)
{
	// Get requested and all derived object types.
	TypedObjectList::iterator itObjMap = m_mObjects.begin();
	pObjectList->clear();
	while(itObjMap != m_mObjects.end())
	{
		
		if((itObjMap->first & eType) == eType)
		{
			ObjectList::iterator itObjList = itObjMap->second.begin();
			while(itObjList != itObjMap->second.end())
			{
				pObjectList->push_back(*itObjList);
				++itObjList;
			}

		}
		++itObjMap;
	}
}

/**
* CObjectManager::updateAllObjects
* @date Modified Mar 17, 2006
*/
void CObjectManager::updateAllObjects(void)
{
	// Update all items in the object list
	TypedObjectList::iterator itObjectMap = m_mObjects.begin();
	ObjectList::iterator itObjectList;
	while(itObjectMap != m_mObjects.end())
	{
		itObjectList = itObjectMap->second.begin();
		while(itObjectList != itObjectMap->second.end())
		{
			if((*itObjectList)->isDeleted())
			{
				deleteObject(*itObjectList);
				itObjectList = itObjectMap->second.erase(itObjectList);
			}
			else
			{
				(*itObjectList)->update();
				++itObjectList;
			}
		}

		++itObjectMap;
	}
}

/**
* CObjectManager::updateType
* @date Modified Mar 17, 2006
*/
void CObjectManager::updateType(EObjectType eType)
{
	// Update all items of eType
	ObjectList& oObjList = m_mObjects[eType];
	ObjectList::iterator itObjectList = oObjList.begin();
	while(itObjectList != oObjList.end())
	{
		if((*itObjectList)->isDeleted())
		{
			deleteObject(*itObjectList);
			itObjectList = oObjList.erase(itObjectList);
		}
		else
		{
			(*itObjectList)->update();
			++itObjectList;
		}
	}
}

/**
* CObjectManager::removeObject
* @date Modified Apr 24, 2006
*/
void CObjectManager::removeObject(CObject* pObj)
{
	pObj->m_bDelete = true;
}

/**
* CObjectManager::removeObjectImmediate
* @date Modified Apr 30, 2006
*/
void CObjectManager::removeObjectImmediate(CObject* pObj)
{
	ObjectList& oObjList = m_mObjects[pObj->getType()];
	ObjectList::iterator itObjectList = oObjList.begin();
	while(itObjectList != oObjList.end())
	{
		if ((*itObjectList) == pObj)
		{
			deleteObject(pObj);
			itObjectList = oObjList.erase(itObjectList);
			return;
		}
		
		++itObjectList;
	}
}

/**
* CObjectManager::deleteObject
* @date Modified May 11, 2006
*/
void CObjectManager::deleteObject(CObject* pObj)
{
	if (pObj == NULL) return;

	// Notify object of removal.
	pObj->destroy();

	if(m_mDeadObjects.find(pObj->getType()) != m_mDeadObjects.end())
	{
		// if we need to keep it around
		if (m_mDeadObjects[pObj->getType()].size() < 100)
		{
			m_mDeadObjects[pObj->getType()].push_back(pObj);
			return;
		}
		
		// the dead list of that object is full so free the memory
		delete pObj;
	}
	else
	{
		std::list<CObject*> lNewObjs;
		lNewObjs.push_back(pObj);
		m_mDeadObjects[pObj->getType()] = lNewObjs;
	}
}

/**
* CObjectManager::newObject
* @date Modified May 11, 2006
*/
CObject* CObjectManager::newObject(EObjectType eType)
{
	if (!m_mDeadObjects[eType].empty())
	{
		CObject* pReuse = m_mDeadObjects[eType].back();
		m_mDeadObjects[eType].pop_back();
		pReuse->init();
		return pReuse;
	}

	// couldn't find it in the dead list so we have to make a new one
	CFunctor<CObject*>* pFunc = m_mObjectCreation[eType];
	CObject* pNewObj = (*pFunc)();
	pNewObj->init();
	return pNewObj;
}