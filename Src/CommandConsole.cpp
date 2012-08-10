/**
* @file CommandConsole.cpp
* @author Matt Rudder
* @date Modified Apr 04, 2006
*
* Console for manipulating objects within the game world.
*/

// Local includes
#include "CommandConsole.h"

// Engine includes
#include "DebugStream.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "DirectInputWrapper.h"
#include "Commands.h"

#define FVF_CONSOLE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define WHITE 0xFFFFFFFF

#define CON_BL 0
#define CON_TL 1
#define CON_BR 2
#define CON_TR 3

CCommandConsole::SVertex CCommandConsole::m_oVerts[] = 
{
	{ D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f),		WHITE,	0.0f,	1.0f	}, // Bottom Left
	{ D3DXVECTOR4(-1.0f, -201.0f, 0.0f, 1.0f),	WHITE,	0.0f,	0.0f	}, // Top Left
	{ D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),		WHITE,	1.0f,	1.0f	}, // Bottom Right
	{ D3DXVECTOR4(0.0f, -201.0f, 0.0f, 1.0f),	WHITE,	1.0f,	0.0f	}, // Top Right
	
};

/**
* CCommandConsole::CCommandConsole
* @date Modified Apr 04, 2006
*/
CCommandConsole::CCommandConsole(float fConsoleSize, float fAnimSpeed) : m_eStatus(CSTAT_CLOSED), 
	m_fY(0.0f), m_fAnimSpeed(fAnimSpeed), m_fConSize(fConsoleSize), m_unOffset(0), m_pVB(0)
{
	m_mCommands["Help"] = NULL;
}

/**
* CCommandConsole::~CCommandConsole
* @date Modified Apr 04, 2006
*/
CCommandConsole::~CCommandConsole(void)
{
	// Release commands
	CommandList::iterator itCommand = m_mCommands.begin();
	while(itCommand != m_mCommands.end())
	{
		delete itCommand->second;
		++itCommand;
	}

	shutdownConsole();
}

/**
* CCommandConsole::doAnimation
* @date Modified Apr 04, 2006
*/
void CCommandConsole::doAnimation(void)
{
	switch(m_eStatus)
	{
	case CSTAT_CLOSING:
		{
			// Check for fully closed
			if(m_fY <= 0.0f)
			{
				m_eStatus = CSTAT_CLOSED;
				return;
			}

			// Animate
			D3DXMATRIX mPosition;
			float fAmount;
			if(m_fY - m_fAnimSpeed < 0.0f)
			{
				fAmount = -m_fY;
				m_fY = 0.0f;
			}
			else
			{
				fAmount = -m_fAnimSpeed;
				m_fY -= m_fAnimSpeed;
			}
			D3DXMatrixTranslation(&mPosition, 0.0f, fAmount, 0.0f);
			for(int i = 0; i < 4; ++i)
				D3DXVec4Transform(&m_oVerts[i].vPos, &m_oVerts[i].vPos, &mPosition);
		}
		break;
	case CSTAT_OPENING:
		{
			// Check for fully open
			if(m_fY >= m_fConSize)
			{
				m_eStatus = CSTAT_OPEN;
				return;
			}

			// Animate
			D3DXMATRIX mPosition;
			float fAmount;
			if(m_fY + m_fAnimSpeed > m_fConSize)
			{
				fAmount = m_fConSize - m_fY;
				m_fY = m_fConSize;
			}
			else
			{
				fAmount = m_fAnimSpeed;
				m_fY += m_fAnimSpeed;
			}
			D3DXMatrixTranslation(&mPosition, 0.0f, fAmount, 0.0f);
			for(int i = 0; i < 4; ++i)
				D3DXVec4Transform(&m_oVerts[i].vPos, &m_oVerts[i].vPos, &mPosition);
		}
		break;
	}
}

/**
* CCommandConsole::loadConsole
* @date Modified Apr 04, 2006
*/
void CCommandConsole::loadConsole(void)
{
	m_pFont = (CBitmapFont*)CResourceManager::getInstance().loadResource(RES_FONT, "Dina.ini");
	m_pTexture = (CTexture2D*)CResourceManager::getInstance().loadResource(RES_TEXTURE2D, "console.png");
	m_pDev = CRenderSystem::getInstance().getRenderDevice().getD3DDevice();
	m_pDev->CreateVertexBuffer(sizeof(SVertex) * 4, D3DUSAGE_DYNAMIC, FVF_CONSOLE, D3DPOOL_DEFAULT, &m_pVB, NULL);

	// Move console
	m_oVerts[CON_TL].vPos.y = m_oVerts[CON_TR].vPos.y = -m_fConSize - 1.0f;
	m_oVerts[CON_BR].vPos.x = m_oVerts[CON_TR].vPos.x = (float)CRenderSystem::getInstance().getDisplayWidth();
	m_oVerts[CON_BR].fU = m_oVerts[CON_TR].fU = m_oVerts[CON_TR].vPos.x /  m_pTexture->getWidth();
	m_oVerts[CON_BR].fV = m_oVerts[CON_BL].fV = 1.0f;
}

