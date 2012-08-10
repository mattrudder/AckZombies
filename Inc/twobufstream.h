/**
* @file twobufstream.h
* @author Matt Rudder
* @date Created Mar 28, 2006
*
* Override of basic_streambuf to allow two buffers. Used for logging system.
*/

#ifndef _TWOBUFSTREAM_H_
#define _TWOBUFSTREAM_H_

#include <iostream>
using namespace std;

/**
* Override of basic_streambuf to allow two buffers. Used for logging system.
*
* @author Matt Rudder
* @date Created Mar 28, 2006
* @date Modified Mar 28, 2006
*/
template<class char_type>
class twobufstream : public basic_streambuf<char_type>
{
	// Types
public:

	typedef char_traits<char_type> traits_type;
	typedef basic_streambuf<char_type> buffer_type;

protected:
	// Members
	buffer_type* m_bufConsole;
	buffer_type* m_bufLog;

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
		int nCon = m_bufConsole->sputc(c),
			nLog = m_bufLog->sputc(c);
		if(nCon == traits_type::eof() || nLog == traits_type::eof())
			return traits_type::eof();
		return c;
	}

	/**
	* Synchronize (flush) buffers
	*
	* @date Created Mar 28, 2006
	* @return If the function cannot succeed, it returns -1. The default 
	*			behavior is to return zero.
	*/
	int sync(void)
	{
		int nCon = m_bufConsole->pubsync(),
			nLog = m_bufLog->pubsync();
		if(nCon == -1 || nLog == -1)
			return -1;
		return 0;
	}
public:

	twobufstream(buffer_type* pBufCon, buffer_type* pBufLog) : basic_streambuf<char_type>(), m_bufConsole(pBufCon), m_bufLog(pBufLog)
	{
		setp(0, 0);
	}

public:

	/**
	* Assign buffer for later input.
	*
	* @date Created Mar 28, 2006
	* @param[in]	pBufCon	Buffer for console.
	* @param[in]	pBufLog	Buffer for log file.
	*/
	void setBuffers(buffer_type* pBufCon, buffer_type* pBufLog)
	{
		m_bufConsole = pBufCon;
		m_bufLog = pBufLog;
	}
};

#endif // _TWOBUFSTREAM_H_