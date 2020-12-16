#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, World); //�����ō��W�ϊ�
    worldNormal = normalize(worldNormal); //���K�����邽�߂̊֐�
    
    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = mul(In.Position, World);
    Out.Normal = worldNormal; //�����Ńs�N�Z���V�F�[�_�[�ɖ@�����𑗂��Ă���B
    Out.Diffuse = In.Diffuse;
    Out.TexCoord = In.TexCoord;
}