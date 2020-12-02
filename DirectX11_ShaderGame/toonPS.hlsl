#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    float4 normal = normalize(In.Normal); //In.Normal = �n���ꂽ���K�������@��
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.a *= In.Diffuse.a;
    
    //�n�[�t�����o�[�g
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
    
    //�֊s
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    float3 Eyepos = eyev;
    eyev = normalize(eyev); //���K�������x�N�g��
    
    float d = dot(eyev, normal.xyz);
    
    if (d > -0.2)
    {
        outDiffuse.rgb *= 0.0;
    }
    
}