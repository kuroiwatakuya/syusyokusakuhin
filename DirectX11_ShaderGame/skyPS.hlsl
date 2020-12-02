#include "common.hlsl"
#define PI                       3.141592
#define INNERRADIUS     100
#define OUTERRADIUS    250
#define KR                      0.0025
#define KM                      0.0025

static const float fsample = 2.0;
static const float3 threePrimaryColors = float3(0.68, 0.55, 0.44);
static const float3 v3InvWave = 1.0 / pow(threePrimaryColors, 4.0);

static const float fouterRadius = OUTERRADIUS;
static const float finnerRadius = INNERRADIUS;

static const float fESun = 20.0;
static const float fKrESun = KR * fESun;
static const float fKmESun = KM * fESun;

static const float fKr4PI = KR * 4.0 * PI;
static const float fKm4PI = KM * 4.0 * PI;

static const float fScale = 1.0 / (OUTERRADIUS - INNERRADIUS);
static const float fScaleDepth = 0.25;
static const float fScaleOverScaleDepth = fScale / fScaleDepth;

static const float g = -0.999f;
static const float g2 = g * g;

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

//===============================================
//大気散乱シミュレーション
//===============================================
float Scale(float fcos)
{
    float x = 1.0 - fcos;
    return fScaleOverScaleDepth * exp(-0.00287 * (0.459 + x * (3.83 + x * (-6.8 + x * 5.25))));
}
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 normal = normalize(In.Normal); //In.Normal = 渡された正規化した法線
    float3 worldpos = In.WorldPosition;
    //ランバート拡散照明
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
    
    //スペキュラ(フォン)
    float3 v3Camerapos = CameraPosition;
    v3Camerapos.y = finnerRadius;
    
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    float3 Eyepos = eyev;
    eyev = normalize(eyev); //正規化視線ベクトル
    
    float fFar = length(eyev);
    eyev /= fFar;
    
    float3 v3Start = CameraPosition;
    float fCameraHeight = length(CameraPosition);
    float fStartAngle = dot(eyev, v3Start) / fCameraHeight;
    float fStartDepth = exp(fScaleOverScaleDepth * (finnerRadius - fCameraHeight));
    float fStartOffset = fStartDepth * Scale(fStartAngle);
    
    float fSampleLength = fFar / fsample;
    float fScaledLength = fSampleLength * fScale;
    float3 v3SampleRay = eyev * fSampleLength;
    float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
    
    float3 v3FrontColor = 0.0;
    
    for (int n = 0; n < int(fsample);n++)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (finnerRadius - fHeight));
        float fLightAngle = dot(light, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(eyev, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth * (Scale(fLightAngle) - Scale(fCameraAngle)));
        float3 v3Attenuate = exp(-fScatter * (v3InvWave * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }
    
    float3 c0 = v3FrontColor * (v3InvWave * fKrESun);
    float3 c1 = v3FrontColor * fKmESun;
    float3 v3Direction = v3Camerapos - worldpos;

    float fcos = dot(light, v3Direction) / length(v3Direction);
    float fcos2 = fcos * fcos;

    float rayleighPhase = 0.75 * (1.0 + fcos2);
    float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fcos2) / pow(1.0 + g2 - 2.0 * g * fcos, 1.5);

    float4 col = 1.0;
    outDiffuse.rgb = rayleighPhase * c0 + miePhase * c1;
}
