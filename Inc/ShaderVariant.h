/**
* @file ShaderVariant.h
* @author Matt Rudder
* @date Created Apr 01, 2006
*
* Defines a core set of data within a shader instance, without annotations or 
* semantics.
* Initial design inspired by Wessam Bahnassi's Effect Parameter Manipulation Framework
* Article found in ShaderX3, Published 2005
*/
#ifndef _SHADERVARIANT_H_
#define _SHADERVARIANT_H_

// System includes
#include <hash_map>
#include <d3dx9.h>

// Forward declarations
class CShader;

class CShaderVariant
{
public:
	typedef stdext::hash_map<int, CShaderVariant*> VariantArrayHash;
protected:
	//! Parent shader
	CShader* m_pParent;
	//! Handle this class is wrapping.
	D3DXHANDLE m_hParam;
	//! Hashtable used for quick array lookup
	VariantArrayHash m_mElements;
public:
	CShaderVariant(CShader* pParent = NULL, D3DXHANDLE hParam = NULL);
	virtual ~CShaderVariant(void);

	/**
	* Type Checking
	*
	* @date Created Apr 01, 2006
	* @{
	*/
	bool isValid(void) const;
	bool isA(D3DXPARAMETER_CLASS eClass) const;
	bool isA(D3DXPARAMETER_TYPE eType) const;
	D3DXPARAMETER_CLASS getClass(void) const;
	D3DXPARAMETER_TYPE getType(void) const;
	UINT getElementCount(void) const;
	UINT getVectorSize(void) const;
	CShader* getParent(void) const { return m_pParent; }
	//!@}

	/**
	* Returns the value currently stored in the parameter
	*
	* @date Created Apr 01, 2006
	* @{
	*/
	operator bool(void);
	operator int(void);
	operator float(void);
	operator D3DXVECTOR2(void);
	operator D3DXVECTOR3(void);
	operator D3DXVECTOR4(void);
	operator D3DXMATRIX(void);
	operator const char*(void);
	operator LPDIRECT3DVERTEXSHADER9 (void);
	operator LPDIRECT3DPIXELSHADER9 (void);
	operator LPDIRECT3DBASETEXTURE9 (void);
	CShaderVariant& operator [](int nIndex);
	//!@}

	/**
	* Sets the value currently stored in the parameter
	*
	* @date Created Apr 01, 2006
	* @return Reference to the left parameter; Useful to assigning lines of 
	*		  parameters at once.
	* @{
	*/
	CShaderVariant& operator= (const CShaderVariant& oVal);
	CShaderVariant& operator= (bool bVal);
	CShaderVariant& operator= (int nVal);
	CShaderVariant& operator= (float fVal);
	CShaderVariant& operator= (const D3DXVECTOR2& vVal);
	CShaderVariant& operator= (const D3DXVECTOR3& vVal);
	CShaderVariant& operator= (const D3DXVECTOR4& vVal);
	CShaderVariant& operator= (const D3DXMATRIX& mVal);
	CShaderVariant& operator= (const char* szVal);
	CShaderVariant& operator= (LPDIRECT3DBASETEXTURE9 pVal);
	//!@}
};

#endif //_SHADERVARIANT_H_