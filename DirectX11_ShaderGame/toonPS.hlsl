#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    float4 normal = normalize(In.Normal); //In.Normal = 渡された正規化した法線
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.a *= In.Diffuse.a;
    
    //ハーフランバート
    float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;
   
    if(light > 0.5)
    {
        light = 1.0;
    }
    else
    {
        light = 0.5;
    }
    
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    
    //輪郭
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    float3 Eyepos = eyev;
    eyev = normalize(eyev); //正規化視線ベクトル
    
    float d = dot(eyev, normal.xyz);
    
    if (d > -0.2)
    {
        outDiffuse.rgb *= 0.0;
    }
    
}