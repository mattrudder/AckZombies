/**
* @file MethodFunctor.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Function object that encapsulates a class method that takes no parameters.
*/ 

#ifndef _METHODFUNCTOR_H_
#define _METHODFUNCTOR_H_

// Local includes
#include "Functor.h"

/**
* Function object that encapsulates a class method that takes no parameters.
*
* @author Matt Rudder
* @version 1.0
*/
template<typename Class, typename RetVal = void>
class CMethodFunctor : public CFunctor<RetVal>
{
protected:
	typedef RetVal (Class::* Func)(void);
	Class* m_pObj;
	Func m_pFunc;
public:
	
	CMethodFunctor(Func pFunc, Class* pObj) : m_pObj(pObj), m_pFunc(pFunc) {}
	virtual ~CMethodFunctor(void) {}
	RetVal operator ()(void)
	{
		return (m_pObj->*m_pFunc)();
	}
};

#endif //_METHODFUNCTOR_H_