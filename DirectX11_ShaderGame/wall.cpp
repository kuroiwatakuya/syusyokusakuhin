#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "wall.h"

//=======================================================
//�X�J�C�h�[��������
//=======================================================
void CWall::Init()
{
	m_Model = new CModel;
	m_Model->Load("asset\\model\\wall.obj");

	m_Position = D3DXVECTOR3(1.0f, 0.0f, -2.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(8.0f, 5.0f, 1.0f);

	//�A���x�h�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ExposedAggregate2_1K_albedo.tif",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	//�@���}�b�v�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ExposedAggregate2_1K__normal.tif",
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	//�n�C�g�}�b�v�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_height.tif",
		NULL,
		NULL,
		&m_TextureHeight,
		NULL);
	assert(m_TextureHeight);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "parallaxocclusionVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "parallaxocclusionPS.cso");
}
//=======================================================
//�X�J�C�h�[���I������
//=======================================================
void CWall::Uninit()
{
	//Create~�ō�����ϐ��̓����[�X����K�v������
	//�X�}�[�g�|�C���^�\�Ƃ������΂������������[�X����K�v�͂Ȃ��Ȃ�
	m_Model->Unload();
	delete m_Model;
	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();

	m_Texture->Release();
	m_TextureNormal->Release();
	m_TextureHeight->Release();
}
//=======================================================
//�X�J�C�h�[���X�V����
//=======================================================
void CWall::Update()
{

}
//=======================================================
//�X�J�C�h�[���`�揈��
//=======================================================
void CWall::Draw()
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

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);		//������:2�Ԗڂ̃e�N�X�`���[�Ƃ��ēo�^���� ������:�z��ԍ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormal);
	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureHeight);
	
	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
