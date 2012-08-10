/**
* @file Texture2D.cpp
* @author Matt Rudder
* @date Modified Mar 28, 2006
*
* Encapsulates a Direct3D 2D Texture.
*/

// Local includes
#include "Texture2D.h"
#include "RenderSystem.h"
	

// Statics 
unsigned short CTexture2D::m_usTexIDGen = 0;

CTexture2D::CTexture2D(unsigned short usResource) : m_poTexture(NULL), m_usTexID(++m_usTexIDGen), m_usResourceID(usResource)
{
}

CTexture2D::~CTexture2D(void)
{
	onDeviceRelease();
}

/**
* CTexture2D::onDeviceLoss
* @date Modified Mar 28, 2006
*/
void CTexture2D::onDeviceLoss(void) {}

/**
* CTexture2D::onDeviceRelease
* @date Modified Mar 28, 2006
*/
void CTexture2D::onDeviceRelease(void)
{
	COM_RELEASE(m_poTexture);
}

/**
* CTexture2D::onDeviceRestore
* @date Modified Mar 28, 2006
*/
void CTexture2D::onDeviceRestore(void)
{
	// Load texture from resource, or file.
	if(m_usResourceID)
	{
		COM_RELEASE(m_poTexture);
		Debug << "Loading CTexture2D from embedded resource:" << endl;
		HMODULE hModule = GetModuleHandle(NULL);
		D3DXCreateTextureFromResource(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(), 
			hModule, MAKEINTRESOURCE(m_usResourceID), &m_poTexture);
	}
	else
	{
		if(!m_sFilename.GetLength())
			return;

		COM_RELEASE(m_poTexture);
		Debug << "Loading CTexture2D '" << m_sFilename.GetBuffer() << "':" << endl;
		D3DXCreateTextureFromFile(CRenderSystem::getInstance().getRenderDevice().getD3DDevice(), 
			m_sFilename, &m_poTexture);
	}

	if(m_poTexture)
	{
		m_poTexture->GetLevelDesc(0, &m_oSurfaceDesc);

		// Determine transparency
		switch(m_oSurfaceDesc.Format)
		{
		case D3DFMT_A8R8G8B8:
		case D3DFMT_A8B8G8R8:
			{
				m_bTransparent = false;
				D3DLOCKED_RECT rLock;
				m_poTexture->LockRect(0, &rLock, NULL, D3DLOCK_READONLY);
				unsigned char * bits = (unsigned char *)rLock.pBits;

				// Check each pixel for alpha values lower than 255.
				for(unsigned int x = 0; x < m_oSurfaceDesc.Width; ++x)
				{
					for(unsigned int y = 0; y < m_oSurfaceDesc.Height; ++y)
					{
						unsigned char* pPixel = (bits) + x * 4 + y * rLock.Pitch;
						
						// Assuming Intel machine (directx, duh)
						// Byte ordering is flipped.
						unsigned char ucAlpha = pPixel[3];
						if(ucAlpha < 255)
						{
							// Found one! Texture has transparency.
							m_bTransparent = true;
							m_poTexture->UnlockRect(0);

							Log << "Texture loaded: " << toString().GetBuffer() << endl;
							return;
						}
					}
				}

				m_poTexture->UnlockRect(0);
			}
			
			break;
		default:
			m_bTransparent = false;
			break;
		}

		Log << "Texture loaded: " << toString().GetBuffer() << endl;
	}
	else
		Log << "Error loading texture!" << endl;
}

/**
* CTexture2D::toString
* @date Modified Apr 01, 2006
*/
CString CTexture2D::toString(void) const
{
	CString sFmt;
	if(m_usResourceID)
		sFmt.Format(_T("[Texture2D: [RES], Height: %u Width: %u Transparent: %s]"), m_oSurfaceDesc.Height, m_oSurfaceDesc.Width, (m_bTransparent ? "yes" : "no"));
	else
		sFmt.Format(_T("[Texture2D: \"%s\", Height: %u Width: %u Transparent: %s]"), m_sFilename.GetBuffer(), m_oSurfaceDesc.Height, m_oSurfaceDesc.Width, (m_bTransparent ? "yes" : "no"));
	return sFmt;
}

/**
* CTexture2D::createTextureFromFile
* @date Modified Mar 28, 2006
*/
CBaseResource* CTexture2D::createTextureFromFile(const CString sFilename)
{
	CTexture2D* pText = new CTexture2D;
	pText->m_sFilename = sFilename;
	pText->onDeviceRestore();
	if(!pText->m_poTexture)
	{
		delete pText;
		return NULL;
	}
	else
		return pText;
}

/**
* CTexture2D::createTextureFromResource
* @date Modified Apr 29, 2006
*/
CBaseResource* CTexture2D::createTextureFromResource(unsigned short usRes)
{
	CTexture2D* pText = new CTexture2D(usRes);
	pText->onDeviceRestore();
	if(!pText->m_poTexture)
	{
		delete pText;
		return NULL;
	}
	else
		return pText;
}