/**
* @file ABT.cpp
* @author Matt Rudder
* @date Modified Apr 17, 2006
*
* Adaptive Binary Tree structure used to optimize the rendering of static world geometry.
* Algorithms referenced from the article sample found in Game Programming Gems 6 by
* Martin Fleisz titled Spatial Partitioning using ABTs.
*/

#define VEC_ARRAY(v, i) (*((&v.x) + i))
#define AABB_LEN(a, i) VEC_ARRAY(a.max, i) - VEC_ARRAY(a.min, i)

// Local includes
#include "ABT.h"

// Engine includes
#include "RenderSystem.h"
#include "Geometry.h"
#include "Material.h"

/**
* CABT::CABT
* @date Modified Apr 17, 2006
*/
CABT::CABT(void) : m_pRoot(0), m_nNumTrianglesRendered(0), m_nNumTrianglesTotal(0), m_dwNumSolids(0), m_dwNumTrans(0), m_pRenderQueue(NULL)
{
}

/**
* CABT::~CABT
* @date Modified Apr 17, 2006
*/
CABT::~CABT(void)
{
}

/**
* CABT::getSplittingPlane
* @date Modified Apr 18, 2006
*/
unsigned int CABT::getSplittingPlane(std::vector<CMesh::SVertex>& vVertex, SAABB& oAABB, SPlane& oPlane)
{
	float fBestSplitScore = 100000000.0f;
	float fLargestAxis;
	size_t nNumVerts = vVertex.size();

	// Determine the largest axis
	D3DXVECTOR3 vLen;
	D3DXVec3Subtract(&vLen, &oAABB.max, &oAABB.min);
	fLargestAxis = max(max(vLen.x, vLen.y), vLen.z);

	// Find best plane
	unsigned int nBestPlane = 0;
	for(unsigned int i = 0; i < 3; ++i)
	{
		// Test plane
		SPlane oTestPlane = {D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)};
		VEC_ARRAY(oTestPlane.normal, i) = 1.0f;

		// Calculate mean
		double dMeanSum = 0.0;
		for(size_t j = 0; j < nNumVerts; ++j)
			dMeanSum += VEC_ARRAY(vVertex[j].vPosition, i);
		float fMean = (float)(dMeanSum / (double)nNumVerts);

		// Calculate standard deviation
		double dStdDevSum = 0.0;
		for(size_t j = 0; j < nNumVerts; ++j)
			dStdDevSum += pow(fMean - VEC_ARRAY(vVertex[j].vPosition, i), 2);
		float fStdDev = (float)(dStdDevSum / (double)(nNumVerts - 1));
		fStdDev = sqrt(fStdDev);
	
		// Sample uniform distances, finding the best fit plane
		VEC_ARRAY(oTestPlane.point, i) = max(fMean - fStdDev, VEC_ARRAY(oAABB.min, i));
		float fDist = abs(min(fMean + fStdDev, VEC_ARRAY(oAABB.max, i))
						- VEC_ARRAY(oTestPlane.point, i)) / (float)(ABT_PLANE_SAMPLES * 2);

		for(unsigned int j = 0; j < (ABT_PLANE_SAMPLES * 2 - 1); ++j)
		{
			// Update plane
			VEC_ARRAY(oTestPlane.point, i) += fDist;

			// Determine face placement
			unsigned int nFrontFaces = 0, nBackFaces = 0, nSplitFaces = 0;
			bool bInsertFront = true;
			ETriClass eClass;
			for(std::vector<CMesh::SVertex>::iterator k = vVertex.begin(); k != vVertex.end(); k += 3)
			{
				// Get triangle classification
				eClass = getTriangleClassification(&(*k), oPlane);
				if(eClass == TRI_FRONT)
					++nFrontFaces;
				else if(eClass == TRI_BACK)
					++nBackFaces;
				else if(eClass == TRI_SPLIT)
					++nSplitFaces;
				else
				{
					if(bInsertFront)
						++nFrontFaces;
					else
						++nBackFaces;

					bInsertFront = !bInsertFront;
				}
			}

			// Calculate plane score
			float f1 = 1.0f - (VEC_ARRAY(vLen, i) / fLargestAxis),
				f2 = fabs(0.5f - getSplitPercent(oTestPlane, oAABB)) * 2.0f,
				f3 = (float)fabs((float)nFrontFaces - nBackFaces) / (float)(nFrontFaces + nBackFaces),
				f4 = (float)nSplitFaces / (float)(nFrontFaces + nBackFaces),
				fFinal = f1 * ABT_WEIGHT_SPACELOCATION +
				f2 * ABT_WEIGHT_CHILDVOLUMES  +
				f3 * ABT_WEIGHT_FACECOUNT	 +
				f4 * ABT_WEIGHT_SPLITFACES;

			// Check for new best fit plane.
			if(fFinal < fBestSplitScore)
			{
				oPlane = oTestPlane;
				fBestSplitScore = fFinal;
				nBestPlane = i;
			}
		}
	}

	return nBestPlane;
}

