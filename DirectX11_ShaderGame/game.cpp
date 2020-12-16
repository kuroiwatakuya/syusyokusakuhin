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
//�Q�[���V�[��������
//===================================================
void CGame::Init()
{
	//CBullet::result = false;

	CBullet::Load();

	//�J����
	AddGameObject<CCamera>(0);

	AddGameObject<CSkydome>(1);
	AddGameObject<CPlayer>(1);

	//�f���z�u
	//�f����ǉ�����Ƃ��͂���
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));			//�n��
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 10.0f));			//�n��
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 0.0f));			//�n��
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(5.0f, 0.0f, 5.0f));			//�n��


	//�A�C�e���z�u
	//�A�C�e���̒ǉ�����Ƃ��͂���
	AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(-3.0f, 0.0f, 2.0f));
	AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(-1.0f, 0.0f, 3.0));
	AddGameObject<CItem>(1)->SetPosition(D3DXVECTOR3(1.0f, 0.0f, 1.0f));

	AddGameObject<CField>(1);

	//2D�I�u�W�F�N�g
	AddGameObject<CPolygon>(2);
}
void CGame::Uninit()
{
	CScene::Uninit();
}
//===================================================
//�Q�[���V�[���X�V����
//===================================================
void CGame::Update()
{
	//�V�[���N���X�̃A�b�v�f�[�g�Ăяo��
	CScene::Update();

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

	//���U���g�J��

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