/**
* @file Math3D.h
* @author Tom Williams
* @date Created March 3, 2006
*
* File for functions and structures associated with vector/matrix math
* and functions dealing with collision and collision objects
*/
#ifndef _MATH3D_H_
#define _MATH3D_H_

#include <d3dx9math.h>
#pragma comment(lib, "D3dx9.lib")

#ifndef M_PI
#define M_PI 3.141592654f
#endif

/**
* extract the components of a D3DCOLOR
*
* @author Tom Williams
* @date Created June 7, 2006
* @date Modified June 7, 2006
* @param[in]	argb	the color to decompose
* @param[out]	a		the alpha
* @param[out]	r		the red
* @param[out]	g		the green
* @param[out]	b		the blue
*/
inline void decomposeD3DCOLOR(const D3DCOLOR &argb, UINT &a, UINT &r, UINT &g, UINT &b)
{
	a = ((0xff<<24) & argb) >> 24;
	r = ((0xff<<16) & argb) >> 16;
	g = ((0xff<<8)  & argb) >> 8;
	b = 0xff        & argb;
}

/**
* Degrees to radians conversion
*
* @author Jordan Leckner
* @date Created May 5, 2006
* @date Modified May 5, 2006
*/
inline float degreesToRadians(float fDegrees)
{
	return fDegrees * (D3DX_PI / 180.0f);
}

/**
* Radians to degrees conversion
*
* @author Jordan Leckner
* @date Created May 5, 2006
* @date Modified May 5, 2006
*/
inline float radiansToDegrees(float fRadians)
{
	return fRadians * (180.0f / D3DX_PI);
}

/**
* Fast square root calculation
*
* @author Jordan Leckner
* @date Created May 4, 2006
* @date Modified May 4, 2006
*/
inline float fastSqrtf(float f)
{
	// Carmack's fast sqaure root
    float fhalf = 0.5f*f;
    int i = *(int*)&f;
    i = 0x5f3759df - (i >> 1);
    f = *(float*)&i;
    f = f*(1.5f - fhalf*f*f);
	f = 1.0f/f;
    return f;
}

/**
* defines the properties of a sphere
*
* @author Tom Williams
* @date Created March 3, 2006
* @date Modified March 3, 2006
*/
struct SSphere
{
	//! center of the sphere
	D3DXVECTOR3	centerPt;
	//! radius of the sphere
	float		fRadius;

	SSphere(void) : centerPt(0.0f, 0.0f, 0.0f), fRadius(0.0f) {}
	SSphere(D3DXVECTOR3 &center, float fRad) : centerPt(center), fRadius(fRad) {}
	~SSphere(void) {}
};

/**
* defines the properties of an AABB
*
* @author Tom Williams
* @date Created April 1, 2006
* @date Modified April 1, 2006
*/
struct SAABB
{
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	
	// Used by CFrustum to improve check performance.
	int nCoherence;

	SAABB() { nCoherence = 0; }
};

/**
* defines the properties of a triangle
*
* @author Tom Williams
* @date Created March 3, 2006
* @date Modified March 3, 2006
*/
struct STriangle
{
	//! the points making the triangle
	D3DXVECTOR3	points[3];
	//! the normal of the triangle
	D3DXVECTOR3	normal;		

	/**
	* Compute the normal of the triangle
	*
	* @date Created March 7, 2006
	*/
	void computeNormal();

	/**
	* Constructor
	*
	* @date Created April 3, 2006
	*/
	STriangle(); 
};

/**
* Check for a collision between two spheres
*
* @date Created March 3, 2006
* @param[out]	pOffsetVec	If there was collision, the vector to offset tOne by
* @param[in]	tOne		A sphere to check collision against.
* @param[in]	tTwo		The other sphere to check against for collision.
* @return	Whether or not there was collision.
*/
bool checkCol_SphereSphere(D3DXVECTOR3 *pOffsetVec, const SSphere &tOne, const SSphere &tTwo);

/**
* Check for a collision between two spheres
*
* @date Created April 26, 2006
* @param[in]	tOne		A sphere to check collision against.
* @param[in]	tTwo		The other sphere to check against for collision.
* @return	-1 : negative side, 0 : collision, 1 : positive side.
*/
int checkCol_PlaneAABB(const SAABB &tBox, const D3DXPLANE &plane);

/**
* Check for a collision between a sphere and a plane
*
* @date Created May 15, 2006
* @param[in]	tSphere		The AABB to test with
* @param[in]	plane		The plane to check collision against
* @param[out]	pPtOfCol	The returned point of collision
* @return	-1 : negative side, 0 : collision, 1 : positive side.
*/
int checkCol_PlaneSphere(D3DXVECTOR3 *pPtOfCol, const SSphere &tSphere, const D3DXPLANE &plane);

/**
* Check if a ray intersects a sphere
*
* @date Created March 3, 2006
* @param[out]	pPtOfCol	If there was collision, the collision point closer
*							to the origin is returned to the function caller.
* @param[in]	point		The origin point of the ray
* @param[in]	dir			The direction vector of the ray
* @param[in]	tSphere		The sphere to check collision against.
* @return	Whether or not there was collision.
*/
bool checkCol_RaySphere(D3DXVECTOR3 *pPtOfCol, const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir, const SSphere &tSphere, float *pfDist = NULL);