/**
* CABT::getPointClassification
* @date Modified Apr 17, 2006
*/
CABT::ETriClass CABT::getTriangleClassification(CMesh::SVertex* pTri, SPlane& oPlane)
{
	int nPositive = 0, nNegative = 0;
	for(int i = 0; i < 3; ++i)
	{
		EPointClass eClass = getPointClassification(pTri[i].vPosition, oPlane);
		if(eClass == PT_FRONT)
			++nPositive;
		else if(eClass == PT_BACK)
			++nNegative;
	}

	if(!nNegative && nPositive)
		return TRI_FRONT;
	if(!nPositive && nNegative)
		return TRI_BACK;
	if(!nPositive && !nNegative)
		return TRI_COPLANAR;
	
	return TRI_SPLIT;
}

/**
* CABT::getPointClassification
* @date Modified Apr 17, 2006
*/
CABT::EPointClass CABT::getPointClassification(D3DXVECTOR3& vPoint, SPlane& oPlane)
{
	// Get vector from vPoint to the point on the plane.
	D3DXVECTOR3 vDir;
	D3DXVec3Subtract(&vDir, &vPoint, &oPlane.point);

	// Dot the direction with the point on the plane
	float fDot = D3DXVec3Dot(&vDir, &oPlane.normal);

	// Check dot product against epsilson in case of floating point error.
	if(fDot > ABT_EPSILSON)
		return PT_FRONT;
	if(fDot < -ABT_EPSILSON)
		return PT_BACK;

	// Close enough to the plane to be considered coplanar.
	return PT_COPLANAR;
}

