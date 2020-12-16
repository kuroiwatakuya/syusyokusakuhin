#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game_object.h"
#include "Fade.h"
#include "title.h"
#include "input.h"
#include "game.h"
//#include "result.h"

static bool g_bEnd = false;
//bool CBullet::result = false;

//===================================================
//ゲームシーン初期化
//===================================================
void CGame::Init()
{
	//CBullet::result = false;

	CBullet::Load();

	//カメラ
	AddGameObject<CCamera>(0);

	AddGameObject<CSkydome>(1);
	AddGameObject<CPlayer>(1);

	//惑星配置
	//惑星を追加するときはここ
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));			//地球
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 10.0f));			//地球
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 0.0f));			//地球
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(5.0f, 0.0f, 5.0f));			//地球


	//アイテム配置
	//アイテムの追加するときはここ
	AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(-3.0f, 0.0f, 2.0f));
	AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(-1.0f, 0.0f, 3.0));
	AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(1.0f, 0.0f, 1.0f));

	AddGameObject<CField>(1);

	//2Dオブジェクト
	AddGameObject<CPolygon>(2);
}
void CGame::Uninit()
{
	CScene::Uninit();
}
//===================================================
//ゲームシーン更新処理
//===================================================
void CGame::Update()
{
	//シーンクラスのアップデート呼び出し
	CScene::Update();

	for (int i = 0; i < GAMEOBJECT_SIZE; i++)
	{
		for (CGameObject* object : m_GameObject[i])
		{
			object->Update();
		}

		//リスト構造の中身も消さなきゃいけない
		m_GameObject[i].remove_if
		(
			[](CGameObject* object)
		{
			return object->Destoroy(); /*ここまでが引数 リストに入ってる要素ごとに実行 関数を実行して戻り値がtrueならリストから消す。*/
		}
		);
	}
	//ラムダ式

	//リザルト遷移

	//if (!g_bEnd)
	//{
	//	if (CBullet::GetResult())
	//	{
	//		CFade::In();
	//		g_bEnd = true;
	//	}
	//}
	//else
	//{
	//	if (!CFade::IsFade())
	//	{
	//		g_bEnd = false;
	//		CFade::Out();
	//		CManager::SetScene<CResult>();
	//	}
	//}

}

void CGame::Draw()
{
	for (int i = 0; i < GAMEOBJECT_SIZE; i++)
	{
		for (CGameObject* object : m_GameObject[i])
		{
			object->Draw();
		}
	}
}