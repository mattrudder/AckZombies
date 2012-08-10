/**
* @file RenderResource.h
* @author Matt Rudder
* @date Created Mar 28, 2006
*
* Defines an interface which all renderable resources must adhere to.
*/

#ifndef _RENDERRESOURCE_H_
#define _RENDERRESOURCE_H_

// Engine includes
#include "BaseResource.h"

/**
* Defines an interface which all renderable resources must adhere to.
*
* @author Matt Rudder
* @date Created Mar 28, 2006
* @date Modified Mar 28, 2006
*/
class CRenderResource : public CBaseResource
{
public:

	CRenderResource();
	~CRenderResource();

	/**
	* Event handler called upon the render device unexpectedly being lost.
	*
	* @date Created Mar 28, 2006
	*/
	virtual void onDeviceLoss(void) = 0;

	/**
	* Event handler called prior to the render device being released, usually
	* just before the shutdown of the application or due to a catastrophic 
	* event.
	*
	* @date Created Mar 28, 2006
	*/
	virtual void onDeviceRelease(void) = 0;

	/**
	* Event handler called upon the restoration or re-creation of the render
	* device.
	*
	* @date Created Mar 28, 2006
	*/
	virtual void onDeviceRestore(void) = 0;
};

#endif //_RENDERRESOURCE_H_