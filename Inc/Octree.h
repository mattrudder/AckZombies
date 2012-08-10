/**
* @file Octree.h
* @author Matt Rudder
* @date Created Apr 08, 2006
*
* 3D spatial structure used to partition level geometry for optimal rendering.
*/

#ifndef _OCTREE_H_
#define _OCTREE_H_

// Engine includes
#include "Mesh.h"
#include "Frustum.h"
#include "RenderQueue.h"
#include "Singleton.h"

class COctant;
class CMaterial;

/**
* 3D spatial structure used to partition level geometry for optimal rendering.
*
* @author Matt Rudder
* @date Created Apr 08, 2006
* @date Modified Apr 08, 2006
*/
class COctree : public CSingleton<COctree>
{
	friend class CSingleton<COctree>;
public:
	// Triangle structure used for splitting geometry.
	struct SStaticTriangle
	{
		CMesh::SVertex oVerts[3];
		CMaterial* pMaterial;
	};

protected:
	//! Vertex data; For tree building
	CMesh::SVertex* m_pVerts;
	std::vector<SRenderTriangle> m_vTriangles;
	SRenderTriangle* m_pTriCollection;
	//! Index data; For tree building
	unsigned short* m_pIndicies;
	//! Beginning of the tree
	COctant* m_pHead;
	CRenderQueue* m_pQueue;

	//! Singleton stuff.
	//!@{
	COctree(void);
	COctree(const COctree&) {}
	operator=(const COctree&){}
	virtual ~COctree(void);
	//!@}

public:

	static const D3DXCOLOR dwDepthColor[];

	/**
	* Builds a octree using a single mesh.
	*
	* @date Created Apr 08, 2006
	* @param[in]	usMinTriangles	The lower triangle limit.
	* @param[in]	usMaxLevel		The upper level limit.
	*/
	void initializeTree(std::vector<CMesh::SVertex>& vVertex, std::vector<CMaterial*>& vMaterials, unsigned short usMinTriangles, unsigned short usMaxLevels);

	/**
	* Cleans up a previously allocated tree.
	*
	* @date Created Apr 08, 2006
	*/
	void deallocateTree(void);

	/**
	* Traverse the tree and draws nodes that fall in the given frustum.
	*
	* @date Created Apr 08, 2006
	* @param[in]	oFrustum		The frustum to check against.
	*/
	void renderTree(CFrustum& oFrustum);
};

#endif //_OCTREE_H_