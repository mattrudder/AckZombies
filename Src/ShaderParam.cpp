/**
* @file ShaderParam.cpp
* @author Matt Rudder
* @date Modified Apr 01, 2006
*
* Wraps up a single parameter inside a shader instance for ease of use.
* Initial design inspired by Wessam Bahnassi's Effect Parameter Manipulation Framework
* Article found in ShaderX3, Published 2005
*/

// Local includes
#include "ShaderParam.h"

// Engine includes
#include "Shader.h"

/**
* CShaderParam::CShaderParam
* @date Modified Apr 01, 2006
*/
CShaderParam::CShaderParam(CShader* pParent, D3DXHANDLE hParam)
	: CShaderVariant(pParent, hParam) { }

/**
* CShaderParam::~CShaderParam
* @date Modified Apr 01, 2006
*/
CShaderParam::~CShaderParam(void)
{
	ParamAnnotationList::iterator itParam = m_mAnnotations.begin();
	while(itParam != m_mAnnotations.end())
	{
		delete itParam->second;
		++itParam;
	}
	m_mAnnotations.clear();
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (bool bVal)
{
	m_pParent->getD3DXEffect()->SetBool(m_hParam, bVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (int nVal)
{
	m_pParent->getD3DXEffect()->SetInt(m_hParam, nVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (float fVal)
{
	m_pParent->getD3DXEffect()->SetFloat(m_hParam, fVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (const D3DXVECTOR2& vVal)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, vVal, 2);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (const D3DXVECTOR3& vVal)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, vVal, 3);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (const D3DXVECTOR4& vVal)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, vVal, 4);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (const D3DXMATRIX& mVal)
{
	m_pParent->getD3DXEffect()->SetMatrix(m_hParam, &mVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (const char* szVal)
{
	m_pParent->getD3DXEffect()->SetString(m_hParam, szVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (LPDIRECT3DBASETEXTURE9 pVal)
{
	m_pParent->getD3DXEffect()->SetTexture(m_hParam, pVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::operator=
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::operator= (DWORD dwVal)
{
	m_pParent->getD3DXEffect()->SetInt(m_hParam, (int)dwVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

/**
* CShaderParam::setVectorArray
* @date Modified Apr 05, 2006
*/
void CShaderParam::setVectorArray(D3DXVECTOR4* pVec, unsigned int unNumVec)
{
	HRESULT hr = m_pParent->getD3DXEffect()->SetVectorArray(m_hParam, pVec, unNumVec);
	m_pParent->getD3DXEffect()->CommitChanges();
}

/**
* CShaderParam::setFloatArray
* @date Modified Apr 05, 2006
*/
void CShaderParam::setFloatArray(float* pFloats, unsigned int unNumFloats)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, pFloats, unNumFloats);
	m_pParent->getD3DXEffect()->CommitChanges();
}

/**
* CShaderParam::setMatrixArray
* @date Modified May 06, 2006
*/
void CShaderParam::setMatrixArray(D3DXMATRIX* pMatrix, unsigned int unNumMat)
{
	m_pParent->getD3DXEffect()->SetMatrixArray(m_hParam, pMatrix, unNumMat);
	m_pParent->getD3DXEffect()->CommitChanges();
}

/**
* CShaderParam::operator()
* @date Modified Apr 05, 2006
*/
CShaderParam& CShaderParam::operator()(const char* szParam)
{
	return m_pParent->operator()(szParam, m_hParam);
}

/**
* CShaderParam::getParamBySemantic
* @date Modified Apr 05, 2006
*/
CShaderParam& CShaderParam::getParamBySemantic(const char* szSemantic)
{
	return m_pParent->operator()(szSemantic, m_hParam);
}

/**
* CShaderParam::getPassAnnotation
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::getPassAnnotation(const char* szPassName, const char* szAnno)
{
	// Search hash
	ParamAnnotationList::const_iterator itAnno = m_mAnnotations.find(szAnno);
	if(itAnno != m_mAnnotations.end())
		return *itAnno->second;

	// Not in hash; create
	D3DXHANDLE hPass = m_pParent->getD3DXEffect()->GetPassByName(m_hParam, szPassName);
	if(hPass == NULL)
		return m_pParent->m_oInvalidParam;

	D3DXHANDLE hAnno = m_pParent->getD3DXEffect()->GetAnnotationByName(hPass, szAnno);
	if(hAnno == NULL)
		return m_pParent->m_oInvalidParam;

	// Return data
	CShaderVariant* pVar = new CShaderVariant(m_pParent, hAnno);
	m_mAnnotations[szAnno] = pVar;
	return *pVar;
}

/**
* CShaderParam::getAnnotation
* @date Modified Apr 05, 2006
*/
CShaderVariant& CShaderParam::getAnnotation(const char* szAnno)
{
	// Search hash
	ParamAnnotationList::const_iterator itAnno = m_mAnnotations.find(szAnno);
	if(itAnno != m_mAnnotations.end())
		return *itAnno->second;

	// Not in hash; create
	D3DXHANDLE hAnno = m_pParent->getD3DXEffect()->GetAnnotationByName(m_hParam, szAnno);
	if(hAnno == NULL)
		return m_pParent->m_oInvalidParam;

	// Return data
	CShaderVariant* pVar = new CShaderVariant(m_pParent, hAnno);
	m_mAnnotations[szAnno] = pVar;
	return *pVar;
}
