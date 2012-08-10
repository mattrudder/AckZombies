/**
* @file ObjectiveTrigger.h
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
*
* Contains the definition of the CObjectiveTrigger class
*/

#ifndef _OBJECTIVETRIGGER_H_
#define _OBJECTIVETRIGGER_H_

#include "Actor.h"
#include <list>
#include "limits.h"
#include "ObjectiveBarrier.h"


/**
* Defines an objective trigger location
*
* @author Jonathan "Awesome" Zimmer
* @date Created June 2, 2006
* @date Modified June 10, 2006
*/
class CObjectiveTrigger : public CActor
{
public:

	//! Defines the Objective Types
	enum EObjectiveTypes
	{
		OJT_GOTO,	//! just get to the objective
		OJT_TIMED,	//! the player must stay alive for the set time
		OJT_DUDES,	//! the player must kill the number of dudes before moving on
		OJT_MAX,
	};

	//! the barriers that this trigger has control over
	std::list<CObjectiveBarrier*> m_loBarriers;

	//! The type of this Objective this trigger is
	EObjectiveTypes m_eObjectiveType;

	//! the time to wait once triggered for stay
	float m_fCountdownTime, m_fCurrentTime;

	//! the number of enemies that need to be killed to move on
	unsigned int m_unNumDudes, m_unCurrentDudes;

	//! the radius of this trigger
	float m_fRadius;

	//! Used to determine if this objective trigger is a box or sphere, 0.0f is this is a sphere
	float m_fDepth;	

	//! whether or not the objective has been triggered and time/dude updates should begin
	bool m_bTriggered;

	//! does the objective finish the level
	bool m_bGoal;

	//! this Objective trigger's order in the level's objectives
	unsigned int m_unOrderNumber;

	//! the number of barrier spawns to control
	unsigned int m_unNumBarriers;

	//! What to display on the hud
	CString m_strToolTip;

	CObjectiveTrigger(void) : CActor(OBJ_OBJECTIVE_TRIGGER), m_eObjectiveType(OJT_GOTO), m_fCountdownTime(0.0f), m_fCurrentTime(0.0f),
		m_unNumDudes(0), m_unCurrentDudes(0), m_fDepth(0.0f), m_unOrderNumber(0), m_bGoal(false), m_bTriggered(false), m_unNumBarriers(0)
	{ this->deactivate(); }

	~CObjectiveTrigger(void) {}

	/**
	* update
	*
	* @date Created June 2, 2006
	*/
	virtual void update(void);

	/**
	* checks to see if the the trigger can be activated
	*
	* @date Created June 2, 2006
	* @return	whether or not the trigger is able to be activated
	*/
	bool checkTriggerActivate(void);

	/**
	* checks to see if the the trigger criteria are fulfilled
	*
	* @date Created June 2, 2006
	* @param[in]	bForceComplete	whether or not to force completion, for cheating
	* @return	whether or not the trigger is satisfied
	*/
	bool checkTriggerComplete(bool bForceComplete = false);

	/**
	* Creates a new objective trigger
	*
	* @date Created June 2, 2006
	*/
	static CObject* create(void)
	{
		CObjectiveTrigger* poObjective = new CObjectiveTrigger;
		poObjective->setMesh(NULL);
		poObjective->deactivate();
		return poObjective;
	}

	/**
	* Sort Functor for sorting a list of Objective Trigger pointers
	*
	* @Author Jordan Leckner
	* @date Created June 5, 2006
	*/
	static bool SortObjectiveTrigger (const CObject *pLeft, CObject *pRight)
	{
		return ((CObjectiveTrigger*)pLeft)->m_unOrderNumber < ((CObjectiveTrigger*)pRight)->m_unOrderNumber;
	}
};

#endif /*_OBJECTIVETRIGGER_H_*/