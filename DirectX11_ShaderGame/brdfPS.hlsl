#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
Texture2D g_TextureRoughness : register(t2);
Texture2D g_TextureMetallic : register(t3);

SamplerState g_SamplerState;

#define DILECTRICF0 0.04  //誘電体の反射率(F0)は4％
#define PI 3.1415926535      //円周率
#define INV_PI 0.31830988618

//===========================================================
//拡散反射
//===========================================================
inline half Fd_Burley(half ndotv, half ndotl, half ldoth, half roughness)
{
    half fd90 =  0.5 * 2 * ldoth * ldoth * roughness;
    half lightScatter = (1 + (fd90 - 1) * pow(1 - ndotl, 5));
    half viewScatter = (1 + (fd90 - 1) * pow(1 - ndotv, 5));
    
    half diffuse = lightScatter * viewScatter; 
    diffuse /= PI;
    return diffuse;
}
//===========================================================
//幾何減衰項
//===========================================================
inline float V_SmithGGXCorrelated(float ndotl, float ndotv, float alpha)
{
    float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
    float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);
    return 0.5f / (lambdaV + lambdaL + 0.0001);
}
//===========================================================
//法線分布
//===========================================================
inline half D_GGX(half perceptualRoughness, half ndoth, half3 normal, half3 halfDir)
{
    half3 ncrossh = cross(normal, halfDir);
    half a = ndoth * perceptualRoughness;
    half k = perceptualRoughness / (dot(ncrossh, ncrossh) + a * a);
    half d = k * k  * INV_PI;
    return min(d, 65504.0h);
}

//===========================================================
//フレネル反射
//===========================================================
inline half3 F_Schlick(half3 f0, half cos)
{
    return f0 + (1 - f0) * pow(1 - cos, 5);
}
//===========================================================
//BRDFを使って色を求める
//===========================================================
inline half4 BRDF(half3 albedo, half metallic, half perceptualRoughness, float3 normal, half3 viewDir, float3 lightDir, float3 lightColor)
{
    float3 halfDir = normalize(lightDir + viewDir);
    half ndotv = abs(dot(normal, viewDir));
    float ndotl = max(0, dot(normal, lightDir));
    float ndoth = max(0, dot(normal, halfDir));
    half ldoth = max(0, dot(lightDir, halfDir));
    //half reflectivity = lerp(DILECTRICF0, 1, metallic);
    half reflectivity = max(DILECTRICF0, metallic);
    half3 f0 = lerp(DILECTRICF0, albedo, metallic);
    
    //ディフューズ
    half diffuseTerm = Fd_Burley(ndotv, ndotl, ldoth, perceptualRoughness) * ndotl;
    half3 diffuse = albedo * (1 - reflectivity) * lightColor * diffuseTerm;
    
     //スペキュラー
    float alpha = perceptualRoughness * perceptualRoughness;
    float V = V_SmithGGXCorrelated(ndotl, ndotv, alpha);
    float D = D_GGX(perceptualRoughness, ndotv, normal, halfDir);
    float3 F = F_Schlick(f0, ldoth);                                                        // マイクロファセットベースのスペキュラBRDFではcosはldothが使われる
    float3 specular = V * D * F * ndotl * lightColor;
    
    //specular *= PI;
    specular = max(0, specular);

    half3 color = diffuse + specular;
    return half4(color, 1);
}
//=============================
//メイン関数
//=============================
void main(in PS_IN In,out float4 outDiffuse : SV_Target)
{
     //法線マッピング
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    normalMap = (normalMap * 2.0) - 1.0; //-1から+1に補正する
    normalMap = (-normalMap.x * In.Tangent) + (normalMap.y * In.Binormal) + (normalMap.z * In.Normal);
    normalMap.w = 0.0f;
    normalMap = normalize(normalMap);
    
    //ラフネス
    float4 roughnessMap = g_TextureRoughness.Sample(g_SamplerState, In.TexCoord);
    roughnessMap = (roughnessMap * 2.0) - 1.0;
    roughnessMap = (-roughnessMap.x * In.Tangent) + (roughnessMap.y * In.Binormal) + (roughnessMap.z * In.Normal);
    roughnessMap.w = 0.0f;
    roughnessMap = normalize(roughnessMap);
    
    //メタリック
    float4 metallMap = g_TextureMetallic.Sample(g_SamplerState, In.TexCoord);
    metallMap = (metallMap * 2.0) - 1.0;
    metallMap = (-metallMap.x * In.Tangent) + (metallMap.y * In.Binormal) + (metallMap.z * In.Normal);
    metallMap.w = 0.0f;
    metallMap = normalize(metallMap);
   
    //座標変換が必要
    float4 normal = normalMap;
    normal = normalize(normal);
    
    half3 albedo = g_Texture.Sample(g_SamplerState, In.TexCoord).rgb;        //ベースカラー
    outDiffuse.a = In.Diffuse.a;
    half metallic = metallMap;                                                                      //メタリック
    half perceptualRoughness = roughnessMap;                                            //ラフネス
    
    //float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;
    
    //if (light > 0.5)
    //{
    //    light = 1.0;
    //}
    //else
    //{
    //    light = 0.5;
    //}
    
    //outDiffuse.rgb *= In.Diffuse.rgb * light;
    
    //視線ベクトル計算
    float3 ViewDir = In.WorldPosition.xyz - CameraPosition.xyz;
    ViewDir = normalize(ViewDir);
   
    outDiffuse = BRDF(albedo, metallic, perceptualRoughness, normal.xyz, ViewDir, Light.Direction.xyz, Light.Diffuse.rgb);
    
    //輪郭描画
    //float d = dot(ViewDir, normal.xyz);
    
    //if (d > -0.3)
    //{
    //    outDiffuse.rgb *= 0.0;
    //}
}