/**
* @file Singleton.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Contains the template class that implements the Singleton design pattern.
*/

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#ifndef NULL
#	define NULL 0L
#endif

/**
 * Contains the template class that implements the Singleton design pattern.
 * 
 * @internal
 * @param singleton_t The child class, used to create the single instance.
 * @remarks This singleton implementation is -not- thread safe.
 */
template<typename singleton_t>
class CSingleton
{
	friend singleton_t;
protected:
	//! Single instance to our class.
	static singleton_t* m_poInstance;

	//! Singleton stuff.
	//!@{
	CSingleton(void) {}
	CSingleton(const CSingleton<singleton_t>&) {}
	CSingleton& operator=(const CSingleton<singleton_t>&){ return *this; }
	virtual ~CSingleton(void) {}
	//!@}
public:

	/**
	* Returns the single instance of the singleton manager.
	*
	* @date Created Mar 07, 2006
	* @return Reference to the static instance of the manager.
	*/
	static inline singleton_t& getInstance(void)
	{
		if(!m_poInstance)
			m_poInstance = new singleton_t;

		return *m_poInstance;
	}

	/**
	* Returns the single instance of the singleton manager.
	*
	* @date Created Mar 07, 2006
	* @return Pointer to the static instance of the manager.
	*/
	static inline singleton_t* getInstancePtr(void)
	{
		if(!m_poInstance)
			m_poInstance = new singleton_t;

		return m_poInstance;
	}

	/**
	* Destroys the single instance of the manager.
	*
	* @date Created Mar 07, 2006
	*/
	static inline void deleteInstance(void)
	{
		if(m_poInstance)
		{
			delete m_poInstance;
			m_poInstance = NULL;
		}
	}
};

template<class singleton_t> singleton_t* CSingleton<singleton_t>::m_poInstance = 0;

#endif //_SINGLETON_H_