/**
* @file CollisionGeometry.h
* @author Tom Williams
* @date Modified April 9, 2006
*
* Bounding volume tree for the world geometry
*/
#ifndef _COLLISIONGEOMETRY_H_
#define _COLLISIONGEOMETRY_H_

// local includes/libraries
#include "Math3d.h"
#include "Singleton.h"

// system includes/libraries
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

// use to grab only the position out of the mesh
struct SColGeomFVF
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL
	};
};

struct STreeNode
{
	//! the bounding sphere
	SAABB tData;
	//! the left child of the node
	STreeNode *ptLeft;
	//! the right child of the node
	STreeNode *ptRight;
	//! the parent of the node
	STreeNode *ptParent;
	//! the triangles in the node
	std::vector<STriangle> triangles;

	/**
	* Constructor for the tree nodes
	*
	* @date Created March 30, 2006
	*/
	STreeNode() {ptLeft = ptRight = ptParent = NULL;}
};

/**
* Loads collision geometry, sorts into an AABB tree, and then checks actors against the tree
*
* @author Tom Williams
* @date Created March 27, 2006
* @date Modified June 7, 2006
*/
class CCollisionGeometry : public CSingleton<CCollisionGeometry>
{
	friend class CSingleton<CCollisionGeometry>;
	//! the root of the tree
	STreeNode *m_ptRoot;

	//! how far to split the model
	int m_nMaxLeafSize;

	/** 
	* Singleton stuff
	*
	* @date Created April 9, 2006
	* @{
	*/
	CCollisionGeometry();
	~CCollisionGeometry();
	CCollisionGeometry(const CCollisionGeometry &oCG) {}
	CCollisionGeometry &operator =(const CCollisionGeometry &oCG) {}
	//!}

	/**
	* build the tree starting at the root (called by init)
	*
	* @date Created April 9, 2006
	* @param[in]	nMoney	The amount to add.
	*/
	void buildTree(STreeNode *ptNode);

	/**
	* split the triangles in a node into two separate lists
	*
	* @date Created April 9, 2006
	* @param[in]	ptNode	the node whose triangles need to be split.
	* @param[out]	one		one list of triangles
	* @param[out]	two		the other list of triangles
	*/
	void splitTriangles(STreeNode *ptNode, std::vector<STriangle> &one, std::vector<STriangle> &two);

public:
	/**
	* accessor for the root of the collision tree
	*
	* @date Created April 9, 2006
	* @return	the root of the tree
	*/
	STreeNode *getRoot() {return m_ptRoot;}

	/**
	* initialize the collision tree
	*
	* @date Created April 9, 2006
	* @param[in]	szXMeshFile		the name of the mesh file to load.
	* @param[in]	nMaxLeafSize	the max amt of triangles allowed for a leaf node
	*/
	void initTree(const char *szXMeshFile, int nMaxLeafSize);

	/**
	* delete all the nodes under the passed in node
	*
	* @date Created March 27, 2006
	* @param[in]	ptNode	the node to begin removal from.
	*/
	void deleteTree(STreeNode *ptNode);

	/**
	* create the AABB for the triangles in the passed in node
	*
	* @date Created April 9, 2006
	* @param[in]	ptNode	the node whose triangles need a bounding volume.
	*/
	void createBV(STreeNode *ptNode);

	/**
	* check sphere collision between a node and its children's bounding volume
	*
	* @date Created April 9, 2006
	* @param[out]	pPtOfCol	the returned collision point.
	* @param[out]	colTris		the collided triangles list
	* @param[in]	ptNode		the node to begin checking collision from
	* @param[in]	tSphere		the sphere to check collision against
	* @return		whether or not collision occured
	*/
	bool checkSphereCollision(std::vector<D3DXVECTOR3> &ptsOfCol, std::vector<STriangle> &colTris, STreeNode *ptNode, const SSphere &tSphere);

	/**
	* check ray collision between a node and its children's bounding volume
	*
	* @date Created April 9, 2006
	* @param[out]	ptsOfCol	the returned collision points.

	* @param[in]	ptNode		the node to begin checking collision from
	* @param[in]	point		the origin point of the ray
	* @param[in]	dir			the direction and magnitude of the ray
	* @return		whether or not collision occured
	*/
	bool checkRayCollision(std::vector<D3DXVECTOR3> &ptsOfCol, std::vector<float> &dirLengths, STreeNode *ptNode, const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir);

	/**
	* draw the bounding volume of all the leaf nodes
	*
	* @date Created April 9, 2006
	* @param[in]	ptNode		the node to begin drawing from
	*/
	void draw(STreeNode *ptNode);
};

#endif