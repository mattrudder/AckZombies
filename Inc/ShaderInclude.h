/**
* @file ShaderInclude.h
* @author Matt Rudder
* @date Created Apr 02, 2006
*
* Handler for shader includes
*/

#ifndef _SHADERINCLUDE_H_
#define _SHADERINCLUDE_H_

// System includes
#include <d3dx9.h>


class CShaderInclude : public ID3DXInclude
{
public:
	/**
	* Handles the opening of include files within shaders
	*
	* @date Created Apr 02, 2006
	* @param[in]	eIncludeType	Location of the included file
	* @param[in]	szFilename		Name of the include file
	* @param[in]	pParentData		Pointer to the container that includes the 
	*								file.
	* @param[out]	ppData			Pointer to the returned buffer that contains 
	*								the include directives
	* @param[out]	pBytes			Bytes returned in ppData.
	* @return Status of the include operation
	*/
	STDMETHOD(Open)(D3DXINCLUDE_TYPE eIncludeType, LPCSTR szFilename, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes);

	/**
	* Handles the closing of include files within shaders
	*
	* @date Created Apr 02, 2006
	* @param[in]	pData	Pointer returned by Open (ppData)
	* @return Status of the include operation
	*/
	STDMETHOD(Close)(LPCVOID pData);
};

#endif //_SHADERINCLUDE_H_