/**
* @file Frustum.cpp
* @author Matt Rudder
* @date Modified Apr 06, 2006
*
* Defines a frustum, derived from a camera view matrix, along with utility 
* functions.
*/

// Local include
#include "Frustum.h"

// Engine includes
#include "RenderSystem.h"

CFrustum::CFrustum(void)
{
}

CFrustum::~CFrustum(void)
{
}

/**
* CFrustum::setMatrix
* @date Modified Apr 06, 2006
*/
void CFrustum::setMatrix(D3DXMATRIX& mProjMatrix, D3DXMATRIX& mViewMatrix, float fFarZ)
{
	// Projection correction.
	//if(fFarZ != 0.0f)
	//{
	//	float zNear = -mProjMatrix._43 / mProjMatrix._33;
	//	float Q = fFarZ / (fFarZ - zNear);
	//	mProjMatrix._43 = -Q * zNear;
	//	mProjMatrix._33 = Q;
	//}


	// Variables:
	D3DXMATRIXA16 mViewProj;

	// Multiply the matrices together
	D3DXMatrixMultiply(&mViewProj, &mViewMatrix, &mProjMatrix);
	D3DXMatrixInverse(&mViewProj, NULL, &mViewProj);

	// Create the 8 points of a cube in unit-space
	m_oVerts[0] = D3DXVECTOR3(-1.0f, -1.0f,  0.0f); // xyz
	m_oVerts[1] = D3DXVECTOR3( 1.0f, -1.0f,  0.0f); // Xyz
	m_oVerts[2] = D3DXVECTOR3(-1.0f,  1.0f,  0.0f); // xYz
	m_oVerts[3] = D3DXVECTOR3( 1.0f,  1.0f,  0.0f); // XYz
	m_oVerts[4] = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); // xyZ
	m_oVerts[5] = D3DXVECTOR3( 1.0f, -1.0f,  1.0f); // XyZ
	m_oVerts[6] = D3DXVECTOR3(-1.0f,  1.0f,  1.0f); // xYZ
	m_oVerts[7] = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); // XYZ

	// Transform all 8 points by the view/proj matrix. Doing this
	// gives us the 8 corners of the frustum area.
	for(int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_oVerts[i], &m_oVerts[i], &mViewProj);

	// Generate and store the 6 planes that make up the frustum
	D3DXPlaneFromPoints(&m_oPlanes[0], &m_oVerts[0], &m_oVerts[1], &m_oVerts[2]); // Near
	D3DXPlaneFromPoints(&m_oPlanes[1], &m_oVerts[6], &m_oVerts[7], &m_oVerts[5]); // Far
	D3DXPlaneFromPoints(&m_oPlanes[2], &m_oVerts[2], &m_oVerts[6], &m_oVerts[4]); // Left
	D3DXPlaneFromPoints(&m_oPlanes[3], &m_oVerts[7], &m_oVerts[3], &m_oVerts[5]); // Right
	D3DXPlaneFromPoints(&m_oPlanes[4], &m_oVerts[2], &m_oVerts[3], &m_oVerts[6]); // Top
	D3DXPlaneFromPoints(&m_oPlanes[5], &m_oVerts[1], &m_oVerts[0], &m_oVerts[4]); // Bottom

	// flip the direction
	m_oPlanes[0].d = -m_oPlanes[0].d;
	m_oPlanes[1].d = -m_oPlanes[1].d;
	m_oPlanes[2].d = -m_oPlanes[2].d;
	m_oPlanes[3].d = -m_oPlanes[3].d;
	m_oPlanes[4].d = -m_oPlanes[4].d;
	m_oPlanes[5].d = -m_oPlanes[5].d;

	// Get View Position.
	m_oPosition.x = mViewMatrix._41;
	m_oPosition.y = mViewMatrix._42;
	m_oPosition.z = mViewMatrix._43;
	
	/// Normalize Planes
	//D3DXPlaneNormalize(&m_oPlanes[CLIP_LEFT], &m_oPlanes[CLIP_LEFT]);
	//D3DXPlaneNormalize(&m_oPlanes[CLIP_RIGHT], &m_oPlanes[CLIP_RIGHT]);
	//D3DXPlaneNormalize(&m_oPlanes[CLIP_BOTTOM], &m_oPlanes[CLIP_BOTTOM]);
	//D3DXPlaneNormalize(&m_oPlanes[CLIP_TOP], &m_oPlanes[CLIP_TOP]);
	//D3DXPlaneNormalize(&m_oPlanes[CLIP_NEAR], &m_oPlanes[CLIP_NEAR]);
	//D3DXPlaneNormalize(&m_oPlanes[CLIP_FAR], &m_oPlanes[CLIP_FAR]);
}

