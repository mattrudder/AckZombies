/**
* @file Octant.cpp
* @author Matt Rudder
* @date Modified Apr 08, 2006
*
* Component of an Octree, one of the eight children. Serves as a node within the
* tree structure.
*/

// Local includes
#include "Octant.h"

// Engine includes
#include "RenderSystem.h"

LPD3DXMESH COctant::m_pSphere = NULL;

/**
* COctant::COctant
* @date Modified Apr 08, 2006
*/ 
COctant::COctant(std::vector<SRenderTriangle*>& vVertex) : m_fSize(0.0f), m_vCenter(0.0f, 0.0f, 0.0f), 
	m_usNumTriangles(0), m_usDepth(0)
{
	memset(m_pChildren, 0, sizeof(COctant*) * 8);
	m_oTriList.swap(vVertex);
	m_pTriList = NULL;
}

/**
* COctant::~COctant
* @date Modified Apr 08, 2006
*/
COctant::~COctant(void)
{
	destroyOctant();
}

/**
* COctant::calculateBounds
* @date Modified Apr 08, 2006
*/
void COctant::calculateBounds(std::vector<CMesh::SVertex>& vVertex)
{

	float fMax[3] = {0};

	// Calculate the center of the box first.
	size_t nNumVerts = vVertex.size();
	m_vCenter.x = m_vCenter.y = m_vCenter.z = 0.0f;
	for(size_t i = 0; i < nNumVerts; ++i)
		m_vCenter += vVertex[i].vPosition;
	m_vCenter /= (float)nNumVerts;
	
	// Get largest axis.
	float fAxis[3] = {0};
	for(DWORD i = 0; i < nNumVerts; ++i)
	{
		// Get distance from the center point
		fAxis[0] = fabsf(vVertex[i].vPosition.x - m_vCenter.x);
		fAxis[1] = fabsf(vVertex[i].vPosition.y - m_vCenter.y);
		fAxis[2] = fabsf(vVertex[i].vPosition.z - m_vCenter.z);
		
		// Check against max so far.
		if(fAxis[0] > fMax[0]) fMax[0] = fAxis[0];
		if(fAxis[1] > fMax[1]) fMax[1] = fAxis[1];
		if(fAxis[2] > fMax[2]) fMax[2] = fAxis[2];
	}

	// Save largest axis as bounds size.
	m_fSize = fMax[0];
	if(fMax[1] > m_fSize) m_fSize = fMax[1];
	if(fMax[2] > m_fSize) m_fSize = fMax[2];

	// Convert half-width to full width.
	m_fSize *= 2;
}

