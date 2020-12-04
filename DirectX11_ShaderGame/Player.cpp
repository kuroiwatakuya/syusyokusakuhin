//========================================
//Player.cpp
//========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Camera.h"
#include "input.h"
#include "scene.h"
#include "Bullet.h"
#include "animation_model.h"
#include "Player.h"

//カメラの移動用グローバル変数
CCamera* g_camera;
D3DXVECTOR3  forward;

//========================================
//プレイヤーの初期化処理
//========================================
void CPlayer::Init()
{
	m_Model = new CAnimationModel;
	m_Model->Load("asset\\model\\Akai_Idle.fbx");
	m_Model->LoadAnimetion("asset\\model\\Akai_Idle.fbx", "Idle");
	m_Model->LoadAnimetion("asset\\model\\Akai_Run2.fbx", "Run");

	m_Position = D3DXVECTOR3(-0.5f, 0.0f, -3.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	m_Frame = 0;

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");
	
}

//========================================
//プレイヤーの終了処理
//========================================
void CPlayer::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_PixelShader->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
}

//========================================
//プレイヤーの更新処理
//========================================
void CPlayer::Update()
{
	forward = GetForward();

	m_Frame++;		//アニメーションフレーム

	m_Model->Update("Idle", "Run", m_BlendRate, m_Frame);

	//プレイヤー移動その2
	if (CInput::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.05f;
	}
	if (CInput::GetKeyPress('D'))
	{
		m_Rotation.y += 0.05f;
	}

	D3DXVECTOR3 forward = GetForward();

	if (CInput::GetKeyPress('W'))
	{
		m_Position += forward * 0.1f;

		m_BlendRate += 0.03f;
		m_Model->Update("Run", "Idle", m_BlendRate, m_Frame);

		D3DXQUATERNION quat;
		
	}
	
	if (CInput::GetKeyPress('S'))
	{
		
		m_Position -= forward * 0.1f;

		m_BlendRate += 0.03f;
		m_Model->Update("Run", "Idle", m_BlendRate, m_Frame);
		
		D3DXQUATERNION quat;
	}
	
	
	if (CInput::GetKeyPress('C'))
	{
		m_Position.y -= 0.5f;
	}
	if (CInput::GetKeyPress('V'))
	{
		m_Position.y += 0.5f;
	}

	//弾の発射処理
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		CScene* scene = CManager::GetScene();
		scene->AddGameObject<CBullet>(1)->SetPosition(m_Position);
	}

	//アイテムとプレイヤーの当たり判定
	CScene* scene = CManager::GetScene();
	std::vector<CItem*> itemList = scene->GetGameObjects<CItem>(1);

	for (CItem* Item : itemList)
	{
		D3DXVECTOR3 ItemPosition = Item->GetPosition();

		D3DXVECTOR3 direction = m_Position - ItemPosition;
		float length = D3DXVec3Length(&direction); //長さを求める関数

		//アイテムを消す処理
		if (length < 1.5f)	//ここの数値変えると当たり判定の大きさ変えられるよ
		{
			Item->SetDestoroy();
			return;
		}
	}
	
}

//========================================
//プレイヤーの描画処理
//========================================
void CPlayer::Draw()
{
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//拡大縮小
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y+3, m_Rotation.x, m_Rotation.z); //回転のマトリックス(設定はy,x,zの順番)

	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);	//座標
	world = scale * rot * trans;	//3つの合成(普通の掛け算ではない)

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}