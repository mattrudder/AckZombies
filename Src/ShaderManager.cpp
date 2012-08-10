/**
* @file ShaderManager.cpp
* @author Matt Rudder
* @date Modified May 05, 2006
*
* Manages shader constants in the engine to be up to date with the current state.
*/

// Local includes
#include "ShaderManager.h"

// Engine includes
#include "Actor.h"
#include "ActorIncludes.h"

CShaderManager::SSemanticType CShaderManager::m_oSemantics[] = 
{
//	{ SEMANTIC,					TYPE,			UPDATE,				NAME									},
	{ SEM_DIFFUSE,				ST_COLOR,		UPDATE_PEROBJECT,	"Diffuse"								},
	{ SEM_SPECULAR,				ST_COLOR,		UPDATE_PEROBJECT,	"Specular"								},
	{ SEM_AMBIENT,				ST_COLOR,		UPDATE_PEROBJECT,	"Ambient"								},
	{ SEM_SPECULARPOW,			ST_SPECPOW,		UPDATE_PEROBJECT,	"SpecularPower"							},
	{ SEM_NORMAL,				ST_TEXTURE,		UPDATE_PEROBJECT,	"Normal"								},
	{ SEM_HEIGHT,				ST_TEXTURE,		UPDATE_PEROBJECT,	"Height"								},
	{ SEM_REFRACT,				ST_TEXTURE,		UPDATE_PEROBJECT,	"Refraction"							},
	{ SEM_OPACITY,				ST_TEXTURE,		UPDATE_PEROBJECT,	"Opacity"								},
	{ SEM_ENVIRON,				ST_TEXTURE,		UPDATE_PEROBJECT,	"Environment"							},
	{ SEM_ENVIRONNORM,			ST_TEXTURE,		UPDATE_PEROBJECT,	"EnvironmentNormal"						},
	{ SEM_FRESNEL,				ST_FLOAT4,		UPDATE_PEROBJECT,	"Fresnel"								},
	{ SEM_WORLD,				ST_MATRIX,		UPDATE_PEROBJECT,	"World"									},
	{ SEM_WORLDINV,				ST_MATRIX,		UPDATE_PEROBJECT,	"WorldInverse"							},
	{ SEM_WORLDT,				ST_MATRIX,		UPDATE_PEROBJECT,	"WorldTranspose"						},
	{ SEM_WORLDIT,				ST_MATRIX,		UPDATE_PEROBJECT,	"WorldInverseTranspose"					},
	{ SEM_PROJ,					ST_MATRIX,		UPDATE_PERFRAME,	"Projection"							},
	{ SEM_PROJINV,				ST_MATRIX,		UPDATE_PERFRAME,	"ProjectionInverse"						},
	{ SEM_PROJT,				ST_MATRIX,		UPDATE_PERFRAME,	"ProjectionTranspose"					},
	{ SEM_PROJIT,				ST_MATRIX,		UPDATE_PERFRAME,	"ProjectionInverseTranspose"			},
	{ SEM_VIEW,					ST_MATRIX,		UPDATE_PERFRAME,	"View"									},
	{ SEM_VIEWINV,				ST_MATRIX,		UPDATE_PERFRAME,	"ViewInverse"							},
	{ SEM_VIEWT,				ST_MATRIX,		UPDATE_PERFRAME,	"ViewTranspose"							},
	{ SEM_VIEWIT,				ST_MATRIX,		UPDATE_PERFRAME,	"ViewInverseTranspose"					},
	{ SEM_WORLDVIEW,			ST_MATRIX,		UPDATE_PEROBJECT,	"WorldView"								},
	{ SEM_WORLDVIEWINV,			ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewInverse"						},
	{ SEM_WORLDVIEWT,			ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewTranspose"					},
	{ SEM_WORLDVIEWIT,			ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewInverseTranspose"				},
	{ SEM_WORLDVIEWPROJ,		ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewProjection"					},
	{ SEM_WORLDVIEWPROJINV,		ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewProjectionInverse"			},
	{ SEM_WORLDVIEWPROJT,		ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewProjectionTranspose"			},
	{ SEM_WORLDVIEWPROJIT,		ST_MATRIX,		UPDATE_PEROBJECT,	"WorldViewProjectionInverseTranspose"	},
	{ SEM_VIEWPROJ,				ST_MATRIX,		UPDATE_PERFRAME,	"ViewProjection"						},
	{ SEM_VIEWPROJINV,			ST_MATRIX,		UPDATE_PERFRAME,	"ViewProjectionInverse"					},
	{ SEM_VIEWPROJT,			ST_MATRIX,		UPDATE_PERFRAME,	"ViewProjectionTranspose"				},
	{ SEM_VIEWPROJIT,			ST_MATRIX,		UPDATE_PERFRAME,	"ViewProjectionInverseTranspose"		},
	//{ SEM_WORLDARRAY,			ST_MATRIXARRAY,	UPDATE_PEROBJECT,	"WorldMatrixArray"						},
	{ SEM_POSITION,				ST_FLOAT3,		UPDATE_PEROBJECT,	"Position"								},
	{ SEM_ORIENTATION,			ST_FLOAT3,		UPDATE_PEROBJECT,	"Orientation"							},
	{ SEM_VIEWPORT,				ST_FLOAT2,		UPDATE_PERFRAME,	"Viewport"								},
	{ SEM_VIEWPORTWIDTH,		ST_FLOAT,		UPDATE_PERFRAME,	"ViewportWidth"							},
	{ SEM_VIEWPORTHEIGHT,		ST_FLOAT,		UPDATE_PERFRAME,	"ViewportHeight"						},
	{ SEM_TIME,					ST_FLOAT,		UPDATE_PERFRAME,	"Time"									},
	{ SEM_LASTTIME,				ST_FLOAT,		UPDATE_PERFRAME,	"LastTime"								},
	{ SEM_ELAPSEDTIME,			ST_FLOAT,		UPDATE_PERFRAME,	"ElapsedTime"							},
	{ SEM_CAMPOSITION,			ST_FLOAT3,		UPDATE_PERFRAME,	"CameraPosition"						},
	// Engine specifics.
	{ SEM_SKINNEDMESH,			ST_BOOL,		UPDATE_PEROBJECT,	"_isSkinned"							},
	{ SEM_ACTORCOLOR,			ST_COLOR,		UPDATE_PEROBJECT,	"ActorColor"							},
	{ SEM_LIGHTCOLOR,			ST_COLOR,		UPDATE_PEROBJECT,	"LightColor"							},
	{ SEM_LIGHTPOS,				ST_FLOAT3,		UPDATE_PEROBJECT,	"LightPosition"							},
	{ SEM_LIGHTMAP,				ST_TEXTURE,		UPDATE_PEROBJECT,	"LightMap"								},
	{ SEM_TEXPROJ,				ST_MATRIX,		UPDATE_PEROBJECT,	"TextureProjection"						},
	{ SEM_LIGHTRANGE,			ST_FLOAT3,		UPDATE_PEROBJECT,	"LightRange"							},
	{ SEM_LIGHTDIR,				ST_FLOAT3,		UPDATE_PEROBJECT,	"LightDirection"						},
	SHADER_END()
};

/**
* CShaderManager::updateParam
* @date Modified May 05, 2006
*/
void CShaderManager::updateParam(SShaderUpdate& oParam)
{
	CRenderSystem& oRS = CRenderSystem::getInstance();
	LPDIRECT3DDEVICE9 pDev = oRS.getRenderDevice().getD3DDevice();
	switch(oParam.oDetails.eSem)
	{
	// Color Values (Could be textures is some cases, ie diffuse, spec)
	case SEM_DIFFUSE:
	case SEM_SPECULAR:
	case SEM_AMBIENT:
	case SEM_SPECULARPOW:
	case SEM_EMISSIVE:
		if(oParam.oDetails.eType == ST_TEXTURE && m_mResources[oParam.oDetails.eSem])
			(*oParam.pParam) = ((CTexture2D*)m_mResources[oParam.oDetails.eSem])->getD3DTexture();
		break;
	// Texture Samplers
	case SEM_NORMAL:
	case SEM_HEIGHT:
	case SEM_REFRACT:
	case SEM_OPACITY:
	case SEM_ENVIRON:
	case SEM_ENVIRONNORM:
		if(m_mResources[oParam.oDetails.eSem])
			(*oParam.pParam) = ((CTexture2D*)m_mResources[oParam.oDetails.eSem])->getD3DTexture();
		break;
	// Vectors
	case SEM_FRESNEL:
		// TODO.
		break;
	// Base Matrices
	case SEM_WORLD:
		{
			D3DXMATRIX mWorld;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			(*oParam.pParam) = mWorld;
		}
		break;
	case SEM_WORLDINV:
		{
			D3DXMATRIX mWorld;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			D3DXMatrixInverse(&mWorld, NULL, &mWorld);
			(*oParam.pParam) = mWorld;
		}
		break;
	case SEM_WORLDT:
		{
			D3DXMATRIX mWorld;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			D3DXMatrixTranspose(&mWorld, &mWorld);
			(*oParam.pParam) = mWorld;
		}
		break;
	case SEM_WORLDIT:
		{
			D3DXMATRIX mWorld;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			D3DXMatrixInverse(&mWorld, NULL, &mWorld);
			D3DXMatrixTranspose(&mWorld, &mWorld);
			(*oParam.pParam) = mWorld;
		}
		break;
	case SEM_PROJ:
		{
			D3DXMATRIX mProj;
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			(*oParam.pParam) = mProj;
		}
		break;
	case SEM_PROJINV:
		{
			D3DXMATRIX mProj;
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixInverse(&mProj, NULL, &mProj);
			(*oParam.pParam) = mProj;
		}
		break;
	case SEM_PROJT:
		{
			D3DXMATRIX mProj;
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixTranspose(&mProj, &mProj);
			(*oParam.pParam) = mProj;
		}
		break;
	case SEM_PROJIT:
		{
			D3DXMATRIX mProj;
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixInverse(&mProj, NULL, &mProj);
			D3DXMatrixTranspose(&mProj, &mProj);
			(*oParam.pParam) = mProj;
		}
		break;  
	case SEM_VIEW:
		{
			D3DXMATRIX mView;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			(*oParam.pParam) = mView;
		}
		break;
	case SEM_VIEWINV:
		{
			D3DXMATRIX mView;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixInverse(&mView, NULL, &mView);
			(*oParam.pParam) = mView;
		}
		break;
	case SEM_VIEWT:
		{
			D3DXMATRIX mView;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixTranspose(&mView, &mView);
			(*oParam.pParam) = mView;
		}
		break;
	case SEM_VIEWIT:
		{
			D3DXMATRIX mView;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixInverse(&mView, NULL, &mView);
			D3DXMatrixTranspose(&mView, &mView);
			(*oParam.pParam) = mView;
		}
		break;
	// Matrix Concatenations
	case SEM_WORLDVIEW:
		{
			D3DXMATRIX mWorld, mView, mWV;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixMultiply(&mWV, &mWorld, &mView);
			(*oParam.pParam) = mWV;
		}
		break;
	case SEM_WORLDVIEWINV:
		{
			D3DXMATRIX mWorld, mView, mWV;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixMultiply(&mWV, &mWorld, &mView);
			D3DXMatrixInverse(&mWV, NULL, &mWV);
			(*oParam.pParam) = mWV;
		}
		break;
	case SEM_WORLDVIEWT:
		{
			D3DXMATRIX mWorld, mView, mWV;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixMultiply(&mWV, &mWorld, &mView);
			D3DXMatrixTranspose(&mWV, &mWV);
			(*oParam.pParam) = mWV;
		}
		break;
	case SEM_WORLDVIEWIT:
		{
			D3DXMATRIX mWorld, mView, mWV;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixMultiply(&mWV, &mWorld, &mView);
			D3DXMatrixInverse(&mWV, NULL, &mWV);
			D3DXMatrixTranspose(&mWV, &mWV);
			(*oParam.pParam) = mWV;
		}
		break;
	case SEM_WORLDVIEWPROJ:
		{
			D3DXMATRIX mWorld, mView, mProj, mWVP;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mWVP, &mWorld, D3DXMatrixMultiply(&mWVP, &mView, &mProj));
			(*oParam.pParam) = mWVP;
		}
		break;
	case SEM_WORLDVIEWPROJINV:
		{
			D3DXMATRIX mWorld, mView, mProj, mWVP;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mWVP, &mWorld, D3DXMatrixMultiply(&mWVP, &mView, &mProj));
			D3DXMatrixInverse(&mWVP, NULL, &mWVP);
			(*oParam.pParam) = mWVP;
		}
		break;
	case SEM_WORLDVIEWPROJT:
		{
			D3DXMATRIX mWorld, mView, mProj, mWVP;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mWVP, &mWorld, D3DXMatrixMultiply(&mWVP, &mView, &mProj));
			D3DXMatrixTranspose(&mWVP, &mWVP);
			(*oParam.pParam) = mWVP;
		}
		break; 
	case SEM_WORLDVIEWPROJIT:
		{
			D3DXMATRIX mWorld, mView, mProj, mWVP;
			pDev->GetTransform(D3DTS_WORLD, &mWorld);
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mWVP, &mWorld, D3DXMatrixMultiply(&mWVP, &mView, &mProj));
			D3DXMatrixInverse(&mWVP, NULL, &mWVP);
			D3DXMatrixTranspose(&mWVP, &mWVP);
			(*oParam.pParam) = mWVP;
		}
		break;
	case SEM_VIEWPROJ:
		{
			D3DXMATRIX mView, mProj, mVP;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mVP, &mView, &mProj);
			(*oParam.pParam) = mVP;
		}
		break;
	case SEM_VIEWPROJINV:
		{
			D3DXMATRIX mView, mProj, mVP;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mVP, &mView, &mProj);
			D3DXMatrixInverse(&mVP, NULL, &mVP);
			(*oParam.pParam) = mVP;
		}
		break;
	case SEM_VIEWPROJT:
		{
			D3DXMATRIX mView, mProj, mVP;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mVP, &mView, &mProj);
			D3DXMatrixTranspose(&mVP, &mVP);
			(*oParam.pParam) = mVP;
		}
		break;
	case SEM_VIEWPROJIT:
		{
			D3DXMATRIX mView, mProj, mVP;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			pDev->GetTransform(D3DTS_PROJECTION, &mProj);
			D3DXMatrixMultiply(&mVP, &mView, &mProj);
			D3DXMatrixInverse(&mVP, NULL, &mVP);
			D3DXMatrixTranspose(&mVP, &mVP);
			(*oParam.pParam) = mVP;
		}
		break;
	// Object info
	case SEM_POSITION:
		if(m_pActor)
			(*oParam.pParam) = m_pActor->getPosition();
		else
			(*oParam.pParam) = D3DXVECTOR3(0, 0, 0);
		break;
	case SEM_ORIENTATION:
		if(m_pActor)
			(*oParam.pParam) = m_pActor->getOrientation();
		else
			(*oParam.pParam) = D3DXVECTOR3(0, 0, 1);
		break;
	// Engine info
	case SEM_VIEWPORT:
		{
			D3DXVECTOR2 vView((float)oRS.getDisplayWidth(), (float)oRS.getDisplayHeight());
			(*oParam.pParam) = vView;
		}
		break;
	case SEM_VIEWPORTWIDTH:
		(*oParam.pParam) = (float)oRS.getDisplayWidth();
		break;
	case SEM_VIEWPORTHEIGHT:
		(*oParam.pParam) = (float)oRS.getDisplayHeight();
		break;
	case SEM_TIME:
		(*oParam.pParam) = CTimer::getInstance().getTime();
		break;
	case SEM_LASTTIME:
		(*oParam.pParam) = CTimer::getInstance().getLastTime();
		break;
	case SEM_ELAPSEDTIME:
		(*oParam.pParam) = CTimer::getInstance().getFrameTime();
		break;
	// Camera info
	case SEM_CAMPOSITION:
		{
			D3DXMATRIX mView;
			D3DXVECTOR3 vViewPos;
			pDev->GetTransform(D3DTS_VIEW, &mView);
			vViewPos.x = mView._41;
			vViewPos.y = mView._42;
			vViewPos.z = mView._43;
			(*oParam.pParam) = vViewPos;
		}
		break;
	// Engine specifics
	case SEM_SKINNEDMESH:
		if(m_pActor)
			(*oParam.pParam) = m_pActor->isSkinned();
		else
			(*oParam.pParam) = false;
		break;
	case SEM_ACTORCOLOR:
		if(m_pActor)
			(*oParam.pParam) = m_pActor->getColor();
		else
			(*oParam.pParam) = D3DCOLOR_ARGB(255, 0, 0, 0);
		break;
	case SEM_LIGHTCOLOR:
		if(m_pLight)
			(*oParam.pParam) = m_pLight->getColor();
		else
			(*oParam.pParam) = D3DCOLOR_ARGB(255, 255, 255, 255);
		break;
	case SEM_LIGHTPOS:
		if(m_pLight)
			(*oParam.pParam) = m_pLight->getPosition();
		else
			(*oParam.pParam) = D3DXVECTOR3(0, 0, 0);
		break;
	case SEM_LIGHTMAP:
		if(m_pLight && m_pLight->getType() == OBJ_LIGHT_SPOT && 
			oParam.oDetails.eType == ST_TEXTURE && m_mResources[SEM_LIGHTMAP])
			(*oParam.pParam) = ((CTexture2D*)m_mResources[SEM_LIGHTMAP])->getD3DTexture();
		break;
	case SEM_TEXPROJ:
		if(m_pLight && m_pLight->getType() == OBJ_LIGHT_SPOT)
			(*oParam.pParam) = ((CSpotLight*)m_pLight)->getProjectionMatrix();
		break;
	case SEM_LIGHTRANGE:
		if(m_pLight && m_pLight->getType() == OBJ_LIGHT_POINT)
		{
			float fRadius = ((CPointLight*)m_pLight)->getRadius();
			(*oParam.pParam) = D3DXVECTOR3(fRadius, fRadius, fRadius);
		}
		break;
	case SEM_LIGHTDIR:
		if(m_pLight && m_pLight->getType() == OBJ_LIGHT_SPOT)
		{
			(*oParam.pParam) = ((CSpotLight*)m_pLight)->getOrientation();
		}
		break;
	}
}

