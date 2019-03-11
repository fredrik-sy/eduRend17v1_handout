Texture2D KaTexture : register(t0);
Texture2D KdTexture : register(t1);
Texture2D KsTexture : register(t2);
Texture2D dTexture : register(t3);
Texture2D bumpTexture : register(t4);
Texture2D LightTexture : register(t5);
sampler Sampler : register(s0);

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
    
    float3 Diffuse = Kd * max(dot(L, N), 0);

    float RdotV = dot(R, V);
    float3 Specular = Ks * max(pow(RdotV, Shininess), 0);
    
    
    float Bias = 0.001f;
    float2 ProjectedTexCoord;
    ProjectedTexCoord.x = input.LightPos.x / input.LightPos.w / 2.0f + 0.5f;
    ProjectedTexCoord.y = -input.LightPos.y / input.LightPos.w / 2.0f + 0.5f;
    
    if (saturate(ProjectedTexCoord.x) == ProjectedTexCoord.x &&
        saturate(ProjectedTexCoord.y) == ProjectedTexCoord.y)
    {
        float DepthValue = LightTexture.Sample(Sampler, ProjectedTexCoord).r;
        
        float LightDepthValue = input.LightPos.z / input.LightPos.w - Bias;
        
        if (LightDepthValue < DepthValue)
        {
            float LightIntensity = saturate(dot(input.Normal, input.LightPos.xyz));

            if (LightIntensity > 0.0f)
            {
                Diffuse = saturate(Diffuse * LightIntensity);
            }
        }
    }

    return float4(Diffuse, 1);
    //return float4(Ka + Diffuse + Specular, 1);
}