/**
* @file Shader.h
* @author Matt Rudder
* @date Created Mar 31, 2006
*
* Encapsulation for shader effects
*/

#ifndef _SHADER_H_
#define _SHADER_H_

// Local includes
#include "RenderResource.h"

// System includes
#include <d3dx9.h>
#include <hash_map>

// Engine includes
#include "ShaderParam.h"
#include "ShaderInclude.h"

class CLight;

/**
* Encapsulates the DirectX effect file format (.fx) and allow for simple
* manipulation of internal data.
*
* @author Matt Rudder
* @date Created Mar 31, 2006
* @date Modified Mar 31, 2006
*/
class CShader : public CRenderResource
{
	friend class CShaderVariant;
	friend class CShaderParam;
public:
	//! Internal Types
	typedef stdext::hash_map<std::string, CShaderParam*> ShaderParamHash;
	typedef stdext::hash_map<std::string, CShaderParam*> ShaderSemanticHash;
protected:
	//! Counter used to generate shader IDs
	static unsigned short m_usEffectIDGen;
	//! Include override class
	static CShaderInclude m_oInclude;
	//! Shader ID; Used for sorting operations
	unsigned short m_usEffectID;
	//! D3D representation of an effect.
	LPD3DXEFFECT m_poEffect;
	//! Number of passes in the current technique
	UINT m_unNumPasses;
	//! Current technique
	D3DXHANDLE m_hTech;
	//! Parameter by name list
	ShaderParamHash m_mNamedParameters;
	//! Parameter by semantic list
	ShaderSemanticHash m_mSemanticParameters;
	//! Error condition parameter
	static CShaderParam m_oInvalidParam;
	//! Technique info
	D3DXTECHNIQUE_DESC m_oTechDesc;

public:
	CShader(void);
	virtual ~CShader(void);

	/**
	* Accessors/Modifiers
	* @{
	*/
	unsigned short getShaderID(void) const { return m_usEffectID; }
	LPD3DXEFFECT getD3DXEffect(void) const { return m_poEffect; }
	LPCSTR getTechniqueName(void) const { return m_oTechDesc.Name; }
	UINT getNumPasses(void) const { return m_unNumPasses; }
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
	* Loads a shader file from disk
	*
	* @date Created Mar 31, 2006
	* @param[in]	sFilename	Absolute path to the file.
	* @return Pointer to the newly created object.
	*/
	static CBaseResource* createShaderFromFile(const CString sFilename);

	/**
	* Loads a shader file from an embedded resource.
	*
	* @date Created Apr 29, 2006
	* @param[in]	pRes	Resource name; See remarks
	* @return Pointer to the newly created object.
	* @remarks This function will load a shader from the current module, 
	* use MAKEINTRESOURCE to generate the resource name.
	*/
	static CBaseResource* createShaderFromResource(unsigned short usRes);

	/**
	* Begins rendering with the current active technique.
	*
	* @date Created Mar 31, 2006
	*/
	void begin(void);

	/**
	* Begins rendering the specified pass in the current active technique.
	*
	* @date Created Mar 31, 2006
	* @param[in]	unPass	The pass to render
	*/
	void beginPass(const UINT unPass) const;

	/**
	* Ends rendering with the last specified pass.
	*
	* @date Created Mar 31, 2006
	*/
	void endPass(void) const;

	/**
	* Ends rendering with the current technique.
	*
	* @date Created Mar 31, 2006
	*/
	void end(void) const;

	/**
	* Allows shader constants to be easily accessed by the rest of the engine.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szParamName	Name of the parameter
	* @param[in]	hParent	Parent parameter, or structure
	* @return Reference to the parameter.
	*/
	CShaderParam& operator()(const char* szParamName, const D3DXHANDLE hParent = NULL);

	/**
	* Allows shader constants to be easily accessed by the rest of the engine.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szSemName	Name of the semantic
	* @param[in]	hParent	Parent parameter, or structure
	* @return Reference to the parameter.
	*/
	CShaderParam& getParamBySemantic(const char* szSemName, const D3DXHANDLE hParent = NULL);

	/**
	* Retrieves a technique from the shader. Only annotations can be read from
	* techniques.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szTechName	The name of the requested technique
	* @return Reference to the technique.
	*/
	CShaderParam& getTechnique(const char* szTechName);

	/**
	* Sets the currently active technique by light type.
	*
	* @date Created Jun 07, 2006
	* @param[in]	pLight	The currently active light.
	*/
	void setActiveTechByLight(CLight* pLight);
};

#endif //_SHADER_H_