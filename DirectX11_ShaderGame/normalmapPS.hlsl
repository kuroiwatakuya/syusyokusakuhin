#include "common.hlsl"

Texture2D g_Texture : register(t0); //�ʏ�e�N�X�`���p
Texture2D g_TextureNormal : register(t1); //�@���}�b�v�p
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In,out float4 outDiffuse : SV_Target)
{
    //�@���}�b�s���O
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    normalMap = (normalMap * 2.0) - 1.0; //-1����+1�ɕ␳����
    
    normalMap = (-normalMap.x * In.Tangent) + (normalMap.y * In.Binormal) + (normalMap.z * In.Normal);
    normalMap.w = 0.0f;
    normalMap = normalize(normalMap);
    
    //���W�ϊ����K�v
    float4 normal = normalMap;
    normal = normalize(normal);
    
    //�����o�[�g�g�U�Ɩ�
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
    
    //�X�y�L����(�t�H��)
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; //�J�����̎����x�N�g���̎擾
    float3 worlddir = eyev;
    eyev = normalize(eyev);
    
    float3 refv = reflect(Light.Direction.xyz, normal.xyz); //���˃x�N�g���擾
    refv = normalize(refv);
    
    float specular = -dot(eyev, refv); //����
    specular = saturate(specular);
    specular = pow(specular, 30); //���˂̉s��
    
    outDiffuse.rgb += specular; //���˂̋���
}