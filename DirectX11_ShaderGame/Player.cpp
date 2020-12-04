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

//�J�����̈ړ��p�O���[�o���ϐ�
CCamera* g_camera;
D3DXVECTOR3  forward;

//========================================
//�v���C���[�̏���������
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
//�v���C���[�̏I������
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
//�v���C���[�̍X�V����
//========================================
void CPlayer::Update()
{
	forward = GetForward();

	m_Frame++;		//�A�j���[�V�����t���[��

	m_Model->Update("Idle", "Run", m_BlendRate, m_Frame);

	//�v���C���[�ړ�����2
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

	//�e�̔��ˏ���
	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		CScene* scene = CManager::GetScene();
		scene->AddGameObject<CBullet>(1)->SetPosition(m_Position);
	}

	//�A�C�e���ƃv���C���[�̓����蔻��
	CScene* scene = CManager::GetScene();
	std::vector<CItem*> itemList = scene->GetGameObjects<CItem>(1);

	for (CItem* Item : itemList)
	{
		D3DXVECTOR3 ItemPosition = Item->GetPosition();

		D3DXVECTOR3 direction = m_Position - ItemPosition;
		float length = D3DXVec3Length(&direction); //���������߂�֐�

		//�A�C�e������������
		if (length < 1.5f)	//�����̐��l�ς���Ɠ����蔻��̑傫���ς������
		{
			Item->SetDestoroy();
			return;
		}
	}
	
}

//========================================
//�v���C���[�̕`�揈��
//========================================
void CPlayer::Draw()
{
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//�g��k��
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y+3, m_Rotation.x, m_Rotation.z); //��]�̃}�g���b�N�X(�ݒ��y,x,z�̏���)

	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);	//���W
	world = scale * rot * trans;	//3�̍���(���ʂ̊|���Z�ł͂Ȃ�)

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}