/**
* CFrustum::containsPoint
* @date Modified Apr 06, 2006
*/
CFrustum::EFrustumTest CFrustum::containsPoint(D3DXVECTOR3& vPoint)
{
	EFrustumTest eTest = TEST_IN;
	float d = 0.0f;
	for(char c = 0; c < 6; ++c)
	{
		d = D3DXPlaneDotCoord(&m_oPlanes[c], &vPoint);
		if(d < 0.0f)
			return TEST_OUT;
		if(d == 0.0f)
			eTest = TEST_INTERSECT;
	}

	return eTest;
}

/**
* CFrustum::containsAABB
* @date Modified Apr 06, 2006
*/
CFrustum::EFrustumTest CFrustum::containsAABB(SAABB& oAABB, bool bCheckIntersect)
{
	int nCollision;
	bool bIntersect = false;
	for(int i = 0; i < 6; ++i)
	{
		nCollision = checkCol_PlaneAABB(oAABB, m_oPlanes[i]);
		if(nCollision == -1)
			return TEST_OUT;
		else if(nCollision == 0)
			bIntersect = true;
	}
	if(bIntersect)
		return TEST_INTERSECT;
	else
		return TEST_IN;

	//D3DXVECTOR3 points[8] = 
	//{
	//	oAABB.min,
	//	oAABB.max,
	//	D3DXVECTOR3(oAABB.min.x, oAABB.min.y, oAABB.max.z),
	//	D3DXVECTOR3(oAABB.max.x, oAABB.max.y, oAABB.min.z),
	//	D3DXVECTOR3(oAABB.max.x, oAABB.max.y, oAABB.max.z),
	//	D3DXVECTOR3(oAABB.min.x, oAABB.max.y, oAABB.min.z),
	//	D3DXVECTOR3(oAABB.min.x, oAABB.max.y, oAABB.max.z),
	//	D3DXVECTOR3(oAABB.max.x, oAABB.min.y, oAABB.min.z)
	//};

	////int corners[ 8 ] = {0, 4, 3, 5, 2, 6, 1, 7};

	////int planes[ 6 ] = { CLIP_TOP, CLIP_BOTTOM,
	////	CLIP_LEFT, CLIP_RIGHT,
	////	CLIP_FAR, CLIP_NEAR };

	////bool all_inside = true;

	////for ( int plane = 0; plane < 6; ++plane )
	////{
	////	bool all_outside = true;

	////	float distance = 0;

	////	for ( int corner = 0; corner < 8; ++corner )
	////	{
	////		distance = D3DXPlaneDotCoord(&m_oPlanes[ planes[ plane ] ], &points[ corners[ corner ] ]);
	////		all_outside = all_outside && ( distance < 0 );
	////		all_inside = all_inside && ( distance >= 0 );

	////		if ( !all_outside && !all_inside )
	////			break;
	////	}

	////	if ( all_outside )
	////		return TEST_OUT;
	////}

	////if ( all_inside )
	////	return TEST_IN;
	////else
	////	return TEST_INTERSECT;


	//int nDX = oAABB.nCoherence - 1;

	//for(int i = 0; i < 6; ++i)
	//{
	//	int nCount = 0;
	//	++nDX;

	//	if(nDX > 5)
	//		nDX = 0;

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[0]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[1]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[2]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[3]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[4]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[5]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[6]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	if(D3DXPlaneDotCoord(&m_oPlanes[nDX], &points[7]) > 0.0f)
	//	{
	//		if(!bCheckIntersect) continue;
	//		++nCount;
	//	}

	//	oAABB.nCoherence = i;

	//	if(nCount == 0)
	//	{
	//		return CFrustum::TEST_OUT;
	//	}

	//	if(nCount < 6)
	//	{
	//		return CFrustum::TEST_INTERSECT;
	//	}
	//}
	//return CFrustum::TEST_IN;
}

