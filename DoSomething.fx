// simple shader that takes a position and color as input and outputs a yellow color
// defines the expected vertex data per vertex
struct VS_INPUT
{
    float3 position : POSITIONT;
   
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return float4(1, 1, 0, 1);
}