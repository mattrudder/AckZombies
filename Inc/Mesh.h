/**
* @file Mesh.h
* @author Matt Rudder
* @date Created Mar 31, 2006
*
* Defines mesh geometry used in the engine.
*/

#ifndef _MESH_H_
#define _MESH_H_

// Local includes
#include "RenderResource.h"
#include "Math3D.h"

// System includes
#include <d3dx9.h>
#include <vector>

class CMaterial;

// Shortcuts for texture stages.
#define SetTextureColorStage(dev, i, arg1, op, arg2)     \
	dev->SetTextureStageState(i, D3DTSS_COLOROP, op);      \
	dev->SetTextureStageState(i, D3DTSS_COLORARG1, arg1); \
	dev->SetTextureStageState(i, D3DTSS_COLORARG2, arg2);

#define SetTextureAlphaStage(dev, i, arg1, op, arg2)     \
	dev->SetTextureStageState(i, D3DTSS_ALPHAOP, op);      \
	dev->SetTextureStageState(i, D3DTSS_ALPHAARG1, arg1);  \
	dev->SetTextureStageState(i  D3DTSS_ALPHAARG2, arg2);

#define DisableColorStage(dev, i)	\
	dev->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);

#define DisableAlphaStage(dev, i)	\
	dev->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

/**
* Encapsulates a ID3DXMesh for use in the engine.
*
* @author Matt Rudder
* @date Created Mar 31, 2006
* @date Modified Mar 31, 2006
*/
class CMesh : public CRenderResource
{
public:
	//! Mesh vertex format
	static D3DVERTEXELEMENT9 VertexElem[];
	struct SVertex
	{
		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vNormal;
		D3DCOLOR Color;
		D3DXVECTOR2 vTexCoord0;
		//D3DXVECTOR2 vTexCoord1;
		//D3DXVECTOR2 vTexCoord2;
		D3DXVECTOR3 vTangent;
		D3DXVECTOR3 vBinormal;
	};

protected:
	//! D3D Mesh instance.
	LPD3DXMESH m_poMesh;
	//! Filename the mesh is loaded from
	CString m_sFilename;
	//! Subset count
	DWORD m_dwSubsets;
	//! Collection of associated materials
	std::vector<CMaterial*> m_vMaterials;
	//! Pointer to the clone parent, if any.
	CMesh* m_pParent;
	//! Determines if any part of the mesh is transparent
	bool m_bTransparent;
	//! Sphere used for frustum culling
	SSphere m_oSphere;

public:
	// FVF used in rendering the mesh.
	static const DWORD FVF;
	CMesh(void);
	virtual ~CMesh(void);

	/**
	* Accessors/Modifiers
	* @{
	*/
	LPD3DXMESH getD3DXMesh(void)		{ return m_poMesh;	}
	SSphere& getBoundingSphere(void)	{ return m_oSphere;	}
	virtual bool isSkinned(void)		{ return false;		}
	//!@}

	/**
	* Event handler called upon the render device unexpectedly being lost.
	*
	* @date Created Mar 28, 2006
	*/
	virtual void onDeviceLoss(void);

	/**
	* Event handler called prior to the render device being released, usually
	* just before the shutdown of the application or due to a catastrophic 
	* event.
	*
	* @date Created Mar 28, 2006
	*/
	virtual void onDeviceRelease(void);

	/**
	* Event handler called upon the restoration or re-creation of the render
	* device.
	*
	* @date Created Mar 28, 2006
	*/
	virtual void onDeviceRestore(void);

	/**
	* Allows a resource to output information about itself for debugging purposes
	*
	* @date Created Apr 01, 2006
	* @return Description string
	*/
	virtual CString toString(void) const;

	/**
	* Called by the Resource Manager when a previously loaded resource is 
	* requested again.
	*
	* @date Created Mar 31, 2006
	* @return New instance of the object
	*/
	virtual CBaseResource* cloneResource(void);

	/**
	* Creates a CMesh instance and loads the associated mesh file.
	*
	* @date Created Mar 31, 2006
	* @param[in]	sFilename	Absolute file path to the mesh.
	* @return Pointer to the new CMesh instance.
	*/
	static CBaseResource* createMeshFromFile(const CString sFilename);

	/**
	* Returns the number of subsets on this mesh.
	*
	* @date Created Mar 31, 2006
	* @return Subset count.
	*/
	DWORD getNumSubsets(void);

	/**
	* Draws a single subset of the mesh, along with the associated materials.
	*
	* @date Created Mar 31, 2006
	* @param[in]	dwSubset	The subset to draw.
	*/
	void drawSubset(DWORD dwSubset);

	/**
	* Draws each subset of the mesh.
	*
	* @date Created Mar 31, 2006
	* @param[in]	bTextured	Draw with texture
	* @param[in]	dwColor		additive color
	*/
	virtual void draw(bool bTextured = true, D3DCOLOR* pColor = NULL);

	/**
	* Returns the material instance associated with the requested subset.
	*
	* @date Created Mar 31, 2006
	* @param[in]	dwSubset	Subset ID
	* @return Pointer to the material
	*/
	CMaterial* getMaterialBySubset(DWORD dwSubset);

	/**
	* Fills a vector with the triangles and materials that make up the mesh.
	*
	* @date Created Mar 31, 2006
	* @param[in]	vList	The list to fill.
	* @param[in]	vMat	The material list to fill.
	*/
	void getVertexList(std::vector<SVertex>* vList, std::vector<CMaterial*>* vMat);
};

#endif //_MESH_H_