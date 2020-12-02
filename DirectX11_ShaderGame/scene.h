#pragma once
#include <list>
#include <vector>
#include "main.h"
#include "game_object.h"
#include "Camera.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Item.h"
#include "polygon.h"
#include "skydome.h"
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
	CScene(){}
	virtual ~CScene(){}

	//==================================
	//�V�[���̏�����
	//==================================
	virtual void Init()
	{
		CBullet::Load();

		//���C���[
		//�J����:0
		//3D�I�u�W�F�N�g:1
		//2D�I�u�W�F�N�g:2
		AddGameObject<CCamera>(0);
		AddGameObject<CField>(1);
		AddGameObject<CPlayer>(1);
		AddGameObject<CSkydome>(1);

		//�G�z�u
		//�G��ǉ�����Ƃ��͂���
		AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(-5.0f,0.0f,5.0f));
		AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 5.0f));
		AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(5.0f,0.0f,5.0f));

		//�A�C�e���z�u
		//�A�C�e���̒ǉ�����Ƃ��͂���
		AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(-3.0f, 0.0f, 2.0f));
		AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(-1.0f, 0.0f, 3.0));
		AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(1.0f, 0.0f, 1.0f));

		//2D�I�u�W�F�N�g
		AddGameObject<CPolygon>(2);
	}

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