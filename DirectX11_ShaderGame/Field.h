#pragma once
#include "game_object.h"
//========================================
//�t�B�[���h
//�����㓯�������̕ϐ��Ȃǂ͓������O�ɂ��邱��
//========================================
class CField:public CGameObject
{
private:
	ID3D11Buffer*							m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*		m_Texture = NULL;

	ID3D11ShaderResourceView* m_TextureNormal ;			//�@���}�b�v�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureHeight;			//�n�C�g�}�b�v�e�N�X�`��

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	
public:
	
	void Init();			//������
	void Uninit();		//�I������
	void Update();	//�X�V����
	void Draw();		//�`�揈��
};
