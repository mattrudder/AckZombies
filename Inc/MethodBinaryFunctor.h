/**
* @file MethodBinaryFunctor.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Function object that encapsulates a class method that takes two parameters.
*/ 

#ifndef _METHODBINFUNCTOR_H_
#define _METHODBINFUNCTOR_H_

// Local includes
#include "Functor.h"

/**
* Function object that encapsulates a class method that takes two parameters.
*
* @author Matt Rudder
* @version 1.0
*/
template<typename Class, typename Param1, typename Param2, typename RetVal = void>
class CMethodBinaryFunctor : public CFunctor<RetVal>
{
protected:
	typedef RetVal (Class::* Func)(Param1, Param2);
	Class* m_pObj;
	Func m_pFunc;
public:
	
	CMethodBinaryFunctor(Func pFunc, Class* pObj) : m_pObj(pObj), m_pFunc(pFunc) {}
	virtual ~CMethodBinaryFunctor(void) {}
	RetVal operator ()(Param1 p1, Param2 p2)
	{
		return (m_pObj->*m_pFunc)(p1, p2);
	}
};

#endif //_METHODBINFUNCTOR_H_