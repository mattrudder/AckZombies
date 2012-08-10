/**
* @file BitmapFont.cpp
* @author Matt Rudder
* @date Modified Mar 29, 2006
*
* Handles displaying of text to the screen, using a bitmap font.
*/

// Local includes
#include "BitmapFont.h"

// Engine includes
#include "AcespaceEngine.h"
#include "ConfigFile.h"
#include "RenderSystem.h"
#include "ResourceManager.h"

/**
* CBitmapFont::getCharRect
* @date Modified Mar 29, 2006
*/
void CBitmapFont::getCharRect(char cChar, RECT* pRect)
{
	unsigned char cRow, cCol;
	cRow = cChar / m_cTilesAcross;
	cCol = cChar % m_cTilesAcross;

	// Fill out rectangle
	pRect->left = (cCol * m_cTileSize) + ((m_cTileSize - m_mCharMap[cChar]) / 2);
	pRect->right = pRect->left + m_mCharMap[cChar];
	pRect->top = cRow * m_cTileSize;
	pRect->bottom = pRect->top + m_cTileSize;
}

/**
* CBitmapFont::measureString
* @date Modified Apr 04, 2006
*/
unsigned int CBitmapFont::measureString(CString sString, RECT* pRect)
{
	RECT rVal = {0};
	char cChar;
	for(size_t i = 0; i < sString.GetLength(); ++i)
	{
		cChar = sString.GetChar(i);
		switch(cChar)
		{
		case '\r':
		case '\n':
			rVal.bottom += m_cLineHeight;
			break;
		case '\t':
			rVal.right += m_mCharMap['X'] * 4;
		default:
			rVal.right += m_mCharMap[cChar];
			break;
		}
	}
	if(pRect)
		*pRect = rVal;
	return rVal.right;
}

/**
* CBitmapFont::CBitmapFont
* @date Modified Mar 28, 2006
*/
CBitmapFont::CBitmapFont(void) : m_poTexture(NULL), m_cTilesAcross(16), m_cTileSize(16)
{
}

/**
* CBitmapFont::~CBitmapFont
* @date Modified Mar 28, 2006
*/
CBitmapFont::~CBitmapFont(void)
{
	COM_RELEASE(m_poTexture);
}

/**
* CBitmapFont::beginSprite
* @date Modified Apr 04, 2006
*/
void CBitmapFont::beginSprite(void)
{
	CRenderDevice& oDev = CRenderSystem::getInstance().getRenderDevice();
	oDev.getD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	oDev.getD3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	oDev.getD3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	oDev.getD3DXSprite()->Begin(D3DXSPRITE_ALPHABLEND); 
}

/**
* CBitmapFont::endSprite
* @date Modified Apr 04, 2006
*/
void CBitmapFont::endSprite(void) 
{
	CRenderDevice& oDev = CRenderSystem::getInstance().getRenderDevice();
	oDev.getD3DXSprite()->End(); 
	oDev.getD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	oDev.getD3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	oDev.getD3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
}

/**
* CBitmapFont::createFontFromFile
* @date Modified Mar 30, 2006
*/
CBaseResource* CBitmapFont::createFontFromFile(const CString sFilename)
{
	// Load font definition and image
	CBitmapFont* pFont = new CBitmapFont;
	CConfigFile cfgFont(sFilename);
	CString sBuffer = cfgFont.GetValue("Font Info", "ImageFile");

	if(sBuffer == CString::Blank)
		return false;

	pFont->m_poTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, sBuffer);
	pFont->m_cTileSize = SET_INT(cfgFont.GetValue("Font Info", "FontSize"), 16);
	pFont->m_cLineHeight = SET_INT(cfgFont.GetValue("Font Info", "LineHeight"), pFont->m_cTileSize);
	pFont->m_cTilesAcross = pFont->m_poTexture->getWidth() / pFont->m_cTileSize;

	// Process Font Definition
	char szChar[5] = {0};
	for(short c = 0; c <= 255; ++c)
	{
		sprintf(szChar, "%i", c);
		pFont->m_mCharMap[(char)c] = (char)(cfgFont.GetValue("Char Widths", szChar).ToInt());
	}

	return pFont;
}

/**
* CBitmapFont::drawString
* @date Modified Mar 29, 2006
*/
void CBitmapFont::drawString(CString str, float fX, float fY, D3DCOLOR dwColor, bool bHandleSprite)
{
	CRenderDevice& oDev = CRenderSystem::getInstance().getRenderDevice();
	LPD3DXSPRITE pSprite = oDev.getD3DXSprite();

	RECT rChar;
	char cChar;
	D3DXVECTOR3 vPos, vZero;
	vPos.x = fX;
	vPos.y = fY;
	vPos.z = 0.0f;
	vZero.x = vZero.y = vZero.z = 0.0f;

	if(bHandleSprite)
		beginSprite();

	// Iterate string and draw characters.
	size_t nSize = str.GetLength();
	for(size_t i = 0; i < nSize; ++i)
	{
		cChar = str.GetChar(i);
		switch(cChar)
		{
		case '\n':
			vPos.y += m_cLineHeight;
			vPos.x = fX;
			break;
		case '\t':
			vPos.x += m_mCharMap['X'] * 4;
			break;
		default:
			getCharRect(str.GetChar(i), &rChar);
			pSprite->Draw(m_poTexture->getD3DTexture(), &rChar, NULL, &vPos, dwColor);
			vPos.x += (rChar.right - rChar.left);
			break;
		}
	}

	if(bHandleSprite)
		endSprite();
}

