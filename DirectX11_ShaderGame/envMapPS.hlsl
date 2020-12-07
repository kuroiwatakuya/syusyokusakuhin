#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);      //���}�b�v�p�e�N�X�`��
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 normal = normalize(In.Normal); //In.Normal = �n���ꂽ���K�������@��
    
    //�����o�[�g�g�U�Ɩ�
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
    
    //�X�y�L����(�t�H��)
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    float3 Eyepos = eyev;
    eyev = normalize(eyev); //���K�������x�N�g��
    
    float3 refv = reflect(eyev, normal.xyz); //���˃x�N�g���擾
    refv = normalize(refv); //���K���������˃x�N�g��
    
    float specular = -dot(eyev, refv); //����
    specular = saturate(specular);
    specular = pow(specular, 30); //���˂̉s��
    
    outDiffuse.rgb += specular; //���˂̋���
   
    //���}�b�v
    float2 envtex;
    envtex.x = -refv.x * 0.3 + 0.5;
    envtex.y = -refv.y * 0.3 + 0.5;
    
    outDiffuse.rgb = g_TextureEnv.SampleBias(g_SamplerState, envtex,1.0);   //�𑜓x�������鏈��
}