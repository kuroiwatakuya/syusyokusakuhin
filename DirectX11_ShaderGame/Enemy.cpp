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


	//�n�C�g�}�b�v�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_albedo.tif",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	//�@���}�b�v�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_normal.tif",
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	//���^���b�N
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_metall.tif",
		NULL,
		NULL,
		&m_TextureMetall,
		NULL);
	assert(m_TextureMetall);

	//���t�l�X
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Stainless_roughness.tif",
		NULL,
		NULL,
		&m_TextureRoughness,
		NULL);
	assert(m_TextureRoughness);
	
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
	m_Texture->Release();
	m_TextureNormal->Release();
	m_TextureMetall->Release();
	m_TextureRoughness->Release();
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

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);		//������:2�Ԗڂ̃e�N�X�`���[�Ƃ��ēo�^���� ������:�z��ԍ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormal);
	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureMetall);
	CRenderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_TextureRoughness);

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}