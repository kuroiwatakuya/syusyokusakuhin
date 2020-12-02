#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "skydome.h"

//=======================================================
//スカイドーム初期化
//=======================================================
void CSkydome::Init()
{	
	m_Model = new CModel;
	m_Model->Load("asset\\model\\skydome.obj");

	m_Position = D3DXVECTOR3(1.0f, 0.0f, -2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "skyVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "skyPS.cso");

}
//=======================================================
//スカイドーム終了処理
//=======================================================
void CSkydome::Uninit()
{
	//Create~で作った変数はリリースする必要がある
	//スマートポインタ―とかいれればいちいちリリースする必要はなくなる
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();
}
//=======================================================
//スカイドーム更新処理
//=======================================================
void CSkydome::Update()
{
	
}
//=======================================================
//スカイドーム描画処理
//=======================================================
void CSkydome::Draw()
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

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}