/**
* CABT::splitTriangle
* @date Modified Apr 18, 2006
*/
void CABT::splitTriangle(CMesh::SVertex* pVertex, SPlane& oPlane, std::vector<CMesh::SVertex>& vFront, std::vector<CMesh::SVertex>& vBack)
{
	unsigned int nResA, nResB;
	CMesh::SVertex vA, vB;

	vA = pVertex[2];
	nResA = getPointClassification(vA.vPosition, oPlane);

	std::vector<CMesh::SVertex> vFrontFaces, vBackFaces;

	// Check triangle edges
	for(unsigned int i = 0; i < 3; ++i)
	{
		// Get the next triangle point
		vB = pVertex[i];
		nResB = getPointClassification(vB.vPosition, oPlane);

		if(nResB == PT_FRONT)
		{
			if(nResA == PT_BACK)
			{
				// Find intersection
				D3DXVECTOR3 vIntersect;
				D3DXPLANE oDXPlane;
				D3DXPlaneIntersectLine(&vIntersect, D3DXPlaneFromPointNormal(&oDXPlane, &oPlane.point, &oPlane.normal), &vA.vPosition, &vB.vPosition);

				// Calculate interpolation factor
				D3DXVECTOR3 vNew, vWhole;
				D3DXVec3Subtract(&vNew, &vA.vPosition, &vIntersect);
				D3DXVec3Subtract(&vWhole, &vA.vPosition, &vB.vPosition);
				float fFactor = D3DXVec3Length(&vNew) / D3DXVec3Length(&vWhole);

				// Interpolate vertex data.
				CMesh::SVertex vNewVertex;
				D3DXCOLOR oColor;
				vNewVertex.vPosition = vIntersect;
				D3DXVec3Lerp(&vNewVertex.vNormal, &vA.vNormal, &vB.vNormal, fFactor);
				D3DXVec2Lerp(&vNewVertex.vTexCoord0, &vA.vTexCoord0, &vB.vTexCoord0, fFactor);
				D3DXColorLerp(&oColor, &D3DXCOLOR(vA.Color), &D3DXCOLOR(vB.Color), fFactor);
				vNewVertex.Color = oColor;
				
				vFrontFaces.push_back(vNewVertex);
				vBackFaces.push_back(vNewVertex);
			}

			vFrontFaces.push_back(vB);
		}
		else if(nResB == PT_BACK)
		{
			if(nResA == PT_FRONT)
			{
				// Find intersection
				D3DXVECTOR3 vIntersect;
				D3DXPLANE oDXPlane;
				D3DXPlaneIntersectLine(&vIntersect, D3DXPlaneFromPointNormal(&oDXPlane, &oPlane.point, &oPlane.normal), &vA.vPosition, &vB.vPosition);

				// Calculate interpolation factor
				D3DXVECTOR3 vNew, vWhole;
				D3DXVec3Subtract(&vNew, &vA.vPosition, &vIntersect);
				D3DXVec3Subtract(&vWhole, &vA.vPosition, &vB.vPosition);
				float fFactor = D3DXVec3Length(&vNew) / D3DXVec3Length(&vWhole);

				// Interpolate vertex data.
				CMesh::SVertex vNewVertex;
				D3DXCOLOR oColor;
				vNewVertex.vPosition = vIntersect;
				D3DXVec3Lerp(&vNewVertex.vNormal, &vA.vNormal, &vB.vNormal, fFactor);
				D3DXVec2Lerp(&vNewVertex.vTexCoord0, &vA.vTexCoord0, &vB.vTexCoord0, fFactor);
				D3DXColorLerp(&oColor, &D3DXCOLOR(vA.Color), &D3DXCOLOR(vB.Color), fFactor);
				vNewVertex.Color = oColor;

				vFrontFaces.push_back(vNewVertex);
				vBackFaces.push_back(vNewVertex);
			}
			vBackFaces.push_back(vB);
		}
		else
		{
			vFrontFaces.push_back(vB);
			vBackFaces.push_back(vB);
		}

		// Next Edge
		vA = vB;
		nResA = nResB;
	}

	// Make a triangle list out of the the vertices created.
	for(unsigned int i = 1; i < vFrontFaces.size() - 1; ++i)
	{
		vFront.push_back(vFrontFaces[0]);
		vFront.push_back(vFrontFaces[i]);
		vFront.push_back(vFrontFaces[i+1]);
	}

	for(unsigned int i = 1; i < vBackFaces.size() - 1; ++i)
	{
		vBack.push_back(vBackFaces[0]);
		vBack.push_back(vBackFaces[i]);
		vBack.push_back(vBackFaces[i+1]);
	}
}

