#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureNormal : register(t1);
Texture2D g_TextureRoughness : register(t2);
Texture2D g_TextureMetallic : register(t3);

SamplerState g_SamplerState;

#define DILECTRICF0 0.04  //�U�d�̂̔��˗�(F0)��4��
#define PI 3.1415926535      //�~����
#define INV_PI 0.31830988618

//===========================================================
//�g�U����
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
//�􉽌�����
//===========================================================
inline float V_SmithGGXCorrelated(float ndotl, float ndotv, float alpha)
{
    float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
    float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);
    return 0.5f / (lambdaV + lambdaL + 0.0001);
}
//===========================================================
//�@�����z
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
//�t���l������
//===========================================================
inline half3 F_Schlick(half3 f0, half cos)
{
    return f0 + (1 - f0) * pow(1 - cos, 5);
}
//===========================================================
//BRDF���g���ĐF�����߂�
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
    
    //�f�B�t���[�Y
    half diffuseTerm = Fd_Burley(ndotv, ndotl, ldoth, perceptualRoughness) * ndotl;
    half3 diffuse = albedo * (1 - reflectivity) * lightColor * diffuseTerm;
    
     //�X�y�L�����[
    float alpha = perceptualRoughness * perceptualRoughness;
    float V = V_SmithGGXCorrelated(ndotl, ndotv, alpha);
    float D = D_GGX(perceptualRoughness, ndotv, normal, halfDir);
    float3 F = F_Schlick(f0, ldoth);                                                        // �}�C�N���t�@�Z�b�g�x�[�X�̃X�y�L����BRDF�ł�cos��ldoth���g����
    float3 specular = V * D * F * ndotl * lightColor;
    
    //specular *= PI;
    specular = max(0, specular);

    half3 color = diffuse + specular;
    return half4(color, 1);
}
//=============================
//���C���֐�
//=============================
void main(in PS_IN In,out float4 outDiffuse : SV_Target)
{
     //�@���}�b�s���O
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    normalMap = (normalMap * 2.0) - 1.0; //-1����+1�ɕ␳����
    normalMap = (-normalMap.x * In.Tangent) + (normalMap.y * In.Binormal) + (normalMap.z * In.Normal);
    normalMap.w = 0.0f;
    normalMap = normalize(normalMap);
    
    //���t�l�X
    float4 roughnessMap = g_TextureRoughness.Sample(g_SamplerState, In.TexCoord);
    roughnessMap = (roughnessMap * 2.0) - 1.0;
    roughnessMap = (-roughnessMap.x * In.Tangent) + (roughnessMap.y * In.Binormal) + (roughnessMap.z * In.Normal);
    roughnessMap.w = 0.0f;
    roughnessMap = normalize(roughnessMap);
    
    //���^���b�N
    float4 metallMap = g_TextureMetallic.Sample(g_SamplerState, In.TexCoord);
    metallMap = (metallMap * 2.0) - 1.0;
    metallMap = (-metallMap.x * In.Tangent) + (metallMap.y * In.Binormal) + (metallMap.z * In.Normal);
    metallMap.w = 0.0f;
    metallMap = normalize(metallMap);
   
    //���W�ϊ����K�v
    float4 normal = normalMap;
    normal = normalize(normal);
    
    half3 albedo = g_Texture.Sample(g_SamplerState, In.TexCoord).rgb;        //�x�[�X�J���[
    outDiffuse.a = In.Diffuse.a;
    half metallic = metallMap;                                                                      //���^���b�N
    half perceptualRoughness = roughnessMap;                                            //���t�l�X
    
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
    
    //�����x�N�g���v�Z
    float3 ViewDir = In.WorldPosition.xyz - CameraPosition.xyz;
    ViewDir = normalize(ViewDir);
   
    outDiffuse = BRDF(albedo, metallic, perceptualRoughness, normal.xyz, ViewDir, Light.Direction.xyz, Light.Diffuse.rgb);
    
    //�֊s�`��
    //float d = dot(ViewDir, normal.xyz);
    
    //if (d > -0.3)
    //{
    //    outDiffuse.rgb *= 0.0;
    //}
}