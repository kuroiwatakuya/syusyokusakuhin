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

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}