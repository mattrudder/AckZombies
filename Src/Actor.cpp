/**
* @file Actor.cpp
* @author Matt Rudder
* @date Modified Mar 07, 2006
*
* Base interactive object within the game engine.
*/

// Local includes
#include "Actor.h"
#include "physicsmanager.h"

/**
* CActor::CActor
* @date Modified May 1, 2006
*/
CActor::CActor(EObjectType eType) : m_pMesh(NULL), CObject(eType), m_vVelocity(0.0f, 0.0f, 0.0f), m_dwColor(D3DCOLOR_XRGB(0, 0, 0))
{
	// set the character to update at 60fps
	m_oTimer.setInterval(0.0167f);
	m_vScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	D3DXMatrixIdentity(&m_ActorMatrix);
}

/**
* CActor::collided
* @date Modified Apr 23, 2006
*/
void CActor::collided(CActor *poActor)
{
}

/**
* CActor::rotateActorY
* @date Modified May 13, 2006
*/
void CActor::rotateActorY(float fAngle)
{
	// rip out the position
	D3DXVECTOR3 pos(m_ActorMatrix._41, m_ActorMatrix._42, m_ActorMatrix._43); 
	m_ActorMatrix._41 = m_ActorMatrix._42 = m_ActorMatrix._43 = 0.0f;

	// get the new rotation matrix
	D3DXMATRIXA16 rot;
	D3DXMatrixRotationY(&rot, fAngle);
	
	// apply the new rotation
	D3DXMatrixMultiply(&m_ActorMatrix, &m_ActorMatrix, &rot);

	// put the translation back in
	m_ActorMatrix._41 = pos.x;
	m_ActorMatrix._42 = pos.y;
	m_ActorMatrix._43 = pos.z;
}

/**
* CActor::rotateActorZ
* @date Modified May 13, 2006
*/
void CActor::rotateActorZ(float fAngle)
{
	// rip out the position
	D3DXVECTOR3 pos(m_ActorMatrix._41, m_ActorMatrix._42, m_ActorMatrix._43); 
	m_ActorMatrix._41 = m_ActorMatrix._42 = m_ActorMatrix._43 = 0.0f;

	// get the new rotation matrix
	D3DXMATRIXA16 rot;
	D3DXMatrixRotationZ(&rot, fAngle);
	
	// apply the new rotation
	D3DXMatrixMultiply(&m_ActorMatrix, &m_ActorMatrix, &rot);

	// put the translation back in
	m_ActorMatrix._41 = pos.x;
	m_ActorMatrix._42 = pos.y;
	m_ActorMatrix._43 = pos.z;
}

/**
* CActor::rotateActorX
* @date Modified May 13, 2006
*/
void CActor::rotateActorX(float fAngle)
{
	// rip out the position
	D3DXVECTOR3 pos(m_ActorMatrix._41, m_ActorMatrix._42, m_ActorMatrix._43); 
	m_ActorMatrix._41 = m_ActorMatrix._42 = m_ActorMatrix._43 = 0.0f;

	// get the new rotation matrix
	D3DXMATRIXA16 rot;
	D3DXMatrixRotationX(&rot, fAngle);
	
	// apply the new rotation
	D3DXMatrixMultiply(&m_ActorMatrix, &m_ActorMatrix, &rot);

	// put the translation back in
	m_ActorMatrix._41 = pos.x;
	m_ActorMatrix._42 = pos.y;
	m_ActorMatrix._43 = pos.z;
}

/**
* CActor::~CActor
* @date Modified Mar 07, 2006
*/
CActor::~CActor(void)
{
	COM_RELEASE(m_pMesh);
}

/**
* CActor::setBV
* @date Modified Apr 23, 2006
*/
void CActor::setBV(const D3DXVECTOR3 &vCpt, const float fRadius)
{
	m_oBV.centerPt = vCpt;
	m_oBV.fRadius = fRadius;
}

/**
* CActor::getPosition
* @date Modified Mar 07, 2006
*/
D3DXVECTOR3 CActor::getPosition(void)
{
	return D3DXVECTOR3(m_ActorMatrix._41, m_ActorMatrix._42, m_ActorMatrix._43);
}

/**
* CActor::setPosition
* @date Modified Mar 07, 2006
*/
void CActor::setPosition(const D3DXVECTOR3 &vPos)
{
	m_ActorMatrix._41 = vPos.x;
	m_ActorMatrix._42 = vPos.y;
	m_ActorMatrix._43 = vPos.z;
}

/**
* CActor::setPosition
* @date Modified Mar 07, 2006
*/
void CActor::setOrientation(D3DXVECTOR3 &vOrient)
{
	// make sure the length of the orientation is about one
	float fLength= D3DXVec3Dot(&vOrient, &vOrient);
	if(fLength > 1.02f || fLength < 0.98f)
	{
		// normalize the orientation vector
		fLength = fastSqrtf(fLength);
		fLength = 1.0f/fLength;
		vOrient *= fLength;
	}

	// find the new right axis of the matrix with the current up vector
	D3DXVECTOR3 vRight(m_ActorMatrix._21/m_vScale.x, m_ActorMatrix._22/m_vScale.y, m_ActorMatrix._23/m_vScale.z);
	D3DXVec3Cross(&vRight, &vRight, &vOrient);
	D3DXVec3Normalize(NULL, &vRight, &vRight);

	// set the matrix with the scale back in it
	m_ActorMatrix._11 = vRight.x*m_vScale.x;
	m_ActorMatrix._12 = vRight.y*m_vScale.y;
	m_ActorMatrix._13 = vRight.z*m_vScale.z;

	m_ActorMatrix._31 = vOrient.x*m_vScale.x;
	m_ActorMatrix._32 = vOrient.y*m_vScale.y;
	m_ActorMatrix._33 = vOrient.z*m_vScale.z;
}

