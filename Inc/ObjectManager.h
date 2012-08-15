/**
* @file ObjectManager.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Manages the creation, deletion and update of individual objects within the
* game world.
*/

#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

// Local includes
#include "Object.h"
#include "Singleton.h"
#include "Functor.h"
#include "ObjectTypes.h"

// System includes
#include <list>
#include <map>

/**
* Manages the creation, deletion and update of individual objects within the
* game world.
*
* @author Matt Rudder
* @date Created Mar 06, 2006
* @date Modified May 17, 2006
*/
class CObjectManager : public CSingleton<CObjectManager>
{
	friend class CSingleton<CObjectManager>;
public:
	struct SObjectCreation
	{
		//! Object Type Identifier
		EObjectType eObjectTypeID;

		//! Function pointer to the object creation function.
		CFunctor<CObject*>* poCreationFunction;
	};

	typedef std::list<CObject*> ObjectList;
	typedef std::map<EObjectType, ObjectList> TypedObjectList;
	typedef std::map<EObjectType, CFunctor<CObject*>*> ObjectCreationList;
protected:
	//! The global list of CObject derived classes.
	TypedObjectList m_mObjects;

	//! The global list of dead CObject derived classes
	TypedObjectList m_mDeadObjects;

	//! The list of object types the manager can create
	ObjectCreationList m_mObjectCreation;

	//! Singleton stuff.
	//!@{
	CObjectManager(void) {}
	CObjectManager(const CObjectManager&) {}
	CObjectManager& operator=(const CObjectManager&) { return *this; }
	virtual ~CObjectManager(void);
	//!@}

	/**
	* Removes a single object from the manager and puts it in the dead list
	*
	* @date Created May 11, 2006
	* @param[in]	pObj	The object to remove.
	*/
	void deleteObject(CObject* pObj);

	/**
	* Removes a single object from the dead list and returns it
	*
	* @date Created May 11, 2006
	* @param[in]	eType	The type of object you want.
	* @return		the object you wanted
	*/
	CObject* newObject(EObjectType eType);
public:

	/**
	* Notify the object manager about the types of available object types in 
	* the game.
	*
	* @date Created Mar 10, 2006
	* @param[in]	pCreate		The array of Object Creation structures.
	* @param[in]	unNumTypes	Number of items in pCreate.
	*/
	void registerObjectTypes(SObjectCreation* pCreate, unsigned int unNumTypes);

	/**
	* Creates an object, determined by the type of pointer passed into the function.
	*
	* @date Created Mar 06, 2006
	* @param[in]	eType		The type to create		
	* @return		The pointer to the new object.
	*/
	CObject* createObject(EObjectType eType);

	/**
	* Puts all the objects in the object manager into the dead objects
	*
	* @date Created Mar 13, 2006
	*/
	void deallocAllObjects(void);

	/**
	* Clears and deletes all objects in the object manager and dead list.
	*
	* @date Created June 2, 2006
	*/
	void deallocAllObjectsPlusDeadList(void);

	/**
	* Retrieves all objects of a given type from the manager.
	*
	* @date Created Mar 13, 2006
	* @param[in]	eType		The type to create
	* @param[in]	pObjectList	Pointer to a ObjectList to fill out with all the
	*							objects of the given type.
	*/
	void getObjects(EObjectType eType, ObjectList* pObjectList);

	/**
	* Updates all the objects within the object manager.
	*
	* @date Created Mar 17, 2006
	*/
	void updateAllObjects(void);

	/**
	* Updates all the objects within the object manager, of a given type
	*
	* @date Created Mar 17, 2006
	* @param[in]	eType		The type to update.
	*/
	void updateType(EObjectType eType);

	/**
	* Removes a single object from the manager.
	*
	* @date Created Apr 24, 2006
	* @param[in]	pObj	The object to remove.
	*/
	void removeObject(CObject* pObj);

	/**
	* Removes and deletes a single object from the manager.
	*
	* @date Created Apr 30, 2006
	* @param[in]	pObj	The object to remove.
	*/
	void removeObjectImmediate(CObject* pObj);

	/**
	* Iterates all objects with a given type and calls the provided function on 
	* them.
	*
	* @date Created May 26, 2006
	* @param[in]	eType	The type of object to return.
	* @param[in]	pFunc	The functor to call on each object. This function 
	*						assumes the functor takes a CObject pointer as a 
	*						parameter.
	*/
	template <typename Functor>
	void iterateObjects(EObjectType eType, Functor* pFunc)
	{
		// Get requested and all derived object types.
		TypedObjectList::iterator itObjMap = m_mObjects.begin();
		while(itObjMap != m_mObjects.end())
		{
			if((itObjMap->first & eType) == eType)
			{
				ObjectList::iterator itObjList = itObjMap->second.begin();
				while(itObjList != itObjMap->second.end())
				{
					(*pFunc)(*itObjList);
					++itObjList;
				}

			}
			++itObjMap;
		}
	}
};

#endif //_OBJECTMANAGER_H_