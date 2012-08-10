/**
* @file Geometry.cpp
* @author Matt Rudder
* @date Modified Apr 07, 2006
*
* Defines structures used in the identification of individual pieces of geometry
* within the game engine and specifically for rendering purposes.
*/

// Engine includes
#include "Material.h"

SRenderTriangle::SRenderTriangle(void) : m_pMaterial(NULL) { }
void SRenderTriangle::calcSquaredViewDistance(D3DXVECTOR3& vEye)
{
	D3DXVECTOR3 vToEye;
	D3DXVec3Subtract(&vToEye, &vEye, &m_vAveragePos);
	fViewDist = D3DXVec3LengthSq(&vToEye);
}
bool SRenderTriangle::isTransparent(void) { return m_pMaterial ? m_pMaterial->isTransparent() : false; }
unsigned int SRenderTriangle::getHash(void) { return m_pMaterial ? m_pMaterial->getHash() : 0; }

/**
* generateAABB
* @date Modified Apr 17, 2006
*/
void generateAABB(std::vector<CMesh::SVertex>& vVertex, SAABB& oAABB)
{
	std::vector<CMesh::SVertex>::iterator itVert = vVertex.begin();
	oAABB.max = oAABB.min = D3DXVECTOR3(0, 0, 0);
	while(itVert != vVertex.end())
	{
		// Check against current bounds
		if(itVert->vPosition.x < oAABB.min.x)
			oAABB.min.x = itVert->vPosition.x;

		if(itVert->vPosition.y < oAABB.min.y)
			oAABB.min.y = itVert->vPosition.y;

		if(itVert->vPosition.z < oAABB.min.z)
			oAABB.min.z = itVert->vPosition.z;

		if(itVert->vPosition.x > oAABB.max.x)
			oAABB.max.x = itVert->vPosition.x;

		if(itVert->vPosition.y > oAABB.max.y)
			oAABB.max.y = itVert->vPosition.y;

		if(itVert->vPosition.z > oAABB.max.z)
			oAABB.max.z = itVert->vPosition.z;

		// Next vertex.
		++itVert;
	}
}