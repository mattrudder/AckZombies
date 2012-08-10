/**
* @file Frustum.h
* @author Matt Rudder
* @date Created Apr 06, 2006
*
* Defines a frustum, derived from a camera view matrix, along with utility 
* functions.
*/

#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

// Engine include
#include "Geometry.h"
#include "Math3D.h"

// System include
#include <d3dx9.h>

/**
* Defines a frustum, derived from a camera view matrix, along with utility 
* functions.
*
* @author Matt Rudder
* @date Created Apr 06, 2006
* @date Modified Apr 06, 2006
*/
class CFrustum
{
public:
	enum EClippingPlane { CLIP_LEFT, CLIP_RIGHT, CLIP_TOP, CLIP_BOTTOM, CLIP_NEAR, CLIP_FAR };
	enum EFrustumTest { TEST_IN, TEST_OUT, TEST_INTERSECT };
	enum EFrustumVerts { NEAR_TL, NEAR_TR, NEAR_BL, NEAR_BR, FAR_TL, FAR_TR, FAR_BL, FAR_BR };

protected:

	struct SDebugVert
	{
		D3DXVECTOR3 pos;
		DWORD dwColor;
	};
	static const DWORD dwDebugFVF = D3DFVF_XYZ|D3DFVF_DIFFUSE;

	//! Planes which define our frustum.
	D3DXPLANE m_oPlanes[6];
	D3DXVECTOR3 m_oVerts[8];
	D3DXVECTOR3 m_oPosition;
public:
	CFrustum(void);
	virtual ~CFrustum(void);

	/**
	* Get one of the clipping planes
	*
	* @date Created May 15, 2006
	* @return	the wanted plane
	*/
	D3DXPLANE getClipPlane(EClippingPlane eWhich) {return m_oPlanes[eWhich];}

	/**
	* Defines the clipping planes of the frustum based on a matrix.
	*
	* @date Created Apr 06, 2006
	* @param[in]	mMatrix		Matrix to use for clipping plane calculations.
	* @param[in]	bNormalize	Should we normalize the clipping planes?
	*/
	void setMatrix(D3DXMATRIX& mProjMatrix, D3DXMATRIX& mViewMatrix, float fFarZ = 0.0f);

	/**
	* Tests the frustum for containment of a single 3D point.
	*
	* @date Created Apr 06, 2006
	* @param[in]	vPoint		The point to test.
	* @return Identifier noting the type of containment, if any.
	*/
	EFrustumTest containsPoint(D3DXVECTOR3& vPoint);

	D3DXVECTOR3& getViewPosition(void) { return m_oPosition; }

	/**
	* Tests the frustum for containment of an axis-aligned bounding box.
	*
	* @date Created Apr 06, 2006
	* @param[in]	oAABB		The bounding box to test.
	* @return Identifier noting the type of containment, if any.
	*/
	EFrustumTest containsAABB(SAABB& oAABB, bool bCheckIntersect = true);

	/**
	* Tests the frustum for containment of a sphere.
	*
	* @date Created Apr 06, 2006
	* @param[in]	oSphere		The sphere to test.
	* @return Identifier noting the type of containment, if any.
	*/
	EFrustumTest containsSphere(SSphere& oSphere);

	void renderFrustum(D3DXCOLOR dwColor);
};

#endif //_FRUSTUM_H_