/**
* Check if a point falls inside a triangle
*
* @date Created March 3, 2006
* @param[in]	point		The point to check.
* @param[in]	tTriangle	The triangle to check against.
* @return	Whether or not there was collision.
*/
bool checkCol_PointTriangle(const D3DXVECTOR3 &point, const STriangle &tTriangle);

/**
* Check if a ray intersects a triangle
*
* @date Created March 3, 2006
* @param[out]	pPtOfCol	If there was collision, the collision point is
*							returned to the function caller.
* @param[out]	fLength		the length of the vector to the triangle
* @param[in]	point		The origin point of the ray.
* @param[in]	dir			The UNIT direction vector of the ray.
* @param[in]	tTriangle	The triangle to check against for intersection.
* @return	Whether or not there was collision.
*/
bool checkCol_RayTriangle(D3DXVECTOR3 *pPtOfCol, float &fLength, const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir, const STriangle &tTriangle);

/**
* Check if a sphere intersects a triangle
*
* @date Created April 3, 2006
* @param[out]	pPtOfCol	If there was collision, the collision point is
*							returned to the function caller.
* @param[in]	tSphere		The sphere to check against the triangle
* @param[in]	tTriangle	The triangle to check against for intersection.
* @return	Whether or not there was collision.
*/
bool checkCol_SphereTriangle(D3DXVECTOR3 *pPtOfCol, const SSphere &tSphere, const STriangle &tTriangle);

/**
* Check if a sphere intersects an AABB
*
* @date Created April 3, 2006
* @param[in]	tSphere		The sphere to check against the triangle
* @param[in]	tBox		The box to check against for intersection.
* @return	Whether or not there was collision.
*/
bool checkCol_SphereAABB(const SSphere &tSphere, SAABB &tBox);

/**
* Check if a segment intersects an AABB
*
* @date Created April 3, 2006
* @param[in]	PtOne		The origin of the segment
* @param[in]	PtTwo		The end point of the segment
* @param[in]	tBox		The box to check against for intersection.
* @return	Whether or not there was collision.
*/
bool checkCol_SegmentAABB(const D3DXVECTOR3 &PtOne, const D3DXVECTOR3 &PtTwo, const SAABB &tBox);

/**
* Check if a ray intersects an AABB
*
* @date Created April 3, 2006
* @param[in]	point		The origin of the ray
* @param[in]	dir			The direction and magnitude of the ray
* @param[in]	tBox		The box to check against for intersection.
* @return	Whether or not there was collision.
*/
bool checkCol_RayAABB(const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir, const SAABB &tBox);

/**
* Compute closest point on triangle
*
* @date Created April 6, 2006
* @param[in]	point		the point to compare against
* @param[in]	tTriangle	the triangle to find point on
* @return		D3DXVECTOR3	the point on the triangle
*/
D3DXVECTOR3 computeClosestPtTriangle(const D3DXVECTOR3 &point, const STriangle &tTriangle);

/** 
* Compute the normal and get the float length from it
*
* @date Created April 3, 2006
* @param[out]	pfLength	the filled length
* @param[out]	pOut		the filled unit vector
* @param[in]	pV			the vector to normalize
* @return		D3DXVECTOR3	same as pOut
*/
D3DXVECTOR3 *D3DXVec3Normalize(float *pfLength, D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV);

/**
* Compute the distance between two points
*
* @date Created March 3, 2006
* @param[in]	one			A point in 3D space
* @param[in]	two			The other point to measure to.
* @return	The distance between the two points
*/
float computeDistance(const D3DXVECTOR3 &one, const D3DXVECTOR3 &two);

/**
* Compute the distance between two points
*
* @date Created May 3, 2006
* @param[in]	one			A point in 3D space
* @param[in]	two			The other point to measure to.
* @return	The squared distance between the two points
*/
float computeDistanceSquared(const D3DXVECTOR3 &one, const D3DXVECTOR3 &two);

/**
* Compute a reflection vector
*
* @date Created April 8, 2006
* @param[out]	pOut	the reflected vector
* @param[in]	pV		The vector to reflect
* @param[in]	pNorm	The normal of the collided plane
* @return		the same pointer as pOut
*/
D3DXVECTOR3 *D3DXVec3Reflect(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXVECTOR3 *pNorm);

/**
* Generates a random float between 0.0f and 1.0f
*
* @date Created May 20, 2006
* @return Random float.
*/
float getRandomFloat(void);

/**
* Generates a random float between fMin and fMax
*
* @date Created May 20, 2006
* @return Random float.
*/
float getRandomFloat(float fMin, float fMax);

/**
* Generates a random vector between two vectors.
*
* @date Created May 20, 2006
* @return Random vector.
*/
void getRandomVector(D3DXVECTOR3* pOut, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax);

/**
* Generates a random vector between two floats
*
* @date Created May 20, 2006
* @return Random vector.
*/
void getRandomVector(D3DXVECTOR3* pOut, float fMin, float fMax);

#endif