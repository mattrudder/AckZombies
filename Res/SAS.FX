/**
* @file stdfxh.fxh
* @author Matt Rudder
* @date Created Apr 06, 2006
*
* Defines basic data used by all shaders written for the game engine. Enables
* engine support for DirectX Standard Annotations and Semantics.
*/

/**
* Core data members needed for SAS Compliance. Filled out by the engine at 
* runtime.
*
* @author Matt Rudder
* @date Created Apr 06, 2006
* @date Modified Apr 28, 2006
*/

/**
* Macros
*/

/**
* Defines
*/
static const int MAX_MATRICES = 26;

/**
* Structures
*/
struct SVertexIn
{
    float3 vPosition			: POSITION;
    float4 vTexCoord0			: TEXCOORD0;
    float3 vNormal				: NORMAL;
	float3 vBinormal			: BINORMAL;
	float3 vTangent				: TANGENT;
	float4 vBlendWeights		: BLENDWEIGHT;
    float4 vBlendIndices		: BLENDINDICES;
};

/**
* Current Light Data (One Per Pass)
*/
float3 vLightPos : LightPosition;
float3 vLightColor : LightColor;
float3 vLightRange : LightRange;
float3 vLightDirection : LightDirection;
float4x4 mLightProj : TextureProjection;

texture txLightProj : LightMap;
sampler sLightProj = sampler_state
{
    texture = <txLightProj>;
	AddressU  = WRAP;        
	AddressV  = WRAP;
	AddressW  = WRAP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

/**
* Standard Provided Data
*/

//! Default diffuse texture; Always provided to objects.
texture txDiffuse : Diffuse;
sampler sDiffuse = sampler_state 
{
    texture = <txDiffuse>;
    AddressU  = WRAP;        
    AddressV  = WRAP;
    AddressW  = WRAP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};
float4 vActorColor : ActorColor;
bool bSkinned : _isSkinned;
float4x3 mWorldMatrixArray[MAX_MATRICES] : WorldMatrixArray;
int CurNumBones : MeshBoneCount = 2;

/**
* Standard Matricies
*/
float4x4 mWorldViewProj : WorldViewProjection;
float4x4 mViewProj : ViewProjection;
float4x4 mTexProj : TextureProjection;

/**
* Functions
*/
float4x4 mWorldInv : WorldInverse;
float4x4 mWorld : World;
void calcLighting(in SVertexIn IN, out float3 vLightTan, out float3 vLightDist)
{
	// Get light direction, in model space.
	float3 L = mul(vLightPos, mWorldInv) - IN.vPosition;
	
	// Calc light distance and tangent.
	vLightDist = (vLightPos - mul(IN.vPosition, mWorld)) / vLightRange;
	vLightTan = float3(dot(L, IN.vTangent), dot(L, IN.vBinormal), dot(L, IN.vNormal));
}

float calcAttenuation(float3 vLightDist)
{
	float fAtten = 1 - saturate(dot(vLightDist,vLightDist));
	return fAtten*fAtten;
}

float4x4 mTextureProj = 
{
	0.5f,	0,		0,	0,
	0,		0.5f,	0,	0,
	0,		0,		0,	0,
	0.5f,	-0.5f,	0,	1
};

float4 calcTexProj(float3 vLightDir)
{
	float4x4 mAdjLightProj = mul(mLightProj, mTextureProj);
	return mul(vLightDir, mAdjLightProj);
}

float3 calcTexProjColor(float4 vTexProj, float3 vLightDir)
{
	float fCosDir = dot(normalize(vLightDir), vLightDirection);
	float fAmount = smoothstep(0.25f, 0.75f, fCosDir);
	return tex2Dproj(sLightProj, vTexProj).rgb * fAmount * vLightColor.rgb;
}

/**
* Alternate transformation for skinned meshes.
*
* @date Created May 06, 2006
* @param	vIn		The input vertex
* @param	nNumBones	The number of bones influencing a vertex
* @return The transformed vertex.
*/
float4 doSkinning(SVertexIn vIn, uniform int nNumBones)
{
    float3      Pos = 0.0f;
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(vIn.vBlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])vIn.vBlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    
    // calculate the pos using the "normal" weights 
    // and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < nNumBones-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(vIn.vPosition, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(vIn.vPosition, mWorldMatrixArray[IndexArray[nNumBones-1]]) * LastWeight);
	
    return mul(float4(Pos.xyz, 1.0f), mViewProj);
}
