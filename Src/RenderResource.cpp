/**
* @file RenderResource.cpp
* @author Matt Rudder
* @date Created Mar 28, 2006
*
* Defines an interface which all renderable resources must adhere to.
*/

// Local includes
#include "RenderResource.h"

// Engine includes
#include "RenderSystem.h"

/**
* CRenderResource::CRenderResource
* @date Created Mar 28, 2006
*/
CRenderResource::CRenderResource()
{
	CRenderSystem::getInstance().addResource(this); 
}

/**
* CRenderResource::~CRenderResource
* @date Created Mar 28, 2006
*/
CRenderResource::~CRenderResource() 
{
	CRenderSystem::getInstance().removeResource(this); 
}