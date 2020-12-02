#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "skydome.h"

//=======================================================
//�X�J�C�h�[��������
//=======================================================
void CSkydome::Init()
{	
	m_Model = new CModel;
	m_Model->Load("asset\\model\\skydome.obj");

	m_Position = D3DXVECTOR3(1.0f, 0.0f, -2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "skyVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "skyPS.cso");

}
//=======================================================
//�X�J�C�h�[���I������
//=======================================================
void CSkydome::Uninit()
{
	//Create~�ō�����ϐ��̓����[�X����K�v������
	//�X�}�[�g�|�C���^�\�Ƃ������΂������������[�X����K�v�͂Ȃ��Ȃ�
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();
}
//=======================================================
//�X�J�C�h�[���X�V����
//=======================================================
void CSkydome::Update()
{
	
}
//=======================================================
//�X�J�C�h�[���`�揈��
//=======================================================
void CSkydome::Draw()
{
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//�g��k��
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //��]�̃}�g���b�N�X(�ݒ��y,x,z�̏���)
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);	//���W
	world = scale * rot * trans;	//3�̍���(���ʂ̊|���Z�ł͂Ȃ�)

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}