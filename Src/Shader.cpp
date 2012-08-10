/**
* @file Shader.cpp
* @author Matt Rudder
* @date Modified Mar 31, 2006
*
* Encapsulation for shader effects
*/

// Local includes
#include "Shader.h"

// Engine includes
#include "RenderSystem.h"
#include "ShaderParam.h"
#include "ShaderManager.h"
#include "Light.h"
#include "SpotLight.h"
#include "PointLight.h"

// System includes
#include <d3dx9.h>

// Static members
unsigned short CShader::m_usEffectIDGen = 0;
CShaderParam CShader::m_oInvalidParam;
CShaderInclude CShader::m_oInclude;

/**
* CShader::CShader
* @date Modified Mar 31, 2006
*/
CShader::CShader(void) : m_usEffectID(m_usEffectIDGen++), m_poEffect(NULL), m_hTech(0), m_unNumPasses(0)
{
	memset(&m_oTechDesc, 0, sizeof(D3DXTECHNIQUE_DESC));
}

/**
* CShader::~CShader
* @date Modified Mar 31, 2006
*/
CShader::~CShader(void)
{
	onDeviceRelease();
}

/**
* CShader::onDeviceLoss
* @date Modified Mar 31, 2006
*/
void CShader::onDeviceLoss(void)
{
	if(m_poEffect)
		m_poEffect->OnLostDevice();
}

/**
* CShader::onDeviceRelease
* @date Modified Mar 31, 2006
*/
void CShader::onDeviceRelease(void)
{
	COM_RELEASE(m_poEffect);

	ShaderParamHash::iterator itParam = m_mNamedParameters.begin();
	while(itParam != m_mNamedParameters.end())
	{
		delete itParam->second;
		++itParam;
	}
	m_mNamedParameters.clear();

	ShaderSemanticHash::iterator itSem = m_mSemanticParameters.begin();
	while(itSem != m_mSemanticParameters.end())
	{
		delete itSem->second;
		++itSem;
	}
	m_mSemanticParameters.clear();
}

/**
* CShader::onDeviceRestore
* @date Modified Mar 31, 2006
*/
void CShader::onDeviceRestore(void)
{
	if(m_poEffect)
		m_poEffect->OnResetDevice();

	ShaderParamHash::iterator itParam = m_mNamedParameters.begin();
	while(itParam != m_mNamedParameters.end())
	{
		delete itParam->second;
		++itParam;
	}
	m_mNamedParameters.clear();
}

/**
* CShader::toString
* @date Modified Apr 01, 2006
*/
CString CShader::toString(void) const
{
	CString sFmt;
	if(m_oTechDesc.Name)
		sFmt.Format(_T("[Shader: Active Technique \"%s\"]"), m_oTechDesc.Name);
	else
		sFmt = "[Shader: Active Technique <Not Set>]";
	return sFmt;
}

/**
* CShader::cloneResource
* @date Modified Mar 31, 2006
*/
CBaseResource* CShader::cloneResource(void)
{
	addRef();
	return this;
	//LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	//CShader* pShader = new CShader();
	//m_poEffect->CloneEffect(pDev, &pShader->m_poEffect);
	//pShader->m_poEffect->FindNextValidTechnique(NULL, &pShader->m_hTech);
	//pShader->m_poEffect->SetTechnique(pShader->m_hTech);
	//pShader->addRef();
	//return pShader;
}

/**
* CShader::createShaderFromFile
* @date Modified Mar 31, 2006
*/
CBaseResource* CShader::createShaderFromFile(const CString sFilename)
{
	CShader* pShader = new CShader;
 
#if DEBUG_SHADERS == 1
	DWORD dwFlags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#else
	DWORD dwFlags = 0;
#endif

	LPD3DXBUFFER pFXBuffer = NULL;
	Debug << "Loading CShader '" << sFilename.GetBuffer() << "':" << endl;
	if(FAILED(D3DXCreateEffectFromFile(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(),
		sFilename, NULL, &m_oInclude, dwFlags, CRenderSystem::getInstance().getRenderDevice().getEffectPool(), 
		&pShader->m_poEffect, &pFXBuffer)) && pFXBuffer)
	{
		Debug << "DEBUG: D3DXCreateEffectFromFile failed! " << (char*)pFXBuffer->GetBufferPointer() << endl;
		return NULL;
	}
	else
	{
		// Set to the first valid technique
		pShader->m_poEffect->FindNextValidTechnique(NULL, &pShader->m_hTech);
		pShader->m_poEffect->GetTechniqueDesc(pShader->m_hTech, &pShader->m_oTechDesc);
		pShader->m_poEffect->SetTechnique(pShader->m_hTech);
		Debug << "Shader loaded: " << pShader->toString().GetBuffer() << endl;
	}

	CShaderManager::getInstance().parseShader(*pShader);
	return pShader;
}

/**
* CShader::createShaderFromResource
* @date Modified Apr 29, 2006
*/
CBaseResource* CShader::createShaderFromResource(unsigned short usRes)
{
	CShader* pShader = new CShader;

	LPD3DXBUFFER pFXBuffer = NULL;
	
#if DEBUG_SHADERS == 1
	DWORD dwFlags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#else
	DWORD dwFlags = 0;
#endif

	Log << "Loading CShader from embedded resource:" << endl;
	HMODULE hModule = GetModuleHandle(NULL);
	if(FAILED(D3DXCreateEffectFromResource(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(),
		hModule, MAKEINTRESOURCE(usRes), NULL, &m_oInclude, dwFlags, CRenderSystem::getInstance().getRenderDevice().getEffectPool(), 
		&pShader->m_poEffect, &pFXBuffer)) && pFXBuffer)
	{
		Log << "D3DXCreateEffectFromResource failed! " << (char*)pFXBuffer->GetBufferPointer() << endl;
		return NULL;
	}
	else
	{
		// Set to the first valid technique
		pShader->m_poEffect->FindNextValidTechnique(NULL, &pShader->m_hTech);
		pShader->m_poEffect->GetTechniqueDesc(pShader->m_hTech, &pShader->m_oTechDesc);
		pShader->m_poEffect->SetTechnique(pShader->m_hTech);
		Log << "Shader loaded: " << pShader->toString().GetBuffer() << endl;
	}

	CShaderManager::getInstance().parseShader(*pShader);
	return pShader;
}


