/**
* @file Projector.h
* @author Matt Rudder
* @date Created Jun 05, 2006
*
* Encapsulates the resourced needed to perform texture projection.
*/

#ifndef _PROJECTOR_H_
#define _PROJECTOR_H_

// Engine includes
#include "Frustum.h"

class CTexture2D;

/**
* Encapsulates the resourced needed to perform texture projection.
*
* @author Matt Rudder
* @date Created Jun 05, 2006
* @date Modified Jun 05, 2006
*/
class CProjector
{
protected:
	//! The texture to project
	CTexture2D* m_pTexture;
	//! Position / Orientation of the camera
	static D3DXVECTOR3 m_vWorldUp;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vAt;
	D3DXVECTOR3 m_vPos;
	//! Projection Matrix
	D3DXMATRIX m_mProjection;
	//! View Matrix
	D3DXMATRIX m_mView;
	//! Frustum describing the projector.
	CFrustum m_oFrustum;

	/**
	* Calculates a new view matrix for the projector; Used after updates to
	* projection vectors.
	*
	* @date Created Jun 05, 2006
	*/
	__forceinline void calcViewMatrix(void);

public:
	CProjector(void);
	virtual ~CProjector(void);

	/**
	* Accessors/Modifiers
	* @date Created Jun 06, 2006
	* @{
	*/
	CFrustum& getFrustum(void);
	void getTransform(D3DXMATRIX& mProj);
	CTexture2D* getTexture(void) { return m_pTexture; }
	//!@}

	/**
	* Sets the texture the projector projects onto geometry.
	*
	* @date Created Jun 05, 2006
	* @param[in]	pTexture	The new texture.
	*/
	void setTexture(CTexture2D* pTexture);

	/**
	* Sets the origin point of the projection.
	*
	* @date Created Jun 05, 2006
	* @param[in]	vPos	The new position.
	*/
	void setPosition(D3DXVECTOR3& vPos);

	/**
	* Sets the direction the projector is facing.
	*
	* @date Created Jun 05, 2006
	* @param[in]	vDir	The direction of the projector.
	*/
	void setDirection(D3DXVECTOR3& vDir);
};

#endif //_PROJECTOR_H_