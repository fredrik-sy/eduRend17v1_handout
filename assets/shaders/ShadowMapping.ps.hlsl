
struct PSIn
{
    float4 Pos : SV_Position;
    float Depth : TEX;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float PS_main(PSIn input) : SV_Target
{
    return input.Depth;
}