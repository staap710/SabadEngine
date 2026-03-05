

cbuffer ParticleBuffer : register(b0)
{
    Matrix wvp;
};

cbuffer ColorBuffer : register(b1)
{
    float4 color;
};

Texture2D textureMap : register(t0);
SamplerState samplerState : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.texcoord = input.texcoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 textureColor = textureMap.Sample(samplerState, input.texcoord);
    return textureColor * color;
}