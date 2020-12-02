#include "common.hlsl"

Texture2D g_Texture : register(t0);
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
     
    //float3 refv = reflect(Light.Direction.xyz, normal.xyz); //���˃x�N�g���擾
    //refv = normalize(refv);                     //���K���������˃x�N�g��
    
    //float specular = -dot(eyev, refv);  //����
    //specular = saturate(specular);
    //specular = pow(specular, 30);       //���˂̉s��
    
    //outDiffuse.rgb += specular;      //���˂̋���
    
    //�������C�g
    float rim = 1.0f + dot(eyev, normal.xyz);
    rim = pow(rim, 2) * 0.5f;
    outDiffuse.rgb += rim;
}