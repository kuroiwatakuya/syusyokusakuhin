//========================================
//�J����
//========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "Player.h"
#include "input.h"

//�J�����w�b�_�[�̑O��DirectX�֌W�̃w�b�_�[��include����
#include "Camera.h"

//========================================
//����������
//========================================
void CCamera::Init()
{
	//�|�W�V�����A�^�[�Q�b�g�̏�����
	m_Position = D3DXVECTOR3(0.0f, 3.0f, -1.0f);		//�J�����̈ʒu
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����_
}

//========================================
//�I������
//========================================
void CCamera::Uninit()
{

}

//========================================
//�X�V����
//========================================
void CCamera::Update()
{
	//�v���C���[���擾
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(1);
	m_Target = player->GetPosition();
	
/*
	//�g�b�v�r���[
	m_Position = m_Target + D3DXVECTOR3(0.0f, 10.0f, -10.0f);
*/

	//�T�[�h�p�[�\��
	D3DXVECTOR3 forward = player->GetForward();
	m_Position = m_Target - forward * 5.0f + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

}

//========================================
//�`�揈��
//========================================
void CCamera::Draw()
{
	//�r���[�}�g���N�X�ݒ�
	//�J�������ǂ̈ʒu�ɒu���̂�
	//D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&m_Matrix, 
									&m_Position, 
									&m_Target, 
									&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&m_Matrix);

	//�v���W�F�N�V�����}�g���N�X
	//���e�ݒ�(��p�A��ʂ̏c����)
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 
												1.0f /*��p*/,
												(float)SCREEN_WIDTH / SCREEN_HEIGHT/*�c����*/, 
												1.0f,				//���W�A��(1.0f��60�x���炢)
												1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
	CRenderer::SetCameraPosition(m_Position);
}
