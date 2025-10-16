//description: Standard shader with diffuse, specular, normal mapping, and environment mapping
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float materialShininess;
}

SamplerState textureSampler : register(s0);
Texture2D diffuseTexture : register(t0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    
    float4 worldPosition = mul(float4(input.position, 1.0f), world);
    output.dirToView = normalize(viewPosition - worldPosition.xyz);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    //update normal value
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    //update normal value
    float3 t = normalize(input.worldTangent);
    float3 b = normalize(cross(n, t));
    float3x3 tbnw = float3x3(t, b, n);
    float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
    float3 unpackedNormalMap = normalize(float3 ((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
    n = normalize(mul(unpackedNormalMap, tbnw));
    // Emissive
    float4 emissive = materialEmissive;
    //ambient
    float4 ambient = lightAmbient * materialAmbient;
    
    //Diffuse
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    //Specular
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialShininess);
    float4 specular = s * lightSpecular * materialSpecular;
    
    //colors
    float4 diffuseMapColor = diffuseMapColor.Sample(textureSampler, input.texCoord);
    float4 specMapColor = specMapColor.Sample(textureSampler, input.texCoord).r;
    float4 finalColor = (emissive + ambient + diffuse) * diffuseMapColor + (specular * specMapColor);
    
    return finalColor;
    
}