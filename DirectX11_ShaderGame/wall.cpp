#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "wall.h"

//=======================================================
//スカイドーム初期化
//=======================================================
void CWall::Init()
{
	m_Model = new CModel;
	m_Model->Load("asset\\model\\wall.obj");

	m_Position = D3DXVECTOR3(1.0f, 0.0f, -2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(8.0f, 5.0f, 1.0f);

	//アルベド読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ExposedAggregate2_1K_albedo.tif",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	//法線マップ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ExposedAggregate2_1K__normal.tif",
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	//ハイトマップ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_height.tif",
		NULL,
		NULL,
		&m_TextureHeight,
		NULL);
	assert(m_TextureHeight);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "parallaxocclusionVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "parallaxocclusionPS.cso");
}
//=======================================================
//スカイドーム終了処理
//=======================================================
void CWall::Uninit()
{
	//Create~で作った変数はリリースする必要がある
	//スマートポインタ―とかいれればいちいちリリースする必要はなくなる
	m_Model->Unload();
	delete m_Model;
	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();

	m_Texture->Release();
	m_TextureNormal->Release();
	m_TextureHeight->Release();
}
//=======================================================
//スカイドーム更新処理
//=======================================================
void CWall::Update()
{

}
//=======================================================
//スカイドーム描画処理
//=======================================================
void CWall::Draw()
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
	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureHeight);
	
	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
