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

/**
* Override of basic_streambuf to allow two buffers. Used for logging system.
*
* @author Matt Rudder
* @date Created Mar 28, 2006
* @date Modified Mar 28, 2006
*/
template<class T>
class twobufstream : public std::basic_streambuf<T>
{
	// Types
public:
	using buffer_type = std::basic_streambuf<T>;
    using char_type = typename buffer_type::char_type;
    using int_type = typename buffer_type::int_type;
	using traits_type = typename buffer_type::traits_type;

protected:
	// Members
	buffer_type* m_bufConsole;
	buffer_type* m_bufLog;
	std::basic_filebuf<char_type> m_fbNull;

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

	twobufstream() : std::basic_streambuf<char_type>(), m_bufConsole(&m_fbNull), m_bufLog(&m_fbNull)
	{
		m_fbNull.open("nul", std::ios::out);
	}

	twobufstream(buffer_type* pBufCon, buffer_type* pBufLog) : std::basic_streambuf<char_type>(), m_bufConsole(pBufCon), m_bufLog(pBufLog)
	{
		this->setp(0, 0);
	}

public:

	/**
	* Assign buffer for later input.
	*
	* @date Created Mar 28, 2006
	* @param[in]	pBufCon	Buffer for console.
	* @param[in]	pBufLog	Buffer for log file.
	*/
	void setBuffers(buffer_type* pBufCon = nullptr, buffer_type* pBufLog = nullptr)
	{
		if (pBufCon == nullptr)
			pBufCon = &m_fbNull;
		else
			m_bufConsole = pBufCon;

		if (pBufLog == nullptr)
			pBufLog = &m_fbNull;
		else
			m_bufLog = pBufLog;
	}
};

#endif // _TWOBUFSTREAM_H_