/**
* CCommandConsole::shutdownConsole
* @date Modified Apr 04, 2006
*/
void CCommandConsole::shutdownConsole(void)
{
	COM_RELEASE(m_pVB);
	COM_RELEASE(m_pFont);
	COM_RELEASE(m_pTexture);
}

void CCommandConsole::draw(void)
{
	if(m_eStatus == CSTAT_CLOSED)
		return;

	// Translate verts
	doAnimation();

	// Get view size
	RECT rView = {5};
	rView.bottom = (LONG)m_fY - 25;
	rView.top = rView.bottom - ((LONG)m_fConSize - 30);
	rView.right = (LONG)m_oVerts[CON_BR].vPos.x;

	LPVOID pData = 0;
	HRESULT hr = m_pVB->Lock(0, 0, &pData, D3DLOCK_DISCARD);
	if(FAILED(hr))
	{
		m_pVB->Unlock();
		return;
	}
	memcpy(pData, &m_oVerts, sizeof(SVertex) * 4);
	m_pVB->Unlock();

	// Change render states
	DWORD dwWrapState;
	m_pDev->GetRenderState(D3DRS_WRAP0, &dwWrapState);
	m_pDev->SetRenderState(D3DRS_WRAP0, D3DWRAP_U);
	m_pDev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	
	// Draw Background.
	m_pDev->SetTexture(0, m_pTexture->getD3DTexture());
	m_pDev->SetFVF(FVF_CONSOLE);
	m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(SVertex));
	m_pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	m_pDev->SetTexture(0, 0);

	// Reset states
	m_pDev->SetRenderState(D3DRS_WRAP0, dwWrapState);
	m_pDev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);

	// Draw Text
	LPD3DXSPRITE pSprite = CRenderSystem::getInstance().getRenderDevice().getD3DXSprite();
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pFont->drawStringView(m_osOutput.str().c_str(), rView, m_unOffset, 0xFFFFFFFF, false);
	CString sDisplayPrompt;
	sDisplayPrompt.Format(">{0xFFFFFFFF} %s", m_sInput.GetBuffer());
	m_pFont->drawStringMarkup(sDisplayPrompt, 5, m_fY - 30, 0xFF00D700, false);
	pSprite->End();
}

/**
* CCommandConsole::clearOutput
* @date Modified Apr 04, 2006
*/
void CCommandConsole::clearOutput(void)
{
	m_osOutput.clear();
	setColor(Console::fgHiWhite);
	m_pDev->SetFVF(FVF_CONSOLE);
}

/**
* CCommandConsole::setColor
* @date Modified Apr 04, 2006
*/
void CCommandConsole::setColor(WORD wRGBI)
{
	switch(wRGBI)
	{
	case Console::fgBlack:
		m_osOutput << "{0xFF000000}";
		break;
	case Console::fgLoRed:
		m_osOutput << "{0xFF400000}";
		break;
	case Console::fgLoGreen:
		m_osOutput << "{0xFF004000}";
		break;
	case Console::fgLoBlue:
		m_osOutput << "{0xFF000040}";
		break;
	case Console::fgLoCyan:
		m_osOutput << "{0xFF004040}";
		break;
	case Console::fgLoMagenta:
		m_osOutput << "{0xFF400040}";
		break;
	case Console::fgLoYellow:
		m_osOutput << "{0xFF404000}";
		break;
	case Console::fgLoWhite:
		m_osOutput << "{0xFFEBEBEB}";
		break;
	case Console::fgGray:
		m_osOutput << "{0xFFC0C0C0}";
		break;
	case Console::fgHiWhite:
		m_osOutput << "{0xFFFFFFFF}";
		break;
	case Console::fgHiBlue:
		m_osOutput << "{0xFF0000D7}";
		break;
	case Console::fgHiGreen:
		m_osOutput << "{0xFF00D700}";
		break;
	case Console::fgHiRed:
		m_osOutput << "{0xFFD70000}";
		break;
	case Console::fgHiCyan:
		m_osOutput << "{0xFF00D7D7}";
		break;
	case Console::fgHiMagenta:
		m_osOutput << "{0xFFD700D7}";
		break;
	case Console::fgHiYellow:
		m_osOutput << "{0xFFD700D7}";
		break;
	}
}

