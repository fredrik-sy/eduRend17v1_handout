
cbuffer MatrixBuffer : register(b0)
{
    matrix ModelToWorld;
    matrix WorldToView;
    matrix Projection;
};

struct VSIn
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEX;
};

struct PSIn
{
    float4 Pos : SV_Position;
    float Depth : TEX;
};

//-----------------------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------------------

PSIn VS_main(VSIn input)
{
    PSIn output = (PSIn) 0;
	
	// Model->View transformation
    matrix MV = mul(WorldToView, ModelToWorld);

	// Model->View->Projection (clip space) transformation
	// SV_Position expects the output position to be in clip space
    matrix MVP = mul(Projection, MV);
	
	// Perform transformations and send to output
    output.Pos = mul(MVP, float4(input.Pos, 1));
    output.Depth = output.Pos.z / output.Pos.w;
    return output;
}