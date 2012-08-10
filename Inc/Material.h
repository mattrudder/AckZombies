/**
* @file Material.h
* @author Matt Rudder
* @date Created Mar 31, 2006
*
* Defines the surface properties of a piece of geometry.
*/

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

// Engine includes
#include "RenderSystem.h"

// Forward declarations
class CTexture2D;

#define EMBEDDED_RES 0
/**
* Collection of all the properties of a geometry surface.
*
* @author Matt Rudder
* @date Created Mar 31, 2006
* @date Modified Mar 31, 2006
*/
class CMaterial
{
	friend class CRenderList;
protected:
	//! Material ID generator
	static unsigned int m_unIDGen;
	//! Material ID
	unsigned int m_unMaterialID;
	//! D3D fixed function material
	D3DMATERIAL9 m_oMaterial;
	//! Default diffuse texture; Used to allow fixed function and shaders
	CTexture2D* m_poDiffuse;
	//! Collection of resources tied to this material; Used for cleanup
	std::vector<CBaseResource*> m_vResources;
	//! Determines if the material is considered transparent
	bool m_bTransparent;
	//! Hash used to sort geometry by material
	unsigned int m_unHash;
	//! Last FF material used
	D3DMATERIAL9 m_matOld;
	//! Default material
	static CTexture2D* m_pDefaultTexture;
public:
	CMaterial(const LPD3DXMATERIAL pMat = NULL, const LPD3DXEFFECTINSTANCE pFX = NULL);
	virtual ~CMaterial(void);

	/**
	* Accessors/Modifiers
	* @{
	*/
	CTexture2D* getDiffuse(void) { return m_poDiffuse; }
	bool isTransparent(void) { return m_bTransparent; }
	unsigned int getHash(void) { return m_unHash; }
	//!@}

	/**
	* Initializes material specifics
	*
	* @date Created Mar 31, 2006
	* @param[in]	pMat	Pointer to a D3DXMATERIAL structure.
	* @param[in]	pFX		Pointer to a D3DXEFFECTINSTANCE structure.
	*/
	void setMaterial(const LPD3DXMATERIAL pMat, const LPD3DXEFFECTINSTANCE pFX);

	/**
	* Makes a copy of the current material, and clones any resources stored within.
	*
	* @date Created Mar 31, 2006
	* @return New instance of the object
	*/
	virtual CMaterial* cloneMaterial(void);

	/**
	* Sets required states in Direct3D to use this material.
	*
	* @date Created Mar 31, 2006
	* @param[in]	pMtrl	Override fixed function material
	*/
	void begin(D3DMATERIAL9* pMtrl = NULL);

	/**
	* Notifies the associated shader to begin a new pass.
	*
	* @date Created Mar 31, 2006
	* @param[in]	unPass	The pass to begin.
	*/
	void beginPass(UINT unPass);

	/**
	* Notifies the associated shader to end the current pass.
	*
	* @date Created Mar 31, 2006
	*/
	void endPass(void);

	/**
	* Clears any state changes made to render with this material.
	*
	* @date Created Mar 31, 2006
	*/
	void end(void);

	/**
	* Calculates the hash for this material.
	*
	* @date Created Apr 25, 2006
	*/
	void calcHash(void);
};

#endif //_MATERIAL_H_