/**
* CFrustum::containsSphere
* @date Modified Apr 06, 2006
*/
CFrustum::EFrustumTest CFrustum::containsSphere(SSphere& oSphere)
{
	int nValue;
	bool bTestIntersect = false;
	for(char c = 0; c < 6; ++c)
	{
		nValue = checkCol_PlaneSphere(NULL, oSphere, m_oPlanes[c]);

		if(nValue == -1)
			return TEST_OUT;
		 else if(nValue == 0)
			bTestIntersect = true;
	}

	if(bTestIntersect)
		return TEST_INTERSECT;

	return TEST_IN;
}

/**
* CFrustum::renderFrustum
* @date Modified Apr 27, 2006
*/
void CFrustum::renderFrustum(D3DXCOLOR dwColor)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();

	CFrustum::SDebugVert pNear[] = 
	{
		{ m_oVerts[NEAR_TL],	dwColor },
		{ m_oVerts[NEAR_TR],	dwColor },
		{ m_oVerts[NEAR_BR],	dwColor },
		{ m_oVerts[NEAR_BL],	dwColor },
		{ m_oVerts[NEAR_TL],	dwColor },
	};
	CFrustum::SDebugVert pFar[] = 
	{
		{ m_oVerts[FAR_TR],		dwColor },
		{ m_oVerts[FAR_TL],		dwColor },
		{ m_oVerts[FAR_BL],		dwColor },
		{ m_oVerts[FAR_BR],		dwColor },
		{ m_oVerts[FAR_TR],		dwColor },
	};
	CFrustum::SDebugVert pBottom[] = 
	{
		{ m_oVerts[NEAR_BL],	dwColor },
		{ m_oVerts[NEAR_BR],	dwColor },
		{ m_oVerts[FAR_BR],		dwColor },
		{ m_oVerts[FAR_BL],		dwColor },
		{ m_oVerts[NEAR_BL],	dwColor },
	};
	CFrustum::SDebugVert pTop[] = 
	{
		{ m_oVerts[NEAR_TR],	dwColor },
		{ m_oVerts[NEAR_TL],	dwColor },
		{ m_oVerts[FAR_TL],		dwColor },
		{ m_oVerts[FAR_TR],		dwColor },
		{ m_oVerts[NEAR_TR],	dwColor },
	};
	CFrustum::SDebugVert pLeft[] = 
	{
		{ m_oVerts[NEAR_TL],	dwColor },
		{ m_oVerts[NEAR_BL],	dwColor },
		{ m_oVerts[FAR_BL],		dwColor },
		{ m_oVerts[FAR_TL],		dwColor },
		{ m_oVerts[NEAR_TL],	dwColor },
	};
	CFrustum::SDebugVert pRight[] = 
	{
		{ m_oVerts[NEAR_BR],	dwColor },
		{ m_oVerts[NEAR_TR],	dwColor },
		{ m_oVerts[FAR_TR],		dwColor },
		{ m_oVerts[FAR_BR],		dwColor },
		{ m_oVerts[NEAR_BR],	dwColor },
	};

	pDev->SetFVF(CFrustum::dwDebugFVF);
	D3DMATERIAL9 mat;
	mat.Ambient = mat.Diffuse = mat.Emissive = mat.Specular = dwColor;
	mat.Power = 5.0f;
	pDev->SetMaterial(&mat);
	D3DXMATRIX mWorld;
	//D3DXMatrixTranslation(&mWorld, m_oPosition.x, m_oPosition.y, m_oPosition.z);
	//pDev->SetTransform(D3DTS_WORLD, &mWorld);
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pTop, sizeof(CFrustum::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pBottom, sizeof(CFrustum::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pLeft, sizeof(CFrustum::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pRight, sizeof(CFrustum::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pNear, sizeof(CFrustum::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pFar, sizeof(CFrustum::SDebugVert));

	// Revert to identity
	//D3DXMatrixIdentity(&mWorld);
	//pDev->SetTransform(D3DTS_WORLD, &mWorld);
}