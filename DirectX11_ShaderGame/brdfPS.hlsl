#include "common.hlsl"

float3 Color = (0.3, 0.3, 0.3, 0);
Texture2D g_Texture;
SamplerState g_SamplerState;

float Metallic;
float roughness;

#define DILECTRICF0 0.04   //誘電体の反射率(F0)は4％
#define PI 3.1415926535      //円周率
#define INV_PI 0.31830988618
#define ROUGHNESS 0.5
#define METALLIC 0.0

//正直サンプルを見てやっただけなのでしっかり理解できてはないです
//これから理解していきます

//===========================================================
//拡散反射
//===========================================================
half Fd_Burley(half ndotv, half ndotl, half ldoth, half roughness)
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
float V_SmithGGXCorrelated(float ndotl, float ndotv, float alpha)
{
    float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
    float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);
    return 0.5f / (lambdaV + lambdaL + 0.0001);
}
//===========================================================
//法線分布
//===========================================================
half D_GGX(half perceptualRoughness, half ndoth, half3 normal, half3 halfDir)
{
    half3 ncrossh = cross(normal, halfDir);
    half a = ndoth * perceptualRoughness;
    half k = perceptualRoughness / (dot(ncrossh, ncrossh) + a * a);
    half d = k * k * INV_PI;
    return min(d, 65504.0h);
}

//===========================================================
//フレネル反射
//===========================================================
half3 F_Schlick(half3 f0, half cos)
{
    return f0 + (1 - f0) * pow(1 - cos, 5);
}
//===========================================================
//BRDFを使って色を求める
//===========================================================
half4 BRDF(half3 albedo, half metallic, half perceptualRoughness, float3 normal, float3 viewDir, float3 lightDir, float3 lightColor)
{
    float3 halfDir = normalize(lightDir + viewDir);
    half ndotv = abs(dot(normal, viewDir));
    float ndotl = max(0, dot(normal, lightDir));
    float ndoth = max(0, dot(normal, halfDir));
    half ldoth = max(0, dot(lightDir, halfDir));
    half reflectivity = lerp(DILECTRICF0, 1, metallic);
    half3 f0 = lerp(DILECTRICF0, albedo, metallic);
    
    //ディフューズ
    half diffuseTerm = Fd_Burley(ndotv, ndotl, ldoth, perceptualRoughness) * ndotl;
    half3 diffuse = albedo * (1 - reflectivity) * lightColor * diffuseTerm;
    
     //スペキュラー
    float alpha = perceptualRoughness * perceptualRoughness;
    float V = V_SmithGGXCorrelated(ndotl, ndotv, alpha);
    float D = D_GGX(perceptualRoughness, ndotv, normal, halfDir);
    float3 F = F_Schlick(f0, ldoth); // マイクロファセットベースのスペキュラBRDFではcosはldothが使われる
    float3 specular = V * D * F * ndotl * lightColor;
    
    //specular *= PI;
    specular = max(0, specular);

    half3 color = diffuse + specular;
    return half4(color, 1);
}

VERTEX vertex(PS_IN d)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    VERTEX o = (VERTEX) 0;
    float4 vt = d.Position;
    o.Position = mul(World, d.Position);
    o.UV = g_Texture.Sample(g_SamplerState, d.TexCoord);
    o.WorldPos = d.WorldPosition;
    o.WorldNormal = d.Normal;
    o.ViewDir = o.WorldPos - CameraPosition.xyz;
    
    //間接光の拡散反射取得
    //Material.Ambirnt.rgb = 
    return o;
}

void main(in VERTEX In,out float4 outDiffuse : SV_Target)
{
    half3 albedo = g_Texture.Sample(g_SamplerState,In.UV).rgb * Color.rgb;
    half metallic = METALLIC;
    half perceptualRoughness = ROUGHNESS;

    In.WorldNormal = normalize(In.WorldNormal);
    In.ViewDir = normalize(In.ViewDir);
   
    //outDiffuse = BRDF(albedo, metallic, perceptualRoughness, In.WorldNormal, In.ViewDir, Light.Direction.xyz, lightcolor0.rgb);
    outDiffuse = BRDF(albedo, metallic, perceptualRoughness,In.WorldNormal.xyz, In.ViewDir, Light.Direction.xyz, Light.Diffuse.rgb);
}