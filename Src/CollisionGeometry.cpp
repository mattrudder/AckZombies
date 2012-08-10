/**
* @file CollisionGeometry.cpp
* @author Tom Williams
* @date Modified April 13, 2006
*
* Bounding volume tree for the world geometry
*/
#include ".\collisiongeometry.h"
#include "RenderSystem.h"

/**
* CCollisionGeometry::CCollisionGeometry()
* @date Modified April 9, 2006
*/
CCollisionGeometry::CCollisionGeometry(void)
{
	m_ptRoot = NULL;
}

/**
* CCollisionGeometry::~CCollisionGeometry()
* @date Modified April 9, 2006
*/
CCollisionGeometry::~CCollisionGeometry(void)
{
}

/**
* CCollisionGeometry::draw()
* @date Modified April 12, 2006
*/
void CCollisionGeometry::draw(STreeNode *ptNode)
{
	// if the node's empty, just stop
	if(!ptNode)
		return;

	// if there's no triangles, continue moving
	if(ptNode->triangles.empty())
	{
		draw(ptNode->ptLeft);
		draw(ptNode->ptRight);
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	LPDIRECT3DVERTEXBUFFER9 pLines;

	// make an array of points to draw
	SColGeomFVF points[16];
	points[0].pos = ptNode->tData.max;
	points[1].pos = D3DXVECTOR3(ptNode->tData.max.x, ptNode->tData.min.y, ptNode->tData.max.z);
	points[2].pos = D3DXVECTOR3(ptNode->tData.min.x, ptNode->tData.min.y, ptNode->tData.max.z);
	points[3].pos = D3DXVECTOR3(ptNode->tData.min.x, ptNode->tData.max.y, ptNode->tData.max.z);
	points[4].pos = D3DXVECTOR3(ptNode->tData.min.x, ptNode->tData.max.y, ptNode->tData.min.z);
	points[5].pos = D3DXVECTOR3(ptNode->tData.max.x, ptNode->tData.max.y, ptNode->tData.min.z);
	points[6].pos = D3DXVECTOR3(ptNode->tData.max.x, ptNode->tData.min.y, ptNode->tData.min.z);
	points[7].pos = ptNode->tData.min;
	points[8].pos = D3DXVECTOR3(ptNode->tData.min.x, ptNode->tData.max.y, ptNode->tData.min.z);
	points[9].pos = ptNode->tData.min;
	points[10].pos = D3DXVECTOR3(ptNode->tData.min.x, ptNode->tData.min.y, ptNode->tData.max.z);
	points[11].pos = D3DXVECTOR3(ptNode->tData.max.x, ptNode->tData.min.y, ptNode->tData.max.z);
	points[12].pos = D3DXVECTOR3(ptNode->tData.max.x, ptNode->tData.min.y, ptNode->tData.min.z);
	points[13].pos = D3DXVECTOR3(ptNode->tData.max.x, ptNode->tData.max.y, ptNode->tData.min.z);
	points[14].pos = ptNode->tData.max;
	points[15].pos = D3DXVECTOR3(ptNode->tData.min.x, ptNode->tData.max.y, ptNode->tData.max.z);

	// create the vertex buffer to draw them
	pDevice->CreateVertexBuffer(sizeof(SColGeomFVF)*16, D3DUSAGE_DYNAMIC, SColGeomFVF::FVF_Flags, D3DPOOL_DEFAULT,
		&pLines, NULL);

	// lock the vertex buffer and fill it
	void *toFill;
	pLines->Lock(0,0,&toFill,0);
	memcpy(toFill, &points[0], sizeof(SColGeomFVF)*16);
	pLines->Unlock();

	// draw the lines finally
	pDevice->SetStreamSource(0, pLines, 0, sizeof(SColGeomFVF));
	pDevice->SetFVF(SColGeomFVF::FVF_Flags);
    pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 15);	

	pLines->Release();
}

