/**
* @file Camera.cpp
* @author Matt Rudder
* @date Modified Apr 03, 2006
*
* 
*/

// Local includes
#include "FreeCamera.h"
#include "Math3D.h"

CFreeCamera::CFreeCamera(void) : m_vAt(0.0f, 0.0f, 1.0f), m_vRight(1.0f, 0.0f, 0.0f), m_vUp(0.0f, 1.0f, 0.0f), m_vPos(0.0f, 2.0f, 0.0f)
{
}

CFreeCamera::~CFreeCamera(void)
{
}

void CFreeCamera::move(float fX, float fY, float fZ)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
	m_vPos.z = fZ;
}

void CFreeCamera::strafe(float fUnits)
{
	m_vPos += m_vRight * fUnits;
}

void CFreeCamera::walk(float fUnits)
{
	m_vPos += m_vAt * fUnits;
}
void CFreeCamera::fly(float fUnits)
{
	m_vPos += m_vUp * fUnits;
}

void CFreeCamera::yaw(float fUnits)
{
	D3DXMATRIX newMat;
	D3DXMatrixRotationY(&newMat, fUnits);

	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &newMat);
	D3DXVec3TransformCoord(&m_vAt, &m_vAt, &newMat);
}

void CFreeCamera::pitch(float fUnits)
{
	D3DXMATRIX newMat;
	D3DXMatrixRotationAxis(&newMat, &m_vRight, fUnits);

	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &newMat);
	D3DXVec3TransformCoord(&m_vAt, &m_vAt, &newMat);
}

void CFreeCamera::roll(float fUnits)
{
	D3DXMATRIX newMat;
	D3DXMatrixRotationAxis(&newMat, &m_vAt, fUnits);

	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &newMat);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &newMat);
}

void CFreeCamera::getViewMatrix(D3DXMATRIX& mView)
{
	// Make the axis of orientation orthonormal
	D3DXVec3Normalize(NULL, &m_vAt, &m_vAt);
	D3DXVec3Cross(&m_vUp, &m_vAt, &m_vRight);
	D3DXVec3Normalize(NULL, &m_vUp, &m_vUp);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vAt);
	D3DXVec3Normalize(NULL, &m_vRight, &m_vRight);

	// Build matrix.
	mView(0, 0) = m_vRight.x;
	mView(0, 1) = m_vUp.x;
	mView(0, 2) = m_vAt.x;
	mView(0, 3) = 0.0f;

	mView(1, 0) = m_vRight.y;
	mView(1, 1) = m_vUp.y;
	mView(1, 2) = m_vAt.y;
	mView(1, 3) = 0.0f;

	mView(2, 0) = m_vRight.z;
	mView(2, 1) = m_vUp.z;
	mView(2, 2) = m_vAt.z;
	mView(2, 3) = 0.0f;

	mView(3, 0) = -(D3DXVec3Dot(&m_vRight, &m_vPos));
	mView(3, 1) = -(D3DXVec3Dot(&m_vUp, &m_vPos));
	mView(3, 2) = -(D3DXVec3Dot(&m_vAt, &m_vPos));
	mView(3, 3) = 1.0f;
}

const D3DXVECTOR3& CFreeCamera::getCameraPosition(void)
{
	return m_vPos;
}