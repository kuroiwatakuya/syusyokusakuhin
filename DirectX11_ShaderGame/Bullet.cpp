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
//���f���̃��[�h
//====================================================
void CBullet::Load()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\Bullet.obj");

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");
}
//====================================================
//���f���̃A�����[�h
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
//�e�̏���������
//=======================================
void CBullet::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
}
//=======================================
//�e�̏I������
//=======================================
void CBullet::Uninit()
{
	
}
//=======================================
//�e�̍X�V����
//=======================================
void CBullet::Update()
{
	//�e��
	m_Position.z += 0.1f;
	m_Rotation.x += 0.5f;
	
	//�e����ʊO�ɏo��������\�������
	if (m_Position.z > 10.0f)
	{
		SetDestoroy();
	}

	//�G�̏��擾
	CScene* scene = CManager::GetScene();
	std::vector<CEnemy*> enemyList = scene->GetGameObjects<CEnemy>(1);

	//�G�ƒe�̓����蔻��
	for (CEnemy* enemy : enemyList)
	{
		//�G�̍��W�擾
		D3DXVECTOR3 enemyPosition = enemy -> GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPosition;
		float length = D3DXVec3Length(&direction); //���������߂�֐�

		if ( length < 1.5f )
		{
			scene->AddGameObject<CEffect>(1)->SetPosition(m_Position);

			enemy->SetDestoroy();	//�G�������\��
			SetDestoroy();				//�e�������\��
			return;
		}
	}
}
//=======================================
//�e�̕`�揈��
//=======================================
void CBullet::Draw()
{
	//���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);									//�g��k��
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);	//��]�̃}�g���b�N�X(�ݒ��y,x,z�̏���)
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);					//���W
	world = scale * rot * trans;	//3�̍���

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}