/**
* CCollisionGeometry::initTree()
* @date Modified April 9, 2006
*/
void CCollisionGeometry::initTree(const char *szXMeshFile, int nMaxLeafSize)
{
	// if there's already a root, delete the tree
	if(m_ptRoot)
	{
		deleteTree(m_ptRoot);
		m_ptRoot = NULL;
	}

	// store the max leaf size
	m_nMaxLeafSize = nMaxLeafSize;

	// load the mesh from the passed in file
	ID3DXMesh *pMesh, *pMeshFVF;
	LPDIRECT3DDEVICE9 pD3DDevice = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	CMesh *pTemp = (CMesh*)CResourceManager::getInstance().loadResource(RES_MESH, szXMeshFile);
	pMesh = pTemp->getD3DXMesh();

	// rip the triangles out of the mesh
	STriangle triangle;
	std::vector<STriangle> triangles;

	// clone the mesh to meet my fvf standard
	pMesh->CloneMeshFVF(D3DXMESH_MANAGED, SColGeomFVF::FVF_Flags, pD3DDevice, &pMeshFVF);
	pTemp->Release();

	// lock the vertex and index buffer
	IDirect3DVertexBuffer9 *pVertexBuffer;
	IDirect3DIndexBuffer9 *pIndexBuffer;
	pMeshFVF->GetVertexBuffer(&pVertexBuffer);
	pMeshFVF->GetIndexBuffer(&pIndexBuffer);

	// get the verts out of the mesh
	SColGeomFVF *pVerts;
	pVertexBuffer->Lock(0, 0, (void**)&pVerts, 0);
	unsigned short *pIndices;
	pIndexBuffer->Lock(0,0,(void**)&pIndices,0);

	for(DWORD i = 0; i < pMeshFVF->GetNumFaces(); ++i)
	{ 
		triangle.points[0] = pVerts[pIndices[i*3]].pos;
		triangle.points[1] = pVerts[pIndices[i*3+1]].pos;
		triangle.points[2] = pVerts[pIndices[i*3+2]].pos;

		// add them to the triangle list
		triangle.normal = pVerts[pIndices[i*3]].normal;
		triangles.push_back(triangle);
	}

	// unlock the vertex and index buffer and release the mesh
	pVertexBuffer->Unlock();
	pIndexBuffer->Unlock();
	pMeshFVF->Release();

	// create the root of the tree
	m_ptRoot = new STreeNode;
	for(size_t i = 0; i < triangles.size(); ++i)
		m_ptRoot->triangles.push_back(triangles[i]);

	// create the BV of the root
	createBV(m_ptRoot);

	// all that's left is to build the tree
	buildTree(m_ptRoot);	
}

/**
* CCollisionGeometry::buildTree()
* @date Modified April 9, 2006
*/
void CCollisionGeometry::buildTree(STreeNode *ptNode)
{
	// if the max leaf size was reached, stop splitting
	size_t i = ptNode->triangles.size();
	if(ptNode->triangles.size() <= (size_t)m_nMaxLeafSize)
		return;

	// create two children
	ptNode->ptLeft = new STreeNode;
	ptNode->ptRight = new STreeNode;
	ptNode->ptLeft->ptParent = ptNode->ptRight->ptParent = ptNode;

	// split the parent's triangle list and give it to the children
	splitTriangles(ptNode, ptNode->ptLeft->triangles, ptNode->ptRight->triangles);
	// build the children's bounding volume
	createBV(ptNode->ptLeft);
	createBV(ptNode->ptRight);

	buildTree(ptNode->ptLeft);
	buildTree(ptNode->ptRight);
}

