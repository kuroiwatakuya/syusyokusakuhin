#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);      //環境マップ用テクスチャ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 normal = normalize(In.Normal); //In.Normal = 渡された正規化した法線
    
    //ランバート拡散照明
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
    
    //スペキュラ(フォン)
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    float3 Eyepos = eyev;
    eyev = normalize(eyev); //正規化視線ベクトル
    
    float3 refv = reflect(eyev, normal.xyz); //反射ベクトル取得
    refv = normalize(refv); //正規化した反射ベクトル
    
    float specular = -dot(eyev, refv); //内積
    specular = saturate(specular);
    specular = pow(specular, 30); //反射の鋭さ
    
    outDiffuse.rgb += specular; //反射の強さ
   
    //環境マップ
    float2 envtex;
    envtex.x = -refv.x * 0.3 + 0.5;
    envtex.y = -refv.y * 0.3 + 0.5;
    
    outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envtex,1.0);   //解像度を下げる処理
}