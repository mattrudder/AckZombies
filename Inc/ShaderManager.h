/**
* @file ShaderManager.h
* @author Matt Rudder
* @date Created May 05, 2006
*
* Manages shader constants in the engine to be up to date with the current state.
*/

#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

// Local includes
#include "Singleton.h"

// Engine includes
#include "AcespaceEngine.h"
#include "Shader.h"
#include "Light.h"

// System includes
#include <map>
#include <list>
using std::multimap;
using std::list;

class CActor;

#define SHADER_END() { SEM_UNKNOWN, ST_UNKNOWN, UPDATE_NONE, "" }

/**
* Manages shader constants in the engine to be up to date with the current state.
*
* @author Matt Rudder
* @date Created May 05, 2006
* @date Modified May 05, 2006
*/
class CShaderManager : public CSingleton<CShaderManager>
{
	friend class CShader;
	friend class CSingleton<CShaderManager>;
public:

	enum EUpdateFreq { UPDATE_NONE = 0, UPDATE_PERFRAME = 1, UPDATE_PERPASS, UPDATE_PEROBJECT };
	enum ESemanticType 
	{ 
		// Standard Types
		ST_UNKNOWN = 0, 
		ST_FLOAT = 0x00000001, ST_FLOAT2 = 0x00000002, ST_FLOAT3 = 0x00000004, ST_FLOAT4 = 0x00000008, 
		ST_INT = 0x00000010, ST_BOOL = 0x00000020, 
		ST_TEXTURE = 0x00000040, 
		ST_MATRIX = 0x00000080, ST_MATRIXARRAY = 0x00000100,
		// Composite Types
		ST_COLOR = ST_FLOAT4 | ST_FLOAT3 | ST_TEXTURE,
		ST_SPECPOW = ST_FLOAT4 | ST_FLOAT3 | ST_FLOAT,
		ST_RAND = ST_FLOAT | ST_FLOAT2 | ST_FLOAT3 | ST_FLOAT4 | ST_INT,
	};
	enum ESemantic
	{
		SEM_UNKNOWN = 0,
		// Color Values (Could be textures is some cases, ie diffuse, spec)
		SEM_DIFFUSE, SEM_SPECULAR, SEM_AMBIENT, SEM_SPECULARPOW, SEM_EMISSIVE,
		// Texture Samplers
		SEM_NORMAL, SEM_HEIGHT, SEM_REFRACT, SEM_OPACITY, SEM_ENVIRON, SEM_ENVIRONNORM,
		// Vectors
		SEM_FRESNEL, 
		// Base Matrices
		SEM_WORLD, SEM_WORLDINV, SEM_WORLDT, SEM_WORLDIT, 
		SEM_PROJ, SEM_PROJINV, SEM_PROJT, SEM_PROJIT,  
		SEM_VIEW, SEM_VIEWINV, SEM_VIEWT, SEM_VIEWIT,
		SEM_WORLDARRAY,
		// Matrix Concatenations
		SEM_WORLDVIEW, SEM_WORLDVIEWINV, SEM_WORLDVIEWT, SEM_WORLDVIEWIT,
		SEM_WORLDVIEWPROJ, SEM_WORLDVIEWPROJINV, SEM_WORLDVIEWPROJT, SEM_WORLDVIEWPROJIT,
		SEM_VIEWPROJ, SEM_VIEWPROJINV, SEM_VIEWPROJT, SEM_VIEWPROJIT,
		// Object info
		SEM_POSITION, SEM_ORIENTATION,
		// Engine info
		SEM_VIEWPORT, SEM_VIEWPORTWIDTH, SEM_VIEWPORTHEIGHT,
		SEM_TIME, SEM_LASTTIME, SEM_ELAPSEDTIME, SEM_SKINNEDMESH, SEM_ACTORCOLOR,
		SEM_LIGHTCOLOR, SEM_LIGHTPOS, SEM_LIGHTMAP, SEM_TEXPROJ, SEM_LIGHTRANGE,
		SEM_LIGHTDIR,
		// Camera info
		SEM_CAMPOSITION,
	};
	struct SSemanticType
	{
		ESemantic eSem;
		ESemanticType eType;
		EUpdateFreq eUpdate;
		const char* szName;
	};
	struct SShaderUpdate
	{
		SSemanticType oDetails;
		CShaderParam* pParam;
	};
	typedef multimap<EUpdateFreq, SShaderUpdate> ParamList;
	typedef map<ESemantic, CBaseResource*> ParamResource;
	typedef pair<EUpdateFreq, SShaderUpdate> ParamEntry;
protected:
	//! Singleton stuff.
	//!@{
	CShaderManager(void) : m_pActor(NULL), m_pShader(NULL), m_pLight(NULL) {}
	CShaderManager(const CShaderManager&) : m_pActor(NULL), m_pShader(NULL), m_pLight(NULL) {}
	operator=(const CShaderManager&){}
	virtual ~CShaderManager(void) {}
	//!@}

	//! Registered list of supported semantic types
	static SSemanticType m_oSemantics[];
	//! List of shader parameters to update
	ParamList m_mUpdates;
	//! List of currently active resources to pass to shaders
	ParamResource m_mResources;
	//! Currently active shader
	CShader* m_pShader;
	//! Currently rendered actor
	CActor* m_pActor;
	//! Current light; Used to select proper technique.
	CLight* m_pLight;

	/**
	* Updates a parameter based on its semantic and given type.
	*
	* @date Created May 05, 2006
	* @param[in]	oParam	Update structure.
	*/
	void updateParam(SShaderUpdate& oParam);

	/**
	* Parses a shader for potential parameters
	*
	* @date Created May 05, 2006
	* @param[in]	oShader		The shader to parse
	*/
	void parseShader(CShader& oShader);

	/**
	* Converts a shader param type to something the manager can use.
	*
	* @date Created May 05, 2006
	* @param[in]	oParam	The shader param to process.
	*/
	ESemanticType convertParamType(CShaderParam& oParam);
public:

	/**
	* Tells the manager that a new object is ready to be rendered. Per object
	* shader params should probably update here.
	*
	* @date Created May 05, 2006
	*/
	void beginObjectRender(void);

	/**
	* Tells the manager that a new frame has begun to render. Per frame shader 
	* params should probably be updated here.
	*
	* @date Created May 05, 2006
	*/
	void beginFrame(void);

	/**
	* Tells the manager that a new pass has begun to render. Per pass shader
	* params should probably be updated here.
	*
	* @date Created May 05, 2006
	*/
	void beginPass(void);

	/**
	* Sets the active shader. Used so we only update shaders being used at the 
	* time.
	*
	* @date Created May 05, 2006
	*/
	void setActiveShader(CShader* pShader);

	/**
	* Sets the active resource for a given semantic.
	*
	* @date Created May 05, 2006
	*/
	void setActiveResource(ESemantic eSem, CBaseResource* pRes);

	/**
	* Sets the active actor being rendered.
	*
	* @date Created May 05, 2006
	* @param[in]	pActor	The current actor being rendered; NULL for world geometry.
	*/
	void setActiveActor(CActor* pActor)
	{
		m_pActor = pActor;
	}

	/**
	* Sets the current light; Used to selected proper rendering technique.
	*
	* @date Created Jun 07, 2006
	* @param[in]	pLight	Current light being rendered.
	*/
	void setCurrentLight(CLight* pLight);
};

#endif //_SHADERMANAGER_H_