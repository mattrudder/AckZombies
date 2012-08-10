/**
* @file MethodSingleFunctor.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Function object that encapsulates a class method that takes one parameter.
*/ 

#ifndef _METHODSINGLEFUNCTOR_H_
#define _METHODSINGLEFUNCTOR_H_

// Local includes
#include "Functor.h"

/**
* Function object that encapsulates a class method that takes one parameter.
*
* @author Matt Rudder
* @version 1.0
*/
template<typename Class, typename Param, typename RetVal = void>
class CMethodSingleFunctor : public CFunctor<RetVal>
{
protected:
	typedef RetVal (Class::* Func)(Param);
	Class* m_pObj;
	Func m_pFunc;
public:
	
	CMethodSingleFunctor(Func pFunc, Class* pObj) : m_pObj(pObj), m_pFunc(pFunc) {}
	virtual ~CMethodSingleFunctor(void) {}
	RetVal operator ()(Param param)
	{
		return (m_pObj->*m_pFunc)(param);
	}
};

#endif //_METHODSINGLEFUNCTOR_H_