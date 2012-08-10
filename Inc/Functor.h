/**
* @file Functor.h
* @author Matt Rudder
* @date Created Mar 06, 2006
*
* Utility class providing the ability to wrap a function in a object.
*/ 

#ifndef _FUNCTOR_H_
#define _FUNCTOR_H_

// System includes
#include "AcespaceEngine.h"
#include "Object.h"

/**
* Utility class providing the ability to wrap a function in a object
*
* @author Matt Rudder
* @version 1.0
* @remark Base class for alternate functors.
*/
template<typename return_type>
class CFunctor
{
public:
	CFunctor(void) {}
	virtual ~CFunctor(void) {}

	virtual return_type operator()() { return return_type(); }
};

#endif //_FUNCTOR_H_