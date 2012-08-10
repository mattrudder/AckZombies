// Vertex-shaded plastic using directional light

/************* TWEAKABLES **************/
float4x4 worldIT : WorldInverseTranspose;
float4x4 wvp : WorldViewProjection;
float4x4 world : World;
float4x4 viewInvTrans : ViewInverse;
texture diffuseTexture : Diffuse
<
	string ResourceName = "default_color.dds";
>;

float4 lightDir1 : Direction
<
	string UIName = "Light Direction 1";
	string Object = "DirectionalLight";
    string Space = "World";
> = {1.0f, -.5f, 1.0f, 0.0f};

float4 liteColor1 : Diffuse
<
    string UIName = "Light Color 1";
    string Object = "DirectionalLight";
> = { 1.0f, 1.0f, 1.0f, 1.0f };

float4 lightDir2 : Direction
<
	string UIName = "Light Direction 2";
	string Object = "DirectionalLight";
    string Space = "World";
> = {-1.0f, .5f, 1.0f, 1.0f};

float4 liteColor2 : Diffuse
<
    string UIName = "Light Color 2";
    string Object = "DirectionalLight";
> = { 0.0f, 0.0f, 0.0f, 1.0f };

float4 ambiColor : Ambient
<
    string UIName = "Ambient Light Color";
    string Space = "material";
> = {0.15f, 0.15f, 0.15f, 1.0f};

float4 surfColor : Diffuse
<
    string UIName = "Surface Color";
    string Space = "material";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float4 surfSpecular : Specular
<
	string UIName = "Surface Specular";
	string Space = "material";
> = {0.1f, 0.1f, 0.1f, 1.0f};


float specExpon : SpecularPower
<
    string UIWidget = "slider";
    float UIMin = 1.0;
    float UIMax = 128.0;
    float UIStep = 1.0;
    string UIName = "specular power";
> = 30.0;

/************* DATA STRUCTS **************/

/* data from application vertex buffer */
struct appdata {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
};

/* data passed from vertex shader to pixel shader */
struct vertexOutput {
    float4 HPosition	: POSITION;
    float4 TexCoord0	: TEXCOORD0;
    float4 diffCol	: COLOR0;
};

/*********** vertex shader ******/

vertexOutput plasticDVS(appdata IN,
    uniform float4x4 WorldViewProj,
    uniform float4x4 WorldIT,
    uniform float4x4 World,
    uniform float4x4 ViewInvTrans,
    uniform float4 ambiColor,
    uniform float4 surfColor,
    uniform float4 liteColor1,
    uniform float4 liteColor2,
    uniform float specExpon,
    uniform float3 DirFromLight1,
    uniform float3 DirFromLight2
) {
    vertexOutput OUT;
    float3 worldNormal = mul(IN.Normal, WorldIT).xyz;
    worldNormal = normalize(worldNormal);

    //build float4
    float4 tempPos;
    tempPos.xyz = IN.Position.xyz;
    tempPos.w   = 1.0;

    //compute worldspace position
    float3 worldSpacePos = mul(tempPos, World).xyz;
    float3 LightVec1 = normalize(-DirFromLight1);
    float3 LightVec2 = normalize(-DirFromLight2);
    float ldn1 = dot(LightVec1,worldNormal);
    float ldn2 = dot(LightVec2,worldNormal);
    float diffComp1 = max(0,ldn1);
    float diffComp2 = max(0,ldn2);
    float4 diffContrib = surfColor * ( (diffComp1 * liteColor1) + (diffComp2 * liteColor2) + ambiColor);

    OUT.TexCoord0 = IN.UV;

    float3 EyePos = ViewInvTrans[3].xyz;
    float3 vertToEye = normalize(EyePos - worldSpacePos);
    float3 halfAngle1 = normalize(vertToEye + LightVec1);
    float3 halfAngle2 = normalize(vertToEye + LightVec2);
    float hdn1 = pow(max(0,dot(halfAngle1,worldNormal)),specExpon);
    float hdn2 = pow(max(0,dot(halfAngle2,worldNormal)),specExpon);

    float4 specContrib = surfSpecular * ((hdn1 * liteColor1) + (hdn2 * liteColor2));
    diffContrib += specContrib;
    
    diffContrib = max(0, diffContrib);
	diffContrib = min(1, diffContrib);
    
	OUT.diffCol = diffContrib;
	OUT.diffCol.w = 1.0;
    
    // transform into homogeneous-clip space
    OUT.HPosition = mul(tempPos, WorldViewProj);
    return OUT;
}

technique diffuse
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 plasticDVS(wvp,worldIT,
					world,viewInvTrans,
					ambiColor,surfColor,liteColor1, liteColor2,
					specExpon,lightDir1, lightDir2);

        ColorOp[0]   = SelectArg1;
        ColorArg1[0] = Diffuse;
        ColorArg2[0] = Texture;
        AlphaOp[0]   = SelectArg1;
        AlphaArg1[0] = Diffuse;
        AlphaArg2[0] = Texture;
    }
}

technique textured
{
    pass p0 
    {		
		VertexShader = compile vs_1_1 plasticDVS(wvp,worldIT,
					world,viewInvTrans,
					ambiColor,surfColor,liteColor1, liteColor2,
					specExpon,lightDir1, lightDir2);

		Texture[0] = <diffuseTexture>;
  		MinFilter[0] = Linear;
  		MagFilter[0] = Linear;
  		MipFilter[0] = Linear;

        ColorOp[0]   = Modulate;
        ColorArg1[0] = Diffuse;
        ColorArg2[0] = Texture;
        AlphaOp[0]   = Modulate;
        AlphaArg1[0] = Diffuse;
        AlphaArg2[0] = Texture;
    }
}



/***************************** eof ***/
