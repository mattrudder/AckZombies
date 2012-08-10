/**
* @file ShaderInclude.h
* @author Matt Rudder
* @date Created Apr 02, 2006
*
* Handler for shader includes
*/

// Local includes
#include "ShaderInclude.h"
#include "resource.h"

// Engine includes
#include "AcespaceEngine.h"
#include "ResourceManager.h"

/**
* 
* @date Modified Apr 02, 2006
*/
HRESULT CShaderInclude::Open(D3DXINCLUDE_TYPE eIncludeType, LPCSTR szFilename, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{

	// Check for engine specific includes
	if(!stricmp(szFilename, "sas/sas.fxh") || 
		!stricmp(szFilename, "sas\\sas.fxh") || 
		!stricmp(szFilename, "sas\\\\sas.fxh") || 
		!stricmp(szFilename, "sas.fxh"))
	{
		// Get resource
		HGLOBAL hGlobal = NULL;
		
		HRSRC hRC = FindResource(NULL, MAKEINTRESOURCE(IDR_SASFXH), RT_RCDATA);
		if(hRC == NULL)
		{
			Debug << "Error Loading SAS header! Error: " << GetLastError() << endl;
			return E_FAIL;
		}

		hGlobal = LoadResource(NULL, hRC);
		if(hGlobal == NULL)
			return E_FAIL;

		LPVOID pIncData = LockResource(hGlobal);
		if(pIncData == NULL)
			return E_FAIL;

		// Copy data
		*pBytes = SizeofResource(NULL, hRC);
		*ppData = (void*)new unsigned char[*pBytes];
		memcpy((void*)*ppData, pIncData, *pBytes);
		FreeResource(hGlobal);
		hGlobal = NULL;
	}
	else
	{
		// Search for file
		CString sPath;
		if(!CResourceManager::getInstance().searchMediaPaths(RES_SHADER, szFilename, sPath))
			return E_FAIL;

		// Found it, read it in.
		HANDLE hFile = CreateFile(sPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}

		BY_HANDLE_FILE_INFORMATION oFileInfo;
		GetFileInformationByHandle(hFile, &oFileInfo);
		*pBytes = oFileInfo.nFileSizeLow;
		*ppData = (void*)new unsigned char[*pBytes];
		if(ppData == NULL)
		{
			CloseHandle(hFile);
			return E_OUTOFMEMORY;
		}

		DWORD dwBytesRead;
		if(!ReadFile(hFile, (void*)*ppData, *pBytes, &dwBytesRead, NULL))
		{
			CloseHandle(hFile);
			return E_FAIL;
		}

		CloseHandle(hFile);
	}

	return S_OK;
}

/**
* 
* @date Modified Apr 02, 2006
*/
HRESULT CShaderInclude::Close(LPCVOID pData)
{
	unsigned char* szInclude = (unsigned char*)pData;
	if(szInclude)
		delete [] szInclude;
	return S_OK;
}