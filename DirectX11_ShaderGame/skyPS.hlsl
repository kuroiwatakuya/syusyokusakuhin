#include "common.hlsl"

//定数
#define PI                       3.14159265359
#define INNERRADIUS     10000
#define OUTERRADIUS    10250
#define KR                      0.0025
#define KM                      0.01
#define SPHERERAD        5125

static const float fsample = 2.0;
static const float3 threePrimaryColors = float3(0.68, 0.55, 0.44);      //空の色
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

float Scale(float fcos)
{
    float x = 1.0 - fcos;
    return fScaleDepth * exp(-0.00287 + x  * (0.459 + x * (3.83 + x * (-6.8 + x * 5.25))));
}

//=======================================
//座標の計算
//=======================================
float3 IntersectionPos(float3 dir,float3 a,float radius)
{
    float b = dot(a, dir);
    float c = dot(a, a) - radius * radius;
    float d = max(b * b - c, 0.0);
    
    return a + dir * (-b + sqrt(d));
}

//=======================================
//メイン関数
//=======================================
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float3 worldpos = In.WorldPosition * fouterRadius;
    worldpos = IntersectionPos(normalize(worldpos), float3(0.0, finnerRadius, 0.0), fouterRadius);
    
    float3 v3Camerapos = float3(0.0,finnerRadius,0.0);
    float3 v3LightDir = normalize(Light.Direction.xyz); 
    
    float3 v3Ray = worldpos - v3Camerapos;
    float fFar = length(v3Ray);
    v3Ray /= fFar;
    
    float3 v3Start =v3Camerapos;
    float fCameraHeight = length(v3Camerapos);
    float fStartAngle = dot(v3Ray, v3Start) / fCameraHeight;
    float fStartDepth = exp(fScaleOverScaleDepth * (finnerRadius - fCameraHeight));
    float fStartOffset = fStartDepth * Scale(fStartAngle);
    
    float fSampleLength = fFar / fsample;
    float fScaledLength = fSampleLength * fScale;
    float3 v3SampleRay = v3Ray * fSampleLength;
    float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
    
    float3 v3FrontColor = 0.0;
    for (int n = 0; n < int(fsample);n++)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (finnerRadius - fHeight));
        float fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth * (Scale(fLightAngle) - Scale(fCameraAngle)));
        float3 v3Attenuate = exp(-fScatter * (v3InvWave * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }
    
    float3 c0 = v3FrontColor * (v3InvWave * fKrESun);
    float3 c1 = v3FrontColor * fKmESun;
    float3 v3Direction = v3Camerapos - worldpos;

    float fcos = dot(v3LightDir, v3Direction) / length(v3Direction);
    float fcos2 = fcos * fcos;

    float rayleighPhase = 0.75 * (1.0 + fcos2);
    float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fcos2) / pow(1.0 + g2 - 2.0 * g * fcos, 1.5);
    
    outDiffuse = 1.0;
    outDiffuse.rgb = rayleighPhase * c0 + miePhase * c1;
}
