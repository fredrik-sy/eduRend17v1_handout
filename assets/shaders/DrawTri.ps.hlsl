Texture2D KaTexture : register(t0);
Texture2D KdTexture : register(t1);
Texture2D KsTexture : register(t2);
Texture2D dTexture : register(t3);
Texture2D bumpTexture : register(t4);
Texture2D ShadowTexture : register(t5);
sampler Sampler : register(s0);
SamplerComparisonState ComparisonSampler : register(s1);

cbuffer PositionBuffer : register(b0)
{
    float3 LightPosition;
    float Padding1;
    float3 CameraPosition;
    float Padding2;
};

cbuffer PhongBuffer : register(b1)
{
    float3 KaConstant;
    float Padding3;
    float3 KdConstant;
    float Padding4;
    float3 KsConstant;
    float Shininess;
};

struct PSIn
{
    float4 Pos : SV_Position;
    float4 LightPos : POSITION0;
    float3 WorldPos : POSITION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEX;
};

float3 SampleTexture(float3 Constant, Texture2D Tex, float2 TexCoord)
{
    float3 rgb = Tex.Sample(Sampler, TexCoord).rgb;
    return rgb.r == 0 && rgb.g == 0 && rgb.b == 0 ? Constant : Constant * rgb;
}

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    float3x3 TBN = transpose(float3x3(
        normalize(input.Tangent),
        normalize(input.Binormal),
        normalize(input.Normal)));
    
    float3 Ka = SampleTexture(KaConstant, KaTexture, input.TexCoord);
    float3 Kd = SampleTexture(KdConstant, KdTexture, input.TexCoord);
    float3 Ks = SampleTexture(KsConstant, KsTexture, input.TexCoord);
    float3 d = dTexture.Sample(Sampler, input.TexCoord).rgb;
    float3 Normal = bumpTexture.Sample(Sampler, input.TexCoord).rgb;
    
    if (Normal.r == 0 && Normal.g == 0 && Normal.b == 0)
    {
        Normal = input.Normal;
    }
    else
    {
        Normal = normalize(Normal * 2.0 - 1.0);
        Normal = mul(TBN, Normal);
    }
    
    float3 L = normalize(LightPosition - input.WorldPos);
    float3 V = normalize(CameraPosition - input.WorldPos);
    float3 N = normalize(Normal);
    float3 R = normalize(reflect(normalize(input.WorldPos - LightPosition), N));
    
    float LdotN = dot(L, N);
    float3 Diffuse = Kd * max(LdotN, 0);

    float RdotV = dot(R, V);
    float3 Specular = Ks * max(pow(RdotV, Shininess), 0);
    
    
    float2 ShadowTexCoord;
    ShadowTexCoord.x = 0.5f + (input.LightPos.x / input.LightPos.w * 0.5f);
    ShadowTexCoord.y = 0.5f - (input.LightPos.y / input.LightPos.w * 0.5f);
    float PixelDepth = input.LightPos.z / input.LightPos.w;
    
    if (saturate(ShadowTexCoord.x) == ShadowTexCoord.x &&
        saturate(ShadowTexCoord.y) == ShadowTexCoord.y &&
        PixelDepth > 0)
    {
        float Margin = acos(saturate(LdotN));
        float Epsilon = clamp(0.0005 / Margin, 0, 0.1);
        
        float Lighting = ShadowTexture.SampleCmpLevelZero(ComparisonSampler, ShadowTexCoord, PixelDepth - Epsilon);
        //float Lighting = ShadowTexture.Sample(Sampler, ShadowTexCoord).r;

        //if (Lighting < (PixelDepth - Epsilon))
        if (Lighting < PixelDepth)
        {
            float3 Shadow = (Lighting * (Ka + Diffuse + Specular)) + (Ka * (1.0f - Lighting));
            return float4(Shadow, 1.0f);
        }
    }
    
    return float4(Ka + Diffuse + Specular, 1);
}