bool bHeh = false;
/**
* CABT::createSubtree
* @date Modified Apr 17, 2006
*/
CABT::SABTNode* CABT::createSubtree(std::vector<CMesh::SVertex>& vVertex, std::vector<CMaterial*>& vMaterials)
{
	if(vVertex.empty())
		return NULL;

	// Generate AABB for the geometry set.
	SABTNode* pNode = new SABTNode;
	pNode->dwColor = D3DCOLOR_XRGB(rand()%255, rand()%255, rand()%255);
	generateAABB(vVertex, pNode->m_oAABB);

	// Check against triangle limits
	if(vVertex.size() > ABT_MAX_NODETRIANGLES * 3)
	{
		// Not a leaf. Break down the vertices.
		pNode->m_bIsLeaf = false;
		pNode->m_pVB = NULL;
		pNode->m_nVertCount = 0;

		SPlane oSplitPlane, oSplitGrow1, oSplitGrow2;
		unsigned int unAxis = getSplittingPlane(vVertex, pNode->m_oAABB, oSplitPlane);

		oSplitGrow1 = oSplitPlane;
		oSplitGrow2 = oSplitPlane;

		// Calculate growth planes
		float fChildAABBLen = (getSplitPercent(oSplitGrow1, pNode->m_oAABB) * AABB_LEN(pNode->m_oAABB, unAxis));
		VEC_ARRAY(oSplitGrow1.point, unAxis) += fChildAABBLen * ABT_GROWTH_FACTOR;
		VEC_ARRAY(oSplitGrow2.point, unAxis) -= (AABB_LEN(pNode->m_oAABB, unAxis) - fChildAABBLen) * ABT_GROWTH_FACTOR;

		// Split geometry
		std::vector<CMesh::SVertex> vVertexLeft, vVertexRight;
		std::vector<CMaterial*> vMaterialLeft, vMaterialRight;
		vVertexLeft.reserve(vVertex.size() * 3 / 4);
		vVertexRight.reserve(vVertex.size() * 3 / 4);
		vMaterialLeft.reserve(vMaterials.size() * 3 / 4);
		vMaterialRight.reserve(vMaterials.size() * 3 / 4);

		unsigned int nSplits = 0;
		CMesh::SVertex vTriangle[3];
		unsigned int unTriangle;
		for(unsigned int i = 0; i < vVertex.size(); i += 3)
		{
			ETriClass eClass = getTriangleClassification(&(vVertex[i]), oSplitPlane);
			unTriangle = i / 3;

			// Split required
			if(eClass == TRI_SPLIT)
			{
				// Check against grown plane
				eClass = getTriangleClassification(&(vVertex[i]), oSplitGrow1);
				if(eClass == TRI_SPLIT)
				{
					eClass = getTriangleClassification(&(vVertex[i]), oSplitGrow2);
					if(eClass == TRI_SPLIT)
					{
						// Split the triangle
						std::vector<CMesh::SVertex> vFront, vBack;
						splitTriangle(&(vVertex[i]), oSplitPlane, vFront, vBack);

						ACEASSERT((vFront.size() % 3) == 0 && (vBack.size() % 3) == 0);

						// Add triangles to global list
						for(unsigned int j = 0; j < vFront.size(); ++j)
							vVertexLeft.push_back(vFront[j]);
						for(unsigned int j = 0; j < vBack.size(); ++j)
							vVertexRight.push_back(vBack[j]);

						// Triangle split; Duplicate material reference
						for(unsigned int j = 0; j < vFront.size() / 3; ++j)
							vMaterialLeft.push_back(vMaterials[unTriangle]);
						for(unsigned int j = 0; j < vBack.size() / 3; ++j)
							vMaterialRight.push_back(vMaterials[unTriangle]);

						++nSplits;
						continue;
					}
				}
			}

			if(eClass == PT_FRONT)
			{
				vVertexLeft.push_back(vVertex[i]);
				vVertexLeft.push_back(vVertex[i+1]);
				vVertexLeft.push_back(vVertex[i+2]);
				vMaterialLeft.push_back(vMaterials[unTriangle]);
			}
			else if(eClass == PT_BACK)
			{
				vVertexRight.push_back(vVertex[i]);
				vVertexRight.push_back(vVertex[i+1]);
				vVertexRight.push_back(vVertex[i+2]);
				vMaterialRight.push_back(vMaterials[unTriangle]);
			}
			else
			{
				// Coplanar. Keep tree balanced.
				if(vVertexLeft.size() < vVertexRight.size())
				{
					vVertexLeft.push_back(vVertex[i]);
					vVertexLeft.push_back(vVertex[i+1]);
					vVertexLeft.push_back(vVertex[i+2]);
					vMaterialLeft.push_back(vMaterials[unTriangle]);
				}
				else
				{
					vVertexRight.push_back(vVertex[i]);
					vVertexRight.push_back(vVertex[i+1]);
					vVertexRight.push_back(vVertex[i+2]);
					vMaterialRight.push_back(vMaterials[unTriangle]);
				}
			}
		}

		// Clear memory allocated to our vertex vector.
		// Hack courtesy of http://www.gotw.ca/gotw/054.htm
		std::vector<CMesh::SVertex>().swap(vVertex);

		if(vVertexLeft.empty() || vVertexRight.empty())
		{
			Debug << "Invalid Split with panel {n: (" << oSplitPlane.normal.x << ", " << oSplitPlane.normal.y << ", " << oSplitPlane.normal.z << ") p: ("
				  << oSplitPlane.point.x << ", " << oSplitPlane.point.y << ", " << oSplitPlane.point.z << ")}" << endl;
		}

		if((vMaterialLeft.size() + vMaterialRight.size()) != ((vVertexLeft.size() + vVertexRight.size()) / 3))
		{
			bool _ = true;
		}

		// Continue
		pNode->m_pLeft = createSubtree(vVertexLeft, vMaterialLeft);
		pNode->m_pRight = createSubtree(vVertexRight, vMaterialRight);
	}
	else
	{
		// Found a leaf node. Store it up.
		pNode->m_bIsLeaf = true;
		pNode->m_pLeft = pNode->m_pRight = NULL;

		// Create and load vertex buffer.
		//CMesh::SVertex* pVert = 0;
		//LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
		//HRESULT hr = pDev->CreateVertexBuffer(sizeof(CMesh::SVertex) * vVertex.size(), D3DUSAGE_WRITEONLY, CMesh::FVF, D3DPOOL_MANAGED, &pNode->m_pVB, NULL);
		//hr = pNode->m_pVB->Lock(0, 0, (LPVOID*)&pVert, 0);
		//memcpy(pVert, &(vVertex[0]), sizeof(CMesh::SVertex) * vVertex.size());
		//pNode->m_pVB->Unlock();

		// Save vertex list
		pNode->m_pTris = new SRenderTriangle[vVertex.size() / 3];
		size_t nCurrentTri;
		for(size_t i = 0; i < vVertex.size(); i+=3)
		{
			nCurrentTri = i / 3;
			pNode->m_pTris[nCurrentTri].m_oVerts[0] = vVertex[i];
			pNode->m_pTris[nCurrentTri].m_oVerts[1] = vVertex[i+1];
			pNode->m_pTris[nCurrentTri].m_oVerts[2] = vVertex[i+2];

			// Save material, and update material count.
			pNode->m_pTris[nCurrentTri].m_pMaterial = vMaterials[nCurrentTri];
			if(pNode->m_pTris[nCurrentTri].m_pMaterial->isTransparent())
				++m_dwNumTrans;
			else
				++m_dwNumSolids;
		}

		pNode->m_nVertCount = vVertex.size();
		m_nNumTrianglesTotal += vVertex.size() / 3;

		// Clear old vector.
		std::vector<CMesh::SVertex>().swap(vVertex);
		std::vector<CMaterial*>().swap(vMaterials);
	}
	return pNode;
}