/**
* CShaderManager::parseShader
* @date Modified May 05, 2006
*/
void CShaderManager::parseShader(CShader& oShader)
{
	// Search for semantics in the shader
	SSemanticType* pType = m_oSemantics;
	SShaderUpdate oUpdate;
	while(pType->eSem != SEM_UNKNOWN && pType->eType != ST_UNKNOWN)
	{
		// Parse parameter
		CShaderParam& oParam = oShader.getParamBySemantic(pType->szName);
		if(oParam.isValid())
		{
			// Copy type info
			oUpdate.oDetails.eType = convertParamType(oParam);
			oUpdate.oDetails.szName = pType->szName;
			oUpdate.oDetails.eUpdate = pType->eUpdate;
			oUpdate.oDetails.eSem = pType->eSem;
			oUpdate.pParam = &oParam;

			// Add to internal list.

			m_mUpdates.insert(ParamEntry(pType->eUpdate, oUpdate));
		}
		
		++pType;
	}
	
	// TODO: Annotation parsing.
}

/**
* CShaderManager::convertParamType
* @date Modified May 05, 2006
*/
CShaderManager::ESemanticType CShaderManager::convertParamType(CShaderParam& oParam)
{
	switch(oParam.getClass())
	{
	case D3DXPC_SCALAR:
		switch(oParam.getType())
		{
		case D3DXPT_BOOL:
			return ST_BOOL;
			break;
		case D3DXPT_INT:
			return ST_INT;
			break;
		case D3DXPT_FLOAT:
			return ST_FLOAT;
			break;
		default:
			return ST_UNKNOWN;
			break;
		}
		break;
	case D3DXPC_VECTOR:
		switch(oParam.getElementCount())
		{
		case 1:
			return ST_FLOAT;
		case 2:
			return ST_FLOAT2;
		case 3:
			return ST_FLOAT3;
		case 4:
			return ST_FLOAT4;
		}
		break;
	case D3DXPC_MATRIX_COLUMNS:
	case D3DXPC_MATRIX_ROWS:
		if(oParam.getElementCount() == 1)
			return ST_MATRIX;
		else
			return ST_MATRIXARRAY;
		break;
	case D3DXPC_OBJECT:
		switch(oParam.getType())
		{
		case D3DXPT_TEXTURE:
		case D3DXPT_TEXTURE1D:
		case D3DXPT_TEXTURE2D:
		case D3DXPT_TEXTURE3D:
		case D3DXPT_TEXTURECUBE:
			return ST_TEXTURE;
			break;
		default:
			return ST_UNKNOWN;
			break;
		}
		break;
	default:
		return ST_UNKNOWN;
		break;
	}

	return ST_UNKNOWN;
}

