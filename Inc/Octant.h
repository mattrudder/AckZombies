/**
* @file Octant.h
* @author Matt Rudder
* @date Created Apr 08, 2006
*
* Component of an Octree, one of the eight children. Serves as a node within the
* tree structure.
*/

#ifndef _OCTANT_H_
#define _OCTANT_H_

// Local includes
#include "Octree.h"

// Engine includes
#include "Mesh.h"
#include "Frustum.h"

#define SHOW_BOUNDS 0

/**
* Component of an Octree, one of the eight children. Serves as a node within the
* tree structure.
*
* @author Matt Rudder
* @date Created Apr 08, 2006
* @date Modified Apr 08, 2006
*/
class COctant
{
	friend class COctree;
public:
	//! ID's for individual octants.
	enum EOctant
	{
		OCT_SPLIT = -1,
		OCT_TOP_FRONT_LEFT = 0,
		OCT_TOP_FRONT_RIGHT,
		OCT_TOP_BACK_LEFT,
		OCT_TOP_BACK_RIGHT,
		OCT_BOTTOM_FRONT_LEFT,
		OCT_BOTTOM_FRONT_RIGHT,
		OCT_BOTTOM_BACK_LEFT,
		OCT_BOTTOM_BACK_RIGHT,
	};
protected:
	//! Size of this octant's bounding box
	float m_fSize;
	//! Center of this octant's bounding box.
	D3DXVECTOR3 m_vCenter;
	//! Number of triangles in the node;
	unsigned short m_usNumTriangles;
	//! Render List
	std::vector<SRenderTriangle*> m_oTriList;
	SRenderTriangle** m_pTriList;
	//! Children nodes
	COctant* m_pChildren[8];
	//! Depth
	unsigned short m_usDepth;
	//! Which octant is this?
	EOctant eOct;
	//! Parent Tree.
	COctree* m_pTree;
	//! Sphere
	static LPD3DXMESH m_pSphere;

	/**
	* Returns the octant the given triangle belongs in
	*
	* @date Created Apr 12, 2006
	* @param[in]	oTri	The triangle to test.
	* @param[in]	vCenter	Center of the current octant.
	* @return The octant the triangle lies in, or -1 if split.
	*/
	EOctant getTriangleOctant(SRenderTriangle* oTri, D3DXVECTOR3& vCenter, std::vector<EOctant>& vSplit);

	/**
	* Converts the internal collection of triangles to a renderable list.
	*
	* @date Created Apr 12, 2006
	*/
	void finalizeOctant(void);
public:

#if SHOW_BOUNDS == 1
	struct SDebugVert
	{
		float x, y, z;
		DWORD dwColor;
	};
	static const DWORD dwDebugFVF = D3DFVF_XYZ|D3DFVF_DIFFUSE;
#endif

	/**
	* Accessors/Modifiers
	* @{
	*/
	float getOctantSize(void)	{return m_fSize;}
	void getOctantCenter(D3DXVECTOR3& vCenter) { vCenter = m_vCenter; }
	unsigned short getNumTriangles(void) { return m_usNumTriangles; }
	COctant** getChildren(void) { return m_pChildren; }
	//!@}

	COctant(std::vector<SRenderTriangle*>& vVertex);
	virtual ~COctant(void);

	/**
	* Calculate the octants bounding box based on a list of vertices. Usually
	* used to prime the root node.
	*
	* @date Created Apr 08, 2006
	*/
	void calculateBounds(std::vector<CMesh::SVertex>& vVertex);

	/**
	* Recursively builds the current node and children.
	*
	* @date Created Apr 08, 2006
	* @param[in]	pMesh			The parent mesh;
	* @param[in]	pVertList		List of verticies to build the tree from.
	* @param[in]	pIndexList		List of indicies to build the tree from.
	* @param[in]	pSubsetList		List of subset/triangle assosications.
	* @param[in]	dwNumVerts		Number of vertices in pIndexList.
	* @param[in]	usMinTriangles	Minimum number of triangles that should be in an octant.
	* @param[in]	usMaxLevels		Maximum number of levels of depth the tree should go.
	*/
	void buildOctant(unsigned short usMinTriangles, unsigned short usMaxLevels);

	/**
	* Deallocates all memory associated with the octant.
	*
	* @date Created Apr 08, 2006
	*/
	void destroyOctant(void);

	/**
	* Calculates the center of a new octant, based on its parent's information,
	* and the type of the new octant.
	*
	* @date Created Apr 08, 2006
	* @param[out]	vChildCenter		The center of the new octant.
	* @param[in]	vParentCenter		The center of the parent octant.
	* @param[in]	fParentSize			Size of the parent's bounding box.
	* @param[in]	eOct				ID of the new octant.
	*/
	void getOctantCenter(D3DXVECTOR3& vChildCenter, D3DXVECTOR3& vParentCenter, 
		float fParentSize, EOctant eOct);

	/**
	* Traverse the tree and draws nodes that fall in the given frustum.
	*
	* @date Created Apr 08, 2006
	* @param[in]	oFrustum		The frustum to check against.
	* @param[in]	eLastTest		The result of the last frustum test.
	*/
	void renderTree(CFrustum& oFrustum, CRenderQueue& oQueue, ERenderFlag eFlag, CFrustum::EFrustumTest eLastTest = CFrustum::TEST_OUT);

	/**
	* Returns the number of geometric primitives in the octant and any suboctants.
	*
	* @date Created Apr 11, 2006
	* @return Primitive count.
	*/
	DWORD getPrimitiveCount(void);
};

#endif //_OCTANT_H_