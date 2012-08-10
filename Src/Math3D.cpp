/**
* @file Math3D.cpp
* @author Tom Williams
* @date Modified April 27, 2006
*
* File for functions and structures associated with vector/matrix math
* and functions dealing with collision and collision objects
*/
#include "Math3D.h"
#include <math.h>

/**
* STriangle::STriangle()
* date Modified April 3, 2006
*/
STriangle::STriangle()
{
	normal = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

/**
* STriangle::computeNormal
* date Modified March 7, 2006
*/
void STriangle::computeNormal()
{
	// get two vector sides of the triangle
	D3DXVECTOR3 ac, ab;
	D3DXVec3Subtract(&ab, &points[1], &points[0]);
	D3DXVec3Subtract(&ac, &points[2], &points[0]);

	// cross the two sides to get the normal
	D3DXVec3Cross(&normal, &ab, &ac);
	D3DXVec3Normalize(NULL, &normal, &normal);
}

/**
* checkCol_SphereSphere
* date Modified May 30, 2006
*/
bool checkCol_SphereSphere(D3DXVECTOR3 *pOffsetVec, const SSphere &tOne, const SSphere &tTwo)
{
	/* 
	* find the distance between the centers, and then compare
	* with the sum of the radii. if the sum of the radii is less
	* than the distance between the centers than the spheres are not colliding.
	* if the sum of the radii is greater or equal, than the spheres are colliding
	*/
	float fRadii = tOne.fRadius+tTwo.fRadius;
	D3DXVECTOR3 vec = tTwo.centerPt - tOne.centerPt;
	if(D3DXVec3Dot(&vec, &vec) > fRadii*fRadii)
		return false;	// no collision

	// if the pointer passed in is valid, then fill in with collision point	
	if(pOffsetVec)
	{
		/*
		* the collision point will be the unit vector from one sphere center to the other
		* scaled by the radius length of a sphere added to that sphere's center point
		*/
		D3DXVec3Normalize(NULL, &vec, &vec);
		D3DXVec3Scale(pOffsetVec, &vec, tOne.fRadius);
		D3DXVec3Add(pOffsetVec, pOffsetVec, &tOne.centerPt);

		// invert the vector
		vec = -vec;
		vec *= tTwo.fRadius;
		vec += tTwo.centerPt;

		// the offset vector will be the difference between the two collision points
		*pOffsetVec = vec - *pOffsetVec;
	}

	return true;		// collision
}

/**
* checkCol_PlaneAABB()
* @date Modified April 27, 2006
*/
int checkCol_PlaneAABB(const SAABB &tBox, const D3DXPLANE &plane)
{
	// Plane = aX + bY + cZ - d = 0, so: (a,b,c) * (x,y,z) = d
	// a normal to the plane = (a,b,c).
	// point on the plane = (x,y,z)

	// get the center of the AABB
	D3DXVECTOR3 vBoxCenter = (tBox.max + tBox.min) * 0.5f;
	// get the positive extents of the box
	D3DXVECTOR3 vPosExtents = tBox.max - vBoxCenter;

	// get the projection radius of the box onto a line to the plane
	float fRadius = vPosExtents.x*abs(plane.a) + vPosExtents.y*abs(plane.b) + vPosExtents.z*abs(plane.c);
	// get the distance from the center of the box from the plane
	float fDist = D3DXVec3Dot(&D3DXVECTOR3(plane.a,plane.b,plane.c), &vBoxCenter) - plane.d;

	// see if the box collides
	if(abs(fDist) <= fRadius)
		return 0;

	// the box doesn't collide, check what side of the plane it's on
	// find a point on the plane
	D3DXVECTOR3 ptOnPlane = D3DXVECTOR3(plane.a,plane.b,plane.c) * plane.d;
	fDist = D3DXVec3Dot(&(ptOnPlane - vBoxCenter), &D3DXVECTOR3(plane.a,plane.b,plane.c)); 

	if(fDist < 0)
		return 1;
	else
		return -1;
}

/**
* checkCol_PlaneSphere
* date Modified May 15, 2006
*/
int checkCol_PlaneSphere(D3DXVECTOR3 *pPtOfCol, const SSphere &tSphere, const D3DXPLANE &plane)
{
	// get the distance from the plane
	D3DXVECTOR3 vec = D3DXVECTOR3(plane.a, plane.b, plane.c) * plane.d - tSphere.centerPt;
	float fDistance = D3DXVec3Dot(&vec, &D3DXVECTOR3(plane.a, plane.b, plane.c));

	if(pPtOfCol)
		*pPtOfCol = tSphere.centerPt + D3DXVECTOR3(plane.a, plane.b, plane.c)*fDistance;

	if(abs(fDistance) <= tSphere.fRadius)
		return 0;
		
	// if there's no intersection, return on positive or negative side of plane
	if(fDistance < 0)
		return 1;
	else
		return -1;
}

/**
* checkCol_RaySphere
* date Modified March 7, 2006
*/
bool checkCol_RaySphere(D3DXVECTOR3 *pPtOfCol, const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir, const SSphere &tSphere, float *pfDist)
{
	// vector m is the vector from the center of the sphere to the ray's origin
	D3DXVECTOR3 vCenterToOrigin = point - tSphere.centerPt;

	// if the direction is facing the same way as the vector to the origin, then collision isn't possible...
	// unless, of course, the point is inside the sphere...
	float fDistOriginToCenter = D3DXVec3Dot(&vCenterToOrigin, &vCenterToOrigin);
	float fAngleFromRay = D3DXVec3Dot(&vCenterToOrigin, &dir);
	if(fAngleFromRay > 0.0f)
	{
		// check if the point lies within the bounds of the sphere
		if(fDistOriginToCenter < tSphere.fRadius*tSphere.fRadius)
		{
			if(pPtOfCol)
				*pPtOfCol = point;
			if(pfDist)
				*pfDist = 0.0f;

			return true;
		}

		// if the point isn't in the sphere, return no collision
		return false;
	}

	// check if the ray could possibly hit the sphere
	// get the projection point
	D3DXVECTOR3 vProjection = point + dir*(-fAngleFromRay);
	// get the vector between the projection point and the center
	D3DXVECTOR3 vProjectionToCenter = vProjection - tSphere.centerPt;
	// if the distance from the projection point to the sphere center is greater than the radius,
	// then collision is only possible if the point lies inside...
	if(D3DXVec3Dot(&vProjectionToCenter, &vProjectionToCenter) > tSphere.fRadius*tSphere.fRadius)
	{
		// check if the point lies within the bounds of the sphere
		if(fDistOriginToCenter < tSphere.fRadius*tSphere.fRadius)
		{
			if(pPtOfCol)
				*pPtOfCol = point;
			if(pfDist)
				*pfDist = 0.0f;

			return true;
		}

		// if the point isn't in the sphere, return no collision
		return false;
	}

	// if all those checks pass then there was collision
	// find the collision point if it's wanted
	if(pPtOfCol)
		*pPtOfCol = vProjection;
	if(pfDist)
		*pfDist = -fAngleFromRay;
	
	return true;	// collision
}

/**
* checkCol_PointTriangle
* date Modified March 7, 2006
*/
bool checkCol_PointTriangle(const D3DXVECTOR3 &point, const STriangle &tTriangle)
{
	// translate the triangle so the point is the origin
	STriangle tTempTri = tTriangle;
	D3DXVec3Subtract(&tTempTri.points[0], &tTempTri.points[0], &point);
	D3DXVec3Subtract(&tTempTri.points[1], &tTempTri.points[1], &point);
	D3DXVec3Subtract(&tTempTri.points[2], &tTempTri.points[2], &point);
	float f01 = D3DXVec3Dot(&tTempTri.points[0], &tTempTri.points[1]);
	float f02 = D3DXVec3Dot(&tTempTri.points[0], &tTempTri.points[2]);
	float f12 = D3DXVec3Dot(&tTempTri.points[1], &tTempTri.points[2]);
	float f22 = D3DXVec3Dot(&tTempTri.points[2], &tTempTri.points[2]);

	// Check if the normals for side 01 and 12 point in the same direction
	if(f12 * f02 - f22 * f01 < 0)
		return false;	// no collision

	// Check if the normals for side 01 and 02 point in the same direction
	float f11 = D3DXVec3Dot(&tTempTri.points[1], &tTempTri.points[1]);
	if(f01 * f12 - f02 * f11 < 0)
		return false;	// no collision

	// If all those passed, then the point must be in the triangle
	return true;

}

/**
* checkCol_RayTriangle
* date Modified March 7, 2006
*/
bool checkCol_RayTriangle(D3DXVECTOR3 *pPtOfCol, float &fLength, const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir, const STriangle &tTriangle)
{
	// variables to hold onto
	D3DXVECTOR3 ab, ac, edgeRayCross, PtToTri, edgePTTCross;
	float fF, fA, fU, fV;

	// get the edges of the triangle
	ab = tTriangle.points[1] - tTriangle.points[0];
	ac = tTriangle.points[2] - tTriangle.points[0];

	// compare the edges and the ray
	D3DXVec3Cross(&edgeRayCross, &dir, &ac);
	fA = D3DXVec3Dot(&edgeRayCross, &ab);

	// check if the ray is parallel to the triangle
	if(fA > -FLT_EPSILON && fA < FLT_EPSILON) return false;

	// get the descriminant
	fF = 1.0f/fA;
	PtToTri = point - tTriangle.points[0];

	// find the u coordinate
	fU = fF * D3DXVec3Dot(&PtToTri, &edgeRayCross);

	// make sure the barycentric coordinate falls b/w 0 and 1
	if(fU < 0.0f || fU > 1.0f) return false;

	// check the relationship between the pt to triangle and the edge
	D3DXVec3Cross(&edgePTTCross, &PtToTri, &ab);

	// get the v coordinate
	fV = fF * D3DXVec3Dot(&dir, &edgePTTCross);

	// make sure the coordinates fall between 0 and 1
	if(fV < 0.0f || fV + fU > 1.0f) return false;

	// if it falls to here, then intersection must have occurred
	// find the collision point and length of the vector
	fLength = fF * D3DXVec3Dot(&ac, &edgePTTCross);

	// make sure the length is in the correct direction
	if(fLength > 0.0f)
	{
		*pPtOfCol = point + dir*fLength;
		return true;
	}

	// there is LINE intersection, but not ray intersection
	return false;
}

/**
* checkCol_SphereTriangle()
* date Modified April 3, 2006
*/
bool checkCol_SphereTriangle(D3DXVECTOR3 *pPtOfCol, const SSphere &tSphere, const STriangle &tTriangle)
{
	// find the closest point on the triangle
	D3DXVECTOR3 closestPt = computeClosestPtTriangle(tSphere.centerPt, tTriangle);
	D3DXVECTOR3 vec(closestPt - tSphere.centerPt);
	float fLengthSquared = D3DXVec3Dot(&vec, &vec);

	// if the length squared is greater than the radius squared, then collision is impossible
	if(fLengthSquared > tSphere.fRadius*tSphere.fRadius)
		return false;

	// return the point of collision (closest point on triangle) if pointer was valid
	if(pPtOfCol)
		*pPtOfCol = closestPt;
	
	// return collision
	return true;
}

/**
* checkCol_SphereAABB()
* date Modified April 3, 2006
*/
bool checkCol_SphereAABB(const SSphere &tSphere, SAABB &tBox)
{
    float fSquare, fDistance = 0; 

    //find the squared distance from the sphere to the box
    for(int i = 0; i < 3; ++i)
    {
        if( tSphere.centerPt[i] < tBox.min[i] )
        {
            fSquare = tSphere.centerPt[i] - tBox.min[i];
            fDistance += fSquare*fSquare; 
        }
        else if( tSphere.centerPt[i] > tBox.max[i] )
        {
            fSquare = tSphere.centerPt[i] - tBox.max[i];
            fDistance += fSquare*fSquare; 
        }
    }

	// if the distance squared is greater than the radius squared, then there is no collision
    return fDistance <= tSphere.fRadius*tSphere.fRadius;
}

/**
* checkCol_SegmentAABB()
* @date Modified April 13, 2006
*/
bool checkCol_SegmentAABB(const D3DXVECTOR3 &PtOne, const D3DXVECTOR3 &PtTwo, const SAABB &tBox)
{
	// find the center point of the AABB
	D3DXVECTOR3 cpt = (tBox.min + tBox.max) * 0.5f;

	// box halflength extents
	D3DXVECTOR3 boxExtent = tBox.max - cpt;
	// mid point of the segment
	D3DXVECTOR3 segMidpt = (PtOne+PtTwo) * 0.5f;
	// segment half length
	D3DXVECTOR3 segExtent = PtTwo - segMidpt;
	// translate the midpoint to the center
	segMidpt = segMidpt - cpt;

	// use world coordinate axes as the separating axes
	// test the x
	float fADX = abs(segExtent.x);
	if(abs(segMidpt.x) > boxExtent.x+fADX) return false;
	// test the y
	float fADY = abs(segExtent.y);
	if(abs(segMidpt.y) > boxExtent.y+fADY) return false;
	// test the z
	float fADZ = abs(segExtent.z);
	if(abs(segMidpt.z) > boxExtent.z+fADZ) return false;

	// add epsilon to reduce floating point error calculations
	fADX += FLT_EPSILON; fADY += FLT_EPSILON; fADZ += FLT_EPSILON;

	// try comparing the cross product of segment direction and separating axes
	if(abs(segMidpt.y*segExtent.z - segMidpt.z*segExtent.y) > boxExtent.y*fADZ + boxExtent.z*fADY)
		return false;
	if(abs(segMidpt.z*segExtent.x - segMidpt.x*segExtent.z) > boxExtent.x*fADZ + boxExtent.z*fADX)
		return false;
	if(abs(segMidpt.x*segExtent.y - segMidpt.y*segExtent.x) > boxExtent.x*fADY + boxExtent.y*fADX)
		return false;

	// if there's no separating axes, then the segment must intersect
	return true;
}

/**
* checkCol_RayAABB()
* @date Modified April 13, 2006
*/
bool checkCol_RayAABB(const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir, const SAABB &tBox)
{
	// get the possible length range of the ray
	float fTMin = 0.0f;
	float fTMax = FLT_MAX;
	
	// loop through all 3 slabs of the AABB
	for(int i = 0; i < 3; ++i)
	{
		// check if the ray is parallel to the AABB
		if(abs(dir[i]) < FLT_EPSILON)
		{
			if(point[i] < tBox.min[i] || point[i] > tBox.max[i])
				return false;
		}
		// otherwise, look for possible collision
		{
			// get the discriminant and value of t
			float fOOD = 1.0f/dir[i];
			float fT1 = (tBox.min[i] - point[i])*fOOD;
			float fT2 = (tBox.max[i] - point[i])*fOOD;

			// make t1 the intersection of the near plane
			if(fT1>fT2)
			{
				float fTemp = fT1;
				fT1 = fT2;
				fT2 = fTemp;
			}

			// compute the slab intersection interval
			if(fT1>fTMin) fTMin=fT1;
			if(fT2>fTMax) fTMax=fT2;

			// exit with no collision if slab interval is empty
			if(fTMin>fTMax) return false;
		}
	}

	return true;
}

/**
* computeClosetsPtTriangle()
* date Modified April 6, 2006
*/
D3DXVECTOR3 computeClosestPtTriangle(const D3DXVECTOR3 &point, const STriangle &tTriangle)
{
	// get the edges of the triangle
	D3DXVECTOR3 edge01 = tTriangle.points[1] - tTriangle.points[0];
	D3DXVECTOR3 edge02 = tTriangle.points[2] - tTriangle.points[0];

	// the vector from triangle point 0 to the point
	D3DXVECTOR3 vec0pt = point - tTriangle.points[0];

	// if the angle is greater than 90 from the two edges, then 0 has to be the closest point
	float fD1 = D3DXVec3Dot(&edge01, &vec0pt);
	float fD2 = D3DXVec3Dot(&edge02, &vec0pt);
	if(fD1 <= 0.0f && fD2 <= 0.0f) return tTriangle.points[0];

	// vector from 1 to point
	D3DXVECTOR3 vec1pt = point - tTriangle.points[1];
	
	// if the angle is greater than 90 from two edges, then 1 has to be the closest point
	float fD3 = D3DXVec3Dot(&edge01, &vec1pt);
	float fD4 = D3DXVec3Dot(&edge02, &vec1pt);
	if(fD3 >= 0.0f && fD4 <= fD3) return tTriangle.points[1];

	// check if the point falls in edge01's region, if so project the point onto it
	float fVC = fD1*fD4 - fD3*fD2;
	if(fVC <= 0.0f && fD1 >= 0.0f && fD3 <= 0.0f)
	{
		// using barycentric coordinate system...
		float fV = fD1/(fD1-fD3);
		return tTriangle.points[0] + fV * edge01;
	}

	// vector from 2 to point
	D3DXVECTOR3 vec2pt = point - tTriangle.points[2];
	
	// if the angle is greater than 90 from two edges, then 1 has to be the closest point
	float fD5 = D3DXVec3Dot(&edge01, &vec2pt);
	float fD6 = D3DXVec3Dot(&edge02, &vec2pt);
	if(fD5 >= 0.0f && fD5 <= fD6) return tTriangle.points[2];	

	// check if the point falls in the edge02's region, if so project the point onto it
	float fVB = fD5*fD2 - fD1*fD6;
	if(fVB <= 0.0f && fD2 >= 0.0f && fD6 <= 0.0f)
	{
		// using barycentric coordinate system
		float fW = fD2 / (fD2 - fD6);
		return tTriangle.points[0] + fW * edge02;
	}

	// check if the point falls in the region of the last edge, if so project the point onto it
	float fVA = fD3*fD6 - fD5*fD4;
	if(fVA <= 0.0f && (fD4-fD3) >= 0.0f && (fD5 - fD6) >= 0.0f)
	{
		// using barycentric coordinate system
		float fW = (fD4-fD3)/((fD4-fD3)+(fD5-fD6));
		return tTriangle.points[1] + fW * (tTriangle.points[2] - tTriangle.points[1]);
	}

	// if it makes it to here, then the point must lie within the face region of the triangle.
	// compute return point through barycentric coordinates
	float fDenom = 1.0f / (fVA + fVB + fVC);
	float fV = fVB * fDenom;
	float fW = fVC * fDenom;
	return tTriangle.points[0] + edge01 * fV + edge02 * fW;
}

/** 
* Compute the normal and get the float length from it
* @date Modified April 3, 2006
*/
D3DXVECTOR3 *D3DXVec3Normalize(float *pfLength, D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)
{
	// get the length
	float fLength = fastSqrtf(D3DXVec3Dot(pV, pV));

	// fill the length out the pointer to return
	if(pfLength)
		*pfLength = fLength;
	
	// fill the passed in vector, and return it
	*pOut = *pV / fLength;
	return pOut;
}

/**
* computeDistance
* date Modified March 3, 2006
*/
float computeDistance(const D3DXVECTOR3 &one, const D3DXVECTOR3 &two)
{
	// store the differences in order to prevent repeating the subtraction
	float fX = one.x - two.x;
	float fY = one.y - two.y;
	float fZ = one.z - two.z;

	// use the distance formula to return the correct distance
	return fastSqrtf(fX*fX + fY*fY + fZ*fZ);
}

/**
* computeDistanceSquared()
* date Modified May 3, 2006
*/
float computeDistanceSquared(const D3DXVECTOR3 &one, const D3DXVECTOR3 &two)
{
	// store the differences in order to prevent repeating the subtraction
	D3DXVECTOR3 vec = one - two;
	return D3DXVec3Dot(&vec, &vec);
}

/**
* D3DXVec3Reflect()
* date Modified April 8, 2006
*/
D3DXVECTOR3 *D3DXVec3Reflect(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXVECTOR3 *pNorm)
{
	// Reflection vector = Incident Vector - 2*Normal*(Normal*Incident)
	*pOut = (*pV) - (*pNorm) * (2*D3DXVec3Dot(pV,pNorm));

	// return the pointer
	return pOut;
}

/**
* getRandomFloat
* @date Modified May 20, 2006
*/
float getRandomFloat(void)
{
	return (float)rand() / (float)RAND_MAX;
}

/**
* getRandomFloat
* @date Modified May 20, 2006
*/
float getRandomFloat(float fMin, float fMax)
{
	return (float)((fMax - fMin) * getRandomFloat() + fMin);
}

/**
* getRandomVector
* @date Modified May 20, 2006
*/
void getRandomVector(D3DXVECTOR3* pOut, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax)
{
	pOut->x = getRandomFloat(pMin->x, pMax->x);
	pOut->y = getRandomFloat(pMin->y, pMax->y);
	pOut->z = getRandomFloat(pMin->z, pMax->z);
}

/**
* getRandomVector
* @date Modified May 20, 2006
*/
void getRandomVector(D3DXVECTOR3* pOut, float fMin, float fMax)
{
	pOut->x = getRandomFloat(fMin, fMax);
	pOut->y = getRandomFloat(fMin, fMax);
	pOut->z = getRandomFloat(fMin, fMax);
}