/**
* COctant::getTriangleOctant
* @date Modified Apr 12, 2006
*/
COctant::EOctant COctant::getTriangleOctant(SRenderTriangle* oTri, D3DXVECTOR3& vCenter, std::vector<EOctant>& vSplit)
{
	vSplit.clear();

#define IS_LEFT(p, c) (p[0] < c[0])
#define IS_TOP(p, c) (p[1] > c[1])
#define IS_FRONT(p, c) (p[2] > c[2])
#define IS_RIGHT(p, c) (!IS_LEFT(p, c))
#define IS_BOTTOM(p, c) (!IS_TOP(p, c))
#define IS_BACK(p, c) (!IS_FRONT(p, c))

	float* pVertex[3] = 
	{
		(float*)oTri->m_oVerts[0].vPosition,
		(float*)oTri->m_oVerts[1].vPosition,
		(float*)oTri->m_oVerts[2].vPosition,
	};
	float* pCenter = (float*)m_vCenter;
	int nIn = 0;

	// Top Back Left
	for(int v = 0; v < 3; ++v)
	{
		if(IS_TOP(pVertex[v], pCenter) && IS_BACK(pVertex[v], pCenter) && IS_LEFT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_TOP_BACK_LEFT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_TOP_BACK_LEFT;

	// Top Back Right
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_TOP(pVertex[v], pCenter) && IS_BACK(pVertex[v], pCenter) && IS_RIGHT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_TOP_BACK_RIGHT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_TOP_BACK_RIGHT;

	// Top Front Left
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_TOP(pVertex[v], pCenter) && IS_FRONT(pVertex[v], pCenter) && IS_LEFT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_TOP_FRONT_LEFT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_TOP_FRONT_LEFT;

	// Top Front Right
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_TOP(pVertex[v], pCenter) && IS_FRONT(pVertex[v], pCenter) && IS_RIGHT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_TOP_FRONT_RIGHT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_TOP_FRONT_RIGHT;

	// Bottom Back Left
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_BOTTOM(pVertex[v], pCenter) && IS_BACK(pVertex[v], pCenter) && IS_LEFT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_BOTTOM_BACK_LEFT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_BOTTOM_BACK_LEFT;

	// Bottom Back Right
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_BOTTOM(pVertex[v], pCenter) && IS_BACK(pVertex[v], pCenter) && IS_RIGHT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_BOTTOM_BACK_RIGHT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_BOTTOM_BACK_RIGHT;

	// Bottom Front Left
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_BOTTOM(pVertex[v], pCenter) && IS_FRONT(pVertex[v], pCenter) && IS_LEFT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_BOTTOM_FRONT_LEFT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_BOTTOM_FRONT_LEFT;

	// Bottom Front Right
	nIn = 0;
	for(int v = 0; v < 3; ++v)
	{
		if(IS_BOTTOM(pVertex[v], pCenter) && IS_FRONT(pVertex[v], pCenter) && IS_RIGHT(pVertex[v], pCenter))
		{
			if(nIn == 0)
			{
				vSplit.push_back(OCT_BOTTOM_FRONT_RIGHT);
			}
			++nIn;
		}
	}
	if(nIn == 3)
		return OCT_BOTTOM_FRONT_RIGHT;

#undef IS_LEFT
#undef IS_TOP
#undef IS_FRONT
#undef IS_RIGHT
#undef IS_BOTTOM
#undef IS_BACK


	// No containment found; Must be split across more than one.
	return OCT_SPLIT;
}

/**
* COctant::finalizeOctant
* @date Modified Apr 12, 2006
*/
void COctant::finalizeOctant(void)
{
	SRenderTriangle oTri;
	if(!m_oTriList.empty())
	{
		m_pTriList = new SRenderTriangle*[m_oTriList.size()];
		std::vector<SRenderTriangle*>::iterator itTriList = m_oTriList.begin();
		size_t i = 0;
		while(itTriList != m_oTriList.end())
		{
			oTri.m_oVerts[0] = (*itTriList)->m_oVerts[0];
			oTri.m_oVerts[1] = (*itTriList)->m_oVerts[1];
			oTri.m_oVerts[2] = (*itTriList)->m_oVerts[2];
			oTri.m_pMaterial = (*itTriList)->m_pMaterial;
			m_pTriList[i++] = (*itTriList);
			//m_oTriangles.pushFront(oTri);

			++itTriList;
		}
	}

	for(int i = 0; i < 8; ++i)
		if(m_pChildren[i])
			m_pChildren[i]->finalizeOctant();
}

/**
* COctant::buildOctant
* @date Modified Apr 08, 2006
*/
void COctant::buildOctant(unsigned short usMinTriangles, unsigned short usMaxLevels)
{
	// Check for limit break (insert Final Fantasy joke here)
	if(m_oTriList.size() <= usMinTriangles || m_usDepth >= usMaxLevels)
		return;
	else
	{
		// Split up triangles between 8 octants
		DWORD dwSize[8] = {0};
		std::vector<SRenderTriangle*> oOctantTris[8];
		oOctantTris[0].reserve(m_oTriList.size()/8);
		oOctantTris[1].reserve(m_oTriList.size()/8);
		oOctantTris[2].reserve(m_oTriList.size()/8);
		oOctantTris[3].reserve(m_oTriList.size()/8);
		oOctantTris[4].reserve(m_oTriList.size()/8);
		oOctantTris[5].reserve(m_oTriList.size()/8);
		oOctantTris[6].reserve(m_oTriList.size()/8);
		oOctantTris[7].reserve(m_oTriList.size()/8);
		
		EOctant eChildOct;
		std::vector<SRenderTriangle*>::iterator itTriangles = m_oTriList.begin();
		std::vector<EOctant> vSplitOcts;
		while(itTriangles != m_oTriList.end())
		{
			SRenderTriangle* oTri = (*itTriangles);
			// Find which octant the triangle is contained in, and move the triangle there.
			eChildOct = getTriangleOctant((*itTriangles), m_vCenter, vSplitOcts);
			if(eChildOct != OCT_SPLIT)
			{
				oOctantTris[eChildOct].push_back(*itTriangles);
				itTriangles = m_oTriList.erase(itTriangles);
			}
			else
			{
				// Split triangles of over bounds
				DWORD dwSplits = (DWORD)vSplitOcts.size();
				if((dwSplits % 2) == 0)
				{
					// TODO: Split triangles
				}
				EOctant eOct;
				for(size_t i = 0; i < dwSplits; ++i)
				{
					eOct = vSplitOcts[i];
					oOctantTris[eOct].push_back(*itTriangles);
				}
				itTriangles = m_oTriList.erase(itTriangles);
				//++itTriangles;
			}
		}

		// Build subnodes if they contain geometry.
		for(int i = 0; i < 8; ++i)
		{
			if(!oOctantTris[i].empty())
			{
				// Fill out sub node
				m_pChildren[i] = new COctant(oOctantTris[i]);
				m_pChildren[i]->eOct = (EOctant)i;
				getOctantCenter(m_pChildren[i]->m_vCenter, m_vCenter, m_fSize, (EOctant)i);
				m_pChildren[i]->m_usDepth = m_usDepth + 1;
				m_pChildren[i]->m_fSize = m_fSize / 2;
				m_pChildren[i]->m_pTree = m_pTree;
				m_pChildren[i]->buildOctant(usMinTriangles, usMaxLevels);
			}
		}
	}
}

/**
* COctant::destroyOctant
* @date Modified Apr 08, 2006
*/
void COctant::destroyOctant(void)
{
	for(int i = 0; i < 8; ++i)
	{
		if(m_pChildren[i])
		{
			m_pChildren[i]->destroyOctant();
			delete m_pChildren[i];
			m_pChildren[i] = NULL;
		}
	}

	if(m_pTriList)
	{
		delete [] m_pTriList;
		m_pTriList = NULL;
	}
}

/**
* COctant::getOctantCenter
* @date Modified Apr 08, 2006
*/
void COctant::getOctantCenter(D3DXVECTOR3& vChildCenter, D3DXVECTOR3& vParentCenter, float fParentSize, EOctant eOct)
{
	// Calculate new octant center based on current center position, and new 
	// octant relative position
	vChildCenter.x = vChildCenter.y = vChildCenter.z = 0.0f;
	float fParentToChild = fParentSize / 4;
	switch(eOct)
	{
	case OCT_TOP_BACK_LEFT:
		vChildCenter.x = vParentCenter.x - fParentToChild;
		vChildCenter.y = vParentCenter.y + fParentToChild;
		vChildCenter.z = vParentCenter.z - fParentToChild;
		break;
	case OCT_TOP_BACK_RIGHT:
		vChildCenter.x = vParentCenter.x + fParentToChild;
		vChildCenter.y = vParentCenter.y + fParentToChild;
		vChildCenter.z = vParentCenter.z - fParentToChild;
		break;
	case OCT_TOP_FRONT_LEFT:
		vChildCenter.x = vParentCenter.x - fParentToChild;
		vChildCenter.y = vParentCenter.y + fParentToChild;
		vChildCenter.z = vParentCenter.z + fParentToChild;
		break;
	case OCT_TOP_FRONT_RIGHT:
		vChildCenter.x = vParentCenter.x + fParentToChild;
		vChildCenter.y = vParentCenter.y + fParentToChild;
		vChildCenter.z = vParentCenter.z + fParentToChild;
		break;
	case OCT_BOTTOM_BACK_LEFT:
		vChildCenter.x = vParentCenter.x - fParentToChild;
		vChildCenter.y = vParentCenter.y - fParentToChild;
		vChildCenter.z = vParentCenter.z - fParentToChild;
		break;
	case OCT_BOTTOM_BACK_RIGHT:
		vChildCenter.x = vParentCenter.x + fParentToChild;
		vChildCenter.y = vParentCenter.y - fParentToChild;
		vChildCenter.z = vParentCenter.z - fParentToChild;
		break;
	case OCT_BOTTOM_FRONT_LEFT:
		vChildCenter.x = vParentCenter.x - fParentToChild;
		vChildCenter.y = vParentCenter.y - fParentToChild;
		vChildCenter.z = vParentCenter.z + fParentToChild;
		break;
	case OCT_BOTTOM_FRONT_RIGHT:
		vChildCenter.x = vParentCenter.x + fParentToChild;
		vChildCenter.y = vParentCenter.y - fParentToChild;
		vChildCenter.z = vParentCenter.z + fParentToChild;
		break;
	}
}

#if SHOW_BOUNDS == 1
void drawBoundingBox(SAABB& oAABB, D3DXCOLOR dwColor)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	COctant::SDebugVert pBottom[] = 
	{
		{ oAABB.min.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.min.y, oAABB.min.z, dwColor },
	};
	COctant::SDebugVert pTop[] = 
	{
		{ oAABB.min.x, oAABB.max.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.min.z, dwColor },
	};
	COctant::SDebugVert pSideOne[] = 
	{
		{ oAABB.min.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.min.z, dwColor },
	};
	COctant::SDebugVert pSideTwo[] = 
	{
		{ oAABB.max.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.max.z, dwColor },
	};
	pDev->SetFVF(COctant::dwDebugFVF);
	D3DMATERIAL9 mat, matLast;
	D3DXMATRIX mWorld;
	mat.Ambient = mat.Diffuse = mat.Emissive = mat.Specular = dwColor;
	mat.Power = 5.0f;
	pDev->GetMaterial(&matLast);
	pDev->SetMaterial(&mat);
	pDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mWorld));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pBottom, sizeof(COctant::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pTop, sizeof(COctant::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, pSideOne, sizeof(COctant::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, pSideTwo, sizeof(COctant::SDebugVert));
	pDev->SetMaterial(&matLast);
}
#endif
/**
* COctant::renderTree
* @date Modified Apr 08, 2006
*/
void COctant::renderTree(CFrustum& oFrustum, CRenderQueue& oQueue, ERenderFlag eFlag, CFrustum::EFrustumTest eLastTest)
{
	// Skip testing if the last test showed that the octant was fully contained.
	CFrustum::EFrustumTest eTesting = CFrustum::TEST_IN;
	float fRadius = m_fSize/2;
	SSphere oSphere;
	SAABB oAABB;
	oSphere.centerPt = m_vCenter;
	oSphere.fRadius = fRadius;
	D3DXVECTOR3 vHalf(fRadius, fRadius, fRadius);
	D3DXVec3Add(&oAABB.max, &m_vCenter, &vHalf);
	D3DXVec3Subtract(&oAABB.min, &m_vCenter, &vHalf);


	if(eLastTest != CFrustum::TEST_IN)
	{
		// Test bounding box
		eTesting = oFrustum.containsAABB(oAABB);
		if(eTesting == CFrustum::TEST_OUT)
			return;
	}


	// This level passed all tests; Queue current geometry if any, and continue
	if(!m_oTriList.empty())
	{
		for(size_t i = 0; i < m_oTriList.size(); ++i)
		{
			// Disable rendering duplicate objects.
			if((m_oTriList[i]->dwRenderFlags & eFlag) == 0)
			{
				oQueue.addRenderable(m_oTriList[i]);
				if(eFlag == RENDER_USED1)
					m_oTriList[i]->dwRenderFlags &= ~RENDER_USED2;
				else if(eFlag == RENDER_USED2)
					m_oTriList[i]->dwRenderFlags &= ~RENDER_USED1;

				m_oTriList[i]->dwRenderFlags |= eFlag;
			}
		}

		// Octant rendering
#if SHOW_BOUNDS == 1
		drawBoundingBox(oAABB, COctree::dwDepthColor[m_usDepth]);
#endif

	}

	// Traverse children.
	for(char c = 0; c < 8; ++c)
		if(m_pChildren[c])
			m_pChildren[c]->renderTree(oFrustum, oQueue, eFlag, eTesting);
}