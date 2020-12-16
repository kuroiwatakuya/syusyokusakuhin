#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "Enemy.h"

//==================================================
//エネミーの初期化
//==================================================
void CEnemy::Init()
{
	m_Model = new CModel;
	m_Model->Load("asset\\model\\Earth.obj");

	m_Position = D3DXVECTOR3(1.0f, 2.0f, -2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	//ハイトマップ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_albedo.tif",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	//法線マップ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_normal.tif",
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	//メタリック
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_metall.tif",
		NULL,
		NULL,
		&m_TextureMetall,
		NULL);
	assert(m_TextureMetall);

	//ラフネス
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_roughness.tif",
		NULL,
		NULL,
		&m_TextureRoughness,
		NULL);
	assert(m_TextureRoughness);
	
	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "brdfVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "brdfPS.cso");
}
//==================================================
//エネミーの終了処理
//==================================================
void CEnemy::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_PixelShader->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_Texture->Release();
	m_TextureNormal->Release();
	m_TextureMetall->Release();
	m_TextureRoughness->Release();
}
//==================================================
//エネミーの更新処理
//==================================================
void CEnemy::Update()
{
	m_Rotation.y += 0.001f;
}
//==================================================
//エネミーの描画処理
//==================================================
void CEnemy::Draw()
{
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

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);		//第一引数:2番目のテクスチャーとして登録する 第二引数:配列番号
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormal);
	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureMetall);
	CRenderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_TextureRoughness);

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}