#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, World); //ここで座標変換
    worldNormal = normalize(worldNormal); //正規化するための関数
    
    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = mul(In.Position, World);
    Out.Normal = worldNormal; //ここでピクセルシェーダーに法線情報を送っている。
    Out.Diffuse = In.Diffuse;
    Out.TexCoord = In.TexCoord;
}