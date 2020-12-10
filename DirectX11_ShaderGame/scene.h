#pragma once
#include <list>
#include <vector>
#include "main.h"
#include "game_object.h"

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Item.h"
#include "polygon.h"
#include "skydome.h"
#include "Camera.h"
#include "scene.h"
#include <typeinfo>

#define GAMEOBJECT_SIZE 3		//リストの数

//===========================================================
//シーンクラス
//===========================================================
class CScene
{
protected:
	std::list<CGameObject*> m_GameObject[GAMEOBJECT_SIZE];

public:
	CScene() {}
	virtual ~CScene() {}

	//==================================
	//シーンの終了処理
	//==================================
	virtual void Uninit()
	{
		for (int i = 0; i < GAMEOBJECT_SIZE; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();
		}

		CBullet::UnLoad();
	}

	//==================================
	//シーンの更新処理
	//==================================
	virtual void Update()
	{
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
	}

	//==================================
	//シーンの描画
	//==================================
	virtual void Draw()
	{
		for (int i = 0; i < GAMEOBJECT_SIZE; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}

	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameobject = new T();
		m_GameObject[Layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template<typename T>
	T* GetGameObject(int Layer)
	{
		//複数のオブジェクトが格納されている
		for (CGameObject* object : m_GameObject[Layer])
		{
			//typeid...型を調べる演算子
			if (typeid(*object) == typeid(T)) //型がTなら(Tの中にいろいろ格納されてるよ)
			{
				return (T*)object;
			}
		}

		return NULL;
	}

	template<typename T>
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects; //STLの配列(要素数0の配列もある)
		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}
};
