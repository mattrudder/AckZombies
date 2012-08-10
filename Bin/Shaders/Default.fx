/**
* @file Default.fx
* @author Matt Rudder
* @date Created Apr 29, 2006
*
* Default material for models in the Acespace Engine.
*/ 

// Engine includes
#include <sas.fxh>

float4 MaterialDiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };

/**
* Standard Diffuse Vertex Shader - Spotlight
* @date Created Jun 06, 2006
*/
struct SVertexOut
{
    float4 vWorldPos	: POSITION;
    float4 vTexCoord0	: TEXCOORD0;
};
SVertexOut VS11_StandardTransform(SVertexIn IN, uniform int nNumBones)
{
    SVertexOut OUT;

    if(bSkinned)
	OUT.vWorldPos = doSkinning(IN, nNumBones);
    else
	OUT.vWorldPos = mul(float4(IN.vPosition.xyz, 1.0), mWorldViewProj);
    OUT.vTexCoord0 = IN.vTexCoord0;
    return OUT;
}

/**
* Standard Diffuse Vertex Shader - Spotlight
* @date Created Jun 06, 2006
*/
struct SVertexOutSpot
{
    float4 vWorldPos	: POSITION;
    float4 vTexCoord0	: TEXCOORD0;
    float3 vNormal		: TEXCOORD1;
    float3 vLightTan	: TEXCOORD2;
    float3 vLightDist	: TEXCOORD3;
    float3 vLightDir	: TEXCOORD4;
    float4 vTexProj		: TEXCOORD5;
  
};
SVertexOutSpot VS11_Transform_Spot(SVertexIn IN, uniform int nNumBones)
{
    SVertexOutSpot OUT;
    //if(bSkinned)
	//	OUT.vWorldPos = doSkinning(IN, nNumBones);
	//else
		OUT.vWorldPos = mul(IN.vPosition, mWorldViewProj);
	OUT.vTexCoord0 = IN.vTexCoord0;
	OUT.vTexCoord0 = IN.vTexCoord0;
	OUT.vNormal = IN.vNormal;
	OUT.vLightDir = mul(IN.vPosition, mWorld) - vLightPos;
	calcLighting(IN, OUT.vLightTan, OUT.vLightDist);
	OUT.vTexProj = calcTexProj(OUT.vLightDir);
	
    return OUT;
}

/**
* Standard Diffuse Vertex Shader - Point light
* @date Created Jun 06, 2006
*/
struct SVertexOutPoint
{
    float4 vWorldPos	: POSITION;
    float4 vTexCoord0	: TEXCOORD0;
    float3 vNormal		: TEXCOORD1;
    float3 vLightTan	: TEXCOORD2;
    float3 vLightDist	: TEXCOORD3;
};
SVertexOutPoint VS11_Transform_Point(SVertexIn IN, uniform int nNumBones)
{
    SVertexOutPoint OUT;
    //if(bSkinned)
	//	OUT.vWorldPos = doSkinning(IN, nNumBones);
	//else
		OUT.vWorldPos = mul(IN.vPosition, mWorldViewProj);
	OUT.vTexCoord0 = IN.vTexCoord0;
	OUT.vNormal = IN.vNormal;
	calcLighting(IN, OUT.vLightTan, OUT.vLightDist);
    return OUT;
}

/**
* Standard Diffuse Pixel Shader
* @date Created Apr 29, 2006
*/
float4 PS11_StandardDiffuse(SVertexOut IN): COLOR
{
  float4 vDiffuseTex = tex2D(sDiffuse, IN.vTexCoord0);
  return (vDiffuseTex) + vActorColor;
}

/**
* Standard Diffuse Pixel Shader - Spotlight
* @date Created Jun 06, 2006
*/
float4 PS20_Diffuse_Spot(SVertexOutSpot IN): COLOR
{
  float fAtten = calcAttenuation(IN.vLightDist);
  float4 vLight = tex2D(sDiffuse, IN.vTexCoord0);
  float3 vL = normalize(IN.vLightTan);
  float3 vNL = saturate(dot(vL,IN.vNormal));
  
  // Calc final light contribution.
  vLight.rgb *= (vLightColor * fAtten * vNL * calcTexProjColor(IN.vTexProj, IN.vLightDir));
  vLight += vActorColor;
  return vLight;
}

/**
* Standard Diffuse Pixel Shader - Point light
* @date Created Jun 06, 2006
*/
float4 PS20_Diffuse_Point(SVertexOutPoint IN): COLOR
{
  float fAtten = calcAttenuation(IN.vLightDist);
  float4 vBase = tex2D(sDiffuse, IN.vTexCoord0);
  float3 vL = normalize(IN.vLightTan);
  float3 vN = float3(0,1,0);
  float3 vNL = saturate(dot(vN,vL));
  
  // Calc final light contribution.
  float3 vDiffuse = vBase.rgb * vLightColor;
  float4 vFinal = float4(vDiffuse * fAtten, vBase.a);
  vFinal += vActorColor;
  return vFinal;
}

// Compile all possible vertex shaders
/*
VertexShader vsArray_Ambient[4] = 
{ 
	compile vs_1_1 VS11_StandardTransform(1),
	compile vs_1_1 VS11_StandardTransform(2),
	compile vs_1_1 VS11_StandardTransform(3),
	compile vs_1_1 VS11_StandardTransform(4)
};

VertexShader vsArray_Spot[4] = 
{ 
	compile vs_1_1 VS11_Transform_Spot(1),
	compile vs_1_1 VS11_Transform_Spot(2),
	compile vs_1_1 VS11_Transform_Spot(3),
	compile vs_1_1 VS11_Transform_Spot(4)
};

VertexShader vsArray_Point[4] = 
{ 
	compile vs_1_1 VS11_Transform_Point(1),
	compile vs_1_1 VS11_Transform_Point(2),
	compile vs_1_1 VS11_Transform_Point(3),
	compile vs_1_1 VS11_Transform_Point(4)
};
*/

/**
* Standard Diffuse Technique
* @date Created Apr 29, 2006
*/
technique Diffuse_Ambient <string LightType = "Ambient";>
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 VS11_StandardTransform(1);
		PixelShader  = compile ps_1_1 PS11_StandardDiffuse();
    }
}

technique Diffuse_Spot <string LightType = "Spot";>
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 VS11_Transform_Spot(1);
		PixelShader  = compile ps_2_0 PS20_Diffuse_Spot();
    }
}

technique Diffuse_Point <string LightType = "Point";>
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 VS11_Transform_Point(1);
		PixelShader  = compile ps_2_0 PS20_Diffuse_Point();
    }
}