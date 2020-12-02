#include "common.hlsl"

Texture2D g_Texture : register(t0); //通常テクスチャ用
Texture2D g_TextureNormal : register(t1); //法線マップ用
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In,out float4 outDiffuse : SV_Target)
{
    //法線マッピング
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    normalMap = (normalMap * 2.0) - 1.0; //-1から+1に補正する
    
    normalMap = (-normalMap.x * In.Tangent) + (normalMap.y * In.Binormal) + (normalMap.z * In.Normal);
    normalMap.w = 0.0f;
    normalMap = normalize(normalMap);
    
    //座標変換が必要
    float4 normal = normalMap;
    normal = normalize(normal);
    
    //ランバート拡散照明
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
    
    //スペキュラ(フォン)
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; //カメラの視線ベクトルの取得
    float3 worlddir = eyev;
    eyev = normalize(eyev);
    
    float3 refv = reflect(Light.Direction.xyz, normal.xyz); //反射ベクトル取得
    refv = normalize(refv);
    
    float specular = -dot(eyev, refv); //内積
    specular = saturate(specular);
    specular = pow(specular, 30); //反射の鋭さ
    
    outDiffuse.rgb += specular; //反射の強さ
}