/**
* CShader::begin
* @date Modified Mar 31, 2006
*/
void CShader::begin(void)
{ 
	if(m_poEffect) 
		m_poEffect->Begin(&m_unNumPasses, 0); 
}

/**
* CShader::beginPass
* @date Modified Mar 31, 2006
*/
void CShader::beginPass(UINT unPass) const
{ 
	if(m_poEffect) 
		m_poEffect->BeginPass(unPass);
}

/**
* CShader::endPass
* @date Modified Mar 31, 2006
*/
void CShader::endPass(void) const
{
	if(m_poEffect) 
		m_poEffect->EndPass();
}

/**
* CShader::end
* @date Modified Mar 31, 2006
*/
void CShader::end(void) const
{
	if(m_poEffect) 
		m_poEffect->End(); 
}

/**
* CShader::operator()
* @date Modified Apr 01, 2006
*/
CShaderParam& CShader::operator()(const char* szParamName, const D3DXHANDLE hParent)
{
	// Return the the param class, if already created
	ShaderParamHash::const_iterator itParam = m_mNamedParameters.find(szParamName);
	if(itParam != m_mNamedParameters.end())
		return *itParam->second;

	// Get parameter handle
	D3DXHANDLE hParam = m_poEffect->GetParameterByName(hParent, szParamName);
	if (!hParam)
		return m_oInvalidParam;

	CShaderParam* pParam = new CShaderParam(this, hParam);
	if(!pParam)
		return m_oInvalidParam;

	// Save and return
	m_mNamedParameters[szParamName] = pParam;
	return *pParam;
}

/**
* CShader::getParamBySemantic
* @date Modified Apr 05, 2006
*/
CShaderParam& CShader::getParamBySemantic(const char* szSemName, const D3DXHANDLE hParent)
{
	// Return the the param class, if already created
	ShaderParamHash::const_iterator itParam = m_mSemanticParameters.find(szSemName);
	if(itParam != m_mSemanticParameters.end())
		return *itParam->second;

	// Get parameter handle
	D3DXHANDLE hParam = m_poEffect->GetParameterBySemantic(hParent, szSemName);
	if (!hParam)
		return m_oInvalidParam;

	CShaderParam* pParam = new CShaderParam(this, hParam);
	if(!pParam)
		return m_oInvalidParam;

	// Save and return
	m_mSemanticParameters[szSemName] = pParam;
	return *pParam;
}

/**
* CShader::getTechnique
* @date Modified Apr 01, 2006
*/
CShaderParam& CShader::getTechnique(const char* szTechName)
{
	// Return the the param class, if already created
	ShaderParamHash::const_iterator itParam = m_mNamedParameters.find(szTechName);
	if(itParam != m_mNamedParameters.end())
		return *itParam->second;

	// Get technique handle
	D3DXHANDLE hParam = m_poEffect->GetTechniqueByName(szTechName);
	if (!hParam)
		return m_oInvalidParam;

	CShaderParam* pParam = new CShaderParam(this, hParam);
	if(!pParam)
		return m_oInvalidParam;

	// Save and return
	m_mNamedParameters[szTechName] = pParam;
	return *pParam;
}

/**
* CShader::setActiveTechByLight
* @date Modified Jun 07, 2006
*/
void CShader::setActiveTechByLight(CLight* pLight)
{
	// Iterate the effect's techniques, searching for a matching light type.
	D3DXHANDLE hTech = NULL;
	m_poEffect->FindNextValidTechnique(NULL, &hTech);
	while(hTech)
	{
		// Only process if the LightType annotation is on the technique.
		CShaderParam oParam(this, hTech);
		if(oParam.getAnnotation("LightType").isValid())
		{
			// If no light is set, do ambient pass.
			if(!pLight)
			{
				if(!stricmp("Ambient", (const char*)oParam.getAnnotation("LightType")))
				{
					m_hTech = hTech;
					m_poEffect->GetTechniqueDesc(m_hTech, &m_oTechDesc);
					m_poEffect->SetTechnique(m_hTech);
					return;
				}
			}

			// Otherwise, process per point.
			switch(pLight->getType())
			{
			case OBJ_LIGHT_POINT:
				if(!stricmp("Point", (const char*)oParam.getAnnotation("LightType")))
				{
					m_hTech = hTech;
					m_poEffect->GetTechniqueDesc(m_hTech, &m_oTechDesc);
					m_poEffect->SetTechnique(m_hTech);
					return;
				}
				break;
			case OBJ_LIGHT_SPOT:
				if(!stricmp("Spot", (const char*)oParam.getAnnotation("LightType")))
				{
					m_hTech = hTech;
					m_poEffect->GetTechniqueDesc(m_hTech, &m_oTechDesc);
					m_poEffect->SetTechnique(m_hTech);
					return;
				}
				break;
			default:
				if(!stricmp("Ambient", (const char*)oParam.getAnnotation("LightType")))
				{
					m_hTech = hTech;
					m_poEffect->GetTechniqueDesc(m_hTech, &m_oTechDesc);
					m_poEffect->SetTechnique(m_hTech);
					return;
				}
				break;
			}
		}

		m_poEffect->FindNextValidTechnique(hTech, &hTech);
	}
}