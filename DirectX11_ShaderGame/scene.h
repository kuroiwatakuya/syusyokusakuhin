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

#define GAMEOBJECT_SIZE 3		//���X�g�̐�

//===========================================================
//�V�[���N���X
//===========================================================
class CScene
{
protected:
	std::list<CGameObject*> m_GameObject[GAMEOBJECT_SIZE];

public:
	CScene() {}
	virtual ~CScene() {}

	//==================================
	//�V�[���̏I������
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
	//�V�[���̍X�V����
	//==================================
	virtual void Update()
	{
		for (int i = 0; i < GAMEOBJECT_SIZE; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}

			//���X�g�\���̒��g�������Ȃ��Ⴂ���Ȃ�
			m_GameObject[i].remove_if
			(
				[](CGameObject* object)
			{
				return object->Destoroy(); /*�����܂ł����� ���X�g�ɓ����Ă�v�f���ƂɎ��s �֐������s���Ė߂�l��true�Ȃ烊�X�g��������B*/
			}
			);
		}
		//�����_��
	}

	//==================================
	//�V�[���̕`��
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
		//�����̃I�u�W�F�N�g���i�[����Ă���
		for (CGameObject* object : m_GameObject[Layer])
		{
			//typeid...�^�𒲂ׂ鉉�Z�q
			if (typeid(*object) == typeid(T)) //�^��T�Ȃ�(T�̒��ɂ��낢��i�[����Ă��)
			{
				return (T*)object;
			}
		}

		return NULL;
	}

	template<typename T>
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects; //STL�̔z��(�v�f��0�̔z�������)
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
