

cbuffer PositionBuffer : register(b1)
{
    float3 LightPosition;
    float Padding1;
    float3 CameraPosition;
    float Padding2;
};

cbuffer MaterialBufferA : register(b2)
{
    float3 Ka;
    float Padding3;
    float3 Kd;
    float Padding4;
    float3 Ks;
    float Shininess;
};

struct PSIn
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEX;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    float3 L = normalize(LightPosition - input.WorldPos);
    float3 V = normalize(CameraPosition - input.WorldPos);
    float3 N = normalize(input.Normal);
    float3 R = normalize(reflect(input.WorldPos - LightPosition, N));

    float3 Diffuse = Kd * max(dot(L, N), 0);

    float RdotV = dot(R, V);
    float3 Specular = Ks * max(pow(RdotV, Shininess), 0);

    return float4(Ka + Diffuse + Specular, 1);

	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1
	//return float4(input.Normal*0.5+0.5, 1);
	
	// Debug shading #2: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);
}