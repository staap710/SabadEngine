//description: Standard shader with diffuse, specular, normal mapping, and environment mapping
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 tangent : TANGENT;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return 1.0f; // Placeholder: always output red
}