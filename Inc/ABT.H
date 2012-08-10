/**
* @file ABT.h
* @author Matt Rudder
* @date Created Apr 17, 2006
*
* Adaptive Binary Tree structure used to optimize the rendering of static world geometry.
* Algorithms referenced from the article sample found in Game Programming Gems 6 by
* Martin Fleisz titled Spatial Partitioning using ABTs.
*/

#ifndef _ABT_H_
#define _ABT_H_

// System includes
#include <vector>

// Engine includes
#include "Mesh.h"
#include "Frustum.h"
#include "RenderQueue.h"

// ABT Settings
#define ABT_MAX_NODETRIANGLES 2500

// ABT Score Weightings
#define ABT_WEIGHT_SPACELOCATION	0.15f
#define ABT_WEIGHT_CHILDVOLUMES		0.10f
#define ABT_WEIGHT_FACECOUNT		0.20f
#define ABT_WEIGHT_SPLITFACES		0.55f

// defines the number of samples along each axis
#define ABT_PLANE_SAMPLES			10

// growth factor (10% here)
#define ABT_GROWTH_FACTOR			0.1f

// 
#define ABT_EPSILSON				0.0000001f

/**
* Binary Tree structure used to optimize the rendering of static world geometry.
*
* @author Matt Rudder
* @date Created Apr 17, 2006
* @date Modified Apr 17, 2006
*/
class CABT : public CSingleton<CABT>
{
	friend class CSingleton<CABT>;
protected:
	struct SDebugVert
	{
		float x, y, z;
		DWORD dwColor;
	};
	static const DWORD dwDebugFVF = D3DFVF_XYZ|D3DFVF_DIFFUSE;

	//! Node that makes up the Adaptive Binary Tree structure.
	struct SABTNode
	{
		//! Determines if this node is a leaf node.
		bool m_bIsLeaf;
		//! AABB that makes up the node
		SAABB m_oAABB;
		//! Collection of triangle in this node.
		SRenderTriangle* m_pTris;
		//! Vertex buffer for rendering. 
		LPDIRECT3DVERTEXBUFFER9	m_pVB;
		//! Number of vertices in this node.
		size_t m_nVertCount;
		//! Child nodes
		SABTNode* m_pLeft, *m_pRight;
		//! Color of the node's geometry (for debug)
		D3DXCOLOR dwColor;
	};

	//! Classification of a point in respect to a plane
	enum EPointClass
	{
		PT_FRONT,
		PT_BACK,
		PT_COPLANAR,
	};

	//! Classification of a triangle in respect to a plane
	enum ETriClass
	{
		TRI_FRONT,
		TRI_BACK,
		TRI_COPLANAR,
		TRI_SPLIT,
	};

	struct SPlane
	{
		D3DXVECTOR3 normal;
		D3DXVECTOR3 point;
	};

	//! Beginning of the tree (contains the entire level)
	SABTNode* m_pRoot;
	//! Number of rendered triangles
	size_t m_nNumTrianglesRendered;
	//! Number of triangles in the final tree
	size_t m_nNumTrianglesTotal;
	//! Queue for render sorting.
	CRenderQueue* m_pRenderQueue;
	//! Counters for geometry types
	DWORD m_dwNumSolids, m_dwNumTrans;

	/**
	* Renders the ABT using a passed in camera frustum.
	* 
	* @date Created Apr 17, 2006
	* @param[in]	oFrustum	The view frustum to check collision with.
	* @param[in]	pNode		The node to render.
	* @param[in]	bContained	Determines if pNode is fully contained in the frustum.
	*/
	void renderTree(CFrustum& oFrustum, SABTNode* pNode, bool bContained = false);

	/**
	* Builds the ABT from a collection of vertex data.
	* 
	* @date Created Apr 17, 2006
	* @param[in]	vVertex		Vector filled with vertex data.
	* @param[in]	vMaterials	Vector filled with material data for the triangles in vVertex.
	* @return Pointer to the beginning of the subtree.
	*/
	SABTNode* createSubtree(std::vector<CMesh::SVertex>& vVertex, std::vector<CMaterial*>& vMaterials);

