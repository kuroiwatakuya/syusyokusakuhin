//========================================
//カメラ
//========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "Player.h"
#include "input.h"

//カメラヘッダーの前にDirectX関係のヘッダーをincludeする
#include "Camera.h"

//========================================
//初期化処理
//========================================
void CCamera::Init()
{
	//ポジション、ターゲットの初期化
	m_Position = D3DXVECTOR3(0.0f, 3.0f, -1.0f);		//カメラの位置
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//注視点
}

//========================================
//終了処理
//========================================
void CCamera::Uninit()
{

}

//========================================
//更新処理
//========================================
void CCamera::Update()
{
	//プレイヤー情報取得
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(1);
	m_Target = player->GetPosition();
	
/*
	//トップビュー
	m_Position = m_Target + D3DXVECTOR3(0.0f, 10.0f, -10.0f);
*/

	//サードパーソン
	D3DXVECTOR3 forward = player->GetForward();
	m_Position = m_Target - forward * 5.0f + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

}

//========================================
//描画処理
//========================================
void CCamera::Draw()
{
	//ビューマトリクス設定
	//カメラをどの位置に置くのか
	//D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&m_Matrix, 
									&m_Position, 
									&m_Target, 
									&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&m_Matrix);

	//プロジェクションマトリクス
	//投影設定(画角、画面の縦横比)
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 
												1.0f /*画角*/,
												(float)SCREEN_WIDTH / SCREEN_HEIGHT/*縦横比*/, 
												1.0f,				//ラジアン(1.0fで60度ぐらい)
												1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
	CRenderer::SetCameraPosition(m_Position);
}