/**
* CBitmapFont::drawStringMarkup
* @date Modified Mar 29, 2006
*/
void CBitmapFont::drawStringMarkup(CString str, float fX, float fY, D3DCOLOR dwColor, bool bHandleSprite)
{
	CRenderDevice& oDev = CRenderSystem::getInstance().getRenderDevice();
	LPD3DXSPRITE pSprite = oDev.getD3DXSprite();
	RECT rChar;
	char cChar;
	D3DXVECTOR3 vPos, vZero;
	vPos.x = fX;
	vPos.y = fY;
	vPos.z = 0.0f;
	vZero.x = vZero.y = vZero.z = 0.0f;

	if(bHandleSprite)
		beginSprite();

	// Split strings
	bool bFirst = false;
	std::vector<CString> vStrings;
	str.ToList(vStrings, TEXT("{}"));
	if(str.GetChar(0) == '{')
		bFirst = true;

	// Iterate split strings, and change color if needed.
	for(size_t v = 0; v < vStrings.size(); ++v)
	{
		// Get string, and read color
		if((bFirst && v % 2 == 0) || (!bFirst && v % 2 == 1))
		{
			dwColor = vStrings[v].ToUlongFromHex();
			continue;						   
		}

		// Iterate string and draw characters.
		size_t nSize = vStrings[v].GetLength();
		for(size_t i = 0; i < nSize; ++i)
		{
			cChar = vStrings[v].GetChar(i);
			switch(cChar)
			{
			case '\n':
				vPos.y += m_cLineHeight;
				vPos.x = fX;
				break;
			case '\t':
				vPos.x += m_mCharMap['X'] * 4;
				break;
			default:
				getCharRect(cChar, &rChar);
				pSprite->Draw(m_poTexture->getD3DTexture(), &rChar, NULL, &vPos, dwColor);
				vPos.x += (rChar.right - rChar.left);
				break;
			}
		}
	}

	if(bHandleSprite)
		endSprite();
}

/**
* CBitmapFont::drawStringView
* @date Modified Apr 04, 2006
*/
void CBitmapFont::drawStringView(CString str, RECT rView, unsigned int unOffset, D3DCOLOR dwColor, bool bHandleSprite)
{
	std::vector<CString> vLines, vWords, vColors;
	unsigned int unWidth = rView.right - rView.left,
				 unHeight = rView.bottom - rView.top,
				 unWordWidth = 0;
	RECT rWord;
	float fX = (float)rView.left, fY = (float)rView.top;
	str.ToList(vLines, "\r\n");

	if(bHandleSprite)
		beginSprite();

	// Calculate max number of lines
	unsigned int unLineCount = unHeight / m_cLineHeight;

	// Iterate lines in string.
	size_t i = ((vLines.size() <= unLineCount) ? 0 : max(vLines.size() - unOffset - unLineCount, 0));
	for(; i < ((vLines.size() <= unLineCount) ? vLines.size() : vLines.size() - unOffset); ++i)
	{
		// Iterate words in string.
		vLines[i].ToList(vWords, " ");
		for(size_t j = 0; j < vWords.size(); ++j)
		{
			bool bFirst = false;
			vWords[j].ToList(vColors, "{}");
			if(vWords[j].GetChar(0) == '{')
				bFirst = true;
				
			if(vColors.size())
			{
				for(size_t k = 0; k < vColors.size(); ++k)
				{
					if((bFirst && k % 2 == 0) || (!bFirst && k % 2 == 1))
					{
						dwColor = vColors[k].ToUlongFromHex();
						continue;						   
					}

					// Take line length into consideration
					unWordWidth = measureString(vColors[k], &rWord);
					if(fX + unWordWidth >= unWidth)
					{
						fX = (float)rView.left;
						fY += m_cLineHeight;
						drawString(vColors[k], fX, fY, dwColor, false);
					}
					else
					{
						drawString(vColors[k], fX, fY, dwColor, false);
						fX += unWordWidth + m_mCharMap[' '];
					}
				}
				
			}
			else
			{
				// Take line length into consideration
				unWordWidth = measureString(vWords[j], &rWord);
				if(fX + unWordWidth >= unWidth)
				{
					fX = (float)rView.left;
					fY += m_cLineHeight;
					drawString(vWords[j], fX, fY, dwColor, false);
				}
				else
				{
					drawString(vWords[j], fX, fY, dwColor, false);
					fX += unWordWidth + m_mCharMap[' '];
				}
			}
		}
		fX = (float)rView.left;
		fY += m_cLineHeight;
	}

	if(bHandleSprite)
		endSprite();
}