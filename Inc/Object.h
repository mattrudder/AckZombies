/**
* @file Object.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Base object within the game engine. All game objects should derive from this
* type.
*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

// Local includes
#include "ObjectTypes.h"

/**
* Base object within the game engine. All game objects should derive from this
* type.
*
* @author Matt Rudder
* @date Created Mar 06, 2006
* @date Modified May 11, 2006
*/
class CObject
{
	friend class CObjectManager;
protected:
	//! Object Type ID
	EObjectType m_eType;
	//! Determines if the object is active in the game world
	bool m_bActive;
	//! Determines if the object should be deleted on next update
	bool m_bDelete;
public:

	/**
	* Constructor
	*
	* @date Created Mar 06, 2006
	*/
	CObject(EObjectType eType = OBJ_UNKNOWN);

	/**
	* Destructor
	*
	* @date Created Mar 06, 2006
	*/
	virtual ~CObject(void);

	/**
	* Returns the type id of the object.
	*
	* @date Created Mar 17, 2006
	* @return The type of the object.
	*/
	EObjectType getType(void);

	/**
	* Performs object dependent updates
	*
	* @date Created Mar 17, 2006
	*/
	virtual void update(void);

	/**
	* Allows the object to receive updates.
	* 
	* @date Created Mar 07, 2006
	*/
	void activate(void);

	/**
	* Turns the active bool off in the object
	* 
	* @date Created Mar 07, 2006
	*/
	void deactivate(void);

	/**
	* Checks to see if the object is active.
	* 
	* @date Created Mar 07, 2006
	* @return true if the object is active, false otherwise.
	*/
	bool isActive(void);

	/**
	* Checks to see if the object is marked for deletion.
	* 
	* @date Created Apr 26, 2006
	* @return true if the object is marked for deletion, false otherwise.
	*/
	bool isDeleted(void) { return m_bDelete; }

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* Used to notify objects of removal from the object manager's active list.
	*
	* @date Created May 26, 2006
	*/
	virtual void destroy(void);
};

#endif //_OBJECT_H_