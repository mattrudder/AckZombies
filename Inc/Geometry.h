/**
* @file Geometry.h
* @author Matt Rudder
* @date Created Mar 13, 2006
*
* Defines structures used in the identification of individual pieces of geometry
* within the game engine and specifically for rendering purposes.
*/

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

// System includes
#include <d3dx9.h>

// Engine includes
#include "Mesh.h"
class CMaterial;

enum ERenderFlag
{
	RENDER_NONE = 0,
	RENDER_USED1 = 0x00000001,
	RENDER_USED2 = 0x00000002,
};
struct SRenderable
{
	DWORD dwRenderFlags;
	float fViewDist;
	SRenderable(void) : dwRenderFlags(RENDER_NONE) { }
	
	virtual void calcSquaredViewDistance(D3DXVECTOR3& vEye) = 0;
	float getSquaredViewDistance(void) { return fViewDist; }
	virtual bool isTransparent(void) = 0;
	virtual unsigned int getHash(void) { return 0; }
};

//! Standalone triangle, stripped from a mesh, for sorting.
struct SRenderTriangle : public SRenderable
{
	CMaterial* m_pMaterial;
	CMesh::SVertex m_oVerts[3];
	D3DXVECTOR3 m_vAveragePos;

	SRenderTriangle();
	void calcSquaredViewDistance(D3DXVECTOR3& vEye);
	bool isTransparent(void);
	virtual unsigned int getHash(void);
};

//! Mesh render object; Used for dynamic objects
struct SRenderMesh : public SRenderable
{
protected:
	//! Mesh instance
	CMesh* poMesh;
	//! Mesh position/orientation
	D3DXMATRIX mMeshWorld;

public:
	void setMesh(CMesh* pMesh) { poMesh = pMesh; }
	void setWorldMat(D3DXMATRIX& mWorld) { mMeshWorld = mWorld; }
	void calcSquaredViewDistance(D3DXVECTOR3& vEye)
	{
		D3DXVECTOR3 vToEye, vPos(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&vPos, &vPos, &mMeshWorld);
		D3DXVec3Subtract(&vToEye, &vEye, &vPos);
		fViewDist = D3DXVec3LengthSq(&vToEye);
	}
	bool isTransparent(void) { return false; }
};

void generateAABB(std::vector<CMesh::SVertex>& vVertex, SAABB& oAABB);


#endif //_GEOMETRY_H_
