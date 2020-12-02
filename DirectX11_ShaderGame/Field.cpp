#include "main.h"
#include "renderer.h"
#include "Field.h"

//========================================
//������
//========================================
void CField::Init()
{
	VERTEX_3D vertex[4];

	//3D��ԏ�̍��W���`����
	vertex[0].Position = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//�@�������͏�Ɍ�������̂�y��1������
	vertex[0].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[0].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[1].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(70.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[2].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 70.0f);

	vertex[3].Position = D3DXVECTOR3(100.0f, 0.0f, -100.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[3].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(70.0f, 70.0f);

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_albedo.tif",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//�e�N�X�`�����ǂݍ��܂�Ȃ������ꍇ�G���[���o������
	assert(m_Texture);

	//�@���}�b�v�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_normal.tif",
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

	m_Position		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "parallaxocclusionVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "parallaxocclusionPS.cso");
}

//========================================
//�I������
//========================================
void CField::Uninit()
{
	//�e�N�X�`���͐���Ă�Ǝv���܂�
	//���������ł�
	//Create~�ō�����ϐ��̓����[�X����K�v������
	//�X�}�[�g�|�C���^�\�Ƃ������΂������������[�X����K�v�͂Ȃ��Ȃ�
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_TextureNormal->Release();
	m_TextureHeight->Release();

	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();
}

//========================================
//�X�V����
//========================================
void CField::Update()
{

}

//========================================
//�`�揈��
//========================================
void CField::Draw()
{

	//���̓��C�A�E�g�ݒ�
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

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormal);		//������:2�Ԗڂ̃e�N�X�`���[�Ƃ��ēo�^���� ������:�z��ԍ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureHeight);

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	CRenderer::GetDeviceContext()->Draw(4, 0);	//����͒��_��4�Ȃ̂�4
}