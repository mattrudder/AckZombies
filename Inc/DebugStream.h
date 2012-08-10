#ifndef _DEBUGSTREAM_H_
#define _DEBUGSTREAM_H_

// Local includes
#include "twobufstream.h"

// System include
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;

class CDebugStream
#ifdef _UNICODE
	: public basic_ostream<wchar_t>
#else
	: public basic_ostream<char>
#endif
{
#ifdef _UNICODE
	typedef basic_ostream<wchar_t> stream_type;
	typedef basic_filebuf<wchar_t> buffer_type;
	typedef wchar_t char_type;
#else
	typedef basic_ostream<char> stream_type;
	typedef basic_filebuf<char> buffer_type;
	typedef char char_type;
#endif

	FILE *								m_fpLog;
	FILE *								m_fpNull;


	buffer_type *						m_fbLog;
	buffer_type							m_fbNull;
	twobufstream<char_type> *			m_fbTwo;
	bool m_bDebugLog;
	static bool m_bIsDebug;

public:
	CDebugStream(bool bDebugLog = true);
	virtual ~CDebugStream(void);

	/**
	* Open and begin writing to log file.
	* 
	* @date Created Mar 28, 2006
	* @param[in]	szFile		File to save log to.
	*/
	void openLog(const char* szFile);

	/**
	* Closes a previously opened log file.
	* 
	* @date Created Mar 28, 2006
	*/
	void closeLog(void);

	/**
	* Clears all data from the current console window.
	*
	* @date Created Mar 28, 2006
	*/
	static void clearConsole();

	/**
	* Sets the color of the current console window.
	*
	* @date Created Apr 04, 2006
	* @param[in]	wRGBI	Color flags.
	* @param[in]	Mask	Background/Foreground flags
	*/
	static void setColor(WORD wRGBI, WORD Mask);
};

// Log info namespace
namespace Console
{
	static const WORD bgMask( BACKGROUND_BLUE      | 
		BACKGROUND_GREEN     | 
		BACKGROUND_RED       | 
		BACKGROUND_INTENSITY   );
	static const WORD fgMask( FOREGROUND_BLUE      | 
		FOREGROUND_GREEN     | 
		FOREGROUND_RED       | 
		FOREGROUND_INTENSITY   );

	static const WORD fgBlack    ( 0 ); 
	static const WORD fgLoRed    ( FOREGROUND_RED   ); 
	static const WORD fgLoGreen  ( FOREGROUND_GREEN ); 
	static const WORD fgLoBlue   ( FOREGROUND_BLUE  ); 
	static const WORD fgLoCyan   ( fgLoGreen   | fgLoBlue ); 
	static const WORD fgLoMagenta( fgLoRed     | fgLoBlue ); 
	static const WORD fgLoYellow ( fgLoRed     | fgLoGreen ); 
	static const WORD fgLoWhite  ( fgLoRed     | fgLoGreen | fgLoBlue ); 
	static const WORD fgGray     ( fgBlack     | FOREGROUND_INTENSITY ); 
	static const WORD fgHiWhite  ( fgLoWhite   | FOREGROUND_INTENSITY ); 
	static const WORD fgHiBlue   ( fgLoBlue    | FOREGROUND_INTENSITY ); 
	static const WORD fgHiGreen  ( fgLoGreen   | FOREGROUND_INTENSITY ); 
	static const WORD fgHiRed    ( fgLoRed     | FOREGROUND_INTENSITY ); 
	static const WORD fgHiCyan   ( fgLoCyan    | FOREGROUND_INTENSITY ); 
	static const WORD fgHiMagenta( fgLoMagenta | FOREGROUND_INTENSITY ); 
	static const WORD fgHiYellow ( fgLoYellow  | FOREGROUND_INTENSITY );
	static const WORD bgBlack    ( 0 ); 
	static const WORD bgLoRed    ( BACKGROUND_RED   ); 
	static const WORD bgLoGreen  ( BACKGROUND_GREEN ); 
	static const WORD bgLoBlue   ( BACKGROUND_BLUE  ); 
	static const WORD bgLoCyan   ( bgLoGreen   | bgLoBlue ); 
	static const WORD bgLoMagenta( bgLoRed     | bgLoBlue ); 
	static const WORD bgLoYellow ( bgLoRed     | bgLoGreen ); 
	static const WORD bgLoWhite  ( bgLoRed     | bgLoGreen | bgLoBlue ); 
	static const WORD bgGray     ( bgBlack     | BACKGROUND_INTENSITY ); 
	static const WORD bgHiWhite  ( bgLoWhite   | BACKGROUND_INTENSITY ); 
	static const WORD bgHiBlue   ( bgLoBlue    | BACKGROUND_INTENSITY ); 
	static const WORD bgHiGreen  ( bgLoGreen   | BACKGROUND_INTENSITY ); 
	static const WORD bgHiRed    ( bgLoRed     | BACKGROUND_INTENSITY ); 
	static const WORD bgHiCyan   ( bgLoCyan    | BACKGROUND_INTENSITY ); 
	static const WORD bgHiMagenta( bgLoMagenta | BACKGROUND_INTENSITY ); 
	static const WORD bgHiYellow ( bgLoYellow  | BACKGROUND_INTENSITY );

	// Color flags
	inline std::ostream& clr( std::ostream& os )
	{
		os.flush();
		CDebugStream::clearConsole();
		return os;
	}

	inline std::ostream& def( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiWhite, bgMask);
		CDebugStream::setColor(bgBlack, fgMask);

		return os;
	}

	inline std::ostream& fg_red( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiRed, bgMask);

		return os;
	}

	inline std::ostream& fg_green( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiGreen, bgMask);

		return os;
	}

	inline std::ostream& fg_blue( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiBlue, bgMask);

		return os;
	}

	inline std::ostream& fg_white( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiWhite, bgMask);

		return os;
	}

	inline std::ostream& fg_cyan( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiCyan, bgMask);

		return os;
	}

	inline std::ostream& fg_magenta( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiMagenta, bgMask);

		return os;
	}

	inline std::ostream& fg_yellow( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgHiYellow, bgMask);

		return os;
	}

	inline std::ostream& fg_black( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgBlack, bgMask);

		return os;
	}

	inline std::ostream& fg_gray( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(fgGray, bgMask);

		return os;
	}

	inline std::ostream& bg_red( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiRed, fgMask);

		return os;
	}

	inline std::ostream& bg_green( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiGreen, fgMask);

		return os;
	}

	inline std::ostream& bg_blue( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiBlue, fgMask);

		return os;
	}

	inline std::ostream& bg_white( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiWhite, fgMask);

		return os;
	}

	inline std::ostream& bg_cyan( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiCyan, fgMask);

		return os;
	}

	inline std::ostream& bg_magenta( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiMagenta, fgMask);

		return os;
	}

	inline std::ostream& bg_yellow( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgHiYellow, fgMask);

		return os;
	}

	inline std::ostream& bg_black( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgBlack, fgMask);

		return os;
	}

	inline std::ostream& bg_gray( std::ostream& os )
	{
		os.flush();
		CDebugStream::setColor(bgGray, fgMask);

		return os;
	}
};

#endif //_DEBUGSTREAM_H_