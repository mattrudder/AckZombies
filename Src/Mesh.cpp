/**
* @file Mesh.cpp
* @author Matt Rudder
* @date Modified Mar 31, 2006
*
* Defines mesh geometry used in the engine.
*/

// Local includes
#include "Mesh.h"

// Engine includes
#include "AcespaceEngine.h"
#include "RenderSystem.h"
#include "Material.h"

static const char* szDeclTypes[] = 
{
	"D3DDECLTYPE_FLOAT1",
	"D3DDECLTYPE_FLOAT2",
	"D3DDECLTYPE_FLOAT3",
	"D3DDECLTYPE_FLOAT4",
	"D3DDECLTYPE_D3DCOLOR",
	"D3DDECLTYPE_UBYTE4",
	"D3DDECLTYPE_SHORT2",
	"D3DDECLTYPE_SHORT4",
	"D3DDECLTYPE_UBYTE4N",
	"D3DDECLTYPE_SHORT2N",
	"D3DDECLTYPE_SHORT4N",
	"D3DDECLTYPE_USHORT2N",
	"D3DDECLTYPE_USHORT4N",
	"D3DDECLTYPE_UDEC3",
	"D3DDECLTYPE_DEC3N",
	"D3DDECLTYPE_FLOAT16_2",
	"D3DDECLTYPE_FLOAT16_4",
	"D3DDECLTYPE_UNUSED",
};

static const char* szMethodTypes[] = 
{
	"D3DDECLMETHOD_DEFAULT",
	"D3DDECLMETHOD_PARTIALU",
	"D3DDECLMETHOD_PARTIALV",
	"D3DDECLMETHOD_CROSSUV",
	"D3DDECLMETHOD_UV",
	"D3DDECLMETHOD_LOOKUP",
	"D3DDECLMETHOD_LOOKUPPRESAMPLED",
};

static const char* szUsageTypes[] = 
{
	"D3DDECLUSAGE_POSITION",
	"D3DDECLUSAGE_BLENDWEIGHT",
	"D3DDECLUSAGE_BLENDINDICES",
	"D3DDECLUSAGE_NORMAL",
	"D3DDECLUSAGE_PSIZE",
	"D3DDECLUSAGE_TEXCOORD",
	"D3DDECLUSAGE_TANGENT",
	"D3DDECLUSAGE_BINORMAL",
	"D3DDECLUSAGE_TESSFACTOR",
	"D3DDECLUSAGE_POSITIONT",
	"D3DDECLUSAGE_COLOR",
	"D3DDECLUSAGE_FOG",
	"D3DDECLUSAGE_DEPTH",
	"D3DDECLUSAGE_SAMPLE",

};

const DWORD CMesh::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX3;

