#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"

CScene* CManager::m_Scene = NULL;

//========================================
//初期化
//========================================
void CManager::Init()
{
	CRenderer::Init();

	//RendererのInitのあとにポリゴン初期化を呼び出す
	CInput::Init();

	m_Scene = new CScene();
	m_Scene->Init();

}

//========================================
//終了処理
//========================================
void CManager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	CInput::Uninit();
	CRenderer::Uninit();
}

//========================================
//更新処理
//========================================
void CManager::Update()
{
	CInput::Update();
	m_Scene->Update();
}

//========================================
//描画処理
//========================================
void CManager::Draw()
{
	//先にRendererを書く
	//カメラ,3D,2Dの順番で描画していくこと
	CRenderer::Begin();
	
	//ライトを使いたいとき
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetLight(light);

	m_Scene->Draw();

	//最後にend
	CRenderer::End();
}
