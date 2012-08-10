/**
* @file Octree.cpp
* @author Matt Rudder
* @date Modified Apr 08, 2006
*
* 3D spatial structure used to partition level geometry for optimal rendering.
*/

// Local includes
#include "Octree.h"
#include "Octant.h"
#include "Material.h"

const D3DXCOLOR COctree::dwDepthColor[] =
{
	D3DCOLOR_XRGB(255, 255, 255),
	D3DCOLOR_XRGB(0, 215, 0),
	D3DCOLOR_XRGB(128, 215, 0),
	D3DCOLOR_XRGB(215, 215, 0),
	D3DCOLOR_XRGB(215, 128, 0),
	D3DCOLOR_XRGB(215, 0, 0),
	D3DCOLOR_XRGB(215, 0, 128),
	D3DCOLOR_XRGB(215, 0, 215),
};

/**
* COctree::COctree
* @date Modified Apr 08, 2006
*/
COctree::COctree(void) : m_pVerts(0), m_pIndicies(0), m_pHead(0), m_pTriCollection(0), m_pQueue(0)
{
}

/**
* COctree::~COctree
* @date Modified Apr 08, 2006
*/
COctree::~COctree(void)
{
	deallocateTree();
}

/**
* COctree::initializeTree
* @date Modified Apr 08, 2006
*/
void COctree::initializeTree(std::vector<CMesh::SVertex>& vVertex, std::vector<CMaterial*>& vMaterials, unsigned short usMinTriangles, unsigned short usMaxLevels)
{
	if(m_pHead)
		deallocateTree();


	// Build triangle list
	size_t nCurTri;
	SRenderTriangle oTri;
	DWORD dwSolid = 0, dwTrans = 0;
	m_pTriCollection = new SRenderTriangle[vMaterials.size()];
	std::vector<SRenderTriangle*> vTris;
	for(size_t i = 0; i < vVertex.size(); i+=3)
	{
		nCurTri = i/3;
		m_pTriCollection[nCurTri].m_oVerts[0] = vVertex[i];
		m_pTriCollection[nCurTri].m_oVerts[1] = vVertex[i+1];
		m_pTriCollection[nCurTri].m_oVerts[2] = vVertex[i+2];
		m_pTriCollection[nCurTri].m_pMaterial = vMaterials[nCurTri];
		vTris.push_back(m_pTriCollection + nCurTri);
		
		if(vMaterials[nCurTri]->isTransparent())
			++dwTrans;
		else
			++dwSolid;
	}

	m_pQueue = new CRenderQueue(dwSolid, dwTrans);

	// Begin building the tree.
	m_pHead = new COctant(vTris);
	m_pHead->calculateBounds(vVertex);
	m_pHead->buildOctant(usMinTriangles, usMaxLevels);
	m_pHead->finalizeOctant();
}

/**
* COctree::deallocateTree
* @date Modified Apr 08, 2006
*/
void COctree::deallocateTree(void)
{
	delete [] m_pTriCollection;
	m_pTriCollection = NULL;
	delete m_pQueue;
	m_pQueue = NULL;
	delete m_pHead;
	m_pHead = NULL;
}

/**
* COctree::renderTree
* @date Modified Apr 08, 2006
*/
void COctree::renderTree(CFrustum& oFrustum)
{
	if(!m_pHead)
		return;

	static ERenderFlag eFlag = RENDER_USED1;
	m_pHead->renderTree(oFrustum, *m_pQueue, eFlag);
	m_pQueue->renderAll(oFrustum.getViewPosition());

	if(eFlag == RENDER_USED1)
		eFlag = RENDER_USED2;
	else
		eFlag = RENDER_USED1;
}