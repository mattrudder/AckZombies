/**
* @file StaticFunctor.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Function object that encapsulates a static class method that takes no parameters.
*/ 

#ifndef _STATICFUNCTOR_H_
#define _STATICFUNCTOR_H_

// Local includes
#include "Functor.h"

/**
* Function object that encapsulates a static class method that takes no parameters.
*
* @author Matt Rudder
* @version 1.0
*/
template<typename RetVal = void>
class CStaticFunctor : public CFunctor<RetVal>
{
protected:
	typedef RetVal (*Func)(void);
	Func m_pFunc;
public:
	
	CStaticFunctor(Func pFunc) : m_pFunc(pFunc) {}
	virtual ~CStaticFunctor(void) {}
	RetVal operator ()(void)
	{
		return (*m_pFunc)();
	}
};

#endif //_STATICFUNCTOR_H_