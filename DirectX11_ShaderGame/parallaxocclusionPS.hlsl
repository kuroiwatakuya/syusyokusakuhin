//視差遮蔽マッピングシェーダー
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
Texture2D g_TextureHeight : register(t2);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float HeightScale = 0.3;
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev); //視線ベクトル
    
    float3 RayPos = In.WorldPosition.xyz;
    float RayHeight = 0.0;
    float ObjHeight = -HeightScale;
    float2 uv = { 0, 0 };
    
    const int HeightSamples = 32;
    const float HeightPerSample = 1.0 / HeightSamples;
    float RayScale = (-HeightScale / eyev.y);
    float3 RayStep = eyev * RayScale * HeightPerSample;
    
    for (int i = 0; i < HeightSamples && ObjHeight < RayHeight; ++i)
    {
        RayPos += RayStep;
        uv = RayPos.xz;
        
        ObjHeight = g_TextureHeight.Sample(g_SamplerState, uv).r; //高さマップサンプリング
        ObjHeight = ObjHeight * HeightScale - HeightScale;
        RayHeight = RayPos.y;
    }
   
    //線形補間で滑らかにする
    float2 nextObjPoint = uv;
    float2 prevObjPoint = uv - RayStep.xz;
    
    float NextHeight = ObjHeight;
    float PrevHeight = g_TextureHeight.Sample(g_SamplerState, prevObjPoint).r * HeightScale - HeightScale;
    NextHeight -= RayHeight;
    PrevHeight -= RayHeight - RayStep.y;
    
    float Weight = NextHeight / (NextHeight - PrevHeight);
    uv = lerp(nextObjPoint, prevObjPoint, Weight);
    
    //2分岐探索で滑らかにする
    const int Relief = 5;
    for (int ray = 0; ray < Relief; ++ray)
    {
        RayStep /= 2;
        ObjHeight = g_TextureHeight.Sample(g_SamplerState, uv).r * HeightScale - HeightScale;
        RayHeight = RayPos.y;
    
        if (RayHeight < ObjHeight)
        {
            RayPos -= RayStep;
        }
        else
        {
            RayPos += RayStep;
        }
    }
    
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, uv); //法線マップサンプリング
    normalMap = (normalMap * 2.0) - 1.0; //-1から+1に補正する
    
    normalMap = (-normalMap.x * In.Tangent) + (-normalMap.y * In.Binormal) + (normalMap.z * In.Normal);
    normalMap.w = 0.0f;
    normalMap = normalize(normalMap);
    
    //テクスチャのサンプリング
    outDiffuse = g_Texture.Sample(g_SamplerState, uv);
    
    //座標変換が必要
    float4 normal = normalMap;
    normal = normalize(normal);
    
    //ランバート拡散照明
    float light = dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light); //ライトベクトル
    
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
}