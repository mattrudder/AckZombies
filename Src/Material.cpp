/**
* @file Material.cpp
* @author Matt Rudder
* @date Modified Mar 31, 2006
*
* Defines the surface properties of a piece of geometry.
*/

// Local includes
#include "Material.h"

// Engine includes
#include "ResourceManager.h"
#include "resource.h"

unsigned int CMaterial::m_unIDGen = 0;
CTexture2D* CMaterial::m_pDefaultTexture = 0;

/**
* CMaterial::CMaterial
* @date Modified Mar 31, 2006
*/
CMaterial::CMaterial(const LPD3DXMATERIAL pMat, const LPD3DXEFFECTINSTANCE pFX) : 
m_poDiffuse(0),m_bTransparent(false), m_unMaterialID(++m_unIDGen)
{
	if(pMat || pFX)
		setMaterial(pMat, pFX);
	else
	{
		// Load defaults
		if(!m_pDefaultTexture)
		{
#if EMBEDDED_RES == 1
			m_pDefaultTexture = (CTexture2D*)CTexture2D::createTextureFromResource(IDR_DEFAULTDIFFUSE);
#else
			m_pDefaultTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, "Default.png");
#endif
			m_poDiffuse = m_pDefaultTexture;
		}
		else
			m_poDiffuse = (CTexture2D*)m_pDefaultTexture->cloneResource();
	}

}

/**
* CMaterial::~CMaterial
* @date Modified Mar 31, 2006
*/
CMaterial::~CMaterial(void)
{
	// Reset default texture to NULL, if this is the last material using it.
	if(m_poDiffuse == m_pDefaultTexture && m_poDiffuse->getRefCount() == 1)
		m_pDefaultTexture = NULL;
	COM_RELEASE(m_poDiffuse);
}


/**
* CMaterial::setMaterial
* @date Modified Mar 31, 2006
*/
void CMaterial::setMaterial(const LPD3DXMATERIAL pMat, const LPD3DXEFFECTINSTANCE pFX)
{
	// Release previous assets if needed.
	COM_RELEASE(m_poDiffuse);

	// Process fixed function material info
	ZeroMemory(&m_oMaterial, sizeof(D3DMATERIAL9));
	if(pMat)
	{
		m_oMaterial = pMat->MatD3D;
		if(pMat->pTextureFilename && pMat->pTextureFilename[0] != '\0')
		{
			m_poDiffuse = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, pMat->pTextureFilename);
			if(!m_poDiffuse)
			{
				Log << "Error loading default diffuse texture \"" << pMat->pTextureFilename << "\" - Falling back to default texture!" << endl;
				if(!m_pDefaultTexture)
				{
#if EMBEDDED_RES == 1
					m_pDefaultTexture = (CTexture2D*)CTexture2D::createTextureFromResource(IDR_DEFAULTDIFFUSE);
#else
					m_pDefaultTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, "Default.png");
#endif
					m_poDiffuse = m_pDefaultTexture;
				}
				else
					m_poDiffuse = (CTexture2D*)m_pDefaultTexture->cloneResource();
			}

			// Check for transparency.
			if(m_poDiffuse->isTransparent())
				m_bTransparent = true;
		}
		else
		{
			Log << "No default texture specified - Falling back to default texture!" << endl;
			if(!m_pDefaultTexture)
			{
#if EMBEDDED_RES == 1
				m_pDefaultTexture = (CTexture2D*)CTexture2D::createTextureFromResource(IDR_DEFAULTDIFFUSE);
#else
				m_pDefaultTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, "Default.png");
#endif
				m_poDiffuse = m_pDefaultTexture;
			}
			else
				m_poDiffuse = (CTexture2D*)m_pDefaultTexture->cloneResource();
		}

		// Copy FF Material.
		m_oMaterial = pMat->MatD3D;
	}
	else
	{
		Log << "No default texture specified - Falling back to default texture!" << endl;
		if(!m_pDefaultTexture)
		{
#if EMBEDDED_RES == 1
			m_pDefaultTexture = (CTexture2D*)CTexture2D::createTextureFromResource(IDR_DEFAULTDIFFUSE);
#else
			m_pDefaultTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, "Default.png");
#endif
			m_poDiffuse = m_pDefaultTexture;
		}
		else
			m_poDiffuse = (CTexture2D*)m_pDefaultTexture->cloneResource();
	}
}

/**
* CMaterial::cloneMaterial
* @date Modified Mar 31, 2006
*/
CMaterial* CMaterial::cloneMaterial(void)
{
	// Clone properties
	CMaterial* pMat = new CMaterial; 
	pMat->m_oMaterial = m_oMaterial;
	pMat->m_bTransparent = m_bTransparent;
	pMat->m_unHash = m_unHash;
	pMat->m_poDiffuse = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, m_poDiffuse->getFilename());

	return pMat;
}

/**
* CMaterial::begin
* @date Modified Mar 31, 2006
*/
void CMaterial::begin(D3DMATERIAL9* pMtrl)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	pDev->SetTexture(0, m_poDiffuse->getD3DTexture());
	//pDev->SetMaterial(&m_oMaterial);
}


/**
* CMaterial::end
* @date Modified Mar 31, 2006
*/
void CMaterial::end(void)
{
	LPDIRECT3DDEVICE9 pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	pDev->SetTexture(0, NULL);
}

/**
* CMaterial::calcHash
* @date Modified Apr 25, 2006
*/
void CMaterial::calcHash(void)
{
	m_unHash = (m_poDiffuse ? m_poDiffuse->getTextureID() : 0);
}