/**
* CActor::setPosition
* @date Modified Mar 07, 2006
*/
D3DXVECTOR3 CActor::getOrientation()
{
	// get the angle of rotation
	D3DXVECTOR3 vOrient(m_ActorMatrix._31, m_ActorMatrix._32, m_ActorMatrix._33);

	// strip the scale out of the orientation
	if(m_vScale.x != 1.0f)
	{
		vOrient.x *= (1.0f/m_vScale.x);
	}
	if(m_vScale.y != 1.0f)
	{
		vOrient.y *= (1.0f/m_vScale.y);
	}
	if(m_vScale.z != 1.0f)
	{
		vOrient.z *= (1.0f/m_vScale.z);
	}

	return vOrient;
}

/**
* CActor::getScale
* @date Modified May 16, 2006
*/
D3DXVECTOR3 CActor::getScale()
{
	return m_vScale;
}

/**
* CActor::setScale
* @date Modified May 16, 2006
*/
void CActor::setScale(const D3DXVECTOR3 &vScale)
{
	// strip the position out and store it
	D3DXVECTOR3 vPos(m_ActorMatrix._41,m_ActorMatrix._42,m_ActorMatrix._43);
	m_ActorMatrix._41 = m_ActorMatrix._42 = m_ActorMatrix._43 = 0.0f;

	// strip the old scale out
	D3DXMATRIX mScale;
	D3DXMatrixIdentity(&mScale);
	D3DXMatrixScaling(&mScale, 1.0f/m_vScale.x, 1.0f/m_vScale.y, 1.0f/m_vScale.z);
	D3DXMatrixMultiply(&m_ActorMatrix, &mScale, &m_ActorMatrix);

	// put the new scale in
	m_vScale = vScale;
	D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixMultiply(&m_ActorMatrix, &mScale, &m_ActorMatrix);

	// put the position back in
	m_ActorMatrix._41 = vPos.x;
	m_ActorMatrix._42 = vPos.y;
	m_ActorMatrix._43 = vPos.z;
}

/**
* CActor::setActorMatrix()
* @date Modified May 08, 2006
*/
void CActor::setActorMatrix(D3DXVECTOR3 &vRot, const D3DXVECTOR3 &vTrans, const D3DXVECTOR3 &vScale)
{
	// set the rotation and the scale
	m_vScale = vScale;
	setOrientation(vRot);

	// set the y direction
	m_ActorMatrix._22 = vScale.y;

	// set the translation
	m_ActorMatrix._41 = vTrans.x;
	m_ActorMatrix._42 = vTrans.y;
	m_ActorMatrix._43 = vTrans.z;
}

/**
* CActor::getActorMatrix()
* @date Modified May 08, 2006
*/
D3DXMATRIX CActor::getActorMatrix()
{
	return m_ActorMatrix;
}

/**
* CActor::update
* @date Modified Apr 07, 2006
*/
void CActor::update()
{
	// store the elapsed time
	float fElapsedTime;
	// update at 60fps
	if(!m_oTimer.tick(&fElapsedTime))
		return;
	if(m_vVelocity == D3DXVECTOR3(0,0,0))
	{
		if(getType() == OBJ_ITEM_MONEY)
			deactivate();

		return;
	}

	// update the position based on the elapsed time and velocity
	D3DXVECTOR3 vt = m_vVelocity*fElapsedTime;
	setPosition(getPosition() + vt);
	m_oBV.centerPt += vt;

	// if the actor is inactive, don't update the physics on it
	if(isActive())
		CPhysicsManager::getInstance().update(this, fElapsedTime);
}

/**
* CActor::init
* @date Modified May 11, 2006
*/
void CActor::init(void)
{
	CObject::init();
}

/**
* CActor::draw
* @date Modified Jun 05, 2006
*/
void CActor::draw(void)
{
	if(!m_pMesh)
		return;

	D3DXMATRIX mActorPos = m_ActorMatrix;
	mActorPos._31 = -mActorPos._31;
	mActorPos._32 = -mActorPos._32;
	mActorPos._33 = -mActorPos._33;
	mActorPos._11 = -mActorPos._11;
	mActorPos._12 = -mActorPos._12;
	mActorPos._13 = -mActorPos._13;

	// Draw!
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	if(m_pMesh->isSkinned())
	{
		D3DXMATRIX mTemp;

		pDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mTemp));
		((CAnimatedMesh*)m_pMesh)->updateFrame(&mActorPos);
	}
	else
		pDev->SetTransform(D3DTS_WORLD, &mActorPos);

	// Draw Textured Pass
	m_pMesh->draw(true, &m_dwColor);
}
