
Texture2D Texture : register(t0);
sampler Sampler : register(s0);


cbuffer PositionBuffer : register(b1)
{
    float3 LightPosition;
    float Padding1;
    float3 CameraPosition;
    float Padding2;
};

cbuffer PhongBuffer : register(b2)
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
    
    float4 TextureColor = Texture.Sample(Sampler, input.TexCoord);
    
    if (TextureColor.x == 0 &&
        TextureColor.y == 0 &&
        TextureColor.z == 0 &&
        TextureColor.w == 0)
        return float4(Ka + Diffuse + Specular, 1);
    
    return float4(Ka + Diffuse + Specular, 1) * TextureColor;
}