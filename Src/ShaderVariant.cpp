/**
* @file ShaderVariant.cpp
* @author Matt Rudder
* @date Modified Apr 01, 2006
*
* Defines a core set of data within a shader instance, without annotations or 
* semantics.
* Initial design inspired by Wessam Bahnassi's Effect Parameter Manipulation Framework
* Article found in ShaderX3, Published 2005
*/

// Local includes
#include "ShaderVariant.h"
#include "Shader.h"

/**
* CShaderVariant::CShaderVariant
* @date Modified Apr 01, 2006
*/
CShaderVariant::CShaderVariant(CShader* pParent, D3DXHANDLE hParam)
	: m_pParent(pParent), m_hParam(hParam) { }

/**
* CShaderVariant::~CShaderVariant
* @date Modified Apr 01, 2006
*/
CShaderVariant::~CShaderVariant(void)
{
	VariantArrayHash::iterator itParam = m_mElements.begin();
	while(itParam != m_mElements.end())
	{
		delete itParam->second;
		++itParam;
	}
	m_mElements.clear();
}


/**
* Type Checking
* @date Modified Apr 01, 2006
* @{
*/
bool CShaderVariant::isValid(void) const
{
	return m_hParam != NULL;
}

bool CShaderVariant::isA(D3DXPARAMETER_CLASS eClass) const
{
	return eClass == getClass();
}

bool CShaderVariant::isA(D3DXPARAMETER_TYPE eType) const
{
	return eType == getClass();
}

D3DXPARAMETER_CLASS CShaderVariant::getClass(void) const
{
	D3DXPARAMETER_DESC descParam;
	HRESULT hr = m_pParent->getD3DXEffect()->GetParameterDesc(m_hParam,&descParam);
	return SUCCEEDED(hr) ? descParam.Class : (D3DXPARAMETER_CLASS)D3DXEDT_FORCEDWORD;
}

D3DXPARAMETER_TYPE CShaderVariant::getType(void) const
{
	D3DXPARAMETER_DESC descParam;
	HRESULT hr = m_pParent->getD3DXEffect()->GetParameterDesc(m_hParam,&descParam);
	return SUCCEEDED(hr) ? descParam.Type : (D3DXPARAMETER_TYPE)D3DXEDT_FORCEDWORD;
}

UINT CShaderVariant::getElementCount(void) const
{
	D3DXPARAMETER_DESC descParam;
	HRESULT hr = m_pParent->getD3DXEffect()->GetParameterDesc(m_hParam,&descParam);
	return SUCCEEDED(hr) ? descParam.Elements : 0;
}

UINT CShaderVariant::getVectorSize(void) const
{
	D3DXPARAMETER_DESC descParam;
	HRESULT hr = m_pParent->getD3DXEffect()->GetParameterDesc(m_hParam,&descParam);
	return SUCCEEDED(hr) ? descParam.Columns : 0;
}

CShaderVariant& CShaderVariant::operator [](int nIndex)
{
	// Check first with the hashtable
	VariantArrayHash::iterator itElem = m_mElements.find(nIndex);
	if(itElem != m_mElements.end())
		return *itElem->second;

	// Then load from effect.
	D3DXHANDLE hElem = m_pParent->getD3DXEffect()->GetParameterElement(m_hParam, nIndex);
	if(hElem == NULL)
		return m_pParent->m_oInvalidParam;

	CShaderVariant* pParam = new CShaderVariant(m_pParent, hElem);
	if(!pParam)
		return m_pParent->m_oInvalidParam;

	m_mElements[nIndex] = pParam;
	return *pParam;
}

//!@}

/**
* Returns the value currently stored in the parameter
*
* @date Created Apr 01, 2006
* @{
*/
CShaderVariant::operator bool(void)
{
	BOOL bValue = FALSE;
	m_pParent->getD3DXEffect()->GetBool(m_hParam,&bValue);
	return (bValue == TRUE);
}

CShaderVariant::operator int(void)
{
	int nValue = 0;
	m_pParent->getD3DXEffect()->GetInt(m_hParam, &nValue);
	return nValue;
}

CShaderVariant::operator float(void)
{
	float fValue = 0.0f;
	m_pParent->getD3DXEffect()->GetFloat(m_hParam, &fValue);
	return fValue;
}

CShaderVariant::operator D3DXVECTOR2(void)
{
	D3DXVECTOR2 vValue(0.0f, 0.0f);
	m_pParent->getD3DXEffect()->GetFloatArray(m_hParam, vValue, 2);
	return vValue;
}

