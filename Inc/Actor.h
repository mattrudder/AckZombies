/**
* @file Actor.h
* @author Matt Rudder
* @date Created Mar 07, 2006
*
* Base interactive object within the game engine.
*/

#ifndef _ACTOR_H_
#define _ACTOR_H_

// Local includes
#include "Object.h"

// Engine includes
#include "RenderSystem.h"
#include "Math3d.h"
#include "TickTimer.h"

// System includes
#include <d3dx9.h>

/**
* Base interactive object within the game engine.
*
* @author Matt Rudder
* @date Created Mar 06, 2006
* @date Modified May 11, 2006
*/
class CActor : public CObject
{
	friend class CRenderSystem;
	friend class CShaderManager;
protected:
	//! Position, scale, orientation of the actor within the game world.
	D3DXMATRIXA16 m_ActorMatrix;
	//! Mesh representation of the actor within the game world.
	CMesh* m_pMesh;
	//! the bounding volume of the actor
	SSphere m_oBV;
	//! the velocity of the actor
	D3DXVECTOR3 m_vVelocity;
	//! timer to update with
	CTickTimer m_oTimer;
	//! the scale of the model
	D3DXVECTOR3 m_vScale;
	//! Color of the actor
	D3DCOLOR m_dwColor;
public:

	/**
	* Constructor
	* 
	* @date Created Mar 07, 2006
	*/
	CActor(EObjectType eType = OBJ_ACTOR);

	/**
	* Destructor
	* 
	* @date Created Mar 07, 2006
	*/
	virtual ~CActor(void);

	/**
	* accessors for the member variable of the actor
	* 
	* @date Created Mar 07, 2006
	* @return The value of the actor member
	* @{
	*/
	D3DXVECTOR3 getPosition(void);
	D3DXVECTOR3 getVelocity() {return m_vVelocity;}
	D3DXVECTOR3 getOrientation();
	D3DXVECTOR3 getScale();
	D3DXMATRIX getActorMatrix();
	SSphere getBV()	{return m_oBV;}
	CMesh* getMesh() {return m_pMesh;}
	bool isSkinned() { return m_pMesh ? m_pMesh->isSkinned() : false; }
	D3DCOLOR getColor() { return m_dwColor; }
	//!@}

	/**
	* Sets the member variable in the actor
	* 
	* @date Created Mar 07, 2006
	* @param[in] The value to set the member of the actor to.
	* @{
	*/
	void setPosition(const D3DXVECTOR3 &vPos);
	void setVelocity(const D3DXVECTOR3 &vVel) {m_vVelocity = vVel;}
	void setActorMatrix(D3DXVECTOR3 &vRot, const D3DXVECTOR3 &vTrans, const D3DXVECTOR3 &vScale);
	void setOrientation(D3DXVECTOR3 &vOrient);
	void setBV(const SSphere &oBV) {m_oBV = oBV;}
	void setBV(const D3DXVECTOR3 &vCpt, const float fRadius);
	void setMesh(CMesh* poMesh) {m_pMesh = poMesh;}
	void setScale(const D3DXVECTOR3 &vScale);
	void setColor(D3DCOLOR dwColor) { m_dwColor = dwColor; }
	//!@}

	/**
	* update the actor based on the object it collided with
	*
	* @date Created April 23, 2006
	* @param[in]	poActor		the object it collided with
	*/
	virtual void collided(CActor *poActor);

	/**
	* update the actor each frame
	*
	* @date Created April 23, 2006
	*/
	virtual void update();

	/**
	* rotate actor about the y
	*
	* @date Created May 13, 2006
	* @param[in]	fAngle	the rotation angle
	*/
	void rotateActorY(float fAngle);

	/**
	* rotate actor about the x
	*
	* @date Created May 13, 2006
	* @param[in]	fAngle	the rotation angle
	*/
	void rotateActorX(float fAngle);

	/**
	* rotate actor about the z
	*
	* @date Created May 13, 2006
	* @param[in]	fAngle	the rotation angle
	*/
	void rotateActorZ(float fAngle);

	/**
	* used to initialize variables for reuse
	*
	* @date Created May 11, 2006
	* @remark base class init must call its parent's
	*/
	virtual void init(void);

	/**
	* create a new actor
	*
	* @date Created May 12, 2006
	* @return the created object
	*/
	static CObject* create()
	{
		return new CActor;
	}


	/**
	* Renders the actor and allows subclasses to 
	* perform specific behavior at render time.
	*
	* @date Created Jun 05, 2006
	*/
	virtual void draw(void);
};

#endif //_ACTOR_H_