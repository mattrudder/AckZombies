/**
* @file Camera.h
* @author Tom Williams
* @date Created March 10, 2006
*
* the camera system for the renderer
*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3dx9math.h>
#pragma comment(lib, "D3dx9.lib")
#include "Singleton.h"

/**
* defines the camera focusing on the player(s) for the renderer
* (Singleton class)
*
* @author Tom Williams
* @date Created March 10, 2006
* @date Modified March 27, 2006
*/
class CCamera : public CSingleton<CCamera>
{
	friend class CSingleton<CCamera>;

	//! the camera's position
	D3DXVECTOR3 m_Position;
	//! the point the camera is looking at
	D3DXVECTOR3 m_Target;
	//! the camera's up vector
	D3DXVECTOR3 m_UpVector;
	//! the vector from the target to the position
	D3DXVECTOR3 m_TargToPos;
	//! the unit vector of the camera to the target
	D3DXVECTOR3 m_UnitTargPos;
	//! the camera's right vector
	D3DXVECTOR3 m_RightVector;
	//! the camera's at vector
	D3DXVECTOR3 m_AtVector;
	
	//! the previous distance b/w the two characters (multiplayer)
	float m_fInitDist;
	//! whether to update the multiplayer camera or not
	bool m_bUpdateMP;
	//! the move distance of the camera
	float m_fMoveDist;
	//! rotation of the camera
	float m_fRotation;

	/**
	* Singleton stuff
	* @{
	*/
	CCamera(void){m_fMoveDist = 5.0f; m_bUpdateMP = true;}
	CCamera(const CCamera &oC){}
	CCamera operator =(const CCamera &oC){}
	virtual ~CCamera(void){}
	//!@}
public:

	/**
	* modifiers for the camera class
	*
	* @date Created March 10, 2006
	* @param[in]	the variable to set the member to
	* @{
	*/
	void setCameraPos(D3DXVECTOR3 &pos);
	void setCameraTarget(D3DXVECTOR3 &targ)	{m_Target = targ;}
	void setMoveDist(float fMove)			{m_fMoveDist = fMove;}
	void setToUpdate(bool bUpdate)			{m_bUpdateMP = bUpdate;}
	//!@}

	/**
	* accesors for the camera class
	*
	* @date Created March 10, 2006
	* @param[out]	the member information
	* @{
	*/
	D3DXVECTOR3 &getCameraPos()		{return m_Position;}
	D3DXVECTOR3 &getCameraTarget()	{return m_Target;}
	D3DXVECTOR3 &getCameraUp()		{return m_UpVector;}
	D3DXVECTOR3 &getCameraRight()	{return m_RightVector;}
	D3DXVECTOR3 &getCameraAt()		{return m_AtVector;}
	float getMoveDist()				{return m_fMoveDist;}
	//!@}

	/**
	* set the initial properties of the camera
	*
	* @date Created March 10, 2006
	* @param[in]	pos		the position of the camera
	* @param[in]	targ	the camera's target to look at
	*/
	void setCamera(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &targ, D3DXVECTOR3 &vPTwoPos = D3DXVECTOR3(0,0,0));

	/**
	* rotate the camera based on the move passed in
	*
	* @date Created March 13, 2006
	* @param[in]	fRot	amount to rotate about the world's y
	*/
	void rotateCamera(float fRot);

	/**
	* update the camera in single player mode
	*
	* @date Created March 10, 2006
	* @param[in]	targ	the camera's target to look at
	*/
	void updateCameraSP(const D3DXVECTOR3 &targ);

	/**
	* update the camera in cooperative mode
	*
	* @date Created March 10, 2006
	* @param[in]	one		first player's position
	* @param[in]	two		second player's position
	*/
	void updateCameraMP(const D3DXVECTOR3 &one, const D3DXVECTOR3 &two);

	/**
	* move the camera forward/backward
	*
	* @date Created March 10, 2006
	* @param[in]	fMove	the distance to move the camera
	*/
	void moveCamera(float fMove);

	/**
	* get the view matrix for the camera from DirectX
	*
	* @date Created March 10, 2006
	* @param[out]	pView	the pointer to fill with the view matrix
	*/
	void getViewMatrix(D3DXMATRIX *pView);
};
#endif