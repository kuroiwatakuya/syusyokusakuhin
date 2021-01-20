#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "Fade.h"
#include "title.h"
#include "game.h"
#include "Player.h"
#include "Field.h"
#include "polygon.h"

CScene* CManager::m_Scene = NULL;
D3DXVECTOR3 g_Position;


//========================================
//初期化
//========================================
void CManager::Init()
{
	CRenderer::Init();

	//RendererのInitのあとにポリゴン初期化を呼び出す
	CInput::Init();

	CInput::Init();

	CFade::Init();

	//タイトル呼び出し
	SetScene<CGame>();

	g_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================
//終了処理
//========================================
void CManager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	CFade::Uninit();
	CInput::Uninit();
	CRenderer::Uninit();
}

//========================================
//更新処理
//========================================
void CManager::Update()
{
	CInput::Update();
	CFade::Update();
	m_Scene->Update();

	if (CInput::GetKeyPress('U'))
	{
		g_Position.x += 0.005f;
		g_Position.y += 0.005f;
	}
	if (CInput::GetKeyPress('I'))
	{
		g_Position.x -= 0.005f;
		g_Position.y -= 0.005f;
	}
}

//========================================
//描画処理
//========================================
void CManager::Draw()
{
	CRenderer::Begin();
	//ライトを使いたいとき
	LIGHT light;
	light.Enable = true;

	light.Direction = D3DXVECTOR4(g_Position.x, g_Position.y, -1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	CRenderer::SetLight(light);

	m_Scene->Draw();
	CFade::Draw();

	//最後にend
	CRenderer::End();
}
