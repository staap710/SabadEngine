//Simple Shader for Post-Processing Effects to a scene
cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    int param1;
    int param2;
    int param3;
};

Texture2D TextureMap0 : register(t0);
Texture2D TextureMap1 : register(t1);
Texture2D TextureMap2 : register(t2);
Texture2D TextureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 position : SV_position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0.0f; //none
    if (mode == 0)
    { 
        finalColor = TextureMap0.Sample(textureSampler, input.texCoord);
    }
    else if(mode==1) //Monochrome
    {
        float4 color = TextureMap0.Sample(textureSampler, input.texCoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    else if (mode == 2) //Invert/Negative
    {
        float4 color = TextureMap0.Sample(textureSampler, input.texCoord);
        finalColor = 1.0f-color;
    }
    else if (mode == 3) //mirror
    {
        float2 texCoord = input.texCoord;
        texCoord.x *= param0;
        texCoord.y *= param1; 
    }
    else if (mode == 4) //blur
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        finalColor=
        TextureMap0.Sample(textureSampler, float2(u, v))
        + TextureMap0.Sample(textureSampler, float2(u + param0, v))
        + TextureMap0.Sample(textureSampler, float2(u - param0, v))
        + TextureMap0.Sample(textureSampler, float2(u, v + param1))
        + TextureMap0.Sample(textureSampler, float2(u, v - param1));
        +TextureMap0.Sample(textureSampler, float2(u + param0, v + param1))
        + TextureMap0.Sample(textureSampler, float2(u - param0, v - param1))
        + TextureMap0.Sample(textureSampler, float2(u - param0, v + param1))
        + TextureMap0.Sample(textureSampler, float2(u - param0, v - param1));
        finalColor *= -0.12f;

    }
    else if (mode == 5) //combine2
        { 
        float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
        finalColor = (color0 + color1)*0.5;
        finalColor *= 0.12f;
    }
    else if (mode == 6) //motion blur
    {
        return 0;
    }
    else if (mode == 7) //chromatic aberration
    {
        float2 distortion = float2(param0, -param1);
        float4 redChannel = TextureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
        float4 greenChannel = TextureMap0.Sample(textureSampler, input.texCoord);
        float4 blueChannel = TextureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
        finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    }
    else if (mode == 8) //wave
    {
        float waveValue = input.texCoord.x * (3.141592f * param1);
        float2 texCoord = input.texCoord;
        texCoord.y += sin(waveValue) * (param0 / 100.0f);
    }
        return finalColor;
}
