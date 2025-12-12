// Simple Shader That Applies Post Processing Effects to a Scene

cbuffer PostProcessBuffer : register(b0)
{
    int mode; // Mode variable to switch between different effects
    float param0;
    float param1;
    float param2;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0);
    output.texCoord = input.texCoord;
    return output;
}

// --- CRT HELPER FUNCTIONS -----------------------------------------------------

float2 BarrelDistort(float2 uv, float amount)
{
    float2 p = uv * 2.0f - 1.0f;
    float r2 = dot(p, p);
    float k = amount * 0.5f;
    p *= (1.0f + k * r2);
    return saturate(p * 0.5f + 0.5f);
}

float Scanline(float y, float frequency)
{
    float s = sin(y * 3.14159265f * frequency);
    return 0.5f + 0.5f * s;
}

float Rand(float2 co)
{
    return frac(sin(dot(co, float2(12.9898, 78.233))) * 43758.5453);
}

// -----------------------------------------------------------------------------


float4 PS(VS_OUTPUT input) : SV_Target
{
    // Sample the texture at the given texture coordinates
    float4 finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    
    if (mode == 0) // None
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    else if (mode == 1) // Monochrome
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    else if (mode == 2) // Invert
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = 1.0f - color;
    }
    else if (mode == 3) // Mirror
    {
        float2 texCoord = input.texCoord;
        texCoord.x *= param0;
        texCoord.y *= param1;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    else if (mode == 4) // Blur
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        
        finalColor =
        textureMap0.Sample(textureSampler, float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + param0, v))
        + textureMap0.Sample(textureSampler, float2(u - param0, v))
        + textureMap0.Sample(textureSampler, float2(u, v + param1))
        + textureMap0.Sample(textureSampler, float2(u, v - param1))
        + textureMap0.Sample(textureSampler, float2(u + param0, v + param1))
        + textureMap0.Sample(textureSampler, float2(u + param0, v - param1))
        + textureMap0.Sample(textureSampler, float2(u - param0, v + param1))
        + textureMap0.Sample(textureSampler, float2(u - param0, v - param1));
        finalColor *= 0.12f;
    }
    else if (mode == 5) // Combine2
    {
        float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
        color1.a *= param0;
        finalColor = (color0 * (1.0f - color1.a)) + (color1 * color1.a);
    }
    else if (mode == 6) // Motion Blur
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        float dist = distance(input.texCoord, float2(0.5f, 0.5f));
        float weight = saturate(lerp(0.0f, 1.0f, (dist - 0.25f) / 0.25f));
        float p0 = param0 * weight;
        float p1 = param1 * weight;
        
        finalColor =
        textureMap0.Sample(textureSampler, float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + p0, v))
        + textureMap0.Sample(textureSampler, float2(u - p0, v))
        + textureMap0.Sample(textureSampler, float2(u, v + p1))
        + textureMap0.Sample(textureSampler, float2(u, v - p1))
        + textureMap0.Sample(textureSampler, float2(u + p0, v + p1))
        + textureMap0.Sample(textureSampler, float2(u + p0, v - p1))
        + textureMap0.Sample(textureSampler, float2(u - p0, v + p1))
        + textureMap0.Sample(textureSampler, float2(u - p0, v - p1));
        finalColor *= 0.12f;
    }
    else if (mode == 7) // Chromatic Aberration
    {
        float2 distortion = float2(param0, -param1);
        float4 redChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
        float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord);
        float4 blueChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
        finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    }
    else if (mode == 8) // Wave
    {
        float waveValue = input.texCoord.x * (3.141592f * param1);
        float2 texCoord = input.texCoord;
        texCoord.y += sin(waveValue) * param0;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    else if (mode == 9) // CRT effect
    {
        float2 uv = input.texCoord;

    // 1) Screen curvature (uses param1)
        float2 curvedUV = BarrelDistort(uv, param1);

    // 2) Chromatic aberration + color bleeding (uses param2)
        float chromaAmount = param2 * 0.01f;
        float2 centre = float2(0.5f, 0.5f);
        float2 dir = curvedUV - centre;
        float dist = length(dir);
        float2 dirNorm = (dist > 0.00001f) ? dir / dist : float2(0, 0);

        float2 redUV = curvedUV + dirNorm * (chromaAmount * dist);
        float2 blueUV = curvedUV - dirNorm * (chromaAmount * dist);

        float4 redSample = textureMap0.Sample(textureSampler, saturate(redUV));
        float4 greenSample = textureMap0.Sample(textureSampler, saturate(curvedUV));
        float4 blueSample = textureMap0.Sample(textureSampler, saturate(blueUV));

        float4 color = float4(redSample.r, greenSample.g, blueSample.b, 1.0f);

    // 3) Scanlines (param0 controls intensity)
        float scanFreq = lerp(300.0f, 900.0f, param0);
        float scan = Scanline(uv.y, scanFreq * 0.001f);
        float scanStrength = lerp(1.0f, scan, param0);
        color.rgb *= scanStrength;

    // 4) Vignette
        float vignette = 1.0f - smoothstep(0.35f, 0.85f, dist);
        color.rgb *= vignette;

    // 5) Contrast (param0 also controls this)
        float contrast = 1.0f + param0 * 0.25f;
        color.rgb = ((color.rgb - 0.5f) * contrast) + 0.5f;

    // 6) Noise (uses param2)
        float n = Rand(floor(uv * 1000.0f));
        float noiseAmount = param2 * 0.05f;
        color.rgb += (n - 0.5f) * noiseAmount;

        finalColor = saturate(color);
    }
    return finalColor;
}
