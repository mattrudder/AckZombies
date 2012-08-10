/**
* @file Camera.cpp
* @author Tom Williams
* @date Modified April 8, 2006
*
* File describing camera properties and behaviors
*/
#include ".\camera.h"
#include "Math3D.h"

#ifndef M_PI
#define M_PI 3.141592654
#endif

/**
* CCamera::setCamera
* date Modified March 13, 2006
*/
void CCamera::setCamera(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &targ, D3DXVECTOR3 &vPTwoPos)
{
	// do this
	if(vPTwoPos == D3DXVECTOR3(0,0,0))
		vPTwoPos = targ;
	// store the position of the camera
	m_Position = pos;
	// store the target position of the camera
	m_Target = (targ+vPTwoPos)*0.5f;
	// store the vector from the target to the position
	m_TargToPos = m_Position - m_Target;
	D3DXVec3Normalize(&m_UnitTargPos, &(m_Target - m_Position)); 

	// set the up vector based on the world's up
	m_UpVector = D3DXVECTOR3(0,1,0);
	// compute the at and right of the camera
	m_AtVector = m_Target - m_Position;
	m_AtVector.y = 0;
	D3DXVec3Normalize(NULL, &m_AtVector, &m_AtVector);
	D3DXVec3Cross(&m_RightVector, &m_UpVector, &m_AtVector);
	D3DXVec3Normalize(NULL, &m_RightVector, &m_RightVector);

	// set the initial rotation of the camera
	D3DXVECTOR3 vec = -m_AtVector;
	m_fRotation = acosf(D3DXVec3Dot(&vec, &D3DXVECTOR3(0,0,1)));
	if(D3DXVec3Dot(&vec, &D3DXVECTOR3(1,0,0)) < 0) m_fRotation = -m_fRotation;

	if (targ == vPTwoPos)
		m_fInitDist = 0.0f;
	else
		// compute the initial distance between the characters
		m_fInitDist = computeDistance(targ, vPTwoPos);
}

/**
* CCamera::setCameraPos
* date Modified May 15, 2006
*/
void CCamera::setCameraPos(D3DXVECTOR3 &pos)
{
	m_Position = pos;
	m_TargToPos = m_Position - m_Target;
	D3DXVec3Normalize(NULL, &m_UnitTargPos, &(m_Target - m_Position));
}

/**
* CCamera::updateCameraSP
* date Modified March 13, 2006
*/
void CCamera::updateCameraSP(const D3DXVECTOR3 &targ)
{
	// move the camera relative to the player's movement
	if(m_Target == targ)
		return;
	
	// set the target and then update the position
	m_Target = targ;
	m_Position = m_Target + m_TargToPos;
}

/**
* CCamera::updateCameraMP
* date Modified March 13, 2006
*/
void CCamera::updateCameraMP(const D3DXVECTOR3 &one, const D3DXVECTOR3 &two)
{
	// check whether to update or not
	if(!m_bUpdateMP)
		return;

	// set the target to the point b/w the two players'
	D3DXVec3Scale(&m_Target, &(one + two), 0.5f);

	// compute the distance b/w the characters
	float fDist = computeDistance(one, two);

	// move the camera forward/back based on the change in distance b/w characters
	m_Position = m_Target + m_TargToPos;
	m_Position += m_UnitTargPos * ((m_fInitDist - fDist)/m_fInitDist*m_fMoveDist);
	m_TargToPos = m_Position - m_Target;
	m_fInitDist = fDist;

	// if the camera moves too far away, stop it
	if((fDist = D3DXVec3Length(&m_TargToPos)) > 150.0f)
	{
		m_TargToPos *= (1.0f/fDist);
		m_TargToPos *= 150.0f;
	}
}

/**
* CCamera::moveCamera
* date Modified March 13, 2006
*/
void CCamera::moveCamera(float fMove)
{
	// the z vector for the camera
	D3DXVECTOR3 at;
	D3DXVec3Normalize(NULL, &at, &m_TargToPos);

	// move the camera based on it's at and the move passed in
	m_Position.x += fMove*at.x;
	m_Position.y += fMove*at.y;
	m_Position.z += fMove*at.z;
	// update the target to position vector
	m_TargToPos = m_Position - m_Target;
}

/**
* CCamera::rotateCamera
* date Modified March 27, 2006
*/
void CCamera::rotateCamera(float fRot)
{
	//update the rotation
	m_fRotation += fRot;

	// if the rotation overlaps, fix it
	if(m_fRotation >= 2*M_PI)
		m_fRotation -= 2*M_PI;
	else if(m_fRotation <= -2*M_PI)
		m_fRotation += 2*M_PI;

	// trig it up, i hate matrix math.
	float fCameraDist = D3DXVec2Length(&(D3DXVECTOR2(m_TargToPos.x, m_TargToPos.z))); 
	m_Position.x = fCameraDist * sin(m_fRotation) + m_Target.x;
	m_Position.z = fCameraDist * cos(m_fRotation) + m_Target.z;

	// recompute the vector from the target to the camera position
	m_TargToPos = m_Position - m_Target;
	D3DXVec3Normalize(NULL, &m_UnitTargPos, &(m_Target - m_Position));

	// compute the new at and right of the camera
	m_AtVector = m_Target - m_Position;
	m_AtVector.y = 0;
	D3DXVec3Normalize(NULL, &m_AtVector, &m_AtVector);
	D3DXVec3Cross(&m_RightVector, &m_UpVector, &m_AtVector);
	D3DXVec3Normalize(NULL, &m_RightVector, &m_RightVector);
}

/**
* CCamera::getViewMatrix
* date Modified March 13, 2006
*/
void CCamera::getViewMatrix(D3DXMATRIX *pView)
{
	// call the D3DXMATRIX function to set up a view matrix
	D3DXMatrixLookAtLH(pView, &m_Position, &m_Target, &m_UpVector);
}