/**
* CCollisionGeometry::splitTriangles()
* @date Modified April 9, 2006
*/
void CCollisionGeometry::splitTriangles(STreeNode *ptNode, std::vector<STriangle> &one, std::vector<STriangle> &two)
{
	// find the largest axis
	// store the highest and lowest values for each axis
	D3DXVECTOR3 pos, neg;
	D3DXVECTOR3 avgPos(0.0f, 0.0f, 0.0f);
	pos = neg = ptNode->triangles[0].points[0];

	// list of center points for the triangles
	std::vector<D3DXVECTOR3> centroids;
	D3DXVECTOR3 temp(avgPos);

	for(size_t i = 0; i < ptNode->triangles.size(); ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			// find and store the farthest x component
			if(ptNode->triangles[i].points[j].x > pos.x)
				pos.x = ptNode->triangles[i].points[j].x;
			else if(ptNode->triangles[i].points[j].x < neg.x)
				neg.x = ptNode->triangles[i].points[j].x;

			// find and store the farthest y component
			if(ptNode->triangles[i].points[j].y > pos.y)
				pos.y = ptNode->triangles[i].points[j].y;
			else if(ptNode->triangles[i].points[j].y < neg.y)
				neg.y = ptNode->triangles[i].points[j].y;

			// find and store the farthest z component
			if(ptNode->triangles[i].points[j].z > pos.z)
				pos.z = ptNode->triangles[i].points[j].z;
			else if(ptNode->triangles[i].points[j].z < neg.z)
				neg.z = ptNode->triangles[i].points[j].z;

			avgPos += ptNode->triangles[i].points[j]; 
			temp += ptNode->triangles[i].points[j];
		}

		centroids.push_back(temp/3.0f);
		temp = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	// find the average point
	avgPos /= ((float)ptNode->triangles.size() * 3.0f);

	// get the difference between the two farthest points.
	pos -= neg;

	// use this char to switch on for the largest axis
	char cLargestAxis;
	// wicked ugly ternary operator check to find the largest axis
	pos.x > pos.y ? pos.x > pos.z ? cLargestAxis = 0 : cLargestAxis = 2 : pos.y > pos.z ? cLargestAxis = 1 : cLargestAxis = 2;

	// split the triangles based on the avg point and largest axis between the two children
	switch(cLargestAxis)
	{
	case 0:
		for(size_t i = 0; i < ptNode->triangles.size(); ++i)
		{
			if(centroids[i].x < avgPos.x)
				one.push_back(ptNode->triangles[i]);
			else 
				two.push_back(ptNode->triangles[i]);
		}
		break;

	case 1:
		for(size_t i = 0; i < ptNode->triangles.size(); ++i)
		{
			if(centroids[i].y < avgPos.y)
				one.push_back(ptNode->triangles[i]);
			else
				two.push_back(ptNode->triangles[i]);
		}
		break;

	case 2:
		for(size_t i = 0; i < ptNode->triangles.size(); ++i)
		{
			if(centroids[i].z < avgPos.z)
				one.push_back(ptNode->triangles[i]);
			else
				two.push_back(ptNode->triangles[i]);
		}
		break;
	}


	// make sure the vectors are fairly balanced
	// do this in case all triangles share the same centroid
	if(two.empty())
	{
		std::vector<STriangle>::iterator iter = one.begin();
		size_t i = one.size()/2;
		while(i)
		{
			two.push_back(*iter);
			iter = one.erase(iter);
			--i;
		}
	}
	else if(one.empty())
	{
		std::vector<STriangle>::iterator iter = two.begin();
		size_t i = two.size()/2;
		while(i)
		{
			one.push_back(*iter);
			iter = two.erase(iter);
			--i;
		}
		i = one.size();
		i = two.size();
	}

	ptNode->triangles.clear();
}

/**
* CCollisionGeometry::deleteTree()
* @date Modified April 9, 2006
*/
void CCollisionGeometry::deleteTree(STreeNode *ptNode)
{
	// delete all the children first
	if(ptNode->ptLeft)
		deleteTree(ptNode->ptLeft);
	if(ptNode->ptRight)
		deleteTree(ptNode->ptRight);

	// remove the node from the tree

	// if the node has a parent, set the parent's pointer to null
	if(ptNode->ptParent)
	{
		if(ptNode->ptParent->ptLeft == ptNode)
			ptNode->ptParent->ptLeft = NULL;
		else
			ptNode->ptParent->ptRight = NULL;
	}

	// delete the node
	delete ptNode;
}

/**
* CCollisionGeometry::createBV()
* @date Modified April 9, 2006
*/
void CCollisionGeometry::createBV(STreeNode *ptNode)
{
	// make sure there are triangles
	if(ptNode->triangles.empty())
		return;

	// find the farthest positive and negative points of the object
	ptNode->tData.max = ptNode->tData.min = ptNode->triangles[0].points[0];

	for(size_t i = 0; i < ptNode->triangles.size(); ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			// find and store the farthest x component
			if(ptNode->triangles[i].points[j].x > ptNode->tData.max.x)
				ptNode->tData.max.x = ptNode->triangles[i].points[j].x;
			else if(ptNode->triangles[i].points[j].x < ptNode->tData.min.x)
				ptNode->tData.min.x = ptNode->triangles[i].points[j].x;

			// find and store the farthest y component
			if(ptNode->triangles[i].points[j].y > ptNode->tData.max.y)
				ptNode->tData.max.y = ptNode->triangles[i].points[j].y;
			else if(ptNode->triangles[i].points[j].y < ptNode->tData.min.y)
				ptNode->tData.min.y = ptNode->triangles[i].points[j].y;

			// find and store the farthest z component
			if(ptNode->triangles[i].points[j].z > ptNode->tData.max.z)
				ptNode->tData.max.z = ptNode->triangles[i].points[j].z;
			else if(ptNode->triangles[i].points[j].z < ptNode->tData.min.z)
				ptNode->tData.min.z = ptNode->triangles[i].points[j].z;
		}
	}
}