/**
* CCommandConsole::parseCommand
* @date Modified Apr 04, 2006
*/
void CCommandConsole::parseCommand(CString sCommand)
{
	std::vector<CString> vParams;
	sCommand.ToList(vParams, " ");
	if(sCommand.Left(4) == CString("help"))
	{
		if(vParams.size() > 1)
		{
			CommandList::iterator itCommand = m_mCommands.find(vParams[1]);
			if(itCommand != m_mCommands.end() && itCommand->second)
			{
				CString sSyntax;
				itCommand->second->getSyntax(sSyntax);
				m_osOutput << sSyntax.GetBuffer() << endl;
			}
			else
			{
				// Print command default help
				m_osOutput << "Command Help - supplies syntax information about a command: help (command)" << endl;
				listCommands(CString::Blank);
			}
		}
		else
		{
			// Print command default help
			m_osOutput << "Command Help - supplies syntax information about a command: help (command)" << endl;
			listCommands("");
		}
	}
	else
	{
		if(vParams.size())
		{
			sCommand = vParams[0];
			vParams.erase(vParams.begin());
		}

		CommandList::iterator itCommand = m_mCommands.find(sCommand);
		if(itCommand != m_mCommands.end())
		{
			CCommand* pCommand = itCommand->second;
			CString sError;
			for(size_t i = 0; i < vParams.size(); ++i)
				pCommand->addParam(new CCommandParam(vParams[i]));
			if(!pCommand->execute(sError))
				m_osOutput << "- {0xFFD70000}Error:{0xFFFFFFFF} " << sError.GetBuffer() << endl;
			pCommand->clearParams();
		}
		else
			m_osOutput << "Invalid command \"" << sCommand << "\"!" << endl;
	}
}

/**
* CCommandConsole::update
* @date Modified Apr 04, 2006
*/
void CCommandConsole::update(void)
{
	if(!isVisible())
		return;

	CDirectInputWrapper* pInput = CDirectInputWrapper::getInstancePtr();
	for(DWORD i = 0; i <= 256; ++i)
	{
		switch(i)
		{
		case DIK_SEMICOLON:
		case DIK_SLASH:
		case DIK_SUBTRACT:
		case DIK_ADD:
		case DIK_APOSTROPHE:
		case DIK_COLON:
		case DIK_COMMA:
		case DIK_DECIMAL:
		case DIK_DIVIDE:
		case DIK_EQUALS:
		case DIK_LBRACKET:
		case DIK_MINUS:
		case DIK_MULTIPLY:
		case DIK_NUMPADCOMMA:
		case DIK_NUMPADEQUALS:
		case DIK_PERIOD:
		case DIK_RBRACKET:
			// TODO: Convert DIK to ASCII
			break;
		case DIK_SPACE:
			if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
				m_sInput += " ";
			break;
		case DIK_NUMPAD0:
		case DIK_NUMPAD1:
		case DIK_NUMPAD2:
		case DIK_NUMPAD3:
		case DIK_NUMPAD4:
		case DIK_NUMPAD5:
		case DIK_NUMPAD6:
		case DIK_NUMPAD7:
		case DIK_NUMPAD8:
		case DIK_NUMPAD9:
		case DIK_0:
		case DIK_1:
		case DIK_2:
		case DIK_3:
		case DIK_4:
		case DIK_5:
		case DIK_6:
		case DIK_7:
		case DIK_8:
		case DIK_9:
			if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
			{
				char szChar[2] = {0};
				szChar[0] = pInput->convertDIKtoString(i)[0];
				m_sInput += szChar;
				return;
			}
			break;
		case DIK_A:
		case DIK_B:
		case DIK_C:
		case DIK_D:
		case DIK_E:
		case DIK_F:
		case DIK_G:
		case DIK_H:
		case DIK_I:
		case DIK_J:
		case DIK_K:
		case DIK_L:
		case DIK_M:
		case DIK_N:
		case DIK_O:
		case DIK_P:
		case DIK_Q:
		case DIK_R:
		case DIK_S:
		case DIK_T:
		case DIK_U:
		case DIK_V:
		case DIK_W:
		case DIK_X:
		case DIK_Y:
		case DIK_Z:
			if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
			{
				if(pInput->isKeyDown(DIK_LSHIFT) || pInput->isKeyDown(DIK_RSHIFT))
					m_sInput += pInput->convertDIKtoString(i);
				else
				{
					char szChar[2] = {0};
					szChar[0] = tolower(pInput->convertDIKtoString(i)[0]);
					m_sInput += szChar;
				}
				return;
			}
			break;
		case DIK_GRAVE:
			//if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
			//	toggleConsole();
			break;
		case DIK_NUMPADENTER:
		case DIK_RETURN:
			if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
			{
				if(m_sInput.GetLength())
				{
					parseCommand(m_sInput);
					m_sInput = "";
				}
				return;
			}
			break;
		case DIK_BACKSPACE:
			if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
			{
				m_sInput = m_sInput.Left(m_sInput.GetLength()-1);
			}
			break;
		case DIK_PGUP:
			if(pInput->isKeyDown(i))
			{
				std::vector<CString> vLines;
				CString sOutput = m_osOutput.str().c_str();
				sOutput.ToList(vLines, "\r\n");
				int nMaxOffset = (int)(((int)vLines.size()) - (m_fConSize - 20) / m_pFont->getLineHeight());
				if(nMaxOffset > 0 && nMaxOffset > (int)m_unOffset)
					++m_unOffset;

			}
			break;
		case DIK_PGDN:
			if(m_unOffset && pInput->isKeyDown(i))
				--m_unOffset;
			break;
		case DIK_TAB:
			if(pInput->isKeyDownBuffered(i, CONSOLE_SYSTEM))
				if(!checkCommand(m_sInput))
					listCommands(m_sInput);
			break;
		}
	}
}