	/**
	* Deallocates a tree from a given point.
	* 
	* @date Created Apr 17, 2006
	* @param[in]	pNode		The node to destroy from.
	*/
	void destroyTree(SABTNode* pNode);

	/**
	* Determines the best splitting plane, given a set of vertices, and a bounding box.
	* 
	* @date Created Apr 18, 2006
	* @param[in]	vVertex		The collection of vertex data.
	* @param[in]	oAABB		Bounding box surrounding all the vertices in vVertex.
	* @param[out]	oPlane		The plane to fill out.	
	* @return The axis of the volume split by the plane.
	*/
	unsigned int getSplittingPlane(std::vector<CMesh::SVertex>& vVertex, SAABB& oAABB, SPlane& oPlane);

	/**
	* Determines which side of a given plane, a given point lies.
	* 
	* @date Created Apr 18, 2006
	* @param[in]	vPoint		The point to check.
	* @param[in]	oPlane		The plane to check against.
	* @return Classification of vPoint in respect to oPlane
	*/
	EPointClass getPointClassification(D3DXVECTOR3& vPoint, SPlane& oPlane);

	/**
	* Determines which side of a given plane, a given triangle lies.
	* 
	* @date Created Apr 18, 2006
	* @param[in]	pTri		The array of verts to check; Makes up a triangle
	* @param[in]	oPlane		The plane to check against.
	* @return Classification of vPoint in respect to oPlane
	*/
	ETriClass getTriangleClassification(CMesh::SVertex* pTri, SPlane& oPlane);

	/**
	* Determines how much volume of an AABB a plane splits off.
	* 
	* @date Created Apr 18, 2006
	* @param[in]	pVertex		The polygon to split.
	* @param[in]	oPlane		The plane to split against.
	* @param[out]	vFront		Collection of triangles that lay on the front of the plane.
	* @param[out]	vBack		Collection of triangles that lay on the back of the plane.
	*/
	void splitTriangle(CMesh::SVertex* pVertex, SPlane& oPlane, std::vector<CMesh::SVertex>& vFront, std::vector<CMesh::SVertex>& vBack);

	/**
	* Determines how much volume of an AABB a plane splits off.
	* 
	* @date Created Apr 18, 2006
	* @param[in]	oPlane		The plane to check against.
	* @param[in]	oAABB		The volume to check.
	* @return Percentage of the volume.
	*/
	float getSplitPercent(SPlane& oPlane, SAABB& oAABB)
	{
		D3DXVECTOR3 vPlaneToMin, vAABBLen;
		D3DXVec3Subtract(&vPlaneToMin, &oPlane.point, &oAABB.min);
		D3DXVec3Subtract(&vAABBLen, &oAABB.max, &oAABB.min);
		return D3DXVec3Dot(&oPlane.normal, &vPlaneToMin) / D3DXVec3Dot(&oPlane.normal, &vAABBLen);
	}

	//! Singleton stuff.
	//!@{
	CABT(void);
	CABT(const CABT&) {}
	operator=(const CABT&){}
	virtual ~CABT(void);
	//!@}

public:

	/**
	* Accessors/Modifiers
	* @{
	*/
	unsigned int getRenderedPrimitiveCount(void) { return (unsigned int)m_nNumTrianglesRendered; }
	unsigned int getPrimitiveCount(void) { return (unsigned int)m_nNumTrianglesTotal; }
	//!@}

	/**
	* Builds the ABT from a collection of vertex data.
	* 
	* @date Created Apr 17, 2006
	* @param[in]	vVertex		Vector filled with vertex data.
	* @param[in]	vMaterials	Vector filled with material data for the triangles in vVertex.
	*/
	void createTree(std::vector<CMesh::SVertex>& vVertex, std::vector<CMaterial*>& vMaterials);

	/**
	* Deallocates all memory belonging to the ABT.
	* 
	* @date Created Apr 17, 2006
	*/
	void destroyTree(void);


	void drawBoundingBox(SAABB& oAABB, D3DXCOLOR dwColor);

	/**
	* Renders the ABT using a passed in camera frustum.
	* 
	* @date Created Apr 17, 2006
	* @param[in]	oFrustum	The view frustum to check collision with.
	*/
	void renderTree(CFrustum& oFrustum);
};

#endif //_ABT_H_