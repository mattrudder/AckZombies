/**
* @file PlacementState.h
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
*
* the editor state
*/ 

#ifndef _PLACEMENTSTATE_H_
#define _PLACEMENTSTATE_H_

// Local Includes
#include "GameState.h"
#include "DirectInputWrapper.h"
#include "CommandConsole.h"
#include "ResourceManager.h"
#include "ObjectTypes.h"
#include "TickTimer.h"
#include "ActorIncludes.h"
#include "ActorSpawn.h"
#include "ObjectiveTrigger.h"

// System Includes
#include <map>
#include <list>
#include <fstream>
using std::ifstream;

/**
* Actor Placement Functionallity deally
*
* @author Jonathan "Awesome" Zimmer
* @date Created April 27, 2006
* @date Modified June 2, 2006
*/
class CPlacementState : public CGameState<CPlacementState>
{
public:
	//! the tooltip to give the next objective that is spawned
	CString m_strToolTip;
private:

	//! are we editing an item that has already been placed
	bool m_bEditing;

	//! the current number of the objectives
	unsigned int m_unCurrentObjectiveNumber;

	//! should the nodes and spawn triggers be drawn
	bool m_bDrawNodesSpawnTriggers;
	
	//! should the objectives triggers and barriers be drawn
	bool m_bDrawObjectiveTriggersBarriers;

	//! The node radius sphere
	ID3DXMesh* m_pSphere;

	//! for drawing the box triggers
	ID3DXMesh* m_pBox;

	//! The cursor to move around and place stuff
	CActor* m_poCursor, *m_poStoredCursor;

	//! The selected type of actor to create
	EObjectType m_eCurrentType;

	//! The current attribute
	unsigned int m_unIndex;

	//! Stores actor's attributes
	std::vector<unsigned int> m_vunAttributes;
	//! Stores the strings that associate with the attributes, first string is the name of current type
	std::vector<CString> m_vunAttributeNames;

	//! stores all the types that can be placed
	std::map<int, std::list<EObjectType> > m_mapOfListsOfTypes;

	//! Whether or not to render the collision geometry
	bool m_bCollisionGeometry;

	//! for unbuffered increasing of the current attribute
	CTickTimer m_tUnbufferedIncrease;

	//! Level Geometry
	CMesh* m_pMesh2;
	//! Collision Geometry
	CMesh* m_pMesh3;
	//! The font to draw to the screen with
	CBitmapFont* m_fntDina;
	//! Display Font
	CBitmapFont* m_fntDinaFPS;

	/**
	* inits the attributes and names
	*
	* @date Created April 29, 2006
	*/
	void initAttributes(void);

	/**
	* checks for input that would change the current type and changes current type
	*
	* @date Created April 30, 2006
	*/
	void checkChangeInType(void);

public:

	CPlacementState(void);
	~CPlacementState(void);

	/**
	* Called by the kernel when the active state is switched to this state. 
	* This allows the state to perform any first frame initialization.
	*
	* @date Created April 27, 2006
	*/
	virtual void onActivate(void);

	/**
	* Called by the kernel when the active state is switched away from this 
	* state. This allows the state to perform any cleanup needed from its 
	* onActivate call.
	*
	* @date Created April 27, 2006
	*/
	virtual void onDeactivate(void);

	/**
	* Called each frame within the kernel's tick function, while this is the 
	* active state.
	*
	* @date Created April 27, 2006
	*/
	virtual void tick(void);

	/**
	* Called each frame within the render system, just before world/actor 
	* geometry is rendered while this is the active state.
	*
	* @date Created April 27, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void preRender(CRenderDevice& oRenderDevice);

	/**
	* Called each frame within the render system, just after world/actor 
	* geometry is rendered but before the scene is presented while this 
	* is the active state.
	*
	* @date Created April 27, 2006
	* @param[in]	oRenderDevice	The device currently in use.
	*/
	virtual void postRender(CRenderDevice& oRenderDevice);

	/**
	* toggle
	*
	* @date Created June 2, 2006
	*/
	inline void toggleCollisionGeometry(void) { m_bCollisionGeometry = !m_bCollisionGeometry; }

	/**
	* toggle
	*
	* @date Created June 2, 2006
	*/
	inline void toggleDrawNodesSpawnTriggers(void) { m_bDrawNodesSpawnTriggers = !m_bDrawNodesSpawnTriggers; }

	/**
	* toggle
	*
	* @date Created June 2, 2006
	*/
	inline void toggleDrawObjectiveTriggerBarriers(void) { m_bDrawObjectiveTriggersBarriers = !m_bDrawObjectiveTriggersBarriers; }

	/**
	* Exports the node in the object manager
	*
	* @date Created April 29, 2006
	*/
	void exportNodes(void);

	/**
	* Exports the enemies in the object manager
	*
	* @date Created April 30, 2006
	*/
	void exportEnemies(void);

	/**
	* Exports the enemy spawns in the object manager
	*
	* @date Created April 30, 2006
	*/
	void exportEnemySpawns(void);

	/**
	* Exports the spawn triggers in the object manager
	*
	* @date Created May 9, 2006
	*/
	void exportSpawnTriggers(void);

	/**
	* Exports the objective triggers in the object manager
	*
	* @date Created June 2, 2006
	*/
	void exportObjectiveTriggers(void);

	/**
	* Exports the objective barriers in the object manager
	*
	* @date Created June 3, 2006
	*/
	void exportObjectiveBarriers(void);

	/**
	* Allows access to edit the closest currently placed item of the current type
	*
	* @date Created June 5, 2006
	*/
	void editClosestCurrentType(void);

	/**
	* Exports the lights in the objectmanager
	*
	* @date Created June 8, 2006
	*/
	void exportLights(void);
};

#endif /*_PLACEMENTSTATE_H_*/