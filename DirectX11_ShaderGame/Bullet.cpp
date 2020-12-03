#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "model.h"
#include "Player.h"
#include "Bullet.h"
#include "Effect.h"

CModel* CBullet::m_Model;
ID3D11VertexShader* CBullet::m_VertexShader;
ID3D11PixelShader* CBullet::m_PixelShader;
ID3D11InputLayout* CBullet::m_VertexLayout;

//====================================================
//モデルのロード
//====================================================
void CBullet::Load()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\Bullet.obj");

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");
}
//====================================================
//モデルのアンロード
//====================================================
void CBullet::UnLoad()
{
	m_Model->Unload();
	delete m_Model;

	m_PixelShader->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
}

//=======================================
//弾の初期化処理
//=======================================
void CBullet::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
}
//=======================================
//弾の終了処理
//=======================================
void CBullet::Uninit()
{
	
}
//=======================================
//弾の更新処理
//=======================================
void CBullet::Update()
{
	//弾道
	m_Position.z += 0.1f;
	m_Rotation.x += 0.5f;
	
	//弾が画面外に出たら消す予約をする
	if (m_Position.z > 10.0f)
	{
		SetDestoroy();
	}

	//敵の情報取得
	CScene* scene = CManager::GetScene();
	std::vector<CEnemy*> enemyList = scene->GetGameObjects<CEnemy>(1);

	//敵と弾の当たり判定
	for (CEnemy* enemy : enemyList)
	{
		//敵の座標取得
		D3DXVECTOR3 enemyPosition = enemy -> GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPosition;
		float length = D3DXVec3Length(&direction); //長さを求める関数

		if ( length < 1.5f )
		{
			scene->AddGameObject<CEffect>(1)->SetPosition(m_Position);

			enemy->SetDestoroy();	//敵を消す予約
			SetDestoroy();				//弾を消す予約
			return;
		}
	}
}
//=======================================
//弾の描画処理
//=======================================
void CBullet::Draw()
{
	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);									//拡大縮小
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);	//回転のマトリックス(設定はy,x,zの順番)
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);					//座標
	world = scale * rot * trans;	//3つの合成

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}