D3DVERTEXELEMENT9 CMesh::VertexElem[] =
{
	//{0, 0,		D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,		0 },
	//{0, 12,		D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,			0 },
	//{0, 24,		D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,			0 },
	//{0, 28,		D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,		0 },
	//{0, 36,		D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,		1 },
	//{0, 44,		D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,		2 },
	//{0, 52,		D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,		0 },
	//{0, 64,		D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,		0 },
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	{0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
	D3DDECL_END()
};

/**
* CMesh::CMesh
* @date Modified Mar 31, 2006
*/
CMesh::CMesh(void) : m_poMesh(NULL), m_dwSubsets(0), m_pParent(0), m_bTransparent(false)
{
}

/**
* CMesh::~CMesh
* @date Modified Mar 31, 2006
*/
CMesh::~CMesh(void)
{
	onDeviceRelease();
}

/**
* CMesh::onDeviceLoss
* @date Modified Mar 31, 2006
*/
void CMesh::onDeviceLoss(void)
{
	COM_RELEASE(m_poMesh);
	for(size_t i = 0; i < m_vMaterials.size(); ++i)
		delete m_vMaterials[i];
}

/**
* CMesh::onDeviceRelease
* @date Modified Mar 31, 2006
*/
void CMesh::onDeviceRelease(void)
{
	COM_RELEASE(m_poMesh);
	for(size_t i = 0; i < m_vMaterials.size(); ++i)
		delete m_vMaterials[i];
	m_vMaterials.clear();
}

/**
* CMesh::onDeviceRestore
* @date Modified Mar 31, 2006
*/
void CMesh::onDeviceRestore(void)
{
	// Load mesh, original ganstas only please
	if(!m_pParent)
	{
		m_bTransparent = false;
		LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
		LPD3DXBUFFER pBufAdjacency, pBufMaterials, pBufEffects;
		DWORD dwEffects;
		LPD3DXMESH pTempMesh = NULL;
		Log << "Loading CMesh '" << m_sFilename.GetBuffer() << "':" << endl;
		HRESULT hr = D3DXLoadMeshFromX(m_sFilename, 0, pDev, &pBufAdjacency, 
			&pBufMaterials, &pBufEffects, &m_dwSubsets, &pTempMesh);
		dwEffects = pBufEffects->GetBufferSize() / sizeof(D3DXEFFECTINSTANCE);
		Log << "Mesh loaded: " << toString().GetBuffer() << endl;


		// TESTING: Get decl format
		Debug << "Mesh " << m_sFilename.GetBuffer() << " : " << endl;
		D3DVERTEXELEMENT9 oElem[MAX_FVF_DECL_SIZE];
		pTempMesh->GetDeclaration(oElem);
		D3DVERTEXELEMENT9* pElem = oElem;
		CString sFormat;
		while(pElem->Stream != 0xFF)
		{
			sFormat.Format("{%d, %d, %s, %s, %s, %d }",
				(int)pElem->Stream, (int)pElem->Offset,
				szDeclTypes[pElem->Type], szMethodTypes[pElem->Method], szUsageTypes[pElem->Usage],
				(int)pElem->UsageIndex);
			Debug << sFormat.GetBuffer() << endl;
			++pElem;
		}

		// Convert mesh to format with tangent/binormal vectors
		if(FAILED(pTempMesh->CloneMesh(pTempMesh->GetOptions(), VertexElem, pDev, &m_poMesh)))
			m_poMesh = pTempMesh;
		else
			COM_RELEASE(pTempMesh);

		Debug << "Mesh Post Clone " << m_sFilename.GetBuffer() << " : " << endl;
		oElem[MAX_FVF_DECL_SIZE];
		m_poMesh->GetDeclaration(oElem);
		pElem = oElem;
		sFormat;
		while(pElem->Stream != 0xFF)
		{
			sFormat.Format("{%d, %d, %s, %s, %s, %d }",
				(int)pElem->Stream, (int)pElem->Offset,
				szDeclTypes[pElem->Type], szMethodTypes[pElem->Method], szUsageTypes[pElem->Usage],
				(int)pElem->UsageIndex);
			Debug << sFormat.GetBuffer() << endl;
			++pElem;
		}

		// Generate bounding sphere
		//SVertex* pVert;
		//m_poMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVert);
		//D3DXComputeBoundingSphere((D3DXVECTOR3*)pVert, m_poMesh->GetNumVertices(), 
		//	D3DXGetDeclVertexSize(CMesh::VertexElem, 0), &m_oSphere.centerPt, &m_oSphere.fRadius);
		//m_poMesh->UnlockVertexBuffer();

		//if(FAILED(hr = D3DXCleanMesh(D3DXCLEAN_SIMPLIFICATION, pTempMesh, (const DWORD *)pBufAdjacency->GetBufferPointer(), &m_poMesh, NULL, NULL)))
		//	m_poMesh = pTempMesh;
		//else if(pTempMesh != m_poMesh) COM_RELEASE(pTempMesh);
		//m_poMesh->OptimizeInplace(D3DXMESHOPT_COMPACT|D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT, 
		//	(const DWORD*)pBufAdjacency->GetBufferPointer(), NULL, NULL, NULL);

		// Iterate materials/effects
		Log << endl;
		CString sTabs;

		LPD3DXMATERIAL pMaterials = (D3DXMATERIAL*)pBufMaterials->GetBufferPointer();
		LPD3DXEFFECTINSTANCE pEffects = (LPD3DXEFFECTINSTANCE)pBufEffects->GetBufferPointer();
		for(DWORD i = 0; i < m_dwSubsets; ++i)
		{
			// Create new material
			CMaterial* pMat = new CMaterial(pMaterials + i, pEffects + i);
			pMat->calcHash();
			m_vMaterials.push_back(pMat);
			if(m_vMaterials[i]->isTransparent())
				m_bTransparent = true;
		}

		// Cleanup
		COM_RELEASE(pBufAdjacency);
		COM_RELEASE(pBufMaterials);
		COM_RELEASE(pBufEffects);
	}
	else // Clone it up, B!
	{
		// Copy attributes
		m_dwSubsets = m_pParent->m_dwSubsets;
		m_poMesh = m_pParent->m_poMesh;
		m_sFilename = m_pParent->m_sFilename;
		m_bTransparent = m_pParent->m_bTransparent;
		m_poMesh->AddRef();

		// Clone materials
		CMaterial* pMat = NULL;
		for(DWORD i = 0; i < m_dwSubsets; ++i)
		{
			pMat = m_pParent->m_vMaterials[i]->cloneMaterial(); 
			m_vMaterials.push_back(pMat);
		}
	}

}

/**
* CMesh::toString
* @date Modified Apr 01, 2006
*/
CString CMesh::toString(void) const
{
	CString sFmt;
	sFmt.Format(_T("[Mesh: \"%s\", Subset Count: %u]"), m_sFilename.GetBuffer(), m_dwSubsets);
	return sFmt;
}

/**
* CMesh::cloneResource
* @date Modified Mar 31, 2006
*/
CBaseResource* CMesh::cloneResource(void)
{
	// Clone mesh
	CMesh* pMesh = new CMesh;
	pMesh->m_dwSubsets = m_dwSubsets;
	pMesh->m_poMesh = m_poMesh;
	pMesh->m_sFilename = m_sFilename;
	pMesh->m_pParent = this;
	m_poMesh->AddRef();

	// Clone materials
	CMaterial* pMat = NULL;
	for(DWORD i = 0; i < m_dwSubsets; ++i)
	{
		pMat = m_vMaterials[i]->cloneMaterial(); 
		pMesh->m_vMaterials.push_back(pMat);
	}

	// Return clone
	return pMesh;
}

/**
* CMesh::createMeshFromFile
* @date Modified Mar 31, 2006
*/
CBaseResource* CMesh::createMeshFromFile(const CString sFilename)
{
	CMesh* pMesh = new CMesh;
	pMesh->m_sFilename = sFilename;
	pMesh->onDeviceRestore();
	return pMesh;
}

/**
* CMesh::getNumSubsets
* @date Modified Mar 31, 2006
*/
DWORD CMesh::getNumSubsets(void)
{
	return m_dwSubsets;
}

/**
* CMesh::drawSubset
* @date Modified Mar 31, 2006
*/
void CMesh::drawSubset(DWORD dwSubset)
{
	if(dwSubset >= m_vMaterials.size())
		return;

	// Check for alpha-blended materials and set states accordingly
	DWORD dwCull;
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	if(m_bTransparent)
	{
		pDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
		pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		// Blend states
		pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// Render mesh.
	CMaterial* pMat = m_vMaterials[dwSubset];
	pMat->begin();

	HRESULT hr = m_poMesh->DrawSubset(dwSubset);
	if(FAILED(hr))
	{
		Debug << "DEBUG: drawSubset failed on " << m_sFilename.GetBuffer()
			<< " drawing subset " << dwSubset << endl;

		pMat->end();
		return;
	}

	pMat->end();
	

	// Reset blend states for transparent models
	if(m_bTransparent)
	{
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDev->SetRenderState(D3DRS_CULLMODE, dwCull);
	}


}

/**
* CMesh::draw
* @date Modified Mar 31, 2006
*/
void CMesh::draw(bool bTextured, D3DCOLOR* dwColor)
{
	if(!m_poMesh)
		return;

	// If a color is provided, apply it as an additive pass.
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	if(dwColor)
	{
		SetTextureColorStage(pDev, 1, D3DTA_CURRENT, D3DTOP_ADD, D3DTA_TFACTOR);
		pDev->SetRenderState(D3DRS_TEXTUREFACTOR, *dwColor);
	}

	for(DWORD i = 0; i < m_dwSubsets; ++i)
		drawSubset(i);

	if(dwColor)
	{
		DisableColorStage(pDev, 1);
		pDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0);
	}
}

/**
* CMesh::getMaterialBySubset
* @date Modified Mar 31, 2006
*/
CMaterial* CMesh::getMaterialBySubset(DWORD dwSubset)
{
	return m_vMaterials[dwSubset];
}

/**
* CMesh::getVertexList
* @date Modified Apr 26, 2006
*/
void CMesh::getVertexList(std::vector<SVertex>* vList, std::vector<CMaterial*>* vMat)
{
	if(!vList)
		return;

	std::vector<SVertex>().swap(*vList);
	vList->reserve(m_poMesh->GetNumFaces() * 3);

	SVertex* oVertices = 0;
	unsigned short* pIndicies = 0;
	if(FAILED(m_poMesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndicies)) || 
	   FAILED(m_poMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&oVertices)))
	   return;

	D3DXATTRIBUTERANGE* pAttrib = NULL;
	DWORD dwAttrib;
	m_poMesh->GetAttributeTable(NULL, &dwAttrib);
	if(vMat)
	{
		pAttrib = new D3DXATTRIBUTERANGE[dwAttrib];
		m_poMesh->GetAttributeTable(pAttrib, &dwAttrib);
	}

	// Get vertex data
	DWORD dwFaces = m_poMesh->GetNumFaces();
	DWORD dwVerts = m_poMesh->GetNumVertices();
	for(unsigned int i = 0; i < dwFaces; ++i)
	{
		vList->push_back(oVertices[pIndicies[(i*3)]]);
		vList->push_back(oVertices[pIndicies[(i*3)+1]]);
		vList->push_back(oVertices[pIndicies[(i*3)+2]]);
	}

	// Get material data if requested.
	if(vMat)
	{
		for(DWORD j = 0; j < dwAttrib; ++j)
		{
			for(DWORD k = pAttrib[j].FaceStart; k < (pAttrib[j].FaceStart + pAttrib[j].FaceCount); ++k)
				vMat->push_back(m_vMaterials[pAttrib[j].AttribId]);
		}
	}

	if(pAttrib)
	{
		delete [] pAttrib;
	}

	m_poMesh->UnlockIndexBuffer();
	m_poMesh->UnlockVertexBuffer();
}