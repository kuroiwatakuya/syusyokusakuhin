#include "main.h"
#include "renderer.h"
#include "polygon.h"

//========================================
//初期化
//========================================
void CPolygon::Init()
{
	VERTEX_3D vertex[4];

	//左上の	テクスチャ
	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(900.0f, 50.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);




	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
																		CRenderer::GetDevice(), 
																		"asset/texture/HPBar.png", 
																		NULL,
																		NULL,
																		&m_Texture,
																		NULL);

	
	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

//========================================
//終了処理
//========================================
void CPolygon::Uninit()
{
	//Create~で作った変数はリリースする必要がある
	//スマートポインタ―とかいれればいちいちリリースする必要はなくなる
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();
}

//========================================
//更新処理
//========================================
void CPolygon::Update()
{

}

//========================================
//描画処理
//========================================
void CPolygon::Draw()
{
	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//拡大縮小
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //回転のマトリックス(設定はy,x,zの順番)
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);	//座標
	world = scale * rot * trans;	//3つの合成(普通の掛け算ではない)
	CRenderer::SetWorldMatrix(&world);

	//ライト無効
	LIGHT light;
	light.Enable = false;
	CRenderer::SetLight(light);

	//マトリクス設定
	//11では2Dポリゴン描画はいちいち設定しなければならない
	CRenderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);	//今回は頂点は4つなので4
}