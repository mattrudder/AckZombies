/**
* @file Camera.h
* @author Matt Rudder
* @date Created Apr 03, 2006
*
* Free roaming camera with mouse look 
*/

#ifndef _FREECAMERA_H_
#define _FREECAMERA_H_

// Local includes
#include "Singleton.h"

// System includes
#include <d3dx9.h>

/**
* Defines the view perspective within the scene graph.
*
* @author Matt Rudder
* @version 1.0
*/
class CFreeCamera : public CSingleton<CFreeCamera>
{
	friend class CSingleton<CFreeCamera>;
	//! Position / Orientation of the camera
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vAt;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vPos;

	//! Singleton stuff.
	//!@{
	CFreeCamera(void);
	CFreeCamera(const CFreeCamera&) {}
	operator=(const CFreeCamera&){}
	virtual ~CFreeCamera(void);
	//!@}
public:
	void move(float fX, float fY, float fZ);
	void strafe(float fUnits);
	void walk(float fUnits);
	void fly(float fUnits);

	void yaw(float fUnits);
	void pitch(float fUnits);
	void roll(float fUnits);

	void getViewMatrix(D3DXMATRIX& mView);
	const D3DXVECTOR3& getCameraPosition(void);
	void setCameraPosition(D3DXVECTOR3& vPos) { m_vPos = vPos; }
};

#endif //_FREECAMERA_H_