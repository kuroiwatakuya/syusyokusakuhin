cbuffer WorldBuffer:register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}
cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
}
cbuffer ParameterBuffer : register(b6)
{
    float4 Parameter;
}

//���_�V�F�[�_
struct VS_IN
{
	float4 Position			:POSITION0;
	float4 Normal				:NORMAL0;
	float4 Diffuse				:COLOR0;
	float2 TexCoord			:TEXCOORD0;
    float4 Tangent : TANGENT0;
    float4 Binormal : BINORMAL0;
};

//�s�N�Z���V�F�[�_
struct PS_IN
{
	float4 Position			:SV_POSITION;
	float4 WorldPosition	:POSITION0;
	float4 Normal				:NORMAL0;
	float4 Diffuse				:COLOR0;
	float2 TexCoord			:TEXCOORD0;
    float4 Tangent           : TANGENT0;
    float4 Binormal          : BINORMAL0;
};

struct VERTEX
{
    float4 Position : SV_POSITION;
    half2 UV : TEXCOORD0;
    float3 WorldPos : TEXCOORD1;
    float4 WorldNormal : TEXCOORD2;
    half3 ViewDir : TEXCOORD3;
};

struct DATA
{
    float4 vertex : POSITION;
    half2 texcoord : TEXCOORD0;
    float4 normal : NORMAL;
    float Dammy;
};


//�}�e���A���o�b�t�@
//�\���̂̃T�C�Y�����܂��Ă���(16�o�C�g�̔{������Ȃ��Ƃ����Ƃ�Ȃ�)
struct MATERIAL
{
    float4 Ambirnt;
    float4 Diffuse;
    float4 Specular;
    float4 Emisson;
    float Shininess;
    float3 Dummy;   //16�o�C�g���E�p
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

struct LIGHT
{
    bool Enable;
    bool3 Dummy;        //16�o�C�g���E�p
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}