/**
* CCollisionGeometry::checkSphereCollision()
* @date Modified April 9, 2006
*/
bool CCollisionGeometry::checkSphereCollision(std::vector<D3DXVECTOR3> &ptsOfCol, std::vector<STriangle> &colTris, STreeNode *ptNode, const SSphere &tSphere)
{
	// check for collision between the bounding sphere and the AABB
	if(checkCol_SphereAABB(tSphere, ptNode->tData))
	{
		// if it has children, continue traversing through the tree
		if(ptNode->ptLeft && ptNode->ptRight)
		{
			// check both the left and right sides for collision
			bool b = false;
			if(checkSphereCollision(ptsOfCol, colTris, ptNode->ptLeft, tSphere)) b = true;
			if(checkSphereCollision(ptsOfCol, colTris, ptNode->ptRight, tSphere))b = true;
			return b;
		}

		// if this is a leaf node, check for collision between the triangles
		D3DXVECTOR3 point;
		size_t nSize = ptsOfCol.size();
		for(size_t i = 0; i < ptNode->triangles.size(); ++i)
		{
			// if the sphere is on the wrong side of the triangle, don't bother checking for collision
			// get the vector between the triangle and the sphere
			point = ptNode->triangles[i].points[0] - tSphere.centerPt;
			// if the player is on the side opposite the way the normal is facing, don't check for collision
			if(D3DXVec3Dot(&point, &ptNode->triangles[i].normal) > 0.0f)
				continue;

			if(checkCol_SphereTriangle(&point, tSphere, ptNode->triangles[i]))
			{
				ptsOfCol.push_back(point);
				colTris.push_back(ptNode->triangles[i]);
			}
		}

		// if the vector size went up, return true
		if(ptsOfCol.size() > nSize)
			return true;

		// return no collision
		return false;
	}

	// if the sphere doesn't even intersect the AABB, return no collision
	return false;
}

/**
* CCollisionGeometry::checkRayCollision()
* @date Modified April 13, 2006
*/
bool CCollisionGeometry::checkRayCollision(std::vector<D3DXVECTOR3> &ptsOfCol, std::vector<float> &dirLengths, STreeNode *ptNode, const D3DXVECTOR3 &point, const D3DXVECTOR3 &dir)
{
	// check for collision between the bounding sphere and the AABB
	if(checkCol_RayAABB(point, dir, ptNode->tData))
	{
		// if it has children, continue traversing through the tree
		if(ptNode->ptLeft && ptNode->ptRight)
		{
			// check both the left and right sides for collision
			bool b = false;
			if(checkRayCollision(ptsOfCol, dirLengths, ptNode->ptLeft, point, dir)) b = true;
			if(checkRayCollision(ptsOfCol, dirLengths, ptNode->ptRight, point, dir))b = true;
			return b;
		}

		// if this is a leaf node, check for collision between the triangles
		D3DXVECTOR3 ptOfCol;
		float fLength;
		size_t nSize = ptsOfCol.size();
		for(size_t i = 0; i < ptNode->triangles.size(); ++i)
		{
			// if the sphere is on the wrong side of the triangle, don't bother checking for collision
			// get the vector between the triangle and the sphere
			ptOfCol = ptNode->triangles[i].points[0] - point;
			// if the player is on the side opposite the way the normal is facing, don't check for collision
			if(D3DXVec3Dot(&ptOfCol, &ptNode->triangles[i].normal) > 0.0f)
				continue;

			if(checkCol_RayTriangle(&ptOfCol, fLength, point, dir, ptNode->triangles[i]))
			{
				ptsOfCol.push_back(ptOfCol);
				dirLengths.push_back(fLength);
			}
		}

		// if the vector size went up, return true
		if(ptsOfCol.size() > nSize)
			return true;

		// return no collision
		return false;
	}

	// if the sphere doesn't even intersect the AABB, return no collision
	return false;
}