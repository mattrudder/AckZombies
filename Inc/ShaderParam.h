/**
* @file ShaderParam.h
* @author Matt Rudder
* @date Created Apr 01, 2006
*
* Wraps up a single parameter inside a shader instance for ease of use.
* Initial design inspired by Wessam Bahnassi's Effect Parameter Manipulation Framework
* Article found in ShaderX3, Published 2005
*/

#ifndef _SHADERPARAM_H_
#define _SHADERPARAM_H_

// System includes
#include <d3dx9.h>
#include <hash_map>

// Engine include
#include "ShaderVariant.h"

/**
* Wraps up a handle to a single parameter inside a shader instance for ease of use.
*
* @author Matt Rudder
* @date Created Apr 01, 2006
* @date Modified Apr 01, 2006
*/
class CShaderParam : public CShaderVariant
{
public:
	//! Internal Types
	typedef stdext::hash_map<std::string, CShaderVariant*> ParamAnnotationList;
protected:
	//! List of parameter annotations.
	ParamAnnotationList m_mAnnotations;
public:
	CShaderParam(CShader* pParent = NULL, D3DXHANDLE hParam = NULL);
	virtual ~CShaderParam(void);

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
	CShaderVariant& operator= (DWORD dwVal);
	//!@}

	/**
	* Sets an array of floats.
	*
	* @date Created Apr 05, 2006
	* @param[in]	pFloats		Array of floats
	* @param[in]	nNumFloats	Size of pFloats
	*/
	void setVectorArray(D3DXVECTOR4* pVec, unsigned int unNumVec);
	void setFloatArray(float* pFloats, unsigned int unNumFloats);
	void setMatrixArray(D3DXMATRIX* pMatrix, unsigned int unNumMat);

	/**
	* Retrieves a child parameter. Useful for structures.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szParam	The parameter name
	* @return Reference to the parameter.
	*/
	CShaderParam& operator()(const char* szParam);
	
	/**
	* Retrieves a child parameter. Useful for structures.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szSemantic	The semantic name
	* @return Reference to the parameter.
	*/
	CShaderParam& getParamBySemantic(const char* szSemantic);

	/**
	* Retrieves an annotation associated with a pass.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szPassName	Name of the pass to search.
	* @param[in]	szAnno		Name of the annotation to get.
	* @return Reference to the annotation.
	*/
	CShaderVariant& getPassAnnotation(const char* szPassName, const char* szAnno);

	/**
	* Retrieves an annotation associated with this parameter.
	*
	* @date Created Apr 01, 2006
	* @param[in]	szPassName	Name of the pass to search.
	* @param[in]	szAnno		Name of the annotation to get.
	* @return Reference to the annotation.
	*/
	CShaderVariant& getAnnotation(const char* szAnno);
};

#endif //_SHADERPARAM_H_