CShaderVariant::operator D3DXVECTOR3(void)
{
	D3DXVECTOR3 vValue(0.0f, 0.0f, 0.0f);
	m_pParent->getD3DXEffect()->GetFloatArray(m_hParam, vValue, 3);
	return vValue;
}

CShaderVariant::operator D3DXVECTOR4(void)
{
	D3DXVECTOR4 vValue(0.0f, 0.0f, 0.0f, 1.0f);
	m_pParent->getD3DXEffect()->GetFloatArray(m_hParam, vValue, 4);
	return vValue;
}

CShaderVariant::operator D3DXMATRIX(void)
{
	D3DXMATRIX mValue(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 
					  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	m_pParent->getD3DXEffect()->GetMatrix(m_hParam, &mValue);
	return mValue;
}

CShaderVariant::operator const char*(void)
{
	const char* szValue = NULL;
	m_pParent->getD3DXEffect()->GetString(m_hParam, &szValue);
	return szValue;
}

CShaderVariant::operator LPDIRECT3DVERTEXSHADER9 (void)
{
	LPDIRECT3DVERTEXSHADER9 pValue = NULL;
	m_pParent->getD3DXEffect()->GetVertexShader(m_hParam, &pValue);

	// GetVertexShader adds to the ref. count - Undo it!
	if(pValue) pValue->Release();

	return pValue;
}

CShaderVariant::operator LPDIRECT3DPIXELSHADER9 (void)
{
	LPDIRECT3DPIXELSHADER9 pValue = NULL;
	m_pParent->getD3DXEffect()->GetPixelShader(m_hParam, &pValue);

	// GetPixelShader adds to the ref. count - Undo it!
	if(pValue) pValue->Release();

	return pValue;
}

CShaderVariant::operator LPDIRECT3DBASETEXTURE9 (void)
{
	LPDIRECT3DBASETEXTURE9 pValue = NULL;
	m_pParent->getD3DXEffect()->GetTexture(m_hParam, &pValue);

	// GetTexture adds to the ref. count - Undo it!
	if(pValue) pValue->Release();

	return pValue;
}

//!@}

/**
* Sets the value currently stored in the parameter
*
* @date Created Apr 01, 2006
* @return Reference to the left parameter; Useful to assigning lines of 
*		  parameters at once.
* @{
*/
CShaderVariant& CShaderVariant::operator= (const CShaderVariant& oVal)
{
	// Get description from both parameters.
	LPD3DXEFFECT pEffect = m_pParent->getD3DXEffect();
	D3DXPARAMETER_DESC desc;
	HRESULT hRetval = pEffect->GetParameterDesc(m_hParam, &desc);
	HRESULT hRetOval = pEffect->GetParameterDesc(oVal.m_hParam, &desc);
	if(FAILED(hRetval) | FAILED(hRetOval))
		return *this;

	// Copy parameter values
	unsigned char *pData = new unsigned char[desc.Bytes];
	if (!pData)
		return *this;

	pEffect->GetValue(oVal.m_hParam, pData, desc.Bytes);
	pEffect->SetValue(m_hParam, pData, desc.Bytes);
	delete [] pData;
	return *this;
}

CShaderVariant& CShaderVariant::operator= (bool bVal)
{
	m_pParent->getD3DXEffect()->SetBool(m_hParam, bVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (int nVal)
{
	m_pParent->getD3DXEffect()->SetInt(m_hParam, nVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (float fVal)
{
	m_pParent->getD3DXEffect()->SetFloat(m_hParam, fVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (const D3DXVECTOR2& vVal)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, vVal, 2);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (const D3DXVECTOR3& vVal)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, vVal, 3);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (const D3DXVECTOR4& vVal)
{
	m_pParent->getD3DXEffect()->SetFloatArray(m_hParam, vVal, 4);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (const D3DXMATRIX& mVal)
{
	m_pParent->getD3DXEffect()->SetMatrix(m_hParam, &mVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (const char* szVal)
{
	m_pParent->getD3DXEffect()->SetString(m_hParam, szVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

CShaderVariant& CShaderVariant::operator= (LPDIRECT3DBASETEXTURE9 pVal)
{
	m_pParent->getD3DXEffect()->SetTexture(m_hParam, pVal);
	m_pParent->getD3DXEffect()->CommitChanges();
	return *this;
}

//!@}