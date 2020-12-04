#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "Enemy.h"

//==================================================
//�G�l�~�[�̏�����
//==================================================
void CEnemy::Init()
{
	m_Model = new CModel;
	m_Model->Load("asset\\model\\Earth.obj");

	m_Position = D3DXVECTOR3(1.0f, 2.0f, -2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "brdfVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "brdfPS.cso");
}
//==================================================
//�G�l�~�[�̏I������
//==================================================
void CEnemy::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_PixelShader->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
}
//==================================================
//�G�l�~�[�̍X�V����
//==================================================
void CEnemy::Update()
{
	m_Rotation.y += 0.001f;
}
//==================================================
//�G�l�~�[�̕`�揈��
//==================================================
void CEnemy::Draw()
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