/**
* CCommandConsole::registerCommand
* @date Modified Apr 05, 2006
*/
void CCommandConsole::registerCommand(CCommand* pCommand)
{
	// Make sure we don't keep duplicate commands.
	CommandList::iterator itCommand = m_mCommands.find(pCommand->m_sCommandName);
	if(itCommand == m_mCommands.end())
		m_mCommands[pCommand->m_sCommandName] = pCommand;
}

/**
* CCommandConsole::checkCommand
* @date Modified Apr 05, 2006
*/
bool CCommandConsole::checkCommand(CString& sPrefix)
{
	// Determine how many items in the list begin with the prefix
	if(sPrefix.GetLength())
	{
		// Check for help
		if(sPrefix.Left(4) == CString("help"))
		{
			// Print command default help
			m_osOutput << "Command Help - supplies syntax information about a command: help (command)" << endl;
			listCommands("");
			return true;
		}

		// Search for perfect command match first
		std::vector<CString> vCommand;
		sPrefix.ToList(vCommand, " ");
		CommandList::iterator itCommand = m_mCommands.find(vCommand[0]);
		if(itCommand != m_mCommands.end() && itCommand->second)
		{
			CString sSyntax;
			itCommand->second->getSyntax(sSyntax);
			m_osOutput << sSyntax.GetBuffer() << endl;
			return true;
		}

		// Item not found, check all items for any match.
		size_t nItems = 0;
		CommandList::iterator itLastMatch;
		itCommand = m_mCommands.begin();
		while(itCommand != m_mCommands.end())
		{
			if(itCommand->first.Left(sPrefix.GetLength()) == sPrefix)
			{
				++nItems;
				itLastMatch = itCommand;
			}

			++itCommand;
		}
		// If only one item matched, replace the prefix with the whole string
		if(nItems == 1)
		{
			sPrefix = itLastMatch->first;
			sPrefix += " ";
			return true;
		}
	}

	return false;
}

/**
* CCommandConsole::listCommands 
* @date Modified Apr 05, 2006
*/
void CCommandConsole::listCommands(CString sPrefix)
{
	// List commands
	m_osOutput << "Current Registered Commands:" << endl;
	CommandList::iterator itCommand = m_mCommands.begin();
	while(itCommand != m_mCommands.end())
	{
		if(sPrefix.GetLength())
		{
			if(itCommand->first.Left(sPrefix.GetLength()) == sPrefix)
			{
				m_osOutput << "\t" << itCommand->first.GetBuffer() << endl;
			}
		}
		else
			m_osOutput << "\t" << itCommand->first.GetBuffer() << endl;

		++itCommand;
	}
}