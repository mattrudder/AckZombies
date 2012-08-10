/**
* @file CommandConsole.h
* @author Matt Rudder
* @date Created Apr 04, 2006
*
* Console for manipulating objects within the game world.
*/

#ifndef _COMMANDCONSOLE_H_
#define _COMMANDCONSOLE_H_

// System includes
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>

// Engine includes
#include "AcespaceEngine.h"

// Defines
#define MAX_CONSOLE_LENGTH 7500

class CTexture2D; 
class CBitmapFont;
class CCommand;

/**
* Console for manipulating objects within the game world.
*
* @author Matt Rudder
* @date Created Apr 04, 2006
* @date Modified Apr 04, 2006
*/
class CCommandConsole
#ifdef _UNICODE
	: public basic_filebuf<wchar_t>, public CSingleton<CCommandConsole>
#else
	: public basic_filebuf<char>, public CSingleton<CCommandConsole>
#endif
{
	friend class CSingleton<CCommandConsole>;
public:
	//! Status Values
	enum EConsoleStatus	{CSTAT_CLOSED, CSTAT_OPENING, CSTAT_CLOSING, CSTAT_OPEN};
	//! Command List Type
	typedef std::map<CString, CCommand*> CommandList;
protected:
	//! Singleton stuff.
	//!@{
	CCommandConsole(float fConsoleSize = 256.0f, float fAnimSpeed = 7.0f);
	CCommandConsole(const CCommandConsole&){}
	operator=(const CCommandConsole&){}
	virtual ~CCommandConsole(void);
	//!@}
	
	struct SVertex
	{
		D3DXVECTOR4 vPos;
		DWORD dwColor;
		float fU, fV;
	};

	static SVertex m_oVerts[];

	//! Is the console visible?
	EConsoleStatus m_eStatus;
	//! Sprite interface
	LPDIRECT3DDEVICE9 m_pDev;
	//! Texture
	CTexture2D* m_pTexture;
	//! Font
	CBitmapFont* m_pFont;
	//! Vertex Buffer
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	//! Current y position; Used for console animation.
	float m_fY;
	//! Animation rate
	float m_fAnimSpeed;
	//! Console size
	float m_fConSize;
	//! Console input
	CString m_sInput;
	//! Console offset
	unsigned int m_unOffset;
	//! Command list
	CommandList m_mCommands;
	
#ifdef _UNICODE
	//! Console output
	std::basic_ostringstream<wchar_t> m_osOutput;
#else
	//! Console output
	std::basic_ostringstream<char> m_osOutput;
#endif

	/**
	* Insert elements into the output stream.
	*
	* @date Created Mar 28, 2006
	* @param[in]	_Meta	The character to insert into the buffer. 
	* @return If both streams return traits_type::eof(), the same is returned,
	*		  otherwise the inserted character.
	*/
	int_type overflow(int_type c)
	{
		m_osOutput.put(c);
		if(m_osOutput.str().size() > MAX_CONSOLE_LENGTH)
		{
			DWORD dwCharsToRemove = (DWORD)(MAX_CONSOLE_LENGTH - m_osOutput.str().size());

			// Split into lines.
			CString sCon(m_osOutput.str().c_str());
			std::vector<CString> vLines;
			sCon.ToList(vLines, "\r\n");
			
			// Clear current stream
			m_osOutput.str("");
			
			// Determine how many lines should be removed.
			size_t nListSize = vLines.size(), nIndex = 0;
			while(dwCharsToRemove)
				dwCharsToRemove -= (DWORD)vLines[nIndex++].GetLength();

			// Ouput last lines
			for(size_t i = nIndex; i < nListSize; ++i)
				m_osOutput << vLines[i].GetBuffer() << '\n';
		}
		return c;
	}

	/**
	* Animates the console depending on the current status.
	*
	* @date Created Apr 04, 2006
	*/
	void doAnimation(void);

	/**
	* Parses a command sent from the console
	*
	* @date Created Apr 04, 2006
	* @param[in]	sCommand	The command to parse
	*/
	void parseCommand(CString sCommand);
public:

	/**
	* Accessors/Modifiers
	* @{
	*/
	bool isVisible(void)		
	{ 
		return m_eStatus == CSTAT_OPEN || m_eStatus == CSTAT_OPENING;		
	}
	void hideConsole(void)		
	{ 
		m_eStatus = CSTAT_CLOSING;	
	}
	void showConsole(void)		
	{ 
		m_eStatus = CSTAT_OPENING;	
	}
	void toggleConsole(void)
	{
		if(m_eStatus == CSTAT_OPENING || m_eStatus == CSTAT_OPEN)
		{
			m_eStatus = CSTAT_CLOSING;
			return;
		}

		if(m_eStatus == CSTAT_CLOSING || m_eStatus == CSTAT_CLOSED)
		{
			m_eStatus = CSTAT_OPENING;
			return;
		}
	}
	//!@}

	/**
	* Loads resources needed to render the console window.
	*
	* @date Created Apr 04, 2006
	*/
	void loadConsole(void);

	/**
	* Cleans up resources needed to render the console window.
	*
	* @date Created Apr 04, 2006
	*/
	void shutdownConsole(void);

	/**
	* Clears the output of the console.
	*
	* @date Created Apr 04, 2006
	*/
	void clearOutput(void);

	/**
	* Sets the current color of the console's output.
	*
	* @date Created Apr 04, 2006
	*/
	void setColor(WORD wRGBI);

	/**
	* Draws the current state of the console.
	*
	* @date Created Apr 04, 2006
	*/
	void draw(void);

	/**
	* Processes input and commands for the console per frame
	*
	* @date Created Apr 04, 2006
	*/
	void update(void);

	/**
	* Adds an entry to the internal command list for later use.
	*
	* @date Created Apr 05, 2006
	* @param[in]	pCommand	The command to register.
	*/
	void registerCommand(CCommand* pCommand);

	/**
	* Check to see if a command can be found from a prefix. If so, return the command
	*
	* @date Created Apr 05, 2006
	* @param[in,out]	sPrefix		The prefix to search for. If found, returns the command.
	* @return true if a single command was found from the prefix, false if a command could not be determined.
	*/
	bool checkCommand(CString& sPrefix);

	/**
	* List all registered commands.
	*
	* @date Created Apr 05, 2006
	* @param[in]	sPrefix		Optional prefix to narrow the search.
	*/
	void listCommands(CString sPrefix);
};

#endif //_COMMANDCONSOLE_H_