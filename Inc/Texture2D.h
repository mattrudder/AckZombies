/**
* @file Texture2D.h
* @author Matt Rudder
* @date Created Mar 28, 2006
*
* Encapsulates a Direct3D 2D Texture.
*/

#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

// Engine includes
#include "AcespaceEngine.h"

// Local includes
#include "RenderResource.h"

// System includes
#include <d3d9.h>

/**
* Encapsulates a Direct3D 2D Texture.
*
* @author Matt Rudder
* @date Created Mar 28, 2006
* @date Modified Mar 28, 2006
*/
class CTexture2D : public CRenderResource
{
protected:
	//! Counter used to generate shader IDs
	static unsigned short m_usTexIDGen;
	//! Shader ID; Used for sorting operations
	unsigned short m_usTexID;
	//! D3D Texture
	LPDIRECT3DTEXTURE9 m_poTexture;
	//! Filename
	CString m_sFilename;
	//! Surface description
	D3DSURFACE_DESC m_oSurfaceDesc;
	//! Is this texture transparent?
	bool m_bTransparent;
	//! Resource identifier for embedded textures; Only for embedded textures
	unsigned short m_usResourceID;
public:
	CTexture2D(unsigned short usResource = 0);
	virtual ~CTexture2D(void);

	/**
	* Accessors/Modifiers
	* @{
	*/
	LPDIRECT3DTEXTURE9 getD3DTexture(void) { return m_poTexture; }
	CString getFilename(void) { return m_sFilename; }
	unsigned int getWidth(void)	{ return m_oSurfaceDesc.Width; }
	unsigned int getHeight(void) { return m_oSurfaceDesc.Height; }
	bool isTransparent(void) { return m_bTransparent; }
	unsigned short getTextureID(void) { return m_usTexID; }
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
	* Loads a supported texture file from disk.
	*
	* @date Created Mar 28, 2006
	* @param[in]	sFilename	The file to load.
	*/
	static CBaseResource* createTextureFromFile(const CString sFilename);

	/**
	* Loads a supported texture file from an embedded resource.
	*
	* @date Created Apr 29, 2006
	* @param[in]	pRes	Resource name; See remarks
	* @return Pointer to the newly created object.
	* @remarks This function will load a shader from the current module, 
	* use MAKEINTRESOURCE to generate the resource name.
	*/
	static CBaseResource* createTextureFromResource(unsigned short usRes);
};

#endif //_TEXTURE2D_H_