/**
* CShaderManager::beginObjectRender
* @date Modified May 05, 2006
*/
void CShaderManager::beginObjectRender(void)
{
	ParamList::iterator itParams = m_mUpdates.find(UPDATE_PEROBJECT);
	if(itParams != m_mUpdates.end())
	{
		while(itParams->first == UPDATE_PEROBJECT)
		{
			// Make sure this 
			if(itParams->second.pParam->getParent() == m_pShader)
				updateParam(itParams->second);
			++itParams;
		}
	}
}

/**
* CShaderManager::beginFrame
* @date Modified May 05, 2006
*/
void CShaderManager::beginFrame(void)
{
	ParamList::iterator itParams = m_mUpdates.find(UPDATE_PERFRAME);
	if(itParams != m_mUpdates.end())
	{
		while(itParams->first == UPDATE_PERFRAME)
		{
			updateParam(itParams->second);
			++itParams;
		}
	}
}

/**
* CShaderManager::beginPass
* @date Modified May 05, 2006
*/
void CShaderManager::beginPass(void)
{
	ParamList::iterator itParams = m_mUpdates.find(UPDATE_PERPASS);
	if(itParams != m_mUpdates.end())
	{
		while(itParams->first == UPDATE_PERPASS)
		{
			updateParam(itParams->second);
			++itParams;
		}
	}
}

/**
* CShaderManager::setActiveShader
* @date Modified May 05, 2006
*/
void CShaderManager::setActiveShader(CShader* pShader)
{
	m_pShader = pShader;

	// Select correct shader by light type
	if(m_pShader)
	{
		m_pShader->setActiveTechByLight(m_pLight);
		Debug << "S - Active Tech: " << m_pShader->getTechniqueName() << (m_pLight ? " (VALID LIGHT) " : " (NULL LIGHT) ") << endl;
	}
}

/**
* CShaderManager::setActiveResource
* @date Modified May 05, 2006
*/
void CShaderManager::setActiveResource(ESemantic eSem, CBaseResource* pRes)
{
	m_mResources[eSem] = pRes;
}

/**
* CShaderManager::setCurrentLight
* @date Modified Jun 07, 2006
*/
void CShaderManager::setCurrentLight(CLight* pLight)
{
	m_pLight = pLight;

	// Select correct shader by light type
	if(m_pShader)
	{
		m_pShader->setActiveTechByLight(m_pLight);
		Debug << "L - Active Tech: " << m_pShader->getTechniqueName() << (m_pLight ? " (VALID LIGHT) " : " (NULL LIGHT) ") << endl;
	}
}