/**
* CABT::createTree
* @date Modified Apr 17, 2006
*/
void CABT::createTree(std::vector<CMesh::SVertex>& vVertex, std::vector<CMaterial*>& vMaterials)
{
	if(m_pRoot)
		destroyTree();

	m_pRoot = createSubtree(vVertex, vMaterials);
	m_pRenderQueue = new CRenderQueue(m_dwNumSolids, m_dwNumTrans);
}

/**
* CABT::destroyTree
* @date Modified Apr 17, 2006
*/
void CABT::destroyTree(SABTNode* pNode)
{
	ACEASSERT(pNode);

	// Save subtrees
	SABTNode* pLeft = pNode->m_pLeft, *pRight = pNode->m_pRight;

	// Deallocate node
	COM_RELEASE(pNode->m_pVB);
	delete pNode;

	// Continue down the tree.
	if(pLeft) destroyTree(pLeft);
	if(pRight) destroyTree(pRight);
}

/**
* CABT::destroyTree
* @date Modified Apr 17, 2006
*/
void CABT::destroyTree(void)
{
	if(m_pRoot) destroyTree(m_pRoot);
	m_dwNumSolids = m_dwNumTrans = 0;
	delete m_pRenderQueue;
	m_pRenderQueue = NULL;
}

void CABT::drawBoundingBox(SAABB& oAABB, D3DXCOLOR dwColor)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	CABT::SDebugVert pBottom[] = 
	{
		{ oAABB.min.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.min.y, oAABB.min.z, dwColor },
	};
	CABT::SDebugVert pTop[] = 
	{
		{ oAABB.min.x, oAABB.max.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.min.z, dwColor },
	};
	CABT::SDebugVert pSideOne[] = 
	{
		{ oAABB.min.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.min.y, oAABB.min.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.min.z, dwColor },
	};
	CABT::SDebugVert pSideTwo[] = 
	{
		{ oAABB.max.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.max.x, oAABB.max.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.min.y, oAABB.max.z, dwColor },
		{ oAABB.min.x, oAABB.max.y, oAABB.max.z, dwColor },
	};
	pDev->SetFVF(CABT::dwDebugFVF);
	D3DMATERIAL9 mat;
	mat.Ambient = mat.Diffuse = mat.Emissive = mat.Specular = dwColor;
	mat.Power = 5.0f;
	pDev->SetMaterial(&mat);
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pBottom, sizeof(CABT::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, pTop, sizeof(CABT::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, pSideOne, sizeof(CABT::SDebugVert));
	pDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, pSideTwo, sizeof(CABT::SDebugVert));
}

/**
* CABT::renderTree
* @date Modified Apr 17, 2006
*/
void CABT::renderTree(CFrustum& oFrustum, SABTNode* pNode, bool bContained)
{
	ACEASSERT(pNode);

	// Process subnodes
	CFrustum::EFrustumTest eTest = CFrustum::TEST_IN;
	eTest = oFrustum.containsAABB(pNode->m_oAABB);
	if(eTest == CFrustum::TEST_OUT)
		return;

	// Draw geometry, if the node is a leaf.
	if(pNode->m_bIsLeaf)
	{
		for(size_t v = 0; v < pNode->m_nVertCount/3; ++v)
			m_pRenderQueue->addRenderable(&pNode->m_pTris[v]);

		m_nNumTrianglesRendered += pNode->m_nVertCount / 3;

#ifdef _DEBUG
		static int i = 0;
		if(true)
		{
			drawBoundingBox(pNode->m_oAABB, D3DCOLOR_XRGB(255, 0, 0));
		}

#endif
	}

	if(pNode->m_pLeft) renderTree(oFrustum, pNode->m_pLeft, eTest == CFrustum::TEST_IN);
	if(pNode->m_pRight) renderTree(oFrustum, pNode->m_pRight, eTest == CFrustum::TEST_IN);
}

/**
* CABT::renderTree
* @date Modified Apr 17, 2006
*/
void CABT::renderTree(CFrustum& oFrustum)
{
	if(!m_pRoot)
		return;

	m_nNumTrianglesRendered = 0;
	renderTree(oFrustum, m_pRoot);
	m_pRenderQueue->renderAll